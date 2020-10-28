#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtGui>
#include "ctr_lcd.h"

namespace Ui {
    class Dialog;
}

#define MAX_PATCH_ENABLED 12

class Dialog : public QDialog {
    Q_OBJECT
public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

protected:
    void changeEvent(QEvent *e);

public slots:
    void on_pushButton_01_pressed();
    void on_pushButton_02_pressed();
    void on_pushButton_03_pressed();
    void on_pushButton_04_pressed();
    void on_pushButton_05_pressed();
    void on_pushButton_06_pressed();
    void on_pushButton_07_pressed();
    void on_pushButton_08_pressed();
    void on_pushButton_09_pressed();
    void on_pushButton_10_pressed();
    void on_pushButton_11_pressed();
    void on_pushButton_12_pressed();

    void on_pushButton_01_released();
    void on_pushButton_02_released();
    void on_pushButton_03_released();
    void on_pushButton_04_released();
    void on_pushButton_05_released();
    void on_pushButton_06_released();
    void on_pushButton_07_released();
    void on_pushButton_08_released();
    void on_pushButton_09_released();
    void on_pushButton_10_released();
    void on_pushButton_11_released();
    void on_pushButton_12_released();

    void soundTimeOut();

private:
    int m_nfrequency[MAX_PATCH_ENABLED];
public:
    void intKeypadMap(QString fileName);
    void sound(int pitch);

private:
    Ui::Dialog *ui;
    bool isSounding;
    bool isPlaying;
    QTimer *soundDelayTimer;
    CTR_lcd     *lcd;
    unsigned int curMusic;
    unsigned int nMusicNumber;
};

#endif // DIALOG_H
