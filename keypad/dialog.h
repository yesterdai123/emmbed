#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
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
    void keyReleaseEvent ( QKeyEvent * e );

public:
void intKeypadMap(QString fileName);

private:
    Ui::Dialog *ui;
    CTR_lcd *lcd;
};

#endif // DIALOG_H
