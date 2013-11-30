#include <QtGui>

#include "XafsM.h"
#include "XYView.h"

XYView::XYView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  setToolTip( tr( "By clicking the 'A. Scale' (Auto Scale) button at left-bottom corner, "
		  "the auto-scale mode is toggled on and off." ) );

  autoScale = true;
  singleScale = false;
  AreaSelecting = false;
  SelLR[ LEFT_AX ] = 0;
  SelLR[ RIGHT_AX ] = 1;

  ShowProgressB = false;
  ProgressScale = 1.0;
  Progress = 0.0;

  Clear();
  valid = false;
  QXafsMode = false;

  showDiff1 = false;
  showDiff2 = false;

  CGroups = FInterval = 100;    // Continuous drawn lines, Interval of foot print lines
  maxGroups = maxLines = inLines = 0;
  cc.SetRealCoord( 0, 0, 1, 1 );
  bgColor = QColor( 255, 255, 255 );
  BLACK = QColor( 0, 0, 0 );
  MCLineC = QColor( 210, 180, 0 );    // mouse cursor line color
  ASelC = QColor( 0, 255, 120 );      // Area Select Color
  ProgressBBC = QColor( 0, 255, 0 );
  ProgressBC = QColor( 220, 220, 0 );
  grouplines = 3;

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

