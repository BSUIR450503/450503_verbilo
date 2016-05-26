#include "addchord.h"
#include "ui_addchord.h"

AddChord::AddChord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChord) {

    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(420,230);
    setWindowTitle("Add");
}

AddChord::~AddChord() {
    delete ui;
}

void AddChord::on_buttonBox_accepted() {
    QString enteredNote;
    chords enteredChord;

    enteredNote = ui->NoteLine->text();

    enteredChord.chordName = ui->ChordLine->text();
    enteredChord.chordSound = ui->AudioURL->text();
    enteredChord.chordPicture = ui->PictureURL->text();
    enteredChord.chordDiagram = ui->bareLine->text() +
            ui->sixthStringLine->text() +
            ui->fifthStringLine->text() +
            ui->fourthStringLine->text() +
            ui->thirdStringLine->text() +
            ui->secondStringLine->text() +
            ui->firstStringLine->text();

    if (enteredChord.chordDiagram.length() > 7 || enteredChord.chordDiagram.length() < 7) {
        QMessageBox::information(0,"Chord Structure","Input error");
        return;
    }

    if(enteredNote == NULL || enteredChord.chordName == NULL ||
            enteredChord.chordPicture == NULL || enteredChord.chordSound == NULL ||
            enteredChord.chordDiagram == NULL) {
        QMessageBox::information(0,"INPUT","Error");
        return;
    }

    AddChordClass Add;

    Add.GetFileDirectory();
    Add.ScanNotes();

    Add.AddChord(enteredChord, enteredNote);
}
