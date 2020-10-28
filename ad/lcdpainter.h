#ifndef LCDPAINTER_H
#define LCDPAINTER_H

#include <QObject>
#include <QPoint>
#include <QPolygon>

#include "ctr_lcd.h"

class LCDPainter : public QObject
{
Q_OBJECT

public:
    LCDPainter();
    ~LCDPainter();

public slots:
    void drawWave();
    void addData(const int data);
    void addDatas(const int *points, int datasCount);
    void setPointsSize(int size);

public:
    void setLcd(CTR_lcd *lcd);

private:
    void checkDataValid();
    void addPoint(const QPoint &point);

private:
    QPolygon         points;
    int              pointsSize;
    CTR_lcd         *lcd;
};

#endif // LCDPAINTER_H
