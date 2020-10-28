#ifndef PAINTWAVEFORM_H
#define PAINTWAVEFORM_H

#include <QWidget>

class QPaintWaveformPrivate;
class QPaintWaveform : public QWidget
{
Q_OBJECT
public:
    explicit QPaintWaveform(QWidget *parent = 0);
    ~QPaintWaveform();
signals:

public slots:
    void addData(const int data);
    void addDatas(const int *data, int datasCount);

    void addPoint(const QPoint &points);
    void addPoints(const QPoint *points, int pointsCount);
    void addPoints(const QPolygon &points);
    void setPointsSize(int size);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPaintWaveformPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QPaintWaveform);

private:
    void checkDataValid();

};

#endif // PAINTWAVEFORM_H
