#include <QtGui>

#include "XafsM.h"
#include "XView.h"

XView::XView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  Clear();
  valid = false;
  SetLineF();
  lines = 0;
  cc.SetWindow( 0, 0, 1, 1 );
  bgColor = QColor( 255, 255, 255 );
  BLACK = QColor( 0, 0, 0 );
  MCLineC = QColor( 210, 180, 0 );     // mouse cursor line color

  LC << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0,   0, 255 ) << QColor(   0, 255,   0 )
     << QColor( 255, 255,   0 ) << QColor( 255,   0, 255 ) << QColor(   0, 255, 255 )
     << QColor( 127,   0,   0 ) << QColor(   0, 127,   0 ) << QColor(   0,   0, 127 )
     << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0, 255,   0 ) << QColor(   0,   0, 255 )
     << QColor( 255, 255,   0 ) << QColor( 255,   0, 255 ) << QColor(   0, 255, 255 )
     << QColor( 127,   0,   0 ) << QColor(   0, 127,   0 ) << QColor(   0,   0, 127 )
     << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0, 255,   0 ) << QColor(   0,   0, 255 )
     << QColor( 255, 255,   0 ) << QColor( 255,   0, 255 ) << QColor(   0, 255, 255 )
     << QColor( 127,   0,   0 ) << QColor(   0, 127,   0 ) << QColor(   0,   0, 127 );
  GType = XYPLOT;
  MonScale = 0;
  nx = ny = 0;
}


// l 番目のラインの p 番目のポイントの X データセット
void XView::setX( int l, int p, double xx )
{
  if ( l < MAXLINES ) {
    if ( p < MAXPOINTS ) {
      x[l][p] = xx;
      if ( p >= points[l] )
	points[l] = p + 1;
      if ( ( l >= lines )&&( l < MAXLINES - 1 ) )
	lines = l + 1;
    }
  }
}

// l 番目のラインの p 番目のポイントの Y データセット
void XView::setY( int l, int p, double yy )
{
  if ( l < MAXLINES ) {
    if ( p < MAXPOINTS ) {
      y[l][p] = yy;
      if ( p >= points[l] )
	points[l] = p + 1;
      if ( ( l >= lines )&&( l < MAXLINES - 1 ) )
	lines = l + 1;
    }
  }
}

// l 番目のラインに新しいデータ1つ追加
void XView::NewPoint( int l, double xx, double yy )
{
  if ( l < MAXLINES ) {
    if ( points[l] < MAXPOINTS - 1 ) {
      x[l][ points[l] ] = xx;
      y[l][ points[l] ] = yy;
      points[l]++;
    }
    if ( ( l >= lines )&&( l < MAXLINES - 1 ) )
      lines = l + 1;
  }
}

// リングバッファへのデータ追加
void XView::NewPointR( int tt, double yy0, double yy1, double yy2 )
{
  mont[ points[0] ] = tt;
  mony[0][ points[0] ] = yy0;
  mony[1][ points[0] ] = yy1;
  mony[2][ points[0] ] = yy2;
  points[0]++;
  if ( points[0] >= RingMax )
    points[0] = 0;
}

/*****************************************************/

void XView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void XView::Draw( QPainter *p )
{
  switch( (int)GType ) {
  case XYPLOT:
    DrawXYPlot( p );
    break;
  case MONITOR:
    DrawMonitor( p );
    break;
  }
}

