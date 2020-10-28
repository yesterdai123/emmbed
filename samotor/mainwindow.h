#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "ctr_lcd.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent ( QKeyEvent * e );

public slots:
    void on_dialSACtrlFine_sliderMoved ( int value );
    void on_dialSACtrlFine_sliderReleased();
    void on_dialSACtrlFine_sliderPressed();
    void on_toolButton_run_clicked();
    void on_radioButton_clock_clicked();
    void on_radioButton_counterclock_clicked();
    void on_horizontalSlider_speed_valueChanged( int value);

signals:
    void clockwise();
    void counterclockwise();
    void speedChange(int value);

public:
    void init();

private:
    Ui::MainWindow *ui;
    bool m_bClockwise;
    int m_nSpeed;
    bool m_bEnable;
    int m_nPreValue;
    CTR_lcd     *lcd;
};

#endif // MAINWINDOW_H
