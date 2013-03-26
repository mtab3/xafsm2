#include <QtGui>

#include "XafsM.h"
#include "XYView.h"

XYView::XYView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  setToolTip( tr( "By clicking the 'A. Scale' (Auto Scale) button at left-bottom corner, "
		  "the auto-scale mode is toggled on and off." ) );

  autoScale = true;
  AreaSelecting = false;
  SelLR[ LEFT_AX ] = 0;
  SelLR[ RIGHT_AX ] = 1;

  Clear();
  valid = false;

  for ( int i = 0; i < MAXLINES; i++ ) {
    SetLR( i, LEFT_AX );
    SetScaleType( i, FULLSCALE );
  }

  lines = 0;
  cc.SetRealCoord( 0, 0, 1, 1 );
  bgColor = QColor( 255, 255, 255 );
  BLACK = QColor( 0, 0, 0 );
  MCLineC = QColor( 210, 180, 0 );    // mouse cursor line color
  ASelC = QColor( 0, 255, 120 );      // Area Select Color

  upp = 1;
  center = 0;

  LC << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0,   0, 255 ) << QColor(   0, 255,   0 )
     << QColor( 200, 200,   0 ) << QColor( 240,   0, 240 ) << QColor(   0, 240, 240 )
     << QColor( 160,   0,   0 ) << QColor(   0, 160,   0 ) << QColor(   0,   0, 160 )
     << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0, 255,   0 ) << QColor(   0,   0, 255 )
     << QColor( 200, 200,   0 ) << QColor( 240,   0, 240 ) << QColor(   0, 240, 240 )
     << QColor( 160,   0,   0 ) << QColor(   0, 160,   0 ) << QColor(   0,   0, 160 )
     << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0, 255,   0 ) << QColor(   0,   0, 255 )
     << QColor( 200, 200,   0 ) << QColor( 240,   0, 240 ) << QColor(   0, 240, 240 )
     << QColor( 160,   0,   0 ) << QColor(   0, 160,   0 ) << QColor(   0,   0, 160 );

  XShift = XShift0 = xshift = 0;
  for ( int i = 0; i < MAXLINES; i++ ) {
    dispf[ i ] = true;
    YShift[ i ] = YShift0[ i ] = yshift[ i ] = 0;
  }
}

void XYView::Clear( void )
{
  for ( int i = 0; i < MAXLINES; i++ ) {
    points[i] = 0;
    for ( int j = 0; j < MAXPOINTS; j++ ) {
      x[i][j] = y[i][j] = 0;
    }
    //    LineLR[i] = LEFT_AX;
    //    scaleType[i] = FULLSCALE;
  }
  lines = 0;
}

// l �Ԗڂ̃��C���ɐV�����f�[�^1�ǉ�
void XYView::NewPoint( int l, double xx, double yy )
{
  if ( l < MAXLINES ) {
    if ( points[l] < MAXPOINTS - 1 ) {
      x[l][ points[l] ] = xx;
      y[l][ points[l] ] = yy;
      points[l]++;
      if ( xx < origMinx ) origMinx = xx;
      if ( xx > origMaxx ) origMaxx = xx;
    }
    if ( ( l >= lines )&&( l < MAXLINES - 1 ) )
      lines = l + 1;
  }
}

int XYView::GetPoints( int l )
{
  if ( l >= lines )
    return -1;

  return points[ l ];
};

double XYView::GetX( int l, int p )
{
  double rv = 0;

  if ( l < lines )
    if ( p < points[l] )
      rv = x[l][p];
  
  return rv;
}

double XYView::GetY( int l, int p )
{
  double rv = 0;

  if ( l < lines )
    if ( p < points[l] )
      rv = y[l][p];

  return rv;
}

