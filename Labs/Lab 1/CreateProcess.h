#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <iostream>
#endif

#ifdef linux
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#endif

using namespace std;

class MyProcess
{
public:
	MyProcess(int, char**);
};