#ifndef ADDCHORD_H
#define ADDCHORD_H

#include <QDialog>
#include "filemanage.h"

namespace Ui {
class AddChord;
}

class AddChord : public QDialog {
    Q_OBJECT

public:
    explicit AddChord(QWidget *parent = 0);
    ~AddChord();
private:
    Ui::AddChord *ui;
private slots:
    void on_buttonBox_accepted();
};

#endif // ADDCHORD_H
