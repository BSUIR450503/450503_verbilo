#include "filemanage.h"

filemanage::filemanage()  //
{

}

QString filemanage::GetFileDirectory() //функция получения адреса папки с ресурсами
{
    QString  DirectoryFile = "E:\\QtProjects\\MrChord\\Directory.txt";

        QFile DirectoryF(DirectoryFile);

        if (DirectoryF.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&DirectoryF);
                FileDirectory = in.readLine();
        }
        return FileDirectory;
}

notes *filemanage::ScanNotes()  //функция получения списка нот и ссылок на файлы с аккордами
{
   Notes = new notes[C];
   QString FileName = FileDirectory + "MrChord\\Chords\\Notes.txt";
   QFile NotesFile(FileName);
   if (!NotesFile.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       QMessageBox::information(0,"Error","file not open SN");
       return NULL;
   }

   QTextStream in(&NotesFile);
   int i=0;
   while(!in.atEnd())
   {

       Notes[i].Note = in.readLine();
       Notes[i].ChordFile = in.readLine();
       if(i == C)
           break;
       i++;
   }
   NotesFile.flush();
   NotesFile.close();
   return Notes;
}



void filemanage::ScanChords(QString FileName, chords* CurrChords) //функция получения списка аккордов и ссылок намедиаресурсы
{
    int i=0;

    QFile ChordsFile(FileDirectory + FileName);
    if (!ChordsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(0,"Error","file not open");
        return;
    }

    QTextStream in(&ChordsFile);
    while(!in.atEnd())
    {
        CurrChords[i].ChordName = in.readLine();
        CurrChords[i].ChordSound = in.readLine();
        CurrChords[i].ChordPicture = in.readLine();
        i++;
    }

    ChordsFile.flush();
    ChordsFile.close();
}


void filemanage::SearchChord(QString Chord, chords &Found)  //функция поиска аккорда
{
    int j = 0, flag = 0;
    for(int i = 0; i < C; i++)
    {
        QFile TempChordsFile(FileDirectory + Notes[i].ChordFile);
        if (!TempChordsFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::information(0,"Search","file not open");
            return;
        }

        QTextStream in(&TempChordsFile);
        while(!in.atEnd())
        {
            Temp.ChordName = in.readLine();
            Temp.ChordSound = in.readLine();
            Temp.ChordPicture = in.readLine();

            if(QString::compare(Temp.ChordName, Chord) == 0)
            {
               QMessageBox::information(0,"Chord found!",Temp.ChordName);
               flag = 1;
               Found.ChordPicture = Temp.ChordPicture;
               Found.ChordSound = Temp.ChordSound;
               Found.ChordName = Temp.ChordName;
               break;
            }

            j++;
        }

        TempChordsFile.flush();
        TempChordsFile.close();
        if(flag == 1)
            break;
    }

    if(flag == 0)
    {
        QMessageBox::information(0,"Search","Nothing found!");
        Found.ChordPicture = Found.ChordSound = Found.ChordName = "\0";
    }
}


void AddChordClass::AddChord(chords AChord, QString ANote)  //функция добавления аккорда
{

  int flag = 0;

  for(int i = 0; i < C; i++)
  {
      if(QString::compare(Notes[i].Note, ANote) == 0)
      {
          QFile TempFile(FileDirectory + Notes[i].ChordFile);

          if (!TempFile.open(QIODevice::Append | QIODevice::Text))
          {
              QMessageBox::information(0,"Add","file not open");
              return;
          }
          QTextStream out(&TempFile);
          out << AChord.ChordName << endl;
          out << AChord.ChordSound << endl;
          out << AChord.ChordPicture << endl;
         TempFile.flush();
         TempFile.close();

         flag = 1;

         break;
      }

  }
  if(flag == 0)
      QMessageBox::information(0,"AddChord","invalid note");
  else
      QMessageBox::information(0,"Success!","Chord added!");
}


void DelChordClass::DeleteChord(QString DNote, QString DChord) //функция удаления аккорда
{
    int flag = 0, j = 0, DSize = 0;

    for(int i = 0; i < C; i++)
    {
        if(QString::compare(Notes[i].Note, DNote) == 0)
        {
            QFile TempFile(FileDirectory + Notes[i].ChordFile);

            if (!TempFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QMessageBox::information(0,"Del","file not open");
                return;
            }

            QTextStream in(&TempFile);
            while(!in.atEnd())
            {

                Temp.ChordName = in.readLine();
                Temp.ChordSound = in.readLine();
                Temp.ChordPicture = in.readLine();

                DelList.push_back(Temp);
            }

           TempFile.flush();
           TempFile.close();

           QFile TempFile2(FileDirectory + Notes[i].ChordFile);

           if (!TempFile2.open(QIODevice::WriteOnly | QIODevice::Text))
           {
               QMessageBox::information(0,"Del","file not open");
               return;
           }

           chords *TempPtr;

           DSize = DelList.size();

           QTextStream out(&TempFile2);
           while(j < DSize)
           {
               TempPtr = &DelList.at(j);

               if(QString::compare(TempPtr->ChordName, DChord) != 0)
               {
               out << TempPtr->ChordName << endl;
               out << TempPtr->ChordSound << endl;
               out << TempPtr->ChordPicture << endl;
               }
               else
                  flag = 1;

               j++;
           }

           TempFile2.flush();
           TempFile2.close();

           break;
        }

    }
    if(flag == 0)
        QMessageBox::information(0,"DelChord","Error");
    else
        QMessageBox::information(0,"DelChord",DChord + " removed!");

}
