#ifndef DELCHORD_H
#define DELCHORD_H

#include <QDialog>
#include "filemanage.h"

namespace Ui {
class DelChord;
}

class DelChord : public QDialog {
    Q_OBJECT

public:
    explicit DelChord(QWidget *parent = 0);
    ~DelChord();
private:
    Ui::DelChord *ui;
private slots:
    void on_buttonBox_accepted();

};

#endif // DELCHORD_H
