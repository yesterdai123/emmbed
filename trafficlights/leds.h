#ifndef QLeds_H
#define QLeds_H

#include <QWidget>

class QColor;

class QLeds : public QWidget
{
public:
    QLeds();

    Q_OBJECT
    Q_ENUMS( State Shape Look )
    Q_PROPERTY( State state READ state WRITE setState )
    Q_PROPERTY( Shape shape READ shape WRITE setShape )
    Q_PROPERTY( Look look READ look WRITE setLook )
    Q_PROPERTY( QColor color READ color WRITE setColor )
    Q_PROPERTY( int darkFactor READ darkFactor WRITE setDarkFactor )

  public:

    enum State { Off, On };

    enum Shape { Rectangular, Circular };

    enum Look  { Flat, Raised, Sunken };

    explicit QLeds( QWidget *parent = 0 );

    explicit QLeds( const QColor &color, QWidget *parent = 0 );

    QLeds( const QColor& color, QLeds::State state, QLeds::Look look, QLeds::Shape shape,
          QWidget *parent = 0 );

    ~QLeds();

    QColor color() const;

    State state() const;

    Look look() const;

    Shape shape() const;

    int darkFactor() const;

    void setColor( const QColor& color );

    void setState( State state );

    void setLook( Look look );

    void setShape( Shape shape );

    void setDarkFactor( int darkFactor );

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

  public Q_SLOTS:

    void toggle();

    void on();

    void off();

   Q_SIGNALS:

    void toggleEvent(QLeds *);

    void onEvent();

    void offEvent();

  protected:
    virtual int ledWidth() const;

    virtual void paintFlat();

    virtual void paintRaised();

    virtual void paintSunken();

    virtual void paintRect();

    virtual void paintRectFrame( bool raised );

    void paintEvent( QPaintEvent* );
    void resizeEvent( QResizeEvent* );
    void mousePressEvent(QMouseEvent *);

    bool paintCachedPixmap();

    void updateCachedPixmap();

    void paintLed(Shape shape, Look look);

  private:
    class Private;
    Private * const d;

};

#endif // QLeds_H
