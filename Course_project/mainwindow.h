#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPainter>
#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QMediaPlayer>
#include <QPushButton>

#include "filemanage.h"
#include "tunerwindow.h"
#include "metronome.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Q_OBJECT

    int chordCounter;

    filemanage Start;

    notes* Notes;
    chords* currentChords;

    QString fileDirectory;
    QString searchChord;

    QString currentNote;
    QString currentChord;
    QString currentPicUrl;
    QString currentSoundUrl;
    QString currentChordDiagram;

    QMediaPlayer *player;

    void paintEvent(QPaintEvent *event);
    void resetNeckPicture();

    Ui::MainWindow *ui;
private slots:
    void on_PlayChord_clicked();
    void on_Camertone_clicked();
    void on_tunerButton_clicked();
    void on_metronomeButton_clicked();

    void on_ChordNote_activated();
    void on_ChordType_activated();

    void on_Search_clicked();
    void on_Add_clicked();
    void on_Del_clicked();
};

#endif // MAINWINDOW_H
