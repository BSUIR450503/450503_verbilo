#ifndef TUNERWINDOW_H
#define TUNERWINDOW_H

#include <QTimer>
#include <QDialog>
#include <QPainter>
#include <QMessageBox>
#include <QCloseEvent>

#define notesQuantity 12
#define octavesQuantity 6

namespace Ui {
class TunerWindow;
}

struct noteInfo {
    double pitch;
    QString name;
};

class TunerWindow : public QDialog
{
    Q_OBJECT

    noteInfo **notesList;

    double lowestNote = 32.7; //C1
    double currentFrequency;

    bool centralBlue;
    bool leftRed, leftOrange, leftYellow;
    bool rightRed, rightOrange, rightYellow;

    QString noteNames = "CCDDEFFGGAAB";
    QTimer *timer;
public:
    explicit TunerWindow(QWidget *parent = 0);
    ~TunerWindow();
private:
    void initializeNotesStruct();
    double calculatePitch(double previousPitch);
    void resetColorFlags();

    void calculateClosestNote();

    void resetIndicationField();
    void paintEvent(QPaintEvent *event);

    Ui::TunerWindow *ui;
private slots:
    void getCurrentPitch();
    void closeEvent(QCloseEvent *event);
};

#endif // TUNERWINDOW_H
