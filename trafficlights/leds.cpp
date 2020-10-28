#include "leds.h"

#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>

class QLeds::Private
{
  public:
    Private()
      : darkFactor( 300 ),
        state( On ), look( Raised ), shape( Circular )
    {
    }

    int darkFactor;
    QColor color;
    State state;
    Look look;
    Shape shape;

    QPixmap cachedPixmap[2]; // for both states
    QStyle::ControlElement ce_indicatorLedCircular;
    QStyle::ControlElement ce_indicatorLedRectangular;
};



QLeds::QLeds( QWidget *parent )
  : QWidget( parent ),
    d( new Private )
{
  setColor( Qt::green );
}


QLeds::QLeds( const QColor& color, QWidget *parent )
  : QWidget( parent ),
    d( new Private )
{
  setColor( color );
}

QLeds::QLeds( const QColor& color, State state, Look look, Shape shape,
            QWidget *parent )
  : QWidget( parent ),
    d( new Private )
{
  d->state = (state == Off ? Off : On);
  d->look = look;
  d->shape = shape;

  setColor( color );
}

QLeds::~QLeds()
{
  delete d;
}

void QLeds::paintEvent( QPaintEvent* )
{
    switch( d->shape ) {
      case Rectangular:
        switch ( d->look ) {
          case Sunken:
            paintRectFrame( false );
            break;
          case Raised:
            paintRectFrame( true );
            break;
          case Flat:
            paintRect();
            break;
        }
        break;
      case Circular:
        switch ( d->look ) {
          case Flat:
            paintFlat();
            break;
          case Raised:
            paintRaised();
            break;
          case Sunken:
            paintSunken();
            break;
        }
        break;
    }
}

int QLeds::ledWidth() const
{
  // Make sure the LED is round!
  int size = qMin(width(), height());

  // leave one pixel border
  size -= 2;

  return qMax(0, size);
}

bool QLeds::paintCachedPixmap()
{
    if (d->cachedPixmap[d->state].isNull()) {
        return false;
    }
    QPainter painter(this);
    painter.drawPixmap(1, 1, d->cachedPixmap[d->state]);
    return true;
}

void QLeds::paintFlat()
{
    paintLed(Circular, Flat);
}

void QLeds::paintRaised()
{
    paintLed(Circular, Raised);
}

void QLeds::paintSunken()
{
    paintLed(Circular, Sunken);
}

void QLeds::paintRect()
{
    paintLed(Rectangular, Flat);
}

void QLeds::paintRectFrame( bool raised )
{
    paintLed(Rectangular, raised ? Raised : Sunken);
}

QLeds::State QLeds::state() const
{
  return d->state;
}

QLeds::Shape QLeds::shape() const
{
  return d->shape;
}

QColor QLeds::color() const
{
  return d->color;
}

QLeds::Look QLeds::look() const
{
  return d->look;
}

void QLeds::setState( State state )
{
  if ( d->state == state)
    return;

  d->state = (state == Off ? Off : On);
  updateCachedPixmap();
}

void QLeds::setShape( Shape shape )
{
  if ( d->shape == shape )
    return;

  d->shape = shape;
  updateCachedPixmap();
}

void QLeds::setColor( const QColor &color )
{
  if ( d->color == color )
    return;

  d->color = color;
  updateCachedPixmap();
}

void QLeds::setDarkFactor( int darkFactor )
{
  if ( d->darkFactor == darkFactor )
    return;

  d->darkFactor = darkFactor;
  updateCachedPixmap();
}

int QLeds::darkFactor() const
{
  return d->darkFactor;
}

void QLeds::setLook( Look look )
{
  if ( d->look == look)
    return;

  d->look = look;
  updateCachedPixmap();
}

void QLeds::toggle()
{
  d->state = (d->state == On ? Off : On);
  updateCachedPixmap();
//  emit toggleEvent(this);
}

void QLeds::on()
{
  if(d->state == Off)
  {
     setState( On );
//     emit onEvent();
   }
}

void QLeds::off()
{
    if(d->state == On)
    {
        setState( Off );
//        emit offEvent();
     }
}

void QLeds::resizeEvent( QResizeEvent * )
{
    updateCachedPixmap();
}

void QLeds::mousePressEvent( QMouseEvent * )
{
     emit toggleEvent(this);
}

QSize QLeds::sizeHint() const
{
    QStyleOption option;
    option.initFrom(this);
    int iconSize = style()->pixelMetric(QStyle::PM_SmallIconSize, &option, this);
    return QSize( iconSize,  iconSize );
}

QSize QLeds::minimumSizeHint() const
{
  return QSize( 16, 16 );
}

void QLeds::updateCachedPixmap()
{
    d->cachedPixmap[Off] = QPixmap();
    d->cachedPixmap[On] = QPixmap();
    update();
}

void QLeds::paintLed(Shape shape, Look look)
{
    if (paintCachedPixmap()) {
        return;
    }

    QSize size(width() - 2, height() - 2);
    if (shape == Circular) {
        const int width = ledWidth();
        size = QSize(width, width);
    }
    QPointF center(size.width() / 2.0, size.height() / 2.0);
    const int smallestSize = qMin(size.width(), size.height());
    QPainter painter;

    QImage image(size, QImage::Format_ARGB32_Premultiplied);
    image.fill(0);

    QRadialGradient fillGradient(center, smallestSize / 2.0, QPointF(center.x(), size.height() / 3.0));
    const QColor fillColor = d->state != Off ? d->color : d->color.dark(d->darkFactor);
    fillGradient.setColorAt(0.0, fillColor.light(250));
    fillGradient.setColorAt(0.5, fillColor.light(130));
    fillGradient.setColorAt(1.0, fillColor);

    QConicalGradient borderGradient(center, look == Sunken ? 90 : -90);
    QColor borderColor = palette().color(QPalette::Dark);
    if (d->state == On) {
        QColor glowOverlay = fillColor;
        glowOverlay.setAlpha(80);
        //borderColor = KColorUtils::overlayColors(borderColor, glowOverlay);
    }
    borderGradient.setColorAt(0.2, borderColor);
    borderGradient.setColorAt(0.5, palette().color(QPalette::Light));
    borderGradient.setColorAt(0.8, borderColor);

    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(look == Flat ? QBrush(fillColor) : QBrush(fillGradient));
    const QBrush penBrush = (look == Flat) ? QBrush(borderColor) : QBrush(borderGradient);
    const qreal penWidth = smallestSize / 8.0;
    painter.setPen(QPen(penBrush, penWidth));
    QRectF r(penWidth / 2.0, penWidth / 2.0, size.width() - penWidth, size.height() - penWidth);
    if (shape == Rectangular) {
        painter.drawRect(r);
    } else {
        painter.drawEllipse(r);
    }
    painter.end();

    d->cachedPixmap[d->state] = QPixmap::fromImage(image);
    painter.begin(this);
    painter.drawPixmap(1, 1, d->cachedPixmap[d->state]);
    painter.end();
}
