#ifndef ADDCHORD_H
#define ADDCHORD_H

#include <QDialog>
#include "filemanage.h"

namespace Ui {
class AddChord;
}

class AddChord : public QDialog
{
    Q_OBJECT

public:
    explicit AddChord(QWidget *parent = 0);  //конструктор
    ~AddChord();                             //деструктор

private slots:                               //слот бокса с кнопками
    void on_buttonBox_accepted();

private:
    Ui::AddChord *ui;
};

#endif // ADDCHORD_H
