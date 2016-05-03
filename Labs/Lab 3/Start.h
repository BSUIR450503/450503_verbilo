#include "CreateProcess.h"

class Start {
public:
	int run(int argc, char *argv[]);
};

int Start::run(int argc, char *argv[]) {
#ifdef _WIN32
	DWORD numRead;
	DWORD numToWrite;

	if (argc != 2)
	{
		HANDLE namedPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Pipe"),
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			1024, 1024,
			NMPWAIT_USE_DEFAULT_WAIT,
			NULL);

		if (namedPipe == INVALID_HANDLE_VALUE)
		{
			cout << "Fatal [CreatePipe]" << GetLastError() << endl;
			_getch();
			exit(0);
		}

		HANDLE serverSemaphore = CreateSemaphore(NULL, 0, 1, TEXT("serverSemaphore"));
		HANDLE clientSemaphore = CreateSemaphore(NULL, 0, 1, TEXT("clientSemaphore"));

		create_Process *clientProcess = new create_Process(argv[0]);

		ConnectNamedPipe(namedPipe, NULL);

		WaitForSingleObject(serverSemaphore, INFINITE);

		char *buffer = NULL;
		buffer = (char *)malloc(sizeof(char) * 1024);

		cout << "SERVER process" << endl;

		if (!WriteFile(namedPipe, "Ready", 1024, &numToWrite, NULL))
			return 0;

		while (1)
		{
			ReleaseSemaphore(clientSemaphore, 1, NULL);
			WaitForSingleObject(serverSemaphore, INFINITE);

			if (ReadFile(namedPipe, buffer, 1024, &numRead, NULL))
				cout << "==========================" << endl;

				cout << "Client message:" << buffer << endl;

			if (!strcmp(buffer, "exit"))
			{
				CloseHandle(namedPipe);
				CloseHandle(serverSemaphore);
				free(buffer);
				return 0;
			}

			cout << "Message to client: ";
			fflush(stdin);
			gets_s(buffer, 1024);

			if (!WriteFile(namedPipe, buffer, 1024, &numToWrite, NULL))
				break;

			ReleaseSemaphore(clientSemaphore, 1, NULL);

			if (!strcmp(buffer, "exit"))
			{
				CloseHandle(namedPipe);
				CloseHandle(serverSemaphore);

				free(buffer);
				return 0;
			}
		}
		return 0;
	}
	else
	{
		HANDLE serverSemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, TEXT("serverSemaphore"));
		HANDLE clientSemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, TEXT("clientSemaphore"));

		HANDLE namedPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (namedPipe == INVALID_HANDLE_VALUE)
		{
			cout << "Fatal [can't create pipe]" << GetLastError() << endl;
			_getch();
			exit(0);
		}

		ReleaseSemaphore(serverSemaphore, 1, NULL);

		char *buffer = NULL;
		buffer = (char *)malloc(sizeof(char) * 1024);

		cout << "CLIENT process" << endl;

		while (1)
		{
			WaitForSingleObject(clientSemaphore, INFINITE);

			if (ReadFile(namedPipe, buffer, 1024, &numRead, NULL))
			{
				if (!strcmp(buffer, "exit"))
				{
					CloseHandle(clientSemaphore);
					free(buffer);
					return 0;
				}

				cout << "==========================" << endl;

				cout << "Server message:" << buffer << endl;

				char input[1024] = { '\0' };
				cout << "Input message to server: ";
				fflush(stdin);
				gets_s(input, 1024);

				if (!WriteFile(namedPipe, input, 1024, &numToWrite, NULL))
					break;

				if (!strcmp(input, "exit"))
				{
					ReleaseSemaphore(serverSemaphore, 1, NULL);
					CloseHandle(clientSemaphore);
					free(buffer);
					return 0;
				}
			}
			ReleaseSemaphore(serverSemaphore, 1, NULL);
		}
		return 0;
	}

