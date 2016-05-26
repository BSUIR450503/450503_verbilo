#ifndef TICKER_H
#define TICKER_H

#include <QDir>
#include <QUrl>

#include <QTimer>
#include <QTime>

#include <QString>

#include <QMediaPlayer>
#include <QMessageBox>

class ticker
{
    int bpm;
    float interval;

    QString soundPath = "E:\\QtProjects\\MrChord\\tick.mp3";

    QTimer *timer;
    QMediaPlayer *Player;
public:
    ticker(int bpm);

    void setBPM(int newBPM);
    void startTicking();
};

#endif // TICKER_H