#if 0
void XYView::PostInit( void )
{
  for ( int i = 0; i < MAXLINES; i++ ) {
    SetLR( i, LEFT_AX );
    SetScaleType( i, FULLSCALE );
  }
}
#endif

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
    for ( int i = 0; i < grouplines; i++ ) {
      int a = g0 * grouplines + i;
      int b = g * grouplines + i;
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

  int g = l / grouplines;
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

  int g = l / grouplines;
  int gg = l % grouplines;
  int B = maxGroups / FInterval;
  if ( g > B * FInterval ) {
    L = ( ( g % CGroups ) + B ) * grouplines + gg;
  } else {
    L = ( g / FInterval ) * grouplines + gg;
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

void XYView::print( QPrinter *p )
{
  QPainter pp( p );
  Draw( &pp );
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

  if ( autoScale ) {
    for ( int i = 0; i <= inLines; i++ ) {
      minGy[i] = 1e300;
      maxGy[i] = -1e300;
    }
    for ( int i = 0; i <= inLines; i++ ) {
      UpDateYWindow( i, scaleType[ i ] );  // ループの前に実行する
      int dg = DispGroup[ i ];
      if ( miny[i] < minGy[ dg ] ) minGy[ dg ] = miny[i];
      if ( maxy[i] > maxGy[ dg ] ) maxGy[ dg ] = maxy[i];
    }
  }

  for ( int l = 0; l <= inLines; l++ ) { // 先に線だけ描画
    bool f = ( QXafsMode ) ? dispf[ l % grouplines ] : dispf[ l ];
    if ( f ) {
      ScaleChange( l );
      pen1.setColor( LC[ l % LC.count() ] );
      pen1.setStyle( Qt::SolidLine );
      nowx = cc.s2rx( m.x() );
      nowxp = 0;

      double *diff1 = NULL, *diff2 = NULL;
      double dmin1, dmin2, dmax1, dmax2;
      if ( showDiff1 || showDiff2 ) {
	diff1 = new double [ points[l] ];
	diff2 = new double [ points[l] ];
	Diff2( false, y[l], diff1, diff2, points[l],
	       dwtype1, dwtype2, &dmin1, &dmax1, &dmin2, &dmax2, 0, points[l] );
      }

      for ( int i = 0; i < points[l] - 1; i++ ) {
	if ( linedir[l] * ( x[l][i+1] - x[l][i] ) < 0 ) {
	  linedir[l] *= -1;
	} else {
	  p->setPen( pen1 );
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
      if ( showDiff1 ) {
	pen1.setStyle( Qt::DashLine );
	p->setPen( pen1 );
	for ( int i = WS0; i < points[l] - 1 - WS0; i++ ) {
	  p->drawLine( cc.r2sx( x[l][i] ), cc.fixy( diff1[i], dmin1, dmax1 ),
		       cc.r2sx( x[l][i+1] ), cc.fixy( diff1[i+1], dmin1, dmax1 ) );
	} 
      }
      if ( showDiff2 ) {
	pen1.setStyle( Qt::DashDotLine );
	p->setPen( pen1 );
	for ( int i = ( WS0 + WS0 ); i < points[l] - 1 - ( WS0 + WS0 ); i++ ) {
	  p->drawLine( cc.r2sx( x[l][i] ), cc.fixy( diff2[i], dmin2, dmax2 ),
		       cc.r2sx( x[l][i+1] ), cc.fixy( diff2[i+1], dmin2, dmax2 ) );
	}
      }
      if ( showDiff1 || showDiff2 ) {
	delete [] diff1;
	delete [] diff2;
      }
      SaveYatNowXp[l] = y[l][nowxp];
    }
    for ( int i = 0; i < 2; i++ ) {
      if ( ND[ i ] < 25 ) {
	SelLR[i] =  FindL[i];
      }
    }
  }
  pen1.setStyle( Qt::SolidLine );
  p->setPen( pen1 );
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

  cc.ShowAButton( p, autoScale, tr( "A. Scale" ), 0, 100, height() );
  cc.ShowAButton( p, singleScale, tr( "S. Scale" ), 110, 100, height() );
  cc.ShowAButton( p, showDiff1, tr( "Diff1" ), 220, 100, height() );
  cc.ShowAButton( p, showDiff2, tr( "Diff2" ), 330, 100, height() );

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
    ScaleChange( l );
#if 0
    if ( !singleScale ) {
      if ( autoScale ) {
	UpDateYWindow( l, scaleType[ l ] );
	cc.SetRealY( miny[l], maxy[l] );
      } else {
	cc.SetRealY( miny[l] - YShift[l], maxy[l] - YShift[l] );
      }
    }
#endif
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

  if ( ShowProgressB ) {
    int PBWidth = HW * 0.8;
    int PBHeight = VH * 0.2;
    p->fillRect( LM + HW * 0.1, height() - TM - VH * 0.4,
		 PBWidth * Progress / ProgressScale, PBHeight, ProgressBC );
    p->setPen( ProgressBBC );
    p->drawRect( LM + HW * 0.1, height() - TM - VH * 0.4, PBWidth, PBHeight );
  }
}

void XYView::ScaleChange( int l )
{
  if ( !singleScale ) {
    cc.SetRealY( miny[l] - YShift[l], maxy[l] - YShift[l] );
#if 0
    if ( autoScale ) {
      cc.SetRealY( miny[l], maxy[l] );
    } else {
      cc.SetRealY( miny[l] - YShift[l], maxy[l] - YShift[l] );
    }
#endif
  } else {
    cc.SetRealY( minGy[ DispGroup[l] ] - YShift[l],
		 maxGy[ DispGroup[l] ] - YShift[l] );
#if 0
    if ( autoScale ) {
      cc.SetRealY( minGy[ DispGroup[l] ], maxGy[ DispGroup[l] ] );
    } else {
      cc.SetRealY( minGy[ DispGroup[l] ] - YShift[l],
		   maxGy[ DispGroup[l] ] - YShift[l] );
    }
#endif
  }
}

void XYView::CheckASPush( void )
{
  if ( m.CheckABPush( 0, height() ) ) {
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
	  else 
	    cc.SetRealY( minGy[ DispGroup[l] ], maxGy[ DispGroup[l] ] );
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

  int i;
  double rx = cc.s2rx( m.x() );
  for ( i = 0; i < points[0] - 1; i++ ) {
    if ( rx < ( x[0][i] + x[0][i+1] ) / 2.0 )
      break;
  }
  if ( i < points[0] ) {
    emit MovedToNewX( i, rx );
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
  if ( m.CheckABPush( 110, height() ) )
    singleScale = ! singleScale;
  if ( m.CheckABPush( 220, height() ) )
    showDiff1 = ! showDiff1;
  if ( m.CheckABPush( 330, height() ) )
    showDiff2 = ! showDiff2;

  update();
}

void XYView::mouseDoubleClickEvent( QMouseEvent *e )
{
  m.DClicked( e );

  emit SelectAPoint( cc.s2rx( m.dcx() ), cc.s2ry( m.dcy() ) );
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


// XYView はできたり消えたりするオブジェクトだが、
// 消えたオブジェクトに対して、外部からコネクトしていると問題が発生する。
// 外部で適切に disconnect すれば良いが、面倒なので、
// 親(parent)のポインタをもらって、中から外につなぐ。
// (すると、自分が消えれば登録されている connect の signal 源も消えるので問題ない)

void XYView::setParent( QWidget *p )
{
  parent = p;
  connect( parent, SIGNAL( NewDiff1( int ) ), this, SLOT( setDiffType1( int ) ),
	   Qt::UniqueConnection );
  connect( parent, SIGNAL( NewDiff2( int ) ), this, SLOT( setDiffType2( int ) ),
	   Qt::UniqueConnection );
}
