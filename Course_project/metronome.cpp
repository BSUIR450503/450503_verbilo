#include "metronome.h"
#include "ui_metronome.h"

Metronome::Metronome(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Metronome)
{
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(250,250);
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok);
    ui->currentBPMLabel->setStyleSheet("QLabel {"
                                 "border-style: solid;"
                                 "border-width: 3px;"
                                 "border-color: black; "
                                 "}");
    setWindowTitle("Johnny The Metronome");

    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(qPrintable(soundPath))));
    player->setVolume(80);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), player, SLOT(play()));
}

Metronome::~Metronome() {
    delete ui;
}

void Metronome::on_startButton_clicked() {
    if (currentBPM > 0 && !isMetronomeActive) {
        timer->start(calculateInterval());
        isMetronomeActive = true;
    }
}

void Metronome::on_setButton_clicked() {
    int enteredBPM;
    QString str = ui->setLine->text();
    enteredBPM = str.toInt();
    if (enteredBPM > 0 && enteredBPM < 301) {
        currentBPM = enteredBPM;
        ui->currentBPMLabel->setText(str + " bpm");
    } else {
        QMessageBox::information(0, "Set BPM", "invalid range");
        ui->setLine->clear();
    }
    timer->setInterval(calculateInterval());
}

float Metronome::calculateInterval() {
   return 1000.0 * 60.0 / float(currentBPM);
}

void Metronome::on_stopButton_clicked() {
    timer->stop();
    isMetronomeActive = false;
}

void Metronome::on_buttonBox_accepted() {
    timer->stop();
}

void Metronome::closeEvent(QCloseEvent *event) {
    timer->stop();
    event->accept();
    delete ui;
}