void XYView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void XYView::Draw( QPainter *p )
{
  if ( valid != true ) 
    return;

  if ( autoScale ) {
    cc.SetRealX( origMinx, origMaxx );
    cc.SetRealX0( origMinx, origMaxx );
  }

  QString buf, buf2;
  int RM, LM, TM, BM, HW, VH;
  QPen pen0, pen1;
  QFont F1;
  QRectF rec;
  double nowx;
  int nowxp;

  p->fillRect( 0, 0, width(), height(), bgColor );
  pen0.setWidth( 2 );
  pen0.setColor( QColor( 95, 95, 195 ) );
  p->setPen( pen0 );

  RM = width() * 0.10;
  LM = width() * 0.10;
  TM = height() * 0.05;
  BM = height() * 0.10;
  HW = width() - LM - RM;
  VH = height() - TM - BM;
  F1.setPointSizeF( ( LM*0.115 < BM*0.23 ) ? LM*0.115 : BM*0.23 );
  p->setFont( F1 );

  cc.SetScreenCoord( LM, TM, width()-RM, height()-BM );
  p->drawRect( LM, TM, width()-RM-LM, height()-BM-TM );

  int AlLC = Qt::AlignLeft | Qt::AlignVCenter;
  int AlRC = Qt::AlignRight | Qt::AlignVCenter;
  double sy, dy;

  int FindL[ 2 ] = { -1, -1 };
  double ND[ 2 ] = { 1e300, 1e300 };
  double x0, x1, y0, y1, hx, hy;
  double a1, a2;
  double d, b;

  for ( int l = 0; l < lines; l++ ) { // ��ɐ������`��
    if ( dispf[ l ] ) {
      if ( autoScale ) {
        UpDateYWindow( l, scaleType[ l ] );
        cc.SetRealY( miny[l], maxy[l] );
      } else {
        cc.SetRealY( miny[l] - YShift[l], maxy[l] - YShift[l] );
      }
      pen1.setColor( LC[ l ] );
      p->setPen( pen1 );
      nowx = cc.s2rx( m.x() );
      nowxp = 0;
      for ( int i = 0; i < points[l] - 1; i++ ) {
        p->drawLine( x0 = cc.r2sx( x[l][i] ), y0 = cc.r2sy( y[l][i] ),
                     x1 = cc.r2sx( x[l][i+1] ), y1 = cc.r2sy( y[l][i+1] ) );
        if (( x[l][i+1] >= nowx )&&( x[l][i] < nowx ))
          nowxp = i;
        if (( x[l][i+1] <= nowx )&&( x[l][i] > nowx ))
          nowxp = i;
	
	// �}�E�X������_�������(�������������̂ɉ��낵�������̑���T��)
	a1 = x1 - x0;
	a2 = y1 - y0;
	b = ( a1 * ( y0 - m.y() ) + a2 * ( m.x() - x0 ) ) / ( a1 * a1 + a2 * a2 );
	hx = m.x() - b * a2;
	hy = m.y() + b * a1;
	if ( hx < x0 ) { hx = x0; hy = y0; }  // ���������̊O�ɗL������
 if ( hx > x1 ) { hx = x1; hy = y1; }  // �����̒[�_�Œu��������
 // ���̑��܂ł̋������ŏ��ɂȂ����T��
 if ( ( d = ( hx - m.x() )*( hx - m.x() ) + ( hy - m.y() )*( hy - m.y() ) )
      < ND[ LineLR[l] ] ) {
   ND[ LineLR[l] ] = d;
   FindL[ LineLR[l] ] = l;
 }
      }
      SaveYatNowXp[l] = y[l][nowxp];
    }
    for ( int i = 0; i < 2; i++ ) {
      if ( ND[ i ] < 25 ) {
	SelLR[i] =  FindL[i];
      }
    }
  }
  // �������_�ɐ�������
  //  p->drawLine( m.x(), m.y(), hx, hy );
  
  if ( AreaSelecting ) {
    p->setPen( ASelC );
    p->drawLine( m.sx(), m.sy(), m.sx(), m.y() );
    p->drawLine( m.sx(), m.y(),  m.x(),  m.y() );
    p->drawLine( m.x(),  m.y(),  m.x(),  m.sy() );
    p->drawLine( m.x(),  m.sy(), m.sx(), m.sy() );
  }

  // ���΂��N���b�s���O ^^;;;
  p->fillRect( 0, 0, LM, height(), bgColor );
  p->fillRect( width()-RM, 0, RM, height(), bgColor );
  p->fillRect( 0, 0, width(), TM, bgColor );
  p->fillRect( 0, height()-BM, width(), BM, bgColor );
  // ���΂��N���b�s���O ^^;;;

  cc.ShowAScaleButton( p, autoScale, height() );

  double sx, dx;
  cc.calcScale( 10, cc.Rminx(), cc.Rmaxx(), &sx, &dx );
  int memc = 0;
  for ( double xx = sx; xx < cc.Rmaxx(); xx += dx ) {
    p->drawLine( cc.r2sx( xx ), TM, cc.r2sx( xx ), height()-BM );  // �c�̌r��
    rec = QRectF( cc.r2sx( xx )-40, height()-BM+5, 80, BM*0.3 ); // ����������
    if ( memc % (int)( 80 / cc.r2sdx( dx ) + 1 ) == 0 ) {
      cc.DrawText( p, rec, F1, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE,
		   QString::number( ( xx - center ) * upp ) );
    }
    memc++;
  }
  rec = QRectF( cc.r2sx( cc.Rmaxx() ), height()-BM+5, 80, BM*0.3 );   // X���̃��x��
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, XName );
  rec = QRectF( cc.r2sx( cc.Rmaxx() )+80, height()-BM+5, 80, BM*0.3 );   // X���̒P��
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, XUnitName );

  for ( int i = 0; i < 2; i++ ) {
    int l = SelLR[i];
    if ( autoScale ) {
      UpDateYWindow( l, scaleType[ l ] );
      cc.SetRealY( miny[l], maxy[l] );
    } else {
      cc.SetRealY( miny[l] - YShift[l], maxy[l] - YShift[l] );
    }
    sy = dy = 0;
    cc.calcScale( 5, cc.Rminy(), cc.Rmaxy(), &sy, &dy );
    pen1.setWidth( 1 );
    pen1.setColor( LC[ l ] );
    p->setPen( pen1 );
    
    for ( double yy = sy; yy < cc.Rmaxy(); yy += dy ) {
      buf.sprintf( "%7.5g", yy );
      if ( LineLR[l] == LEFT_AX ) {
	p->drawLine( LM, cc.r2sy( yy ), width()-RM, cc.r2sy( yy ) );   // ���̌r��
	rec = QRectF( LM * 0.05, cc.r2sy( yy )-BM*0.3/2, LM * 0.9, BM * 0.3 );
	cc.DrawText( p, rec, F1, AlRC, SCALESIZE, buf );
      } else {
	p->drawLine( LM, cc.r2sy( yy ), LM+3, cc.r2sy( yy ) );
	p->drawLine( width()-RM, cc.r2sy( yy ), width()-RM-3, cc.r2sy( yy ) );
	rec = QRectF( width()-RM*0.9, cc.r2sy( yy )-BM*0.3/2, 60, BM * 0.3 );
	cc.DrawText( p, rec, F1, AlLC, SCALESIZE, buf );
      }
    }
    if ( LineLR[l] == LEFT_AX ) {
      rec = QRectF( LM * 0.1, cc.r2sy( cc.Rmaxy() )-BM*0.35, 60, BM * 0.3 );
      cc.DrawText( p, rec, F1, AlRC, SCALESIZE, LeftName );
    } else {
      rec = QRectF( width()-RM*0.95, cc.r2sy( cc.Rmaxy() )-BM*0.35,
		    RM*0.9, BM * 0.3 );    // ���̃��x��
      cc.DrawText( p, rec, F1, AlLC, SCALESIZE, RightName );
    }    
    pen1.setColor( LC[ l ] );
    p->setPen( pen1 );
    rec = QRectF( LM * 1.2 + i * 2 * HW / 5.,
		  cc.r2sy( cc.Rmaxy() )-TM*0.9, HW / 5, TM * 0.8 );
    cc.DrawText( p, rec, F1, AlRC, SCALESIZE,
		 QString( "%1: %2" ).arg( l ).arg( LNames[l] ) );
    rec = QRectF( LM * 1.2 + ( i * 2 + 1 ) * HW / 5.,
		  cc.r2sy( cc.Rmaxy() )-TM*0.9, HW / 5, TM * 0.8 );
    cc.DrawText( p, rec, F1, AlRC, SCALESIZE,
		 QString( "%1" ).arg( SaveYatNowXp[l] ) );
  }

  if ( ( m.x() > LM ) && ( m.x() < width()-RM ) ) {
    p->setPen( MCLineC );
    p->drawLine( m.x(), TM, m.x(), height()-BM );
    p->setPen( BLACK );
    rec = QRectF( LM + HW, TM + VH + BM * 0.5, LM * 0.9, BM * 0.45 );
    cc.DrawText( p, rec, F1, AlLC, SCALESIZE,
		 QString( "%1" ).arg( ( cc.s2rx( m.x() ) - center ) * upp ) );
  }
}

