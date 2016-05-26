#include "filemanage.h"

filemanage::filemanage() {}

QString filemanage::GetFileDirectory() {
    QString  dirFilePath = "E:\\QtProjects\\MrChord\\Directory.txt";

        QFile dirFile(dirFilePath);

        if (dirFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&dirFile);
                fileDirectory = in.readLine();
        }
        return fileDirectory;
}

notes *filemanage::ScanNotes() {
   Notes = new notes[C];

   QString FileName = fileDirectory + "MrChord\\Chords\\Notes.txt";
   QFile NotesFile(FileName);
   if (!NotesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
       QMessageBox::information(0,"Error","file not open SN");
       return NULL;
   }

   QTextStream in(&NotesFile);
   int i=0;

   while(!in.atEnd()) {
       Notes[i].note = in.readLine();
       Notes[i].chordFile = in.readLine();
       if(i == C)
           break;
       i++;
   }

   NotesFile.flush();
   NotesFile.close();

   return Notes;
}



void filemanage::ScanChords(QString FileName, chords* CurrChords) {
    int i=0;

    QFile ChordsFile(fileDirectory + FileName);
    if (!ChordsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(0,"Error","file not open");
        return;
    }

    QTextStream in(&ChordsFile);
    while(!in.atEnd()) {
        CurrChords[i].chordName = in.readLine();
        CurrChords[i].chordSound = in.readLine();
        CurrChords[i].chordPicture = in.readLine();
        CurrChords[i].chordDiagram = in.readLine();
        i++;
    }

    ChordsFile.flush();
    ChordsFile.close();
}


void filemanage::SearchChord(QString Chord, chords &Found) {
    int j = 0;
    bool flag = false;
    chords tempChordStruct;

    for(int i = 0; i < C; i++) {
        QFile TempChordsFile(fileDirectory + Notes[i].chordFile);
        if (!TempChordsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::information(0,"Search","file not open");
            return;
        }

        QTextStream in(&TempChordsFile);
        while(!in.atEnd()) {
            tempChordStruct.chordName = in.readLine();
            tempChordStruct.chordSound = in.readLine();
            tempChordStruct.chordPicture = in.readLine();
            tempChordStruct.chordDiagram = in.readLine();

            if(QString::compare(tempChordStruct.chordName, Chord) == 0) {
               flag = true;
               Found.chordPicture = tempChordStruct.chordPicture;
               Found.chordSound = tempChordStruct.chordSound;
               Found.chordName = tempChordStruct.chordName;
               Found.chordDiagram = tempChordStruct.chordDiagram;
               break;
            }

            j++;
        }

        TempChordsFile.flush();
        TempChordsFile.close();

        if(flag)
            break;
    }

    if(!flag) {
        QMessageBox::information(0,"Search","Nothing found!");
        Found.chordPicture = Found.chordSound = Found.chordName = "\0";
    }
}


void AddChordClass::AddChord(chords newChord, QString Note) {
  bool flag;

  for(int i = 0; i < C; i++) {
      if(QString::compare(Notes[i].note, Note) == 0) {
          QFile TempFile(fileDirectory + Notes[i].chordFile);

          if (!TempFile.open(QIODevice::Append | QIODevice::Text)) {
              QMessageBox::information(0,"Add","file not open");
              return;
          }
          QTextStream out(&TempFile);
          out << newChord.chordName << endl;
          out << newChord.chordSound << endl;
          out << newChord.chordPicture << endl;
          out << newChord.chordDiagram << endl;
         TempFile.flush();
         TempFile.close();

         flag = true;

         break;
      }

  }
  if(!flag)
      QMessageBox::information(0,"AddChord","invalid note");
  else
      QMessageBox::information(0,"Success!","Chord added!");
}


void DelChordClass::DeleteChord(QString Note, QString deleteChord) {

    int j = 0, listSize = 0;
    bool flag;
    chords tempChordStruct;

    for(int i = 0; i < C; i++) {
        if(QString::compare(Notes[i].note, Note) == 0) {
            QFile tempFile(fileDirectory + Notes[i].chordFile);

            if (!tempFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::information(0,"Del","file not open");
                return;
            }

            QTextStream in(&tempFile);
            while(!in.atEnd()) {
                tempChordStruct.chordName = in.readLine();
                tempChordStruct.chordSound = in.readLine();
                tempChordStruct.chordPicture = in.readLine();
                tempChordStruct.chordDiagram = in.readLine();

                DelList.push_back(tempChordStruct);
            }

           tempFile.flush();
           tempFile.close();

           QFile tempFileRewrite(fileDirectory + Notes[i].chordFile);

           if (!tempFileRewrite.open(QIODevice::WriteOnly | QIODevice::Text)) {
               QMessageBox::information(0,"Del","file not open");
               return;
           }

           chords *tempChordPtr;
           listSize = DelList.size();

           QTextStream out(&tempFileRewrite);
           while(j < listSize) {
               tempChordPtr = &DelList.at(j);

               if(QString::compare(tempChordPtr->chordName, deleteChord) != 0) {
               out << tempChordPtr->chordName << endl;
               out << tempChordPtr->chordSound << endl;
               out << tempChordPtr->chordPicture << endl;
               out << tempChordPtr->chordDiagram << endl;
               } else {
                  flag = true;
               }

               j++;
           }

           tempFileRewrite.flush();
           tempFileRewrite.close();

           break;
        }

    }

    if(!flag) {
        QMessageBox::information(0,"DelChord","Error");
    } else {
        QMessageBox::information(0,"DelChord",deleteChord + " removed!");
    }

}
