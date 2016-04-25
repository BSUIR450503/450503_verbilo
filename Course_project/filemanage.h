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

extern QString FileDirectory;

struct chords  // структура содержащая название аккорда и пути к файлу со схемой и аудиофайлу
{
    QString ChordName;
    QString ChordPicture;
    QString ChordSound;
};

struct notes   // структура содержащая название ноты и путь к файлу с аккордами
{
    QString Note;
    QString ChordFile;
};

class filemanage   //класс предназначен для работы с файлами
{
protected:
    notes *Notes;
    chords Temp;

    QString FileDirectory;

    std::deque<chords> DelList;  //инициализация STL контейнера-итератора deque
public:
    explicit filemanage();  //конструктор

    notes* ScanNotes();
    void ScanChords(QString FileName, chords* CurrCords);
    void SearchChord(QString Chord, chords &Found);

    QString GetFileDirectory();

};


class AddChordClass : public filemanage  //производный класс
{

public:
   void AddChord(chords AChord, QString ANote);
};


class DelChordClass : public filemanage //производный класс
{

public:
   void DeleteChord(QString DNote, QString DChord);
};


#endif // FILEMANAGE_H
