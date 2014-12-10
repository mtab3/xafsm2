
#include <QStylePainter>

#include "cBar.h"

cBar::cBar( QWidget *p ) : QFrame( p )
{
  setupUi( this );
  initColor();
  zmax = 1;
  zmin = 0;
  autoScale = true;
  catching = CNONE;
}

void cBar::initColor( void )
{
  White = QColor( 255, 255, 255 );
  Black = QColor( 0, 0, 0 );
  LimitC = QColor( 150, 255, 150 );
  
  for ( int i = 0; i < 256; i++ )
    cbar << new QColor( i, 0, 0 );
  for ( int i = 0; i < 256; i++ )
    cbar << new QColor( 255, i, 0 );
  for ( int i = 0; i < 256; i++ )
    cbar << new QColor( 255, 255, i );

  colors = cbar.count();
  cmin = 0;
  cmax = colors;
  rmin = cmin;
  rmax = cmax;
}

void cBar::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void cBar::Draw( QPainter *p )
{
  int h = height();
  int w = width();
  
  cc.SetScreenCoord( 0, 0, w, h );
  cc.SetRealCoord( 0, rmin, 1, rmax );
  //  p->fillRect( 0, 0, w, h, QColor( 255, 255, 255 ) );

  int ph = cc.r2sdy( 1 )+1;
  if ( ph < 1 ) ph = 1;
  p->fillRect( 0, cc.r2sy( cmin ), w, cc.r2sy( rmin ) - cc.r2sy( cmin ), Black );
  for ( int i = 0; i < colors; i++ ) {
    p->fillRect( 0, cc.r2sy( i ), w, ph, *cbar[i] );
  }
  p->fillRect( 0, cc.r2sy( rmax ),
	       w, cc.r2sy( cmax ) - cc.r2sy( rmax ), White );
  p->setPen( LimitC );
  cmaxShowP = cc.r2sy( cmax );
  cminShowP = cc.r2sy( cmin );
  if ( cmaxShowP >= h ) cmaxShowP = h - 1;
  if ( cmaxShowP < 0 ) cmaxShowP = 0;
  if ( cminShowP >= h ) cminShowP = h - 1;
  if ( cminShowP < 0 ) cminShowP = 0;
  p->drawLine( 0, cmaxShowP, w, cmaxShowP );
  p->drawLine( 0, cminShowP, w, cminShowP );
}

void cBar::mouseMoveEvent( QMouseEvent *e )
{
  m.Moved( e );

  if ( m.inPress() ) {
    int h = height();
    int ncmaxs = cc.r2sy( cmax );
    int ncmins = cc.r2sy( cmin );
    switch ( catching ) {
    case CMAX:
      ncmaxs = m.y();
      break;
    case CMIN:
      ncmins = m.y();
      break;
    default:
      break;
    }
    if ( ncmaxs != ncmins ) {
      rmax = (double)( cmax - cmin ) / ( ncmaxs - ncmins ) * ( 0 - ncmins ) + cmin;
      rmin = (double)( cmax - cmin ) / ( ncmaxs - ncmins ) * ( h - ncmins ) + cmin;
      emit newScale();
    }
  }
  
  update();
}

void cBar::mousePressEvent( QMouseEvent *e )
{
  m.Pressed( e );
  
  if ( abs( m.sy() - cc.r2sy( cmax ) ) < 10 )
    catching = CMAX;
  if ( abs( m.sy() - cc.r2sy( cmin ) ) < 10 )
    catching = CMIN;

  update();
}

void cBar::mouseReleaseEvent( QMouseEvent *e )
{
  m.Released( e );
  update();
}

QColor *cBar::c( double z )
{
  int cnum = ( rmax - rmin ) / ( zmax - zmin ) * ( z - zmin ) + rmin;
  if ( cnum < 0 ) cnum = 0;
  if ( cnum >= colors ) cnum = colors - 1;
  return cbar[ cnum ];
}

void cBar::newAutoZmax( double max )
{
  if ( autoScale ) {
    if ( zmax != max ) {
      zmax = max;
      showZZ();
      emit newScale();
    }
  }
}

void cBar::newAutoZmin( double min )
{
  if ( autoScale ) {
    if ( zmin != min ) {
      zmin = min;
      showZZ();
      emit newScale();
    }
  }
}

void cBar::newInputZmax( double max ) {
  if ( ! autoScale ) {
    if ( zmax != max ) {
      zmax = max;
      showZZ();
      emit newScale();
    }
  } else {
    showZZ();
  }
}

void cBar::newInputZmin( double min ) {
  if ( ! autoScale ) {
    if ( zmin != min ) {
      zmin = min;
      showZZ();
      emit newScale();
    }
  } else {
    showZZ();
  }
}

void cBar::showZZ( void )
{
  if ( zmax < zmin ) {
    double tmp = zmax;
    zmax = zmin;
    zmin = tmp;
    emit newScale();
  }
  emit newZZ( QString::number( zmax ), QString::number( zmin ) );
}

