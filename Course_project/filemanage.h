#ifndef FILEMANAGE_H
#define FILEMANAGE_H

#include <QUrl>
#include <QDir>
#include <QDebug>

#include <QFile>
#include <QString>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>

#include <deque>

#define N 80
#define C 12

extern QString fileDirectory;

struct notes {
    QString note;
    QString chordFile;
};

struct chords {
    QString chordName;
    QString chordPicture;
    QString chordSound;
    QString chordDiagram;
};

class filemanage {
protected:
    notes *Notes;
    QString fileDirectory;

    std::deque<chords> DelList;
public:
    explicit filemanage();

    notes* ScanNotes();
    void ScanChords(QString FileName, chords* CurrCords);
    void SearchChord(QString Chord, chords &Found);

    QString GetFileDirectory();

};


class AddChordClass : public filemanage {
public:
   void AddChord(chords AChord, QString ANote);
};


class DelChordClass : public filemanage {
public:
   void DeleteChord(QString Note, QString deleteChord);
};


#endif // FILEMANAGE_H
