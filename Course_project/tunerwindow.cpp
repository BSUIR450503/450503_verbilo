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
            currentPitch = calculatePitch(previousPitch);

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

double TunerWindow::calculatePitch(double previousPitch) {
    return previousPitch * 1.0595;
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
    bool positionFound = false;

    for (int i = 0; i < octavesQuantity; i++) {
        for (int j = 0; j < notesQuantity; j++) {

         if(currentFrequency < (notesList[i][j].pitch + maxRange) && currentFrequency > (notesList[i][j].pitch - maxRange)) {

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

            if(currentFrequency < notesList[i][j].pitch && currentFrequency > (notesList[i][j].pitch - leftBorder/16)) {
                ui->closestNote->setText(notesList[i][j].name);
                resetColorFlags();
                centralBlue = true;
                positionFound = true;
                break;
            } else if (currentFrequency > notesList[i][j].pitch && currentFrequency < (notesList[i][j].pitch + rightBorder/16)) {
                ui->closestNote->setText(notesList[i][j].name);
                resetColorFlags();
                centralBlue = true;
                positionFound = true;
                break;
            }

            if(currentFrequency < notesList[i][j].pitch && currentFrequency > (notesList[i][j].pitch - leftBorder/8)) {
                ui->closestNote->setText(notesList[i][j].name);
                resetColorFlags();
                leftYellow = true;
                positionFound = true;
                break;
            }

            if(currentFrequency > notesList[i][j].pitch && currentFrequency < (notesList[i][j].pitch + rightBorder/8)) {
                ui->closestNote->setText(notesList[i][j].name);
                resetColorFlags();
                rightYellow = true;
                positionFound = true;
                break;
            }

            if(currentFrequency < notesList[i][j].pitch && currentFrequency > (notesList[i][j].pitch - leftBorder/4)) {
                ui->closestNote->setText(notesList[i][j].name);
                resetColorFlags();
                leftOrange = true;
                positionFound = true;
                break;
            }

            if(currentFrequency > notesList[i][j].pitch && currentFrequency < (notesList[i][j].pitch + rightBorder/4)) {
                ui->closestNote->setText(notesList[i][j].name);
                resetColorFlags();
                rightOrange = true;
                positionFound = true;
                break;
            }


            if(currentFrequency < notesList[i][j].pitch && currentFrequency > (notesList[i][j].pitch - leftBorder/1.5)) {
                ui->closestNote->setText(notesList[i][j].name);
                resetColorFlags();
                leftRed = true;
                positionFound = true;
                break;
            }

            if(currentFrequency > notesList[i][j].pitch && currentFrequency < (notesList[i][j].pitch + rightBorder/1.5)) {
                ui->closestNote->setText(notesList[i][j].name);
                resetColorFlags();
                rightRed = true;
                positionFound = true;
                break;
            }

          }
        }
        if(positionFound) {
            return;
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

    if(leftRed) {
        paintpen.setColor("red");
        painter.setPen(paintpen);
        colorChanged = true;
    }
    painter.drawEllipse(15, 25, 10, 10);
    if (colorChanged) {
        paintpen.setColor("black");
        painter.setPen(paintpen);
        colorChanged = false;
    }

    if(leftOrange) {
        paintpen.setColor("orange");
        painter.setPen(paintpen);
        colorChanged = true;
    }
    painter.drawEllipse(45, 25, 10, 10);
    if (colorChanged) {
        paintpen.setColor("black");
        painter.setPen(paintpen);
        colorChanged = false;
    }

    if(leftYellow) {
        paintpen.setColor("yellow");
        painter.setPen(paintpen);
        colorChanged = true;
    }
    painter.drawEllipse(70, 25, 10, 10);
    if (colorChanged) {
        paintpen.setColor("black");
        painter.setPen(paintpen);
        colorChanged = false;
    }

    if(centralBlue) {
        paintpen.setColor("blue");
        painter.setPen(paintpen);
        colorChanged = true;
    }
    painter.drawEllipse(95, 25, 10, 10);
    if (colorChanged) {
        paintpen.setColor("black");
        painter.setPen(paintpen);
        colorChanged = false;
    }

    if(rightYellow) {
        paintpen.setColor("yellow");
        painter.setPen(paintpen);
        colorChanged = true;
    }
    painter.drawEllipse(120, 25, 10, 10);
    if (colorChanged) {
        paintpen.setColor("black");
        painter.setPen(paintpen);
        colorChanged = false;
    }

    if(rightOrange) {
        paintpen.setColor("orange");
        painter.setPen(paintpen);
        colorChanged = true;
    }
    painter.drawEllipse(145, 25, 10, 10);
    if (colorChanged) {
        paintpen.setColor("black");
        painter.setPen(paintpen);
        colorChanged = false;
    }

    if(rightRed) {
        paintpen.setColor("red");
        painter.setPen(paintpen);
        colorChanged = true;
    }
    painter.drawEllipse(175, 25, 10, 10);

    ui->indicationLabel->setPixmap(QPixmap::fromImage(tmp));
}

void TunerWindow::resetColorFlags() {
    centralBlue = leftRed = leftOrange = leftYellow =
    rightRed = rightOrange = rightYellow = false;
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
