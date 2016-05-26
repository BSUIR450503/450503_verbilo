#ifndef TUNERWINDOW_H
#define TUNERWINDOW_H

#include <QTimer>
#include <QDialog>
#include <QPainter>
#include <QMessageBox>
#include <QCloseEvent>

#define notesQuantity 12
#define octavesQuantity 6
#define indicationCircles 7

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

    bool colorFlags[indicationCircles];
    double rangeDevider[4] = {1.5, 4.0, 8.0, 16.0};
    QString circleColors[indicationCircles] =
    {"red", "orange", "yellow", "blue", "yellow", "orange", "red"};

    QString noteNames = "CCDDEFFGGAAB";
    QTimer *timer;
public:
    explicit TunerWindow(QWidget *parent = 0);
    ~TunerWindow();
private:
    void initializeNotesStruct();
    void calculateClosestNote();
    void resetColorFlags();

    void resetIndicationField();
    void paintEvent(QPaintEvent *event);

    Ui::TunerWindow *ui;
private slots:
    void getCurrentPitch();
    void closeEvent(QCloseEvent *event);
};

#endif // TUNERWINDOW_H
