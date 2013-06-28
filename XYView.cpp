#include <QtGui>

#include "XafsM.h"
#include "XYView.h"

XYView::XYView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  setToolTip( tr( "By clicking the 'A. Scale' (Auto Scale) button at left-bottom corner, "
		  "the auto-scale mode is toggled on and off." ) );

  autoScale = true;
  singleScale = true;
  AreaSelecting = false;
  SelLR[ LEFT_AX ] = 0;
  SelLR[ RIGHT_AX ] = 1;

  Clear();
  valid = false;
  QXafsMode = false;

  for ( int i = 0; i < MAXLINES; i++ ) {
    SetLR( i, LEFT_AX );
    SetScaleType( i, FULLSCALE );
  }

  CGroups = FInterval = 100;    // Continuous drawn lines, Interval of foot print lines
  maxGroups = maxLines = inLines = 0;
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
  CGroups = FInterval = 100;    // Continuous drawn lines, Interval of foot print lines
  maxGroups = maxLines = inLines = 0;
  QXafsMode = false;
}

void XYView::ReFillFirst10Groups( void )
{
  for ( int g = 10; g < 100; g += 10 ) {
    int g0 = g / 10;
    for ( int i = 0; i < GROUPLINES; i++ ) {
      int a = g0 * GROUPLINES + i;
      int b = g * GROUPLINES + i;
      for ( int j = 0; j < points[ b ]; j++ ) {
	x[ a ][j] = x[ b ][j];
	y[ a ][j] = x[ b ][j];
      }
      points[ a ] = points[ b ]; points[ b ] = 0;
      LineLR[ a ] = LineLR[ b ];
      scaleType[ a ] = scaleType[ b ];
      LNames[ a ] = LNames[ b ];
      YShift[ a ] = YShift[ b ];
      YShift0[ a ] = YShift0[ b ];
      yshift[ a ] = yshift[ b ];
      dispf[ a ] = dispf[ b ];
      miny[ a ] = miny[ b ];
      maxy[ a ] = maxy[ b ];
      SaveYatNowXp[ a ] = SaveYatNowXp[ b ];
    }
  }
}

// l 番目のラインに新しいデータ1つ追加
void XYView::NewPoint( int l, double xx, double yy )
{
  if ( l >= MAXLINES )
    l = MAXLINES - 1;
  if ( l > maxLines )
    maxLines = l;

  int g = l / GROUPLINES;
  if ( g > maxGroups ) {  // 新しいグループ
    if (( maxGroups < 100 )&&( g >= 100 )) {
      ReFillFirst10Groups();
      CGroups = FInterval = 10;
    }
    if (( maxGroups < 1000 )&&( g >= 1000 )) {
      ReFillFirst10Groups();
      CGroups = FInterval = 100;
    }
    maxGroups = g;
  }

  int L = getL( l );
  //  if ( points[L] == 0 ) {  qDebug() << "L L " << L << getL( l );  }

  if ( L > inLines )
    inLines = L;

  if ( points[L] < MAXPOINTS - 1 ) {
    x[L][ points[L] ] = xx;
    y[L][ points[L] ] = yy;
    points[L]++;
    if ( xx < origMinx ) origMinx = xx;
    if ( xx > origMaxx ) origMaxx = xx;
  }
}

int XYView::getL( int l )
{
  int L;

  int g = l / GROUPLINES;
  int gg = l % GROUPLINES;
  int B = maxGroups / FInterval;
  if ( g > B * FInterval ) {
    L = ( ( g % CGroups ) + B ) * GROUPLINES + gg;
  } else {
    L = ( g / FInterval ) * GROUPLINES + gg;
  }

  return L;
}

int XYView::GetPoints( int l )
{
  if ( l > maxLines )
    return -1;
  return points[ getL( l ) ];
};

