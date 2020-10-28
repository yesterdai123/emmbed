#include "dialog.h"
#include "ui_dialog.h"
#include <QtGui>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    system("echo 1 > /sys/devices/platform/davinci_ctr.2/enable");

    ui->setupUi(this);

    lcd = new CTR_lcd();
    if(lcd)
        lcd->setShowBitsFile(QString::fromUtf8("/home/root/ctr/data/333.bin"));

}

Dialog::~Dialog()
{
    if(lcd)
        delete lcd;

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
    QToolButton *toolButtons[9];

    toolButtons[0] = ui->toolButton_1;
    toolButtons[1] = ui->toolButton_2;
    toolButtons[2] = ui->toolButton_3;
    toolButtons[3] = ui->toolButton_4;
    toolButtons[4] = ui->toolButton_5;
    toolButtons[5] = ui->toolButton_6;
    toolButtons[6] = ui->toolButton_7;
    toolButtons[7] = ui->toolButton_8;
    toolButtons[8] = ui->toolButton_9;

    toolButtons[e->text().toInt() - 1]->setFocus(Qt::MouseFocusReason);
    toolButtons[e->text().toInt() - 1]->setDown(true);
    toolButtons[e->text().toInt() - 1]->setIconSize(QSize(96, 96));
    if(lcd)
    {
        lcd->setShowBitsFile(QString::fromUtf8("/home/root/ctr/data/key-%1.bin").arg(e->text()));
    }
}

void Dialog::keyReleaseEvent ( QKeyEvent * e )
{
    QToolButton *toolButtons[9];

    toolButtons[0] = ui->toolButton_1;
    toolButtons[1] = ui->toolButton_2;
    toolButtons[2] = ui->toolButton_3;
    toolButtons[3] = ui->toolButton_4;
    toolButtons[4] = ui->toolButton_5;
    toolButtons[5] = ui->toolButton_6;
    toolButtons[6] = ui->toolButton_7;
    toolButtons[7] = ui->toolButton_8;
    toolButtons[8] = ui->toolButton_9;

    toolButtons[e->text().toInt() - 1]->setDown(false);
    toolButtons[e->text().toInt() - 1]->setIconSize(QSize(128, 64));
}

void Dialog::intKeypadMap(QString fileName)
{
    QToolButton *toolButtons[9];

    toolButtons[0] = ui->toolButton_1;
    toolButtons[1] = ui->toolButton_2;
    toolButtons[2] = ui->toolButton_3;
    toolButtons[3] = ui->toolButton_4;
    toolButtons[4] = ui->toolButton_5;
    toolButtons[5] = ui->toolButton_6;
    toolButtons[6] = ui->toolButton_7;
    toolButtons[7] = ui->toolButton_8;
    toolButtons[8] = ui->toolButton_9;

    QFile *initFile = new QFile();
    initFile->setFileName(fileName);
    if(!initFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        delete initFile;
        QMessageBox::warning(0,tr("warning"),tr("Open init file error!"),QMessageBox::Ok);
        return;
    }
    QString readString;
    QStringList list;

    int i = 0;
    while( i < 9)
    {
        toolButtons[i]->setIconSize(QSize(80, 80));;

        readString = initFile->readLine(0);

        if(readString.isEmpty()) break;

        list = readString.split(',');

        QIcon icon;
        if(list.size() > 0)
        {
            icon.addFile(list.at(0).trimmed(), QSize(), QIcon::Normal, QIcon::Off);
        }

        if(list.size() > 1)
        {
            icon.addFile(list.at(1).trimmed(), QSize(), QIcon::Active, QIcon::Off);
        }
        toolButtons[i]->setIcon(icon);

        i++;

    }

    initFile->close();

    delete initFile;
}