void XYView::CheckASPush( void )
{
  if ( ( m.ex() > 5 )&&( m.ex() < 19 )
       &&( m.ey() > ( height() - 19 ) )&&( m.ey() < ( height() - 5 ) ) ) {
    if ( autoScale ) {
      autoScale = false;
    } else {
      autoScale = true;
      XShift = XShift0 = xshift = 0;
      for ( int l = 0; l < lines; l++ ) {
	YShift[l] = YShift0[l] = yshift[l] = 0;
      }
    }
  }
}

void XYView::UpDateYWindow( int l, SCALET s )
{
  bool noPoints = true;
  double nmaxy = -1e300;
  double nminy = +1e300;

  if ( points[l] > 0 )
    noPoints = false;
  for ( int i = 0; i < points[l]; i++ ) {
    if ( y[l][i] < nminy )
      nminy = y[l][i];
    if ( y[l][i] > nmaxy )
      nmaxy = y[l][i];
  }

  double dy = nmaxy - nminy;

  switch( s ) {
  case FULLSCALE:
    miny[l] = nminy - dy * 0.05;
    maxy[l] = nmaxy + dy * 0.05;
    break;
  case I0TYPE:
    miny[l] = nminy - dy * 5;
    maxy[l] = nmaxy + dy * 1;
    break;
  default:
    qDebug() << "Unknown scale type";
    break;
  }

  if ( noPoints ) {
    miny[l] = 0;
    maxy[l] = 1;
  }
}

