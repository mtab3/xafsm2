
#include <QtGui>

#include "XafsM.h"
#include "MCAView.h"

MCAView::MCAView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  MCA = NULL;
  MCALen = 0;
  valid = false;
  dispLog = false;

  White = QColor( 255, 255, 255 );
  Red = QColor( 255, 0, 0 );
  Green = QColor( 0, 255, 0 );
  Blue = QColor( 0, 0, 255 );
  Black = QColor( 0, 0, 0 );

  inPress = false;
  nx = ny = sx = sy = ex = ey = 0;
}

void MCAView::setMCAdataPointer( int *mca, int len )
{
  MCA = mca;
  MCALen = len;
}

void MCAView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void MCAView::Draw( QPainter *p )
{
  int w = width();
  int h = height();

  int LM = w * 0.15;
  int RM = w * 0.05;
  int HW = w - LM - RM;
  int TM = h * 0.05;
  int BM = h * 0.1;
  int VW = h - TM - BM;

  SetView( LM, TM, LM+HW, TM+VW );
  p->fillRect( 0, 0, w, h, White );

  double max = 0;
  for ( int i = 0; i < MCALen; i++ ) {
    if ( dispLog ) { 
      if ( MCA[i] > 0 ) {
	if ( log( MCA[i] ) > max ) {
	  max = log( MCA[i] );
	}
      }
    } else {
      if ( MCA[i] > max ) {
	max = MCA[i];
      }
    }
  }
  SetWindow( 0, 0, MCALen, max );

  int tex;
  if ( inPress ) {
    tex = nx;
  } else {
    tex = ex;
  }
  int roisx = sx;
  int roiex = tex;
  if ( roisx > roiex ) {
    int tmp = roisx;
    roisx = roiex;
    roiex = tmp;
  }
  if ( inPress ) {
    emit newROI( r2wx( roisx ), r2wx( roiex ) );
  }

  int sum = 0;
  for ( int i = 0; i < MCALen; i++ ) {
    if (( w2rx( i ) >= roisx )&&( w2rx( i ) <= roiex )) {
      p->setPen( Green );
      sum += MCA[i];
    } else {
      p->setPen( Blue );
    }
    if ( dispLog ) {
      if ( MCA[i] > 0 )
	p->drawLine( w2rx( i ), w2ry( log( MCA[i] ) ), w2rx( i ), w2ry( 0 ) );
    } else {
      p->drawLine( w2rx( i ), w2ry( MCA[i] ), w2rx( i ), w2ry( 0 ) );
    }
  }
  p->setPen( Black );
  p->drawRect( LM, TM, HW, VW );

  if ( ( nx > LM )&&( nx < LM+HW ) ) {
    p->setPen( Red );
    p->drawLine( nx, TM, nx, TM+VW );
  }
  emit CurrentValues( MCA[ (int)r2wx( nx ) ], sum );
}

void MCAView::mouseMoveEvent( QMouseEvent *e )
{
  nx = e->x();
  ny = e->y();

  update();
}

void MCAView::mousePressEvent( QMouseEvent *e )
{
  sx = e->x();
  sy = e->y();
  inPress = true;

  update();
}

void MCAView::mouseReleaseEvent( QMouseEvent *e )
{
  ex = e->x();
  ey = e->y();
  inPress = false;

  update();
}

void MCAView::mouseDoubleClickEvent( QMouseEvent * ) {}


/**********************************************************************/

void MCAView::SetView( int x1, int y1, int x2, int y2 )
{
  minx = x1;
  maxx = x2;
  miny = y1;
  maxy = y2;
}

void MCAView::SetWindow( double x1, double y1, double x2, double y2 )
{
  wminx = x1;
  wmaxx = x2;
  wminy = y1;
  wmaxy = y2;
}

int MCAView::w2rx( double x )
{
  return ( maxx - minx ) / ( wmaxx - wminx ) * ( x - wminx ) + minx; 
}

int MCAView::w2ry( double y )
{
  return maxy - ( maxy - miny ) / ( wmaxy - wminy ) * ( y - wminy );
}

int MCAView::w2rdx( double x )
{
  return ( maxx - minx ) / ( wmaxx - wminx ) * x;
}

int MCAView::w2rdy( double y )
{
  return ( maxy - miny ) / ( wmaxy - wminy ) * y;
}

double MCAView::r2wx( int x )
{
  return ( wmaxx - wminx ) / ( maxx - minx ) * ( x - minx ) + wminx;
}

double MCAView::r2wy( int y )
{
  return ( wmaxy - wminy ) / ( maxy - miny ) * ( maxy - y ) + wminy;
}

double MCAView::r2wdx( int x )
{
  return ( wmaxx - wminx ) / ( maxx - minx ) * x;
}

double MCAView::r2wdy( int y )
{
  return ( wmaxy - wminy ) / ( maxy - miny ) * y;
}

