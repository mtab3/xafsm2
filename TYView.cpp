
#include <QStylePainter>

#include <math.h>

#include "XafsM.h"
#include "TYView.h"

TYView::TYView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );
  setParent( parent );

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

void TYView::SetLName( int i, QString Name )
{
  if ( i < LNames.count() )
    LNames[i] = Name;
}


void TYView::setParent( QWidget *p )
{
  parent = p;
  if ( parent != NULL ) {
    connect( this, SIGNAL( UpScale() ), parent, SLOT( TYVUpScale() ),
	     Qt::UniqueConnection );
    connect( this, SIGNAL( DownScale() ), parent, SLOT( TYVDownScale() ),
	     Qt::UniqueConnection );
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
void TYView::NewPointR( int tt, double *yy, int n )
{
  mont[ ep ] = tt;
  for ( int i = 0; i < n; i++ ) {
    mony[i][ ep ] = yy[i];
  }
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

void TYView::print( QPrinter *p )
{
  QPainter pp( p );
  Draw( &pp );
}

void TYView::Draw( QPainter *p )
{
  if ( valid != true ) return;

  QString buf, buf2;
  double RM, LM, TM, TW, BM, HDiv, VDiv;
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

  int topLines = ((int)(( lines - 1 )/ 5 )) + 1;
  int topClms = lines;
  if ( topClms > 5 )
    topClms = 5;
  
  RM = width() * 0.03;    // 描画領域の中でのグラフの右マージン
  LM = width() * 0.12;    // 描画領域の中でのグラフの左マージン
  TM = height() * 0.05 * ( topLines + 0.1 );   // 描画領域の中でのグラフの上(top)マージン
  BM = height() * 0.10;   // 描画領域の中でのグラフの下(bottom)マージン
  TW = width() - RM - LM;
  HDiv = TW / HDNum;     // グリッド幅
  VDiv = ( height() - TM - BM ) / VDNum;    // グリッド高さ

  int topLW = TW / topClms;
  int topLH = ( TM * 0.9 ) / topLines;
  
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
    
    double now1, now2;
    for ( int i = 0; i < datas - 1; i++ ) { // データプロット
      pp1 = ep - 1 - i;
      pp2 = ep - 1 - ( i + 1 );
      if ( pp1 < 0 ) pp1 += RingMax;
      if ( pp2 < 0 ) pp2 += RingMax;
      if ( ( t0 - mont[ pp2 ] ) > ( ms * 6 + timeShift ) ) {
	break;
      }
      if ( ( t0 - mont[ pp2 ] ) > timeShift ) {
	now1 = ( !logScale ) ? mony[j][pp1] : log10( fabs( mony[j][pp1] ) );
	now2 = ( !logScale ) ? mony[j][pp2] : log10( fabs( mony[j][pp2] ) );
	p->drawLine( cc.r2sx( mont[pp1] - t0 + timeShift ), cc.r2sy( now1 ),
		     cc.r2sx( mont[pp2] - t0 + timeShift ), cc.r2sy( now2 ) );
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

  pen1.setWidth( 1 );
  pen1.setColor( BLACK );
  p->setPen( pen1 );
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

  double sy, ey, dy, ty;
  if ( autoScale )
    // 最大値最小値(5%マージン)を探す (Rminy, Rmaxy に返す)
    UpDateYWindowRing();

  int nearLine = 0;
  double nearD = 10000000;
  for ( int j = 0; j < lines; j++ ) {
    if ( !logScale ) {   // 縦軸リニアスケールの時
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
	if ( !logScale ) {
	  buf.sprintf( "%6.4g", yy );
	} else {
	  buf.sprintf( "%6.4e", yy );
	}
	cc.DrawText( p, rec, F1, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, buf );
	p->drawLine( LM * 0.88, ty + VDiv * 0.21, LM * 0.98, cc.r2sy( yy ) );
      }
    }

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
    int TL = j % 5;
    rec = QRectF( LM + HDiv * 0.05 + topLW * j, TM * 0.05 + TL, 
		  topLW, TM * 0.9 );  // 軸のラベル
    cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 LNames[j] + " : " + QString::number(mony[j][nowtp]) );

    if ( logScale ) {  // log スケールの罫線を引くための下準備
      if ( fabs( cc.r2sy( mony[j][nowtp] ) - m.y() ) < nearD ) {
	nearD = fabs( cc.r2sy( mony[j][nowtp] ) - m.y() );
	nearLine = j;
      }
    }
  }

  pen1.setWidth( 1 );
  pen1.setColor( BLACK );
  p->setPen( pen1 );
  if ( !logScale )  {   // 縦軸がリニアスケールの時、軸メモリは描画済み。罫線だけ
    cc.SetRealY( 0, 1 );
    for ( double yy = 0; yy < 1; yy += 0.1 ) {
      p->drawLine( LM, cc.r2sy( yy ), width()-RM, cc.r2sy( yy ) );   // 横の罫線
    }
  } else {   // 縦軸が log スケールの時、軸メモリと罫線の描画
    double sy = Rwminy[ nearLine ];
    double ey = Rwmaxy[ nearLine ];
    int isy = floor( sy );    // 最小の数字のlog10に満たない最大の整数
    int iey = ceil( ey );    // 最大の数字のlog10より大きい大小の整数
    bool lineFirst = true;
    bool numFirst = true;
    double oldbottom = 0;
    QRectF brec;
    double scale = 1;
    while ( lineFirst && ( scale > 1e-5 ) ) { // 1e-5 にしてるけど、実際には -2 でも十分
      for ( double y = (double)iey; y >= (double)isy; y -= 1. ) {
	for ( double yy = 10; yy >= 1; yy -= scale ) {
	  double lyy = log10( yy );
	  if (( ( y + lyy ) >= sy )&&( ( y + lyy ) <= ey )) {
	    // メモリの数字を描く予定の箱
	    rec = QRectF( LM * 0.1, ty = ( cc.r2sy( y + lyy ) - VDiv * 0.5 ),
			  LM * 0.75, VDiv );
	    // この箱が前に書いた数字と重なっていなければ、メモリの数字を描く
	    if (( numFirst )||( oldbottom < rec.top() )) {
	      double ry = pow( 10, y + lyy );
	      int y1 = floor( log10( ry ) );
	      double y2 = ry / pow( 10, y1 );
	      buf.sprintf( "%3.1fx10^%d", y2, y1 );
	      brec = cc.DrawText( p, rec, F1, Qt::AlignRight | Qt::AlignVCenter,
				  SCALESIZE, buf );
	      oldbottom = ( rec.bottom() + rec.top() ) / 2. + brec.height() / 2.;
	      numFirst = false;
	      // メモリの数字を書いたところにはヒゲ線を入れる
	      p->drawLine( LM * 0.9, cc.r2sy( y+lyy ), LM, cc.r2sy( y+lyy ) );
	    }
	    // 横の罫線
	    p->drawLine( LM, cc.r2sy( y+lyy ), width()-RM, cc.r2sy( y+lyy ) );
	    lineFirst = false;
	  }
	}
      }
      scale *= 0.1;
    }
  }

  cc.ShowAButton( p, autoScale, tr( "A. Scale" ),   0, 100, height() );
  cc.ShowAButton( p, logScale,  tr( "L. Scale" ), 110, 100, height() );

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
    double test;
    for ( int i = 0; i < datas; i++ ) {
      p = ep - 1 - i;
      if ( p < 0 ) p += RingMax;
      if ( ( (  mont[ p ] - t0 ) > ( - dx * 6 * 1000 - timeShift ) )
	   &&( ( mont[ p ] - t0 ) < ( -timeShift ) ) ){
	test = ( ( !logScale ) ? mony[j][p] : log10( fabs( mony[j][p] ) ) );
	if ( test < nminy )
	  nminy = test;
	if ( test > nmaxy ) 
	  nmaxy = test;
      }
    }
    double dy = nmaxy - nminy;
    if ( dy == 0 )
      dy = 1.;
    Rwminy[j] = nminy - dy * 0.05;
    Rwmaxy[j] = nmaxy + dy * 0.05;
  }
}

void TYView::CheckASPush( QMouseEvent *e )
{
  if ( m.CheckABPosition( e, 0, height() ) ) {
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

void TYView::CheckLSPush( QMouseEvent *e )
{
  if ( m.CheckABPosition( e, 110, height() ) ) {
    logScale = !logScale;
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
  CheckASPush( e );
  CheckLSPush( e );

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

  if ( autoScale ) {
    if ( step > 0 ) {
      emit DownScale();
    } else {
      emit UpScale();
    }
  } else {
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
  }
  update();
}