void XView::DrawXYPlot( QPainter *p )
{
  if ( valid != true ) 
    return;

  QString buf, buf2;
  int RM, LM, TM, BM;
  QPen pen0, pen1;
  QFont F1;
  QRectF rec;

  p->fillRect( 0, 0, width(), height(), bgColor );
  pen0.setWidth( 2 );
  pen0.setColor( QColor( 95, 95, 195 ) );
  p->setPen( pen0 );

  RM = width() * 0.10;
  LM = width() * 0.10;
  TM = height() * 0.05;
  BM = height() * 0.10;
  F1.setPointSizeF( ( LM*0.115 < BM*0.23 ) ? LM*0.115 : BM*0.23 );
  p->setFont( F1 );

  cc.SetView( LM, TM, width()-RM, height()-BM );
  p->drawRect( LM, TM, width()-RM-LM, height()-BM-TM );

  double sx, dx;
  cc.calcScale( 10, cc.Wminx(), cc.Wmaxx(), &sx, &dx );
  int memc = 0;
  for ( double xx = sx; xx < cc.Wmaxx(); xx += dx ) {
    p->drawLine( cc.w2rx( xx ), TM, cc.w2rx( xx ), height()-BM );  // 縦の罫線
    rec = QRectF( cc.w2rx( xx )-40, height()-BM+5, 80, BM*0.3 ); // メモリ数字
    if ( memc % (int)( 80 / cc.w2rdx( dx ) + 1 ) == 0 ) {
      cc.DrawText( p, rec, F1, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE,
		   QString::number( xx ) );
    }
    memc++;
  }
  rec = QRectF( cc.w2rx( cc.Wmaxx() ), height()-BM+5, 80, BM*0.3 );   // X軸のラベル
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, XName );

  int inc;
  double tmp;
  double sy, dy;
  if ( SLineL >= 0 ) {                            // 左の y 軸に関連した描画
    UpDateYWindow( SLineL, ScaleTL );
    pen1.setWidth( 1 );
    pen1.setColor( LC[ SLineL ] );
    p->setPen( pen1 );
    inc = 0;

    sy = dy = 0;
    cc.calcScale( 5, cc.Wminy(), cc.Wmaxy(), &sy, &dy );

    for ( double yy = sy; yy < cc.Wmaxy(); yy += dy ) {
      p->drawLine( LM, cc.w2ry( yy ), width()-RM, cc.w2ry( yy ) );   // 横の罫線
      rec = QRectF( LM * 0.05, cc.w2ry( yy )-BM*0.3/2, LM * 0.9, BM * 0.3 ); // メモリ数字
      buf.sprintf( "%7.5g", yy );
      cc.DrawText( p, rec, F1, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, buf );
    }
    rec = QRectF( LM * 0.1, cc.w2ry( cc.Wmaxy() )-BM*0.35, 60, BM * 0.3 );  // 軸のラベル
    cc.DrawText( p, rec, F1, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE,
		 LNames[SLineL] );
    
    for ( int l = 0; l < lines; l++ ) { // データプロット
      if ( LineF[l] == LEFT ) {
	pen1.setColor( LC[ l ] );
	p->setPen( pen1 );
	double nowx = cc.r2wx( nx );
	int nowxp = 0;
	for ( int i = 0; i < points[l] - 1; i++ ) {
	  p->drawLine( cc.w2rx( x[l][i] ), cc.w2ry( y[l][i] ),
		       cc.w2rx( x[l][i+1] ), cc.w2ry( y[l][i+1] ) );
	  if (( x[l][i+1] >= nowx )&&( x[l][i] < nowx ))
	    nowxp = i;
	  if (( x[l][i+1] <= nowx )&&( x[l][i] > nowx ))
	    nowxp = i;
	}
	rec = QRectF( LM * 1.2, cc.w2ry( cc.Wmaxy() )-BM*0.35, 60, BM * 0.3 );
	// 軸のラベル
	cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
		     QString::number( y[l][nowxp] ) );
      }
    }
  }

  if ( SLineR >= 0 ) {                           // 右の y 軸に関連した描画
    UpDateYWindow( SLineR, ScaleTR );
    pen1.setWidth( 1 );
    pen1.setColor( LC[ SLineR ] );
    p->setPen( pen1 );
    inc = 0;
    sy = dy = 0;
    for (;;) {
      cc.calcScale( 5, cc.Wminy(), cc.Wmaxy(), &sy, &dy );
      buf.sprintf( "%5.3g", sy );
      buf2.sprintf( "%5.3g", sy + dy );
      if ( ( inc >= 20 )||( buf != buf2 ) )
	break;
      inc++;
      tmp = cc.Wmaxy() - cc.Wminy();
      cc.SetWindowY( cc.Wmaxy() + tmp, cc.Wminy() - tmp * 5 );
    }

    for ( double yy = sy; yy < cc.Wmaxy(); yy += dy ) {
      p->drawLine( LM, cc.w2ry( yy ), LM+3, cc.w2ry( yy ) );                 // 横の罫線(短い)
      p->drawLine( width()-RM, cc.w2ry( yy ), width()-RM-3, cc.w2ry( yy ) ); // 横の罫線(短い)
      rec = QRectF( width()-RM*0.9, cc.w2ry( yy )-BM*0.3/2, 60, BM * 0.3 ); // メモリ数字
      buf.sprintf( "%5.3g", yy );
      cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, buf );
    }
    rec = QRectF( width()-RM*0.95, cc.w2ry( cc.Wmaxy() )-BM*0.35,
		 RM*0.9, BM * 0.3 );    // 軸のラベル
    //    DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
    //              LNames[ SLineR ] );
    
    for ( int l = 0; l < lines; l++ ) { // データプロット
      if ( LineF[l] == RIGHT ) {
	pen1.setColor( LC[ l ] );
	p->setPen( pen1 );
	double nowx = cc.r2wx( nx );
	int nowxp = 0;
	for ( int i = 0; i < points[l] - 1; i++ ) {  // データプロット
	  p->drawLine( cc.w2rx( x[l][i] ), cc.w2ry( y[l][i] ),
		       cc.w2rx( x[l][i+1] ), cc.w2ry( y[l][i+1] ) );
	  if (( x[l][i+1] >= nowx )&&( x[l][i] < nowx ))
	    nowxp = i;
	  if (( x[l][i+1] <= nowx )&&( x[l][i] > nowx ))
	    nowxp = i;
	}
	rec = QRectF( width()-RM*0.95, cc.w2ry( cc.Wmaxy() )-BM*0.35,
		      RM*0.9, BM * 0.3 );    // 軸の値
	cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
		     LNames[SLineR] + " : " + QString::number(y[l][nowxp]) );
      }
    }
  }
  if ( ( nx > LM ) && ( nx < width()-RM ) ) {
    p->setPen( MCLineC );
    p->drawLine( nx, TM, nx, height()-BM );
  }
}