#elif __linux__
	pid_t pid;
	key_t key = ftok("/home/", 0);

	struct sembuf serverSemaphore;
	struct sembuf clientSemaphore;
	int semaphoreId;

	int sharedMemoryId;
	char *segmentPtr;

	if (argc != 2)
	{
		switch (pid = fork())
		{
		case -1:
			cout << "Can't fork process" << pid << endl;
			break;
		case 0:
			execlp("gnome-terminal", "gnome-terminal", "-x", argv[0], "1", NULL);
		default:
			if ((sharedMemoryId = shmget(key, 1024, IPC_CREAT | IPC_EXCL | 0660)) == -1)
			{
				if ((sharedMemoryId = shmget(key, 1024, 0)) == -1)
				{
					cout << "Shmget error" << endl;
					exit(1);
				}
			}

			if ((segmentPtr = (char*)shmat(sharedMemoryId, NULL, 0)) == (char*)(-1))
			{
				cout << "Can't attach shared memory" << endl;
				exit(1);
			}

			semaphoreId = semget(key, 1, 0666 | IPC_CREAT);

			if (semaphoreId < 0)
			{
				cout << "Can't get semaphore" << endl;
				exit(EXIT_FAILURE);
			}

			if (semctl(semaphoreId, 0, SETVAL, (int)0) < 0)
			{
				cout << "Can't initialize semaphore" << endl;
				exit(EXIT_FAILURE);
			}

			serverSemaphore.sem_num = 0;
			serverSemaphore.sem_op = 0;
			serverSemaphore.sem_flg = 0;

			semop(semaphoreId, &serverSemaphore, 1);

			cout << "SERVER process: " << endl;

			while (1) {
				char *message = NULL;
				message = (char*)malloc(1024 * sizeof(char));

				if (semop(semaphoreId, &serverSemaphore, 1) < 0)
					cout << "Can't initialize server semaphore" << endl;

				cout << "Input message to CLIENT: ";
				fflush(stdin);
				cin >> message;

				strcpy(segmentPtr, message);

				serverSemaphore.sem_op = 3;
				semop(semaphoreId, &serverSemaphore, 1);

				serverSemaphore.sem_op = 0;
				semop(semaphoreId, &serverSemaphore, 1);

				if (!strcmp("exit", message))
					return 0;

				strcpy(message, segmentPtr);
				cout << "CLIENT message:" << message << endl;
			}
			return 0;
		}
	}
	else
	{
		if ((sharedMemoryId = shmget(key, 1024, IPC_CREAT | IPC_EXCL | 0660)) == -1)
		{
			if ((sharedMemoryId = shmget(key, 1024, 0)) == -1)
			{
				cout << "Shmget error" << endl;
				exit(1);
			}
		}

		if ((segmentPtr = (char*)shmat(sharedMemoryId, NULL, 0)) == (char*)(-1))
		{
			cout << "Can't attach shared memory" << endl;
			exit(1);
		}

		semaphoreId = semget(key, 1, 0666 | IPC_CREAT);

		if (semaphoreId < 0)
		{
			cout << "Semget error" << endl;
			exit(EXIT_FAILURE);
		}

		if (semctl(semaphoreId, 0, SETVAL, (int)0) < 0)
		{
			cout << "Semctl error" << endl;
			exit(EXIT_FAILURE);
		}

		cout << "CLIENT process: " << endl;

		clientSemaphore.sem_num = 0;
		clientSemaphore.sem_op = 1;
		clientSemaphore.sem_flg = 0;
		semop(semaphoreId, &clientSemaphore, 1);

		clientSemaphore.sem_op = -1;
		semop(semaphoreId, &clientSemaphore, 1);
		clientSemaphore.sem_op = -2;

		if ((sharedMemoryId = shmget(key, 1024, 0)) == -1)
		{
			cout << "Shmget error" << endl;
			exit(1);
		}

		while (1) {
			char *message = NULL;
			message = (char*)malloc(1024 * sizeof(char));

			semop(semaphoreId, &clientSemaphore, 1);

			strcpy(message, segmentPtr);

			if (!strcmp("exit", message))
			{
				clientSemaphore.sem_op = -1;
				semop(semaphoreId, &clientSemaphore, 1);
				return 0;
			}

			cout << "==========================" << endl;

			cout << "SERVER message:" << message << endl;

			cout << "Input message to server: ";
			fflush(stdin);
			cin >> message;

			strcpy(segmentPtr, message);

			clientSemaphore.sem_op = -1;
			semop(semaphoreId, &clientSemaphore, 1);

			clientSemaphore.sem_op = -2;
		}
		return 0;
	}
#endif
}