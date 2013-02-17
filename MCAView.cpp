
#include <QtGui>

#include "XafsM.h"
#include "MCAView.h"

MCAView::MCAView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  MCA = NULL;
  MCALen = 0;
  MCACh = -1;
  realTime = 0;
  liveTime = 0;

  valid = false;
  dispLog = false;

  White = QColor( 255, 255, 255 );
  Red = QColor( 255, 0, 0 );
  Green = QColor( 0, 255, 0 );
  Blue = QColor( 0, 0, 255 );
  Black = QColor( 0, 0, 0 );
  Orange = QColor( 255, 200, 0 );

  setROIrequest = false;
  reqsx = reqex = 0;
}

MCAView::~MCAView( void )
{
  if ( MCA != NULL )
    delete MCA;
}

int *MCAView::setMCAdataPointer( int len )
{
  MCA = new int[ MCALen = len ];
  return MCA;
}

void MCAView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void MCAView::Draw( QPainter *p )
{
  if ( valid != true ) 
    return;

  int w = width();
  int h = height();

  int LM = w * 0.25;
  if ( LM > 180 ) LM = 180;
  int RM = w * 0.05;
  if ( RM > 40 ) RM = 40;
  int HW = w - LM - RM;
  int TM = h * 0.05;
  if ( TM > 40 ) TM = 40;
  int BM = h * 0.1;
  if ( BM > 80 ) BM = 80;
  int VW = h - TM - BM;
  double dLM = LM / 10;
  double dVW = VW / 20;     // 1行の高さ(文字の高さ)
  double dVW2 = dVW * 1.2;  // 行間

  cc.SetScreenCoord( LM, TM, LM+HW, TM+VW );
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
  cc.SetRealCoord( 0, 0, MCALen-1, max );

  if ( setROIrequest ) {
    setROIrequest = false;
    m.setSx( cc.r2sx( reqsx ) );
    m.setEx( cc.r2sx( reqex ) );
  }

  double tex;
  if ( m.inPress() ) {
    tex = m.x();
  } else {
    tex = m.ex();
  }
  double roisx = m.sx();
  double roiex = tex;
  int rroisx, rroiex;
  if ( roisx > roiex ) {
    int tmp = roisx;
    roisx = roiex;
    roiex = tmp;
  }
  rroisx = (int)( cc.s2rxLimit( roisx ) + 0.5 );
  rroiex = (int)( cc.s2rxLimit( roiex ) + 0.5 );
  if ( m.inPress() ) {
    emit newROI( rroisx, rroiex );
  }

  int sum = 0;
  for ( int i = 0; i < MCALen; i++ ) {
    if (( i >= rroisx )&&( i <= rroiex )) {
      p->setPen( Green );
      sum += MCA[i];
    } else {
      p->setPen( Blue );
    }
    if ( dispLog ) {
      if ( MCA[i] > 0 )
	p->drawLine( cc.r2sx( i ), cc.r2sy( log( MCA[i] ) ), cc.r2sx( i ), cc.r2sy( 0 ) );
    } else {
      p->drawLine( cc.r2sx( i ), cc.r2sy( MCA[i] ), cc.r2sx( i ), cc.r2sy( 0 ) );
    }
  }
  p->setPen( Black );
  p->drawRect( LM, TM, HW, VW );

  if ( ( m.x() > LM )&&( m.x() < LM+HW ) ) {
    p->setPen( Red );
    p->drawLine( m.x(), TM, m.x(), TM+VW );
  }
  int curp;
  emit CurrentValues( MCA[ curp = (int)cc.s2rxLimit( m.x() ) ], sum );

  if ( nearf ) {
    p->setPen( Orange );
    p->drawLine( nearX, TM, nearX, TM+VW );
  }

  QFont f;
  QRectF rec;
  p->setPen( Black );

  int LINE = 0;
  if ( MCACh >= 0 ) {
    rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
    cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 tr( "MCA Ch. : " ) );
    rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
    cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
		 QString::number( MCACh ) );
    LINE++;
  }

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Cursor point : " ) );
  rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( curp ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Val. at Cursor : " ) );
  rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( MCA[ curp ] ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "ROI start : " ) );
  rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( rroisx ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "ROI end : " ) );
  rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( rroiex ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "ROI value : " ) );
  rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( sum ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Real Time : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( realTime, 'f', 2 ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Live Time : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( liveTime, 'f', 2 ) );
  LINE++;

  double dt = ( realTime != 0 ) ? ( realTime - liveTime )/realTime * 100: 0;
  //　　 青   ->  緑   ->  黄   -> オレンジ -> 赤　　にするとしたら？
  //    0 0 1 -> 0 1 0 -> 1 1 0 ->    ->      1 0 0
  //     0 %      10%      20%        ->       100%
  int r, g, b;
  r = g = b = 0;
  if ( dt < 10 ) { r = 0; g = (int)( dt / 10 * 255 ); b = 255 - g; };
  if (( 10 <= dt )&&( dt < 20 )) { r = (int)( ( dt - 10 ) / 10 * 255 ); g = 255; b = 0; };
  if ( dt >= 20 ) { r = 255; g = 255 - (int)( ( dt - 20 ) / 80 * 255 ); b = 0; };
  if ( r < 0 ) r = 0;
  if ( g < 0 ) g = 0;
  if ( b < 0 ) b = 0;
  QColor DTC = QColor( r, g, b );
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Dead Time : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  p->setPen( DTC );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( dt, 'f', 2 ) );
  LINE++;
}

#define NEAR ( 10 )

void MCAView::mouseMoveEvent( QMouseEvent *e )
{
  m.Moved( e );
  nearf = false;
  if ( !m.inPress() ) {
    int dsx = abs( e->x() - m.sx() );
    int dex = abs( e->x() - m.ex() );

    if ( ( dsx < NEAR ) || ( dex < NEAR ) ) {
      if ( dsx < dex ) {
	nearX = m.sx();
      } else {
	nearX = m.ex();
      }
      nearf = true;
    }
  }
  update();
}

void MCAView::mousePressEvent( QMouseEvent *e )
{
  int dsx = abs( e->x() - m.sx() );
  int dex = abs( e->x() - m.ex() );
  int setX;
  if ( ( dsx < NEAR ) || ( dex < NEAR ) ) {
    if ( dsx < dex ) {
      setX = m.ex();
    } else {
      setX = m.sx();
    }
  } else {
    setX = e->x();
  }
  m.Pressed( e );
  m.setSx( setX );
  update();
}

void MCAView::mouseReleaseEvent( QMouseEvent *e )
{
  m.Released( e );
  update();
}

void MCAView::mouseDoubleClickEvent( QMouseEvent * ) {}

void MCAView::setROI( int s, int e )
{
  setROIrequest = true;
  reqsx = s;
  reqex = e;
}