int XView::PeakSearch( int l )
{
  int maxp = 0;
  double maxv = -1e200;

  for ( int i = 0; i < points[l]; i++ ) {
    if ( y[l][i] > maxv ) {
      maxp = i;
      maxv = y[l][i];
    }
  }

  return x[l][maxp];
}

void XView::SetLines( int Lines )
{
  lines = Lines;
}


void XView::DrawMonitor( QPainter *p )
{
  QString buf, buf2;

  if ( valid != true ) {
    return;
  }

  double RM, LM, TM, BM, HDiv, VDiv;
  QPen pen0, pen1;
  QFont F1;
  QRectF rec;
  int ms = MScales[ MonScale ].div * 1000;

  double HDNum = 6;
  double VDNum = 10;

  cc.SetWindow( -ms * HDNum, 0, 0, 1 );

  p->fillRect( 0, 0, width(), height(), bgColor );
  pen0.setWidth( 1 );
  pen0.setColor( QColor( 0, 0, 0 ) );
  p->setPen( pen0 );

  RM = width() * 0.03;
  LM = width() * 0.17;
  TM = height() * 0.05;
  BM = height() * 0.10;
  HDiv = ( width() - RM - LM ) / HDNum;
  VDiv = ( height() - TM - BM ) / VDNum;
  p->setFont( F1 );

  cc.SetView( LM, TM, width()-RM, height()-BM );
  p->drawRect( LM, TM, width()-RM-LM, height()-BM-TM );

  for ( double xx = cc.Wminx(); xx <= cc.Wmaxx(); xx += ms ) {
    p->drawLine( cc.w2rx( xx ), TM, cc.w2rx( xx ), height()-BM );  // 縦の罫線
    rec = QRect( cc.w2rx( xx )-HDiv/2, height()-BM*0.95, HDiv, BM*0.4 ); // メモリ数字
    cc.DrawText( p, rec, F1, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE, 
		 QString( tr( "%1" ) )
		 .arg( (int)((cc.Wmaxx()-xx)/ms) * MScales[ MonScale ].dispDiv ) );
  }
  rec = QRect( width()-VDiv, height()-BM*0.5, RM*0.9, BM*0.4 );   // X軸のラベル
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
	       MScales[ MonScale ].unit );
  
  pen1.setWidth( 1 );
  pen1.setColor( BLACK );
  p->setPen( pen1 );
  for ( double yy = 0; yy < 1; yy += 0.1 ) {
    p->drawLine( LM, cc.w2ry( yy ), width()-RM, cc.w2ry( yy ) );   // 横の罫線
  }

  double sy, ey, dy;
  UpDateYWindowRing();

  for ( int j = 0; j < lines; j++ ) {
    //      sy = Rwminy[j];
    //      dy = ( Rwmaxy[j] - Rwminy[j] ) / 10.;
    cc.SetWindowY( Rwminy[j], Rwmaxy[j] );
    cc.getSEDy( &sy, &ey, &dy );
    pen1.setColor( LC[ j ] );
    p->setPen( pen1 );
    for ( double yy = sy; yy <= cc.Wmaxy(); yy += dy ) {
      rec = QRectF( LM - LM * 0.32 * ( j + 1 ), cc.w2ry( yy )-VDiv*0.45,
		    LM * 0.3, VDiv * 0.9 ); // メモリ数字
      buf.sprintf( "%6.4g", yy );
      cc.DrawText( p, rec, F1, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, buf );
    }
    rec = QRectF( LM + HDiv * 0.1 + HDiv * 2 * j, TM * 0.05, 
		  HDiv * 2, TM * 0.9 );  // 軸のラベル
    
    int pp1, pp2;
    int t0 = mont[ points[0] - 1 ];
    pen1.setWidth( 2 );
    pen1.setColor( LC[ j ] );
    p->setPen( pen1 );
    int nowt = cc.r2wx( nx ) + t0;
    int nowtp = 0;
    
    for ( int i = 0; i < RingMax; i++ ) { // データプロット
      pp1 = points[ 0 ] - 1 - i;
      pp2 = points[ 0 ] - 1 - ( i + 1 );
      if ( pp1 < 0 ) pp1 += RingMax;
      if ( pp2 < 0 ) pp2 += RingMax;
      if ( ( mont[ pp2 ] - t0 ) < ( - ms * 6 ) ) {
	break;
      }
      p->drawLine( cc.w2rx( mont[pp1] - t0 ), cc.w2ry( mony[j][pp1] ),
		   cc.w2rx( mont[pp2] - t0 ), cc.w2ry( mony[j][pp2] ) );
      if (( mont[pp1] >= nowt )&&( mont[pp2] < nowt ))
	nowtp = pp1;
    }
    cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 LNames[j] + " : " + QString::number(mony[j][nowtp]) );
  }

  if ( ( nx > LM ) && ( nx < width()-RM ) ) {
    p->setPen( MCLineC );
    p->drawLine( nx, TM, nx, height()-BM );
  }
}

