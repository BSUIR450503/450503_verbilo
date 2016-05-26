#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filemanage.h"
#include "addchord.h"
#include "delchord.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    setFixedSize(800, 300);
    setWindowTitle("Mr.Chord");

    fileDirectory = Start.GetFileDirectory();

    QPixmap Icon(fileDirectory + "MrChord\\Pictures\\icon.png");
    setWindowIcon(QIcon(Icon));

    Notes = new notes [C];
    player = new QMediaPlayer;
    Notes = Start.ScanNotes();

    for(int i = 0; i < C; i++)
      ui->ChordNote->addItem(Notes[i].note);

    resetNeckPicture();
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_Search_clicked() {

    chords FoundChord;

    searchChord = ui->SearchLine->text();
    Start.SearchChord(searchChord, FoundChord);

    QPixmap ChordPic(fileDirectory + FoundChord.chordPicture);
    ui->ChordPic->setPixmap(ChordPic);

    resetNeckPicture();

    currentChordDiagram = FoundChord.chordDiagram;
    currentSoundUrl = FoundChord.chordSound;
}

void MainWindow::on_Camertone_clicked() {

    QString Camertone = fileDirectory + "MrChord\\ChordSounds\\Camertone.mp3";
    QMessageBox::information(this, "Tune the guitar", "1 string, 5 fret");

    player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(qPrintable(Camertone))));
    player->setVolume(50);
    player->play();
}

void MainWindow::on_PlayChord_clicked() {

    if(currentSoundUrl == NULL) {
        QMessageBox::information(this,"PlayChord","No audiofile");
        return;
    }

    player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(qPrintable(fileDirectory + currentSoundUrl))));
    player->setVolume(50);
    player->play();
}

void MainWindow::on_ChordNote_activated() {

    currentNote =  ui->ChordNote->currentText();

    for(int i = 0; i < C; i++) {
      if(QString::compare(currentNote,Notes[i].note) == 0) {
          QFile Temp(fileDirectory + Notes[i].chordFile);

          if (Temp.open(QIODevice::ReadOnly | QIODevice::Text)) {
              chordCounter = QTextStream(&Temp).readAll().split('\n').count();
              chordCounter /= 4;
              currentChords = new chords [chordCounter];
          } else {
              QMessageBox::information(0,"Error","file not open");
              return;
          }

          Temp.flush();
          Temp.close();

          Start.ScanChords(Notes[i].chordFile,currentChords);
          break;
       }
    }

    ui->ChordType->clear();
    for(int i = 0; i < chordCounter; i++) {
      ui->ChordType->addItem(currentChords[i].chordName);
    }
}


void MainWindow::on_ChordType_activated() {

    currentChord = ui->ChordType->currentText();

    resetNeckPicture();

    for(int i = 0; i < chordCounter; i++)
    {
        if(QString::compare(currentChord,currentChords[i].chordName) == 0)
         {
            currentSoundUrl = currentChords[i].chordSound;
            currentPicUrl = currentChords[i].chordPicture;
            currentChordDiagram = currentChords[i].chordDiagram;
            QPixmap ChordPic(fileDirectory + currentPicUrl);
            ui->ChordPic->setPixmap(ChordPic);
            break;
         }
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event);

    int xStart = 55, yStart = 150, yEnd = 20, zeroFretX = 35;
    int xStep = 38, yStep = 27;
    int xPos, yPos;
    int currentString, currentFret;
    QString temp;

    QImage tmp(ui->neckDiagram->pixmap()->toImage());
    QPainter painter(&tmp);
    QPen paintpen(Qt::black);

    if(!currentChordDiagram.isEmpty()) {

    for(int i = 0; i < 7; i++) {

        xPos = yPos = 0;
        temp = currentChordDiagram[i];

        if (i == 0 && QString::compare(temp, "n") != 0) {
            currentFret = currentChordDiagram.at(i).digitValue();

            yPos = yStart;
            xPos = xStart + (xStep * (currentFret - 1)) + 6;

            paintpen.setColor("red");
            paintpen.setWidth(20);
            painter.setPen(paintpen);
            painter.drawLine(xPos, yPos, xPos, yEnd);
        }

        if (i > 0 && QString::compare(temp, "n") != 0) {
           currentString = i;
           currentFret = currentChordDiagram.at(i).digitValue();

           if(QString::compare(temp, "0") == 0) {
               xPos = zeroFretX;
               paintpen.setColor("blue");
           } else {
               xPos = xStart + (xStep * (currentFret - 1));
               paintpen.setColor("orange");
           }

           yPos = yStart - (yStep * (currentString - 1));

           paintpen.setWidth(10);
           painter.setPen(paintpen);
           painter.drawEllipse(xPos, yPos, 10, 10);
        }
      }
    }

    ui->neckDiagram->setPixmap(QPixmap::fromImage(tmp));
}

void MainWindow::resetNeckPicture() {
    QPixmap NeckPic(fileDirectory + "MrChord\\Pictures\\neckDiagram.png");
    ui->neckDiagram->setPixmap(NeckPic);
}

void MainWindow::on_tunerButton_clicked() {
    TunerWindow *tuner = new TunerWindow;
    tuner->show();
}

void MainWindow::on_metronomeButton_clicked() {
    Metronome *metronome = new Metronome;
    metronome->show();
}

void MainWindow::on_Add_clicked() {
   AddChord *AddAction = new AddChord;
   AddAction->show();
}


void MainWindow::on_Del_clicked() {
    DelChord *DelAction = new DelChord;
    DelAction->show();
}
