#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QKeyEvent>

#include "ctr_lcd.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT
public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

    enum E_DRAW_GRAPH
    {
        line = 0,
        rect,
        circle,
        random
    };

public slots:
    void on_pushButton_line_clicked();
    void on_pushButton_rect_clicked();
    void on_pushButton_circle_clicked();
    void drawGraph(enum E_DRAW_GRAPH graph);
    void drawGraph();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent ( QKeyEvent * e );

private:
    Ui::Dialog       *ui;
    CTR_lcd          *lcd;
    QTimer           *drawTimer;
    enum E_DRAW_GRAPH curGraph;
};

#endif // DIALOG_H
