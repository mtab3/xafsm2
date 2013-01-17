#include <QtGui>

#include "XafsM.h"
#include "XYView.h"

XYView::XYView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  Clear();
  valid = false;
  Groups = 0;
  SetLineG();
  SetScaleType();

  lines = 0;
  cc.SetRealCoord( 0, 0, 1, 1 );
  bgColor = QColor( 255, 255, 255 );
  BLACK = QColor( 0, 0, 0 );
  MCLineC = QColor( 210, 180, 0 );     // mouse cursor line color
  upp = 1;
  center = 0;

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
}

void XYView::Clear( void )
{
  for ( int i = 0; i < MAXLINES; i++ ) {
    points[i] = 0;
    for ( int j = 0; j < MAXPOINTS; j++ ) {
      x[i][j] = y[i][j] = 0;
    }
    LineG[i] = 0;
  }
  lines = 0;
}

// l 番目のラインに新しいデータ1つ追加
void XYView::NewPoint( int l, double xx, double yy )
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

int XYView::getPoints( int l )
{
  if ( l >= lines )
    return -1;

  return points[ l ];
};

double XYView::getX( int l, int p )
{
  double rv = 0;

  if ( l < lines )
    if ( p < points[l] )
      rv = x[l][p];
  
  return rv;
}

double XYView::getY( int l, int p )
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

  cc.SetScreenCoord( LM, TM, width()-RM, height()-BM );
  p->drawRect( LM, TM, width()-RM-LM, height()-BM-TM );

  double sx, dx;
  cc.calcScale( 10, cc.Rminx(), cc.Rmaxx(), &sx, &dx );
  int memc = 0;
  for ( double xx = sx; xx < cc.Rmaxx(); xx += dx ) {
    p->drawLine( cc.r2sx( xx ), TM, cc.r2sx( xx ), height()-BM );  // 縦の罫線
    rec = QRectF( cc.r2sx( xx )-40, height()-BM+5, 80, BM*0.3 ); // メモリ数字
    if ( memc % (int)( 80 / cc.r2sdx( dx ) + 1 ) == 0 ) {
      cc.DrawText( p, rec, F1, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE,
		   QString::number( xx ) );
    }
    memc++;
  }
  rec = QRectF( cc.r2sx( cc.Rmaxx() ), height()-BM+5, 80, BM*0.3 );   // X軸のラベル
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, XName );

  double sy, dy;
  for ( int g = 0; g < Groups; g++ ) { // グループを順番に回る
    // g 番目のグループに属する線を全部調べて描画スケールを決める。
    UpDateYWindow( g, ScaleType[ g ] );
    pen1.setWidth( 1 );
    if ( g == LeftG ) {  // もし、そのグループが左軸と関連付けられたグループなら
      pen1.setColor( LC[ g ] );
      p->setPen( pen1 );

      sy = dy = 0;
      cc.calcScale( 5, cc.Rminy(), cc.Rmaxy(), &sy, &dy );
      
      for ( double yy = sy; yy < cc.Rmaxy(); yy += dy ) {
	p->drawLine( LM, cc.r2sy( yy ), width()-RM, cc.r2sy( yy ) );   // 横の罫線
	rec = QRectF( LM * 0.05, cc.r2sy( yy )-BM*0.3/2, LM * 0.9, BM * 0.3 );
	// メモリ数字
	buf.sprintf( "%7.5g", yy * upp );
	cc.DrawText( p, rec, F1, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, buf );
      }
      for ( int l = 0; l < lines; l++ ) {
	if ( LineG[l] == g ) {
	  rec = QRectF( LM * 0.1, cc.r2sy( cc.Rmaxy() )-BM*0.35, 60, BM * 0.3 );
	  // 軸のラベル
	  cc.DrawText( p, rec, F1, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE,
		   LNames[g] );
	  break;
	}
      }
    }
    if ( g == RightG ) {  // もし、そのグループが右軸と関連付けられたグループなら
      pen1.setColor( LC[ g ] );
      p->setPen( pen1 );

      sy = dy = 0;
      cc.calcScale( 5, cc.Rminy(), cc.Rmaxy(), &sy, &dy );
      
      for ( double yy = sy; yy < cc.Rmaxy(); yy += dy ) {
	p->drawLine( LM, cc.r2sy( yy ), LM+3, cc.r2sy( yy ) );
	// 横の罫線(短い)
	p->drawLine( width()-RM, cc.r2sy( yy ), width()-RM-3, cc.r2sy( yy ) );
	// 横の罫線(短い)
	rec = QRectF( width()-RM*0.9, cc.r2sy( yy )-BM*0.3/2, 60, BM * 0.3 );
	// メモリ数字
	buf.sprintf( "%5.3g", yy );
	cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, buf );
      }
      for ( int l = 0; l < lines; l++ ) {
	if ( LineG[l] == g ) {
	  rec = QRectF( width()-RM*0.95, cc.r2sy( cc.Rmaxy() )-BM*0.35,
			RM*0.9, BM * 0.3 );    // 軸のラベル
	  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
		       LNames[ g ] );
	  break;
	}
      }
    }

    for ( int l = 0; l < lines; l++ ) { // データプロット
      if ( LineG[l] == g ) {
	pen1.setColor( LC[ l ] );
	p->setPen( pen1 );
	double nowx = cc.s2rx( m.x() );
	int nowxp = 0;
	for ( int i = 0; i < points[l] - 1; i++ ) {
	  p->drawLine( cc.r2sx( x[l][i] ), cc.r2sy( y[l][i] ),
		       cc.r2sx( x[l][i+1] ), cc.r2sy( y[l][i+1] ) );
	  if (( x[l][i+1] >= nowx )&&( x[l][i] < nowx ))
	    nowxp = i;
	  if (( x[l][i+1] <= nowx )&&( x[l][i] > nowx ))
	    nowxp = i;
	}
#if 0
	// マウスポインタ位置の値の表示　左
	rec = QRectF( LM * 1.2, cc.r2sy( cc.Rmaxy() )-BM*0.35, 60, BM * 0.3 );
	// 軸のラベル
	cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
		     QString( "at %1(%2) : %3" )
		     .arg( ( nowxp - center ) * upp ).arg( nowxp )
		     .arg( y[l][nowxp] ) );
	
	// マウスポインタ位置の値の表示 右
	rec = QRectF( width()-RM*0.95, cc.r2sy( cc.Rmaxy() )-BM*0.35,
		      RM*0.9, BM * 0.3 );    // 軸の値
	cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
		     LNames[l] + " : " + QString::number(y[l][nowxp]) );