double XYView::GetX( int l, int p )
{
  double rv = 0;

  if ( l <= maxLines )
    if ( p < points[ getL( l ) ] )
      rv = x[ getL( l ) ][p];
  
  return rv;
}

double XYView::GetY( int l, int p )
{
  double rv = 0;

  if ( l <= maxLines )
    if ( p < points[ getL( l ) ] )
      rv = y[ getL( l ) ][p];

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
  x0 = x1 = y0 = y1 = 0;

  int linedir[ MAXLINES ];           // x軸の数字の並び
  for ( int i = 0; i <= inLines; i++ ) {
    if ( x[i][0] > x[i][1] )   // 
      linedir[i] = -1;
    else 
      linedir[i] = 1;
  }

  for ( int l = 0; l <= inLines; l++ ) { // 先に線だけ描画
    bool f = ( QXafsMode ) ? dispf[ l % GROUPLINES ] : dispf[ l ];
    if ( f ) {
      if ( autoScale ) {
	//	UpDateYWindow( l, scaleType[ l ] );  // ループの前に実行する
	cc.SetRealY( miny[l], maxy[l] );
      } else {
	cc.SetRealY( miny[l] - YShift[l], maxy[l] - YShift[l] );
      }
      pen1.setColor( LC[ l % LC.count() ] );
      p->setPen( pen1 );
      nowx = cc.s2rx( m.x() );
      nowxp = 0;
      for ( int i = 0; i < points[l] - 1; i++ ) {
	if ( linedir[l] * ( x[l][i+1] - x[l][i] ) < 0 ) {
	  linedir[l] *= -1;
	} else {
	  p->drawLine( x0 = cc.r2sx( x[l][i] ), y0 = cc.r2sy( y[l][i] ),
		       x1 = cc.r2sx( x[l][i+1] ), y1 = cc.r2sy( y[l][i+1] ) );
	  if (( x[l][i+1] >= nowx )&&( x[l][i] < nowx ))  // カーソルがある場所に最も近い
	    nowxp = i;                                    // 点を探して記録
	  if (( x[l][i+1] <= nowx )&&( x[l][i] > nowx ))  // 同上(横軸が降順の場合)
	    nowxp = i;
	}
	
	// マウスがある点から線分(を延長したものに下ろした垂線の足を探す)
	a1 = x1 - x0;
	a2 = y1 - y0;
	b = ( a1 * ( y0 - m.y() ) + a2 * ( m.x() - x0 ) ) / ( a1 * a1 + a2 * a2 );
	hx = m.x() - b * a2;
	hy = m.y() + b * a1;
	if ( hx < x0 ) { hx = x0; hy = y0; }  // 足が線分の外に有ったら
	if ( hx > x1 ) { hx = x1; hy = y1; }  // 線分の端点で置き換える
	// その足までの距離が最小になる線を探す
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
  // 見つけた点に線を引く
  //  p->drawLine( m.x(), m.y(), hx, hy );
  
  if ( AreaSelecting ) {
    p->setPen( ASelC );
    p->drawLine( m.sx(), m.sy(), m.sx(), m.y() );
    p->drawLine( m.sx(), m.y(),  m.x(),  m.y() );
    p->drawLine( m.x(),  m.y(),  m.x(),  m.sy() );
    p->drawLine( m.x(),  m.sy(), m.sx(), m.sy() );
  }
  
  // お笑いクリッピング ^^;;;
  p->fillRect( 0, 0, LM, height(), bgColor );
  p->fillRect( width()-RM, 0, RM, height(), bgColor );
  p->fillRect( 0, 0, width(), TM, bgColor );
  p->fillRect( 0, height()-BM, width(), BM, bgColor );
  // お笑いクリッピング ^^;;;
  
  cc.ShowAScaleButton( p, autoScale, height() );
  
  double sx, dx;
  cc.calcScale( 10, cc.Rminx(), cc.Rmaxx(), &sx, &dx );
  int memc = 0;
  for ( double xx = sx; xx < cc.Rmaxx(); xx += dx ) {
    p->drawLine( cc.r2sx( xx ), TM, cc.r2sx( xx ), height()-BM );  // 縦の罫線
    rec = QRectF( cc.r2sx( xx )-40, height()-BM+5, 80, BM*0.3 ); // メモリ数字
    if ( memc % (int)( 80 / cc.r2sdx( dx ) + 1 ) == 0 ) {
      cc.DrawText( p, rec, F1, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE,
		   QString::number( ( xx - center ) * upp ) );
    }
    memc++;
  }
  rec = QRectF( cc.r2sx( cc.Rmaxx() ), height()-BM+5, 80, BM*0.3 );   // X軸のラベル
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, XName );
  rec = QRectF( cc.r2sx( cc.Rmaxx() )+80, height()-BM+5, 80, BM*0.3 );   // X軸の単位
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, XUnitName );

  for ( int i = 0; i < 2; i++ ) {
    int l = SelLR[i];
    if ( !singleScale ) {
      if ( autoScale ) {
	UpDateYWindow( l, scaleType[ l ] );
	cc.SetRealY( miny[l], maxy[l] );
      } else {
	cc.SetRealY( miny[l] - YShift[l], maxy[l] - YShift[l] );
      }
    }
    sy = dy = 0;
    cc.calcScale( 5, cc.Rminy(), cc.Rmaxy(), &sy, &dy );
    pen1.setWidth( 1 );
    pen1.setColor( LC[ l % LC.count() ] );
    p->setPen( pen1 );
    
    for ( double yy = sy; yy < cc.Rmaxy(); yy += dy ) {
      buf.sprintf( "%7.5g", yy );
      if ( LineLR[l] == LEFT_AX ) {
	p->drawLine( LM, cc.r2sy( yy ), width()-RM, cc.r2sy( yy ) );   // 横の罫線
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
		    RM*0.9, BM * 0.3 );    // 軸のラベル
      cc.DrawText( p, rec, F1, AlLC, SCALESIZE, RightName );
    }    
    pen1.setColor( LC[ l % LC.count() ] );
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
      for ( int l = 0; l <= inLines; l++ ) {
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
    case Qt::NoModifier:                          // 平行移動
      if ( !autoScale ) {
	xshift = cc.s2rx0( m.x() ) - cc.s2rx0( m.sx() );
	for ( int l = 0; l <= inLines; l++ ) {
	  if ( !singleScale )
	    cc.SetRealY( miny[l], maxy[l] );
	  yshift[l] = cc.s2ry0( m.y() ) - cc.s2ry0( m.sy() );
	}
	XShift = XShift0 + xshift;
	cc.RecallRealX();
	cc.SetRealX( cc.Rminx() - XShift, cc.Rmaxx() - XShift );
	for ( int l = 0; l <= inLines; l++ ) 
	  YShift[l] = YShift0[l] + yshift[l];
      }
      break;
    case Qt::ShiftModifier:                       // 領域選択拡大
      break;
    }
  }
  
  update();
}

void XYView::mousePressEvent( QMouseEvent *e )
{
  m.Pressed( e );

  switch( m.modifier() ) {
  case Qt::NoModifier:                          // 平行移動
    break;
  case Qt::ShiftModifier:                       // 領域選択拡大
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
  case Qt::NoModifier:                          // 平行移動
    if ( !autoScale ) {
      cc.SetRealX0();
      XShift0 = 0;
      xshift = 0;
      for ( int l = 0; l <= inLines; l++ ) {
	YShift0[l] += yshift[l];
	yshift[l] = 0;
      }
    }
    break;
  case Qt::ShiftModifier:                       // 領域選択拡大
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
      for ( int l = 0; l <= inLines; l++ ) {
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
  if ( autoScale )
    return;

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

  for ( int l = 0; l <= inLines; l++ ) {
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
  dispf[ getL( i ) ] = f;
  update();
}
