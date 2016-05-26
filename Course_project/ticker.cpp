#include "ticker.h"

ticker::ticker(int bpm) {
    this->bpm = bpm;

    Player = new QMediaPlayer;
    Player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(qPrintable(soundPath))));
    Player->setVolume(50);
}

void ticker::setBPM(int bpm) {
    this->bpm = bpm;
    interval = 1000.0 * 60.0 / float(bpm);
}

void ticker::startTicking() {
       Player->play();
}
