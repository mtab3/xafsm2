#include <QtGui>

#include "XafsM.h"
#include "TYView.h"

TYView::TYView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  setToolTip( tr( "By clicking the 'A. Scale' (Auto Scale) button at left-bottom corner, "
		  "the auto-scale mode is toggled on and off." ) );

  valid = false;
  cc.SetRealCoord( 0, 0, 1, 1 );
  bgColor = QColor( 255, 255, 255 );
  BLACK = QColor( 0, 0, 0 );
  MCLineC = QColor( 210, 180, 0 );     // mouse cursor line color
  for ( int i = 0; i < MaxMon; i++ ) {
    LNames << "";
  }
  autoScale = true;
  AreaSelecting = false;
  ASelC = QColor( 0, 255, 120 );      // Area Select Color

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
  MonScale = 0;
  ep = 0;
  datas = 0;
  timeShift = 0;
  timeShift0 = 0;
  tts = 0;
  for ( int i = 0; i < MaxMon; i++ ) {
    YShift[ i ] = YShift0[ i ] = yshift[ i ] = 0;
  }
}

void TYView::ClearDataR( void )
{
  for ( int j = 0; j < RingMax; j++ ) {
    for ( int i = 0; i < MaxMon; i++ ) {
      mony[ i ][ j ] = 0;
    }
    mont[ j ] = 0;
  }
  ep = 0;
  datas = 0;
}

// リングバッファへのデータ追加
void TYView::NewPointR( int tt, double yy0, double yy1, double yy2 )
{
  mont[ ep ] = tt;
  mony[0][ ep ] = yy0;
  mony[1][ ep ] = yy1;
  mony[2][ ep ] = yy2;
  ep++;
  if ( ep >= RingMax )
    ep = 0;
  if ( datas < RingMax )
    datas++;
}