#endif
      }
    }
  }

  if ( ( m.x() > LM ) && ( m.x() < width()-RM ) ) {
    p->setPen( MCLineC );
    p->drawLine( m.x(), TM, m.x(), height()-BM );
  }
}

void XYView::UpDateYWindow( int g, SCALET s )
{
  bool noPoints = true;
  double nmaxy = -1e300;
  double nminy = +1e300;

  for ( int l = 0; l < lines; l++ ) {
    if ( LineG[l] == g ) {
      if ( points[l] > 0 )
	noPoints = false;
      for ( int i = 0; i < points[l]; i++ ) {
	if ( y[l][i] < nminy )
	  nminy = y[l][i];
	if ( y[l][i] > nmaxy )
	  nmaxy = y[l][i];
      }
    }
  }
  double dy = nmaxy - nminy;

  switch( s ) {
  case FULLSCALE:
    cc.SetRealY( nminy - dy * 0.05, nmaxy + dy * 0.05 );
    break;
  case I0TYPE:
    cc.SetRealY( nminy - dy * 5, nmaxy + dy * 1 );
    break;
  default:
    qDebug() << "Unknown scale type";
    break;
  }

  if ( noPoints ) {
    cc.SetRealY( 0, 1 );
  }
}

/******************* Handle Mouse Events ************************/

void XYView::mouseMoveEvent( QMouseEvent *e )
{
  m.Moved( e );
  update();
}

void XYView::mousePressEvent( QMouseEvent *e )
{
  m.Pressed( e );
  update();
}

void XYView::mouseReleaseEvent( QMouseEvent *e )
{
  m.Released( e );
  update();
}

void XYView::mouseDoubleClickEvent( QMouseEvent * )
{
}

