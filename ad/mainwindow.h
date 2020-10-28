#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QKeyEvent>

class MainWindowPrivate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void getADInputDatas();
    void drawWave();

signals:
    void newData(const int data);
    void newDatas(const int *datas, int datasCount);

    void newPoint(const QPoint &points);
    void newPoints(const QPoint *points, int pointsCount);
    void newPoints(const QPolygon &points);
    void watchSizeChange(int size);

    void refreshCTRlcd();
    void stopRefreshCTRlcd();

public:
    void setPortFileName(QString name);
    void startReadData();
protected:
    void keyPressEvent ( QKeyEvent * e );

private:
    MainWindowPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(MainWindow);
};

#endif // MAINWINDOW_H
