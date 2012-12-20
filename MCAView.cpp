
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

  inPress = false;
  nx = ny = sx = sy = ex = ey = 0;
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

  cc.SetView( LM, TM, LM+HW, TM+VW );
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
  cc.SetWindow( 0, 0, MCALen, max );

  int tex;
  if ( inPress ) {
    tex = nx;
  } else {
    tex = ex;
  }
  int roisx = sx;
  int roiex = tex;
  int wroisx, wroiex;
  if ( roisx > roiex ) {
    int tmp = roisx;
    roisx = roiex;
    roiex = tmp;
  }
  wroisx = cc.r2wxLimit( roisx );
  wroiex = cc.r2wxLimit( roiex );
  if ( inPress ) {
    emit newROI( wroisx, wroiex );
  }

  int sum = 0;
  for ( int i = 0; i < MCALen; i++ ) {
    if (( i >= wroisx )&&( i <= wroiex )) {
      p->setPen( Green );
      sum += MCA[i];
    } else {
      p->setPen( Blue );
    }
    if ( dispLog ) {
      if ( MCA[i] > 0 )
	p->drawLine( cc.w2rx( i ), cc.w2ry( log( MCA[i] ) ), cc.w2rx( i ), cc.w2ry( 0 ) );
    } else {
      p->drawLine( cc.w2rx( i ), cc.w2ry( MCA[i] ), cc.w2rx( i ), cc.w2ry( 0 ) );
    }
  }
  p->setPen( Black );
  p->drawRect( LM, TM, HW, VW );

  if ( ( nx > LM )&&( nx < LM+HW ) ) {
    p->setPen( Red );
    p->drawLine( nx, TM, nx, TM+VW );
  }
  int curp;
  emit CurrentValues( MCA[ curp = (int)cc.r2wxLimit( nx ) ], sum );

  QFont f;
  QRectF rec;
  p->setPen( Black );

  int LINE = 0;
  if ( MCACh >= 0 ) {
    rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
    cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 "MCA Ch. : " );
    rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
    cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
		 QString::number( MCACh ) );
    LINE++;
  }

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       "Cursor point : " );
  rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( curp ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       "Val. at Cursor : " );
  rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( MCA[ curp ] ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       "ROI start : " );
  rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( wroisx ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       "ROI end : " );
  rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( wroiex ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       "ROI value : " );
  rec.setRect( dLM*7, TM + dVW2 * LINE, dLM * 2, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( sum ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       "Real Time : " );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( realTime, 'f', 2 ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       "Live Time : " );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( liveTime, 'f', 2 ) );
  LINE++;

  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       "Dead Time : " );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( ( realTime != 0 ) ?
				( realTime - liveTime )/realTime * 100: 0, 'f', 2 ) );
  LINE++;
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

