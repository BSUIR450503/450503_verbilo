#include "delchord.h"
#include "ui_delchord.h"

DelChord::DelChord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DelChord)
{
    ui->setupUi(this);

    setFixedSize(190,150);

    setWindowTitle("Delete");
}

DelChord::~DelChord()    //тело деструктора
{
    delete ui;
}

void DelChord::on_buttonBox_accepted()  //слот, вызываемый при нажатии ОК
{
    QString DNote;
    QString DChord;

    DNote = ui->NoteLine->text();

    DChord = ui->ChordLine->text();

    if(DNote == NULL || DChord == NULL)
    {
        QMessageBox::information(0,"INPUT","Error");
        return;
    }

    DelChordClass Del;

    Del.GetFileDirectory();
    Del.ScanNotes();

    Del.DeleteChord(DNote, DChord);
}
