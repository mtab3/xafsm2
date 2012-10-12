#include <QtGui>

#include "XafsM.h"
#include "XView.h"

XView::XView( QWidget *parent ) : QWidget( parent )
{
  Clear();
  valid = false;
  SetLineF();
  lines = 0;
  SetWindow( 0, 0, 1, 1 );
  bgColor = QColor( 255, 255, 255 );
  LC << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0, 255,   0 ) << QColor(   0,   0, 255 )
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
}

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

void XView::NewPoint( int l, double xx, double yy )
{
  //  qDebug() << tr( "NewPoint %1 %2 %3" ).arg( l ).arg(xx).arg(yy);
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

void XView::NewPointR( double yy0, double yy )
{
  mony[0][ points[0] ] = yy0;
  mony[1][ points[0] ] = yy;
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
  char buf[ 256 ], buf2[ 256 ];

  if ( valid != true ) 
    return;

  int RM, LM, TM, BM;
  QPen pen0, pen1;
  QFont F1;
  QRect rec;

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

  SetView( LM, TM, width()-RM, height()-BM );
  p->drawRect( LM, TM, width()-RM-LM, height()-BM-TM );

  double sx, dx;
  calcScale( 10, wminx, wmaxx, &sx, &dx );
  int memc = 0;
  for ( double xx = sx; xx < wmaxx; xx += dx ) {
    p->drawLine( w2rx( xx ), TM, w2rx( xx ), height()-BM );  // 縦の罫線
    rec = QRect( w2rx( xx )-40, height()-BM+5, 80, BM*0.3 ); // メモリ数字
    if ( memc % (int)( 80 / w2rdx( dx ) + 1 ) == 0 ) {
      p->drawText( rec, Qt::AlignHCenter | Qt::AlignVCenter,
		   QString( tr( "%1" ) ).arg(xx) );
    }
    memc++;
  }
  rec = QRect( w2rx( wmaxx ), height()-BM+5, 80, BM*0.3 );   // X軸のラベル
  p->drawText( rec, Qt::AlignLeft | Qt::AlignVCenter, XName );
  

  int inc;
  double tmp;
  double sy, dy;
  if ( SLineL >= 0 ) {                            // 左の y 軸に関連した描画
    UpDateYWindow( SLineL, ScaleTL );
    qDebug() << "ScaleTL" << ScaleTL;
    pen1.setWidth( 1 );
    pen1.setColor( LC[ SLineL ] );
    p->setPen( pen1 );
    inc = 0;

    sy = dy = 0;
    calcScale( 5, wminy, wmaxy, &sy, &dy );
#if 0   
    for (;;) {
      sprintf( buf, "%7.5g", sy );
      sprintf( buf2, "%7.5g", sy + dy );
      calcScale( 5, wminy, wmaxy, &sy, &dy );
      if ( ( inc >= 20 )||( strcmp( buf, buf2 ) != 0 ) )
	break;
      inc++;
      tmp = wmaxy - wminy;
      wmaxy += tmp * 5;
      wminy -= tmp * 5;
    }
#endif

    for ( double yy = sy; yy < wmaxy; yy += dy ) {
      p->drawLine( LM, w2ry( yy ), width()-RM, w2ry( yy ) );   // 横の罫線
      rec = QRect( LM * 0.05, w2ry( yy )-BM*0.3/2, LM * 0.9, BM * 0.3 ); // メモリ数字
      sprintf( buf, "%7.5g", yy );
      p->drawText( rec, Qt::AlignRight | Qt::AlignVCenter, QString( "%1" ).arg(buf) );
    }
    rec = QRect( LM * 0.1, w2ry( wmaxy )-BM*0.35, 60, BM * 0.3 );  // 軸のラベル
    p->drawText( rec, Qt::AlignRight | Qt::AlignVCenter, LNames[SLineL] );
    
    for ( int l = 0; l < lines; l++ ) { // データプロット
      if ( LineF[l] == LEFT ) {
	pen1.setColor( LC[ l ] );
	p->setPen( pen1 );
	for ( int i = 0; i < points[l] - 1; i++ ) {
	  p->drawLine( w2rx( x[l][i] ), w2ry( y[l][i] ),
		       w2rx( x[l][i+1] ), w2ry( y[l][i+1] ) );
	}
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
      calcScale( 5, wminy, wmaxy, &sy, &dy );
      sprintf( buf, "%5.3g", sy );
      sprintf( buf2, "%5.3g", sy + dy );
      if ( ( inc >= 20 )||( strcmp( buf, buf2 ) != 0 ) )
	break;
      inc++;
      tmp = wmaxy - wminy;
      wmaxy += tmp;
      wminy -= tmp * 5;
    }
    
    for ( double yy = sy; yy < wmaxy; yy += dy ) {
      p->drawLine( LM, w2ry( yy ), LM+3, w2ry( yy ) );                 // 横の罫線(短い)
      p->drawLine( width()-RM, w2ry( yy ), width()-RM-3, w2ry( yy ) ); // 横の罫線(短い)
      rec = QRect( width()-RM*0.9, w2ry( yy )-BM*0.3/2, 60, BM * 0.3 ); // メモリ数字
      sprintf( buf, "%5.3g", yy );
      p->drawText( rec, Qt::AlignLeft | Qt::AlignVCenter, QString( "%1" ).arg(buf) );
    }
    rec = QRect( width()-RM*0.95, w2ry( wmaxy )-BM*0.35,
		 RM*0.9, BM * 0.3 );    // 軸のラベル
    p->drawText( rec, Qt::AlignLeft | Qt::AlignVCenter, LNames[ SLineR ] );
    
    for ( int l = 0; l < lines; l++ ) { // データプロット
      if ( LineF[l] == RIGHT ) {
	pen1.setColor( LC[ l ] );
	p->setPen( pen1 );
	for ( int i = 0; i < points[l] - 1; i++ ) {  // データプロット
	  p->drawLine( w2rx( x[l][i] ), w2ry( y[l][i] ),
		       w2rx( x[l][i+1] ), w2ry( y[l][i+1] ) );
	}
      }
    }
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

void XView::DrawMonitor( QPainter *p )
{
  char buf[ 256 ], buf2[ 256 ];

  if ( valid != true ) 
    return;

  int RM, LM, TM, BM;
  QPen pen0, pen1;
  QFont F1;
  QRect rec;
  int ms = MScales[ MonScale ].div;

  wminx = 0;
  wmaxx = ms * 6;
  p->fillRect( 0, 0, width(), height(), bgColor );
  pen0.setWidth( 2 );
  pen0.setColor( QColor( 0, 0, 0 ) );
  p->setPen( pen0 );

  RM = width() * 0.10;
  LM = width() * 0.10;
  TM = height() * 0.05;
  BM = height() * 0.10;
  F1.setPointSizeF( ( LM*0.115 < BM*0.23 ) ? LM*0.115 : BM*0.23 );
  p->setFont( F1 );

  SetView( LM, TM, width()-RM, height()-BM );
  p->drawRect( LM, TM, width()-RM-LM, height()-BM-TM );

  for ( double xx = 00; xx <= wmaxx; xx += ms ) {
    p->drawLine( w2rx( xx ), TM, w2rx( xx ), height()-BM );  // 縦の罫線
    rec = QRect( w2rx( xx )-40, height()-BM+5, 80, BM*0.3 ); // メモリ数字
    p->drawText( rec, Qt::AlignHCenter | Qt::AlignVCenter,
		 QString( tr( "%1" ) )
		 .arg( (int)((wmaxx-xx)/ms) * MScales[ MonScale ].dispDiv ) );
  }
  rec = QRect( width()-RM*0.9, height()-BM+5, RM*0.8, BM*0.3 );   // X軸のラベル
  p->drawText( rec, Qt::AlignLeft | Qt::AlignVCenter, MScales[ MonScale ].unit );

  int inc;
  double tmp;
  double sy, dy;                      // 左の y 軸に関連した描画
  UpDateYWindowRing( 1 ); // 1: Left,  0: Right
  pen1.setWidth( 1 );
  pen1.setColor( LC[ 1 ] );
  p->setPen( pen1 );

#if 1
  calcScale( 5, wminy, wmaxy, &sy, &dy );
#else
  inc = 0;
  sy = dy = 0;
  for (;;) {
    sprintf( buf, "%7.5g", sy );
    sprintf( buf2, "%7.5g", sy + dy );
    calcScale( 5, wminy, wmaxy, &sy, &dy );
    if ( ( inc >= 20 )||( strcmp( buf, buf2 ) != 0 ) )
      break;
    inc++;
    tmp = wmaxy - wminy;
    wmaxy += tmp * 5;
    wminy -= tmp * 5;
  }
#endif

  for ( double yy = sy; yy < wmaxy; yy += dy ) {
    p->drawLine( LM, w2ry( yy ), width()-RM, w2ry( yy ) );   // 横の罫線
    rec = QRect( LM * 0.05, w2ry( yy )-BM*0.3/2, LM * 0.9, BM * 0.3 ); // メモリ数字
    sprintf( buf, "%7.5g", yy );
    p->drawText( rec, Qt::AlignRight | Qt::AlignVCenter, QString( "%1" ).arg(buf) );
  }
  rec = QRect( LM * 0.1, w2ry( wmaxy )-BM*0.35, 60, BM * 0.3 );  // 軸のラベル
  p->drawText( rec, Qt::AlignRight | Qt::AlignVCenter, LNames[SLineL] );

  pen1.setWidth( 2 );
  pen1.setColor( LC[ 1 ] );
  p->setPen( pen1 );

  int dx = MScales[ MonScale ].div / 10;
  int pp1, pp2;
  for ( int i = 0; i < 60; i++ ) { // データプロット
    pp1 = points[ 0 ] - 1 - i * dx;
    pp2 = points[ 0 ] - 1 - ( i + 1 ) * dx;
    if ( pp1 < 0 ) pp1 += RingMax;
    if ( pp2 < 0 ) pp2 += RingMax;
    p->drawLine( w2rx( ms * 6 - dx * i ), w2ry( mony[1][pp1] ),
		 w2rx( ms * 6 - dx * ( i + 1 ) ), w2ry( mony[1][pp2] ) );
  }

#if 0
  if ( SLineR >= 0 ) {                           // 右の y 軸に関連した描画
    UpDateYWindow( SLineR, ScaleTR );
    pen1.setWidth( 1 );
    pen1.setColor( LC[ SLineR ] );
    p->setPen( pen1 );
    inc = 0;
    for (;;) {
      calcScale( 5, wminy, wmaxy, &sy, &dy );
      sprintf( buf, "%5.3g", sy );
      sprintf( buf2, "%5.3g", sy + dy );
      if ( ( inc >= 20 )||( strcmp( buf, buf2 ) != 0 ) )
	break;
      inc++;
      tmp = wmaxy - wminy;
      wmaxy += tmp;
      wminy -= tmp * 5;
    }
    
    for ( double yy = sy; yy < wmaxy; yy += dy ) {
      p->drawLine( LM, w2ry( yy ), LM+3, w2ry( yy ) );                 // 横の罫線(短い)
      p->drawLine( width()-RM, w2ry( yy ), width()-RM-3, w2ry( yy ) ); // 横の罫線(短い)
      rec = QRect( width()-RM*0.9, w2ry( yy )-BM*0.3/2, 60, BM * 0.3 ); // メモリ数字
      sprintf( buf, "%5.3g", yy );
      p->drawText( rec, Qt::AlignLeft | Qt::AlignVCenter, QString( "%1" ).arg(buf) );
    }
    rec = QRect( width()-RM*0.95, w2ry( wmaxy )-BM*0.35,
		 RM*0.9, BM * 0.3 );    // 軸のラベル
    p->drawText( rec, Qt::AlignLeft | Qt::AlignVCenter, LNames[ SLineR ] );
    
    for ( int l = 0; l < lines; l++ ) { // データプロット
      if ( LineF[l] == RIGHT ) {
	pen1.setColor( LC[ l ] );
	p->setPen( pen1 );
	for ( int i = 0; i < points[l] - 1; i++ ) {  // データプロット
	  p->drawLine( w2rx( x[l][i] ), w2ry( y[l][i] ),
		       w2rx( x[l][i+1] ), w2ry( y[l][i+1] ) );
	}
      }
    }
  }
#endif
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
  for ( int i = 0; i < RingMax; i++ ) {
    mony[0][i] = mony[1][i] = 0;
  }
  lines = 0;
}

/*****************************************************/

void XView::calcScale( double div, double min, double max, double *s, double *d )
{
  if ( max == min ) {
    *s = min;
    *d = 1;
    return;
  }

  double d0 = ( max - min ) / div;
  double Ld0 = 0;

  while ( d0 >= 10 ) {
    Ld0++;
    d0 /= 10.;
  }
  while( d0 < 1 ) {
    Ld0--;
    d0 *= 10;
  }
  if ( d0 <= 2.5 )
    d0 = 1.0;
  else if ( d0 <= 7.5 )
    d0 = 5.0;
  else 
    d0 = 10.0;
  d0 *= pow( 10, Ld0 );

  *s = (int)(min / d0 + 1) * d0;
  *d = d0;
}

void XView::UpDateYWindow( int l, SCALET s )
{
  if ( points[l] == 0 ) {
    wminy = 0;
    wmaxy = 1;
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
    wminy = nminy - dy * 0.05;
    wmaxy = nmaxy + dy * 0.05;
    break;
  case I0TYPE:
    wminy = nminy - dy * 5;
    wmaxy = nmaxy + dy * 1;
    break;
  default:
    qDebug() << "Unknown scale type";
    break;
  }
  qDebug() << "UpDateY " << nmaxy << nminy << wminy << wmaxy << s << FULLSCALE << I0TYPE;
}

void XView::UpDateYWindowRing( int LR )
{
  double nmaxy = -1e300;
  double nminy = +1e300;
  int dx = MScales[ MonScale ].div / 10;
  int p;

  for ( int i = 0; i < 60; i++ ) {
    p = points[ 0 ] - 1 - i * dx;
    if ( p < 0 ) p += RingMax;
    if ( mony[ LR ][ p ] < nminy )
      nminy = mony[ LR ][ p ];
    if ( mony[ LR ][ p ] > nmaxy )
      nmaxy = mony[ LR ][ p ];
  }
  double dy = nmaxy - nminy;
  switch( LR ) {
  case 0:
    wminy = nminy - dy * 5;
    wmaxy = nmaxy + dy * 1;
    break;
  case 1:
    wminy = nminy - dy * 0.05;
    wmaxy = nmaxy + dy * 0.05;
    break;
  }
}


void XView::SetView( int x1, int y1, int x2, int y2 )
{
  minx = x1;
  maxx = x2;
  miny = y1;
  maxy = y2;
}

void XView::SetWindow( double x1, double y1, double x2, double y2 )
{
  wminx = x1;
  wmaxx = x2;
  wminy = y1;
  wmaxy = y2;
}

int XView::w2rx( double x )
{
  return ( maxx - minx ) / ( wmaxx - wminx ) * ( x - wminx ) + minx; 
}

int XView::w2ry( double y )
{
  return maxy - ( maxy - miny ) / ( wmaxy - wminy ) * ( y - wminy );
}

int XView::w2rdx( double x )
{
  return ( maxx - minx ) / ( wmaxx - wminx ) * x;
}

int XView::w2rdy( double y )
{
  return ( maxy - miny ) / ( wmaxy - wminy ) * y;
}

double XView::r2wx( int x )
{
  return ( wmaxx - wminx ) / ( maxx - minx ) * ( x - minx ) + wminx;
}

double XView::r2wy( int y )
{
  return ( wmaxy - wminy ) / ( maxy - miny ) * ( maxy - y ) + wminy;
}

double XView::r2wdx( int x )
{
  return ( wmaxx - wminx ) / ( maxx - minx ) * x;
}

double XView::r2wdy( int y )
{
  return ( wmaxy - wminy ) / ( maxy - miny ) * y;
}
