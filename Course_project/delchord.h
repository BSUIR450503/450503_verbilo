#ifndef DELCHORD_H
#define DELCHORD_H

#include <QDialog>
#include "filemanage.h"

namespace Ui {
class DelChord;
}

class DelChord : public QDialog
{
    Q_OBJECT

public:
    explicit DelChord(QWidget *parent = 0);  //конструктор
    ~DelChord();                             //деструктор

private slots:                               //слот бокса с кнопками
    void on_buttonBox_accepted();

private:
    Ui::DelChord *ui;
};

#endif // DELCHORD_H
