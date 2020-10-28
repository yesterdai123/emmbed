#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "leds.h"
#include <QtGui>
#include "ctr_lcd.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT
public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent ( QKeyEvent * e );

public slots:
    void trafficled_light_clicked(QLeds *);
    void on_pushButton_run_clicked();
    void testLights();
Q_SIGNALS:
    void toggleLeds(QLeds *);

public:
    void addLedsMap(QLeds *, QString);

private:
    Ui::Dialog *ui;
private:
    bool isRunning;
    int  indexToggle;
    QTimer *timer;
    CTR_lcd     *lcd;
};

#endif // DIALOG_H