/******************* Handle Mouse Events ************************/

void XYView::mouseMoveEvent( QMouseEvent *e )
{
  m.Moved( e );

  if ( m.inPress() ) {
    switch( m.modifier() ) {
    case Qt::NoModifier:                          // ���s�ړ�
      if ( !autoScale ) {
	xshift = cc.s2rx0( m.x() ) - cc.s2rx0( m.sx() );
	for ( int l = 0; l < lines; l++ ) {
	  cc.SetRealY( miny[l], maxy[l] );
	  yshift[l] = cc.s2ry0( m.y() ) - cc.s2ry0( m.sy() );
	}
	XShift = XShift0 + xshift;
	cc.RecallRealX();
	cc.SetRealX( cc.Rminx() - XShift, cc.Rmaxx() - XShift );
	for ( int l = 0; l < lines; l++ ) 
	  YShift[l] = YShift0[l] + yshift[l];
      }
      break;
    case Qt::ShiftModifier:                       // �̈�I���g��
      break;
    }
  }
  
  update();
}

void XYView::mousePressEvent( QMouseEvent *e )
{
  m.Pressed( e );

  switch( m.modifier() ) {
  case Qt::NoModifier:                          // ���s�ړ�
    break;
  case Qt::ShiftModifier:                       // �̈�I���g��
    if ( !autoScale )
      AreaSelecting = true;
    break;
  }

  update();
}

void XYView::mouseReleaseEvent( QMouseEvent *e )
{
  double dummy;
  double nminx, nmaxx;
  double nminy, nmaxy;

  m.Released( e );

  switch( m.modifier() ) {
  case Qt::NoModifier:                          // ���s�ړ�
    if ( !autoScale ) {
      cc.SetRealX0();
      XShift0 = 0;
      xshift = 0;
      for ( int l = 0; l < lines; l++ ) {
	YShift0[l] += yshift[l];
	yshift[l] = 0;
      }
    }
    break;
  case Qt::ShiftModifier:                       // �̈�I���g��
    if ( !autoScale ) {
      AreaSelecting = false;
      nminx = cc.s2rx( m.sx() );
      nmaxx = cc.s2rx( m.ex() );
      if ( nminx > nmaxx ) {
	dummy = nminx;
	nminx = nmaxx;
	nmaxx = dummy;
      }
      cc.SetRealX( nminx, nmaxx );
      cc.SetRealX0();
      for ( int l = 0; l < lines; l++ ) {
	cc.SetRealY( miny[l], maxy[l] );
	nminy = cc.s2ry( m.sy() );
	nmaxy = cc.s2ry( m.ey() );
	if ( nminy > nmaxy ) {
	  dummy = nminy;
	  nminy = nmaxy;
	  nmaxy = dummy;
	}
	miny[l] = nminy;
	maxy[l] = nmaxy;
      }
    }
    break;
  }
  CheckASPush();
  update();
}

void XYView::mouseDoubleClickEvent( QMouseEvent * )
{
}

void XYView::wheelEvent( QWheelEvent *e )
{
  double step = ( e->delta() / 8. ) / 15.;     // deg := e->delta / 8.
  double rx = cc.s2rx( e->x() );
  double drx = cc.Rmaxx() - cc.Rminx();
  if ( step > 0 ) {
    drx *= 0.9;
  } else {
    drx /= 0.9;
  }
  double nminx = rx - ( e->x() - cc.Sminx() ) / ( cc.Smaxx() - cc.Sminx() ) * drx;
  double nmaxx = nminx + drx;
  cc.SetRealX( nminx, nmaxx );
  cc.SetRealX0( nminx, nmaxx );

  for ( int l = 0; l < lines; l++ ) {
    cc.SetRealY( miny[l], maxy[l] );
    double ry = cc.s2ry( e->y() );
    double dry = cc.Rmaxy() - cc.Rminy();
    if ( step > 0 ) {
      dry *= 0.9;
    } else {
      dry /= 0.9;
    }
    double nmaxy = ry + ( e->y() - cc.Sminy() ) / ( cc.Smaxy() - cc.Sminy() ) * dry;
    double nminy = nmaxy - dry;
    miny[l] = nminy;
    maxy[l] = nmaxy;
  }
  update();
}

void XYView::ChooseAG( int i, bool f )
{
  dispf[ i ] = f;
  update();
}
