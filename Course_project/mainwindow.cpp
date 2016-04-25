#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filemanage.h"

#include "addchord.h"
#include "delchord.h"


MainWindow::MainWindow(QWidget *parent) : //конструктор виджета
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(280,260);

    setWindowTitle("Mr.Chord");

    FileDirectory = Start.GetFileDirectory();

    QPixmap Icon(FileDirectory + "MrChord\\Pictures\\icon.png");

    setWindowIcon(QIcon(Icon));

    Notes = new notes [C];

    Player = new QMediaPlayer;

    Notes = Start.ScanNotes();

    for(int i = 0; i < C; i++)
      ui->ChordNote->addItem(Notes[i].Note);

}


MainWindow::~MainWindow()  //тело деструктора
{
    delete ui;
}

void MainWindow::on_Search_clicked() //слот, вызываемый при нажатии Search
{
    chords FoundChord;

    SearchC = ui->SearchLine->text();

    Start.SearchChord(SearchC, FoundChord);

    QPixmap ChordPic(FileDirectory + FoundChord.ChordPicture);
    ui->ChordPic->setPixmap(ChordPic);

    CurrentSoundUrl = FoundChord.ChordSound;
}

void MainWindow::on_Camertone_clicked()
{
    QString Camertone = FileDirectory + "MrChord\\ChordSounds\\Camertone.mp3";
    QMessageBox::information(this,"Tune the guitar","1 string, 5 fret");
    Player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(qPrintable(Camertone))));
    Player->setVolume(50);
    Player->play();
}

void MainWindow::on_PlayChord_clicked() //слот, вызываемый при нажатии PlayChord
{
    if(CurrentSoundUrl == NULL)
    {
        QMessageBox::information(this,"PlayChord","No audiofile");
        return;
    }
    Player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(qPrintable(FileDirectory + CurrentSoundUrl))));

    Player->setVolume(50);
    Player->play();
}

void MainWindow::on_ChordNote_activated()  //слот, вызываемый при нажатии на список нот
{
    CurrentNote =  ui->ChordNote->currentText();

    for(int i = 0; i < C; i++)
    {
      if(QString::compare(CurrentNote,Notes[i].Note) == 0)
       {
          QFile Temp(FileDirectory + Notes[i].ChordFile);

          if (Temp.open(QIODevice::ReadOnly | QIODevice::Text))
          {
              ChordsN = QTextStream(&Temp).readAll().split('\n').count();
              ChordsN /= 3;
              CurrChords = new chords [ChordsN];
          }
          else
          {
              QMessageBox::information(0,"Error","file not open");
              return;
          }

          Temp.flush();
          Temp.close();

          Start.ScanChords(Notes[i].ChordFile,CurrChords);
          break;
       }
    }

    ui->ChordType->clear();

    for(int i = 0; i < ChordsN; i++)
      ui->ChordType->addItem(CurrChords[i].ChordName);
}


void MainWindow::on_ChordType_activated() //слот, вызываемый при нажатии на список аккордов
{

    CurrentChord = ui->ChordType->currentText();

    for(int i = 0; i < ChordsN; i++)
    {
        if(QString::compare(CurrentChord,CurrChords[i].ChordName) == 0)
         {
            CurrentSoundUrl = CurrChords[i].ChordSound;
            CurrentPicUrl = CurrChords[i].ChordPicture;
            QPixmap ChordPic(FileDirectory + CurrentPicUrl);
            ui->ChordPic->setPixmap(ChordPic);
            break;
         }

    }

}


void MainWindow::on_Add_clicked()  //слот, вызываемый при нажатии на Add
{
   AddChord *AddAction = new AddChord;

   AddAction->show();

}


void MainWindow::on_Del_clicked()  //слот, вызываемый при нажатии на Del
{
    DelChord *DelAction = new DelChord;

    DelAction->show();
}
