#include "tunerwindow.h"
#include "ui_tunerwindow.h"

TunerWindow::TunerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TunerWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(220, 250);
    setWindowTitle("MicroTune");
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok);
    ui->closestNote->setStyleSheet("QLabel {"
                                 "border-style: solid;"
                                 "border-width: 3px;"
                                 "border-color: black; "
                                 "}");
    ui->indicationLabel->setStyleSheet("QLabel {"
                                 "border-style: solid;"
                                 "border-width: 2px;"
                                 "border-color: black; "
                                 "}");
    resetColorFlags();
    resetIndicationField();
    initializeNotesStruct();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getCurrentPitch()));
    timer->start(200);
}

void TunerWindow::initializeNotesStruct() {

    double currentPitch, previousPitch;

    notesList = new noteInfo* [octavesQuantity];
    for(int i = 0; i < octavesQuantity; i++) {
        notesList[i] = new noteInfo[notesQuantity];
    }

    for (int i = 0; i < octavesQuantity; i++) {
        for (int j = 0; j < notesQuantity; j++) {

            if(i == 0 && j == 0) {
                currentPitch = previousPitch = lowestNote;
            } else {
            currentPitch = previousPitch * 1.0595;

            notesList[i][j].pitch = currentPitch;
            notesList[i][j].name = noteNames.at(j);

            if(j == 1 || j == 3 ||
               j == 6 || j == 8 ||
               j == 10) {
                notesList[i][j].name += "#";
              }

            notesList[i][j].name += QString::number(i + 1);

            previousPitch = currentPitch;
            }

        }
    }
}

void TunerWindow::getCurrentPitch() {
    QString readLine;
    readLine = ui->enteredFrequency->text();
    currentFrequency = readLine.toDouble();
    calculateClosestNote();
}

void TunerWindow::calculateClosestNote() {

    double leftBorder, rightBorder;
    double maxRange = 60;

    for (int i = 0; i < octavesQuantity; i++) {
        for (int j = 0; j < notesQuantity; j++) {

         if(currentFrequency < (notesList[i][j].pitch + maxRange) &&
                 currentFrequency > (notesList[i][j].pitch - maxRange)) {

            if(i == 0 && j == 0) {
                leftBorder = 0;
                rightBorder = notesList[i][j+1].pitch - notesList[i][j].pitch;
            } else if(j == (notesQuantity - 1)) {
                leftBorder = notesList[i][j].pitch - notesList[i][j-1].pitch;
                rightBorder = notesList[i+1][0].pitch - notesList[i][j].pitch;
            } else {
                leftBorder = notesList[i][j].pitch - notesList[i][j-1].pitch;
                rightBorder = notesList[i][j+1].pitch - notesList[i][j].pitch;
            }

            for(int k = 3; k >= 0; k--) {
                if(currentFrequency < notesList[i][j].pitch &&
                        currentFrequency > (notesList[i][j].pitch - leftBorder/rangeDevider[k])) {
                    ui->closestNote->setText(notesList[i][j].name);
                    resetColorFlags();
                    colorFlags[k] = true;
                    return;
                } else if (currentFrequency > notesList[i][j].pitch &&
                           currentFrequency < (notesList[i][j].pitch + rightBorder/rangeDevider[k])) {
                    ui->closestNote->setText(notesList[i][j].name);
                    resetColorFlags();
                    colorFlags[indicationCircles - k - 1] = true;
                    return;
                }
            }
          }
        }
    }
}

void TunerWindow::paintEvent(QPaintEvent *event) {

   bool colorChanged = false;

    Q_UNUSED(event);

    QImage tmp(ui->indicationLabel->pixmap()->toImage());
    QPainter painter(&tmp);
    QPen paintpen(Qt::black);
    paintpen.setWidth(10);
    painter.setPen(paintpen);

    for(int i = 0; i < indicationCircles; i++) {
        if(colorFlags[i]) {
            paintpen.setColor(circleColors[i]);
            painter.setPen(paintpen);
            colorChanged = true;
        }
        painter.drawEllipse((10 + i*28), 25, 10, 10);
        if (colorChanged) {
            paintpen.setColor("black");
            painter.setPen(paintpen);
            colorChanged = false;
        }
    }

    ui->indicationLabel->setPixmap(QPixmap::fromImage(tmp));
}

void TunerWindow::resetColorFlags() {
    for(int i = 0; i < indicationCircles; i++) {
        colorFlags[i] = false;
    }
}

void TunerWindow::resetIndicationField() {
    QPixmap picture("E:\\QtProjects\\MrChord\\Pictures\\indicationField.PNG");
    ui->indicationLabel->setPixmap(picture);
}

TunerWindow::~TunerWindow() {
    delete ui;
}

void TunerWindow::closeEvent(QCloseEvent *event) {
    event->accept();
    delete ui;
}
