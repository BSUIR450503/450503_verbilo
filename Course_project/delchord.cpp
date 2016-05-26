#include "delchord.h"
#include "ui_delchord.h"

DelChord::DelChord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DelChord) {

    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(190,150);
    setWindowTitle("Delete");
}

DelChord::~DelChord() {
    delete ui;
}

void DelChord::on_buttonBox_accepted() {
    QString enteredNote;
    QString enteredChord;

    enteredNote = ui->NoteLine->text();
    enteredChord = ui->ChordLine->text();

    if(enteredNote == NULL || enteredChord == NULL) {
        QMessageBox::information(0,"INPUT","Error");
        return;
    }

    DelChordClass Del;
    Del.GetFileDirectory();
    Del.ScanNotes();
    Del.DeleteChord(enteredNote, enteredChord);
}