void XView::ReDraw( void )
{
  update();
}

void XView::Clear( void )
{
  for ( int i = 0; i < MAXLINES; i++ ) {
    points[i] = 0;
    for ( int j = 0; j < MAXPOINTS; j++ ) {
      x[i][j] = y[i][j] = 0;
    }
    LineF[i] = 0;
  }
  lines = 0;
}

/*****************************************************/


void XView::UpDateYWindow( int l, SCALET s )
{
  if ( points[l] == 0 ) {
    cc.SetWindowY( 0, 1 );
    return;
  }

  double nmaxy = -1e300;
  double nminy = +1e300;

  for ( int i = 0; i < points[l]; i++ ) {
    if ( y[l][i] < nminy )
      nminy = y[l][i];
    if ( y[l][i] > nmaxy )
      nmaxy = y[l][i];
  }
  double dy = nmaxy - nminy;
  switch( s ) {
  case FULLSCALE:
    cc.SetWindowY( nminy - dy * 0.05, nmaxy + dy * 0.05 );
    break;
  case I0TYPE:
    cc.SetWindowY( nminy - dy * 5, nmaxy + dy * 1 );
    break;
  default:
    qDebug() << "Unknown scale type";
    break;
  }
}

void XView::ClearDataR( void )
{
  for ( int j = 0; j < RingMax; j++ ) {
    for ( int i = 0; i < MaxMon; i++ ) {
      mony[ i ][ j ] = 0;
    }
    mont[ j ] = -1000000000;
  }
  for ( int i = 0; i < MaxMon; i++ ) {
    points[ i ] = 0;
  }
}

void XView::UpDateYWindowRing( void )
{
  int dx = MScales[ MonScale ].div;
  int p, t0;

  t0 = mont[ points[0] - 1 ];

  for ( int j = 0; j < MaxMon; j++ ) {
    double nmaxy = -1e300;
    double nminy = 1e300;
    for ( int i = 0; i < RingMax; i++ ) {
      p = points[ 0 ] - 1 - i;
      if ( p < 0 ) p += RingMax;
      if ( (  mont[ p ] - t0 ) > ( - dx * 6 * 1000 ) ) {
	if ( mony[j][ p ] < nminy )
	  nminy = mony[j][ p ];
	if ( mony[j][ p ] > nmaxy ) 
	  nmaxy = mony[j][ p ];
      }
    }
    double dy = nmaxy - nminy;
    if ( dy == 0 )
      dy = 1.;
    Rwminy[j] = nminy - dy * 0.05;
    Rwmaxy[j] = nmaxy + dy * 0.05;
  }
}

void XView::SetMonScale( int ms )
{
  MonScale = ms;
}


/******************* Handle Mouse Events ************************/

void XView::mouseMoveEvent( QMouseEvent *e )
{
  nx = e->x();
  ny = e->y();

  switch( (int)GType ) {
  case XYPLOT:
    break;
  case MONITOR:
    break;
  }

  update();
}

void XView::mousePressEvent( QMouseEvent * )
{
}

void XView::mouseReleaseEvent( QMouseEvent * )
{
}

void XView::mouseDoubleClickEvent( QMouseEvent * )
{
}

