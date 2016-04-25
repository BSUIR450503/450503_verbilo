#include "addchord.h"
#include "ui_addchord.h"

AddChord::AddChord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChord)
{
    ui->setupUi(this);

    setFixedSize(190,205);

    setWindowTitle("Add");
}

AddChord::~AddChord()  //тело деструктора
{
    delete ui;
}

void AddChord::on_buttonBox_accepted()   //слот, вызываемый при нажатии ОК
{
    QString ANote;
    chords AChord;

    ANote = ui->NoteLine->text();

    AChord.ChordName = ui->ChordLine->text();
    AChord.ChordSound = ui->AudioURL->text();
    AChord.ChordPicture = ui->PictureURL->text();

    if(ANote == NULL || AChord.ChordName == NULL || AChord.ChordPicture == NULL || AChord.ChordSound == NULL)
    {
        QMessageBox::information(0,"INPUT","Error");
        return;
    }

    AddChordClass Add;

    Add.GetFileDirectory();
    Add.ScanNotes();

    Add.AddChord(AChord, ANote);
}
