#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
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
    void on_pushButton_run_clicked();
    void on_radioButton_clock_clicked();
    void on_radioButton_conter_clicked();
    void on_horizontalSlider_speed_valueChanged( int value);

signals:
    void clockwise();
    void counterclockwise();
    void speedChange(int value);
public:
    void init();

private:
    bool m_bClockwise;
    bool m_bEnable;
    int  m_nSpeed;

private:
    Ui::Dialog *ui;
    CTR_lcd     *lcd;
};

#endif // DIALOG_H
