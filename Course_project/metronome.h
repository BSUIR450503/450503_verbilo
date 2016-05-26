#ifndef METRONOME_H
#define METRONOME_H

#include <QDialog>
#include <QMessageBox>
#include <ticker.h>
#include <QCloseEvent>

namespace Ui {
class Metronome;
}

class Metronome : public QDialog {

    Q_OBJECT

public:
    explicit Metronome(QWidget *parent = 0);
    float calculateInterval();
    ~Metronome();
private:
    QString soundPath = "E:\\QtProjects\\MrChord\\tick.wav";
    QTimer *timer;
    QMediaPlayer *player;

    bool isMetronomeActive = false;
    int currentBPM = 120;

    Ui::Metronome *ui;
private slots:
    void on_setButton_clicked();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_buttonBox_accepted();
    void closeEvent(QCloseEvent *event);
};

#endif // METRONOME_H
