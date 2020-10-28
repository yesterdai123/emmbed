#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    system("echo 1 > /sys/devices/platform/davinci_ctr.2/enable");

    ui->setupUi(this);

    lcd = new CTR_lcd();
    if(lcd)
        lcd->clearBuffer();

    curGraph = random;
    qsrand(0);

    drawTimer = new QTimer();
    connect(drawTimer, SIGNAL(timeout()), this, SLOT(drawGraph()));
    drawTimer->start(1000);
}

Dialog::~Dialog()
{

    if(drawTimer)
        delete drawTimer;

    if(lcd)
    {
        delete lcd;
    }

    delete ui;

    system("echo 0 > /sys/devices/platform/davinci_ctr.2/enable");
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dialog::keyPressEvent ( QKeyEvent * e )
{
    switch(e->key())
    {
     case Qt::Key_9:
         QApplication::quit();
        break;

    }
}

void Dialog::on_pushButton_line_clicked()
{
    if(drawTimer)
    {
        drawTimer->stop();
        if(lcd)
            lcd->clearBuffer();
        curGraph = line;
        drawTimer->start();
    }
}

void Dialog::on_pushButton_rect_clicked()
{
    if(drawTimer)
    {
        drawTimer->stop();
        if(lcd)
            lcd->clearBuffer();
        curGraph = rect;
        drawTimer->start();
    }
}

void Dialog::on_pushButton_circle_clicked()
{
    if(drawTimer)
    {
        drawTimer->stop();
        if(lcd)
            lcd->clearBuffer();
        curGraph = circle;
        drawTimer->start();
    }
}

void Dialog::drawGraph()
{
    drawGraph(curGraph);
}

void Dialog::drawGraph(enum E_DRAW_GRAPH graph)
{
    if(!lcd) return;

    if(random == graph)
        graph = (enum E_DRAW_GRAPH)(qrand() % ((int)random));

    switch(graph)
    {
    case line:
      {
          int x0, y0, x1, y1;

          x0 = qrand() % lcd->width;
          y0 = qrand() % lcd->height;
          x1 = qrand() % lcd->width;
          y1 = qrand() % lcd->height;

          lcd->LCDDrawLine(x0, y0, x1, y1, 1);
          lcd->flush();
      }
      break;
    case rect:
      {
          int x0, y0, x1, y1;

          x0 = qrand() % lcd->width;
          y0 = qrand() % lcd->height;
          x1 = qrand() % lcd->width;
          y1 = qrand() % lcd->height;

          lcd->LCDDrawRect(x0, y0, x1, y1, 1);
          lcd->flush();
      }
      break;
    case circle:
      {
          int x, y, r;

          x = qrand() % lcd->width;
          y = qrand() % lcd->height;
          r = (qrand() % lcd->height)/2;

          lcd->LCDDrawCircle(x, y, r, 0, 1);
          lcd->flush();
      }
      break;
    }
}
