#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QMediaPlayer>
#include <QPushButton>

#include "filemanage.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow  //класс главного окна
{
public:
    explicit MainWindow(QWidget *parent = 0);//конструктор
    ~MainWindow(); //деструктор

private:

    Q_OBJECT

    int ChordsN;

    filemanage Start; //создания объекта класса filemanage

    notes* Notes;
    chords* CurrChords;

    QString FileDirectory;
    QString SearchC;
    QString CurrentNote;
    QString CurrentChord;
    QString CurrentPicUrl;
    QString CurrentSoundUrl;

    QMediaPlayer *Player; //создания объекта класса QMediaPlayer для работы с аудио

    Ui::MainWindow *ui;
private slots:                 //слоты кнопок и выдвижных списков
    void on_PlayChord_clicked();
    void on_Camertone_clicked();

    void on_ChordNote_activated();
    void on_ChordType_activated();

    void on_Search_clicked();
    void on_Add_clicked();
    void on_Del_clicked();
};

#endif // MAINWINDOW_H