void TYView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void TYView::Draw( QPainter *p )
{
  if ( valid != true ) return;

  QString buf, buf2;
  double RM, LM, TM, BM, HDiv, VDiv;
  QPen pen0, pen1;
  QFont F1;
  QRectF rec;
  int ms = MScales[ MonScale ].div * 1000;   // １グリッドの時間 秒 x 1000 = ミリ秒

  double HDNum = 6;    // 水平方向のグリッド数
  double VDNum = 10;   // 垂直方向のグリッド数

  if ( autoScale )
    cc.SetRealCoord( -ms * HDNum, 0, 0, 1 );   // 実座標の指定
                                               // 縦方向は、この時点では [0,1]

  p->fillRect( 0, 0, width(), height(), bgColor ); // 背景全体の塗りつぶし

  RM = width() * 0.03;    // 描画領域の中でのグラフの右マージン
  LM = width() * 0.12;    // 描画領域の中でのグラフの左マージン
  TM = height() * 0.05;   // 描画領域の中でのグラフの上(top)マージン
  BM = height() * 0.10;   // 描画領域の中でのグラフの下(bottom)マージン
  HDiv = ( width() - RM - LM ) / HDNum;     // グリッド幅
  VDiv = ( height() - TM - BM ) / VDNum;    // グリッド高さ

  cc.SetScreenCoord( LM, TM, width()-RM, height()-BM );
  // screen 座標の指定。左上、右下の座標指定
  // (ここで上下裏がえった座標を指定することで real 座標で普通に左下が 0,0 の座標になる)

  if ( autoScale )
    // 最大値最小値(5%マージン)を探す (Rminy, Rmaxy に返す)
    UpDateYWindowRing();

  for ( int j = 0; j < lines; j++ ) {
    if ( autoScale )
      cc.SetRealY( Rwminy[j], Rwmaxy[j] );
    else
      cc.SetRealY( Rwminy[j] - YShift[j], Rwmaxy[j] - YShift[j] );

    int t0 = mont[ ( ep == 0 ) ? RingMax - 1 : ep - 1 ];  // 最新時刻
    int pp1, pp2;
    pen1.setWidth( 2 );
    pen1.setColor( LC[ j ] );
    p->setPen( pen1 );

    for ( int i = 0; i < datas - 1; i++ ) { // データプロット
      pp1 = ep - 1 - i;
      pp2 = ep - 1 - ( i + 1 );
      if ( pp1 < 0 ) pp1 += RingMax;
      if ( pp2 < 0 ) pp2 += RingMax;
      if ( ( t0 - mont[ pp2 ] ) > ( ms * 6 + timeShift ) ) {
	break;
      }
      if ( ( t0 - mont[ pp2 ] ) > timeShift ) {
	p->drawLine( cc.r2sx( mont[pp1] - t0 + timeShift ), cc.r2sy( mony[j][pp1] ),
		     cc.r2sx( mont[pp2] - t0 + timeShift ), cc.r2sy( mony[j][pp2] ) );
      }
    }
  }
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

  pen0.setWidth( 1 );
  pen0.setColor( QColor( 0, 0, 0 ) );
  p->setPen( pen0 );
  p->setFont( F1 );
  p->drawRect( LM, TM, width()-RM-LM, height()-BM-TM );  // グラフの枠線

  double sx, dx;
  if ( !autoScale ) {
    cc.calcScale( 10, cc.Rminx(), cc.Rmaxx(), &sx, &dx );
  } else {
    sx = cc.Rminx();
    dx = ms;
  }
  for ( double xx = sx; xx <= cc.Rmaxx(); xx += dx ) {   // 横方向方向
    p->drawLine( cc.r2sx( xx ), TM, cc.r2sx( xx ), height()-BM );  // 縦罫線
                                                                   // 横軸のメモリ数字
    rec = QRect( cc.r2sx( xx )-HDiv/2, height()-BM*0.95, HDiv, BM*0.4 );
    cc.DrawText( p, rec, F1, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE, 
		 QString( tr( "%1" ) )
		 .arg( ((double)timeShift/ms * MScales[MonScale].dispDiv)
		       + (int)( (cc.Rmaxx()-xx)/ms * MScales[ MonScale ].dispDiv ) ) );
  }
  rec = QRect( width()-VDiv, height()-BM*0.5, RM*0.9, BM*0.4 );  // 横軸のラベル
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
	       MScales[ MonScale ].unit );

  pen1.setWidth( 1 );
  pen1.setColor( BLACK );
  p->setPen( pen1 );
  cc.SetRealY( 0, 1 );
  for ( double yy = 0; yy < 1; yy += 0.1 ) {
    p->drawLine( LM, cc.r2sy( yy ), width()-RM, cc.r2sy( yy ) );   // 横の罫線
  }

  double sy, ey, dy, ty;
  if ( autoScale )
    // 最大値最小値(5%マージン)を探す (Rminy, Rmaxy に返す)
    UpDateYWindowRing();

  for ( int j = 0; j < lines; j++ ) {
    if ( autoScale )
      cc.SetRealY( Rwminy[j], Rwmaxy[j] );
    else
      cc.SetRealY( Rwminy[j] - YShift[j], Rwmaxy[j] - YShift[j] );
    cc.getSEDy( &sy, &ey, &dy, 5 );
    pen1.setWidth( 1 );
    pen1.setColor( LC[ j ] );
    p->setPen( pen1 );

    for ( double yy = sy + dy * 0.5; yy <= cc.Rmaxy(); yy += dy ) {
      rec = QRectF( LM * 0.1, ty = ( cc.r2sy( yy ) - VDiv * 0.5 + VDiv * 0.45 * j ),
		    LM * 0.75, VDiv * 0.42 ); // メモリ数字
      buf.sprintf( "%6.4g", yy );
      cc.DrawText( p, rec, F1, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, buf );
      p->drawLine( LM * 0.88, ty + VDiv * 0.21, LM * 0.98, cc.r2sy( yy ) );
    }
    rec = QRectF( LM + HDiv * 0.1 + HDiv * 2 * j, TM * 0.05, 
		  HDiv * 2, TM * 0.9 );  // 軸のラベル
    
    int t0 = mont[ ( ep == 0 ) ? RingMax - 1 : ep - 1 ];  // 最新時刻
    int nowt = cc.s2rx( m.x() ) + t0 - timeShift;
    int nowtp = 0;

    int pp1, pp2;
    pen1.setWidth( 2 );
    pen1.setColor( LC[ j ] );
    p->setPen( pen1 );

    for ( int i = 0; i < datas - 1; i++ ) { // データプロット
      pp1 = ep - 1 - i;
      pp2 = ep - 1 - ( i + 1 );
      if ( pp1 < 0 ) pp1 += RingMax;
      if ( pp2 < 0 ) pp2 += RingMax;
      if ( ( t0 - mont[ pp2 ] ) > ( ms * 6 + timeShift ) ) {
	break;
      }
      // マウスポインタの指す時刻に出来るだけ近い実測点
      if (( mont[pp1] >= nowt )&&( mont[pp2] < nowt ))
	nowtp = pp1;
    }
    cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 LNames[j] + " : " + QString::number(mony[j][nowtp]) );
  }

  cc.ShowAScaleButton( p, autoScale, height() );

  // マウスポインタの位置に縦線を引く
  if ( ( m.x() > LM ) && ( m.x() < width()-RM ) ) {
    p->setPen( MCLineC );
    p->drawLine( m.x(), TM, m.x(), height()-BM );
  }
}

