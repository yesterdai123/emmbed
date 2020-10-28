#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <QTimeLine>
#include <QFile>
#include "ctr_lcd.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT
public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

    enum curve {
        square,
        sine,
        white,
        none
    };

public slots:
    void on_toolButton_square_clicked();
    void on_toolButton_sine_clicked();
    void on_toolButton_white_clicked();
    void valueChange(qreal value);
    void frameChanged ( int frame );

signals:
    void squareSinal();
    void sineSinal();
    void whiteSinal();

public:
    void setPortFileName(QString name);
    void outData(int data);

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent ( QKeyEvent * e );

private:
    Ui::Dialog *ui;
    QString portName;
    curve flag;
    QTimeLine  *timeLine;

    CTR_lcd     *lcd;
};

#endif // DIALOG_H