void TYView::UpDateYWindowRing( void )
{
  int dx = MScales[ MonScale ].div;
  int p, t0;

  t0 = mont[ ( ep == 0 ) ? RingMax -1 : ep - 1 ];

  for ( int j = 0; j < lines; j++ ) {
    double nmaxy = -1e300;
    double nminy = 1e300;
    for ( int i = 0; i < datas; i++ ) {
      p = ep - 1 - i;
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

void TYView::CheckASPush( void )
{
  if ( ( m.ex() > 5 )&&( m.ex() < 19 )
       &&( m.ey() > ( height() - 19 ) )&&( m.ey() < ( height() - 5 ) ) ) {
    if ( autoScale ) {
      autoScale = false;
    } else {
      autoScale = true;
      timeShift = timeShift0 = tts = 0;
      for ( int i = 0; i < lines; i++ ) {
	YShift[i] = YShift0[i] = yshift[i] = 0;
      }
    }
  }
}

/*******************************************************************/

void TYView::mouseMoveEvent( QMouseEvent *e )
{
  m.Moved( e );

  switch( m.modifier() ) {
  case Qt::NoModifier:    // 平行移動
    if ( m.inPress() ) {
      tts = cc.s2rx0( m.x() ) - cc.s2rx0( m.sx() );
      timeShift = timeShift0 + tts;
      
      if ( !autoScale ) {
	for ( int i = 0; i < lines; i++ ) {
	  cc.SetRealY( Rwminy[i], Rwmaxy[i] );
	  yshift[i] = cc.s2ry0( m.y() ) - cc.s2ry0( m.sy() );
	  YShift[i] = YShift0[i] + yshift[i];
	}
      }
    }
    break;
  case Qt::ShiftModifier:  // 領域拡大
    break;
  }
  update();
}

void TYView::mousePressEvent( QMouseEvent *e )
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

void TYView::mouseReleaseEvent( QMouseEvent *e )
{
  double dummy;
  double nminx, nmaxx;
  double nminy, nmaxy;

  m.Released( e );

  switch( m.modifier() ) {
  case Qt::NoModifier:                          // 平行移動
    if ( !autoScale ) {
      for ( int i = 0; i < lines; i++ ) {
	YShift0[i] += yshift[i];
	yshift[i] = 0;
      }
    }
    timeShift0 += tts;
    tts = 0;
    if ( autoScale ) {
      if ( timeShift0 < 0 ) {
	timeShift0 = 0;
	timeShift = 0;
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
      //      cc.SetRealX0();
      for ( int i = 0; i < MaxMon; i++ ) {
	cc.SetRealY( Rwminy[i], Rwmaxy[i] );
	nminy = cc.s2ry( m.sy() );
	nmaxy = cc.s2ry( m.ey() );
	if ( nminy > nmaxy ) {
	  dummy = nminy;
	  nminy = nmaxy;
	  nmaxy = dummy;
	}
	Rwminy[i] = nminy;
	Rwmaxy[i] = nmaxy;
      }
    }
    break;
  }
  CheckASPush();

  update();
}

void TYView::mouseDoubleClickEvent( QMouseEvent * )
{
}

void TYView::wheelEvent( QWheelEvent *e )
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

  for ( int i = 0; i < lines; i++ ) {
    cc.SetRealY( Rwminy[i], Rwmaxy[i] );
    double ry = cc.s2ry( e->y() );
    double dry = cc.Rmaxy() - cc.Rminy();
    if ( step > 0 ) {
      dry *= 0.9;
    } else {
      dry /= 0.9;
    }
    double nmaxy = ry + ( e->y() - cc.Sminy() ) / ( cc.Smaxy() - cc.Sminy() ) * dry;
    double nminy = nmaxy - dry;
    Rwminy[i] = nminy;
    Rwmaxy[i] = nmaxy;
  }
  update();
}

