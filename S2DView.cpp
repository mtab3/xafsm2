
#include <QRectF>
#include <QStylePainter>

#include "S2DView.h"


S2DView::S2DView( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  showIx = showIy = lastIx = lastIy = 0;
  showIx0 = showIy0 = -1;
  data = NULL;
  valid = NULL;
  rType = AS_SCREEN;
  invXf = invYf = false;
  nowRx = nowRy = 0;
  minx = miny = 0;
  maxx = maxy = 1;

  setRange( -10., 10., -10., 10., 2., 2. );
  cc.SetRealCoord( minx, maxx, miny, maxy );

  Grey = QColor( 220, 220, 220 );
  Pink = QColor( 255, 220, 220 );
  AutoScale = true;
  setMouseTracking( true );
}

void S2DView::setRange( double Sx, double Sy, double Dx, double Dy, int ix, int iy )
{
  double ex, ey;
  sx = Sx;
  sy = Sy;
  dx = Dx;
  dy = Dy;

  lastIx = lastIy = showIx = showIy = 0;

  ex = sx + dx * ix;
  ey = sy + dy * iy;
  if ( sx > ex ) { maxx = sx; minx = ex; } else { maxx = ex; minx = sx; }
  if ( sy > ey ) { maxy = sy; miny = ey; } else { maxy = ey; miny = sy; }
  cc.SetRealCoord( minx, maxx, miny, maxy );

  if ( data != NULL ) {
    for ( int i = 0; i < maxix; i++ ) {
      delete [] data[i];
    }
    delete [] data;
  }
  if ( valid != NULL ) {
    for ( int i = 0; i < maxix; i++ ) {
      delete [] valid[i];
    }
    delete [] valid;
  }

  maxix = ix;
  maxiy = iy;
  data = new double * [ maxix + 1 ];        // x, y 両方向に 1ずつ大きく取る
  valid = new bool * [ maxix + 1 ];        // x, y 両方向に 1ずつ大きく取る
  for ( int i = 0; i < maxix + 1; i++ ) {
    data[i] = new double [ maxiy + 1 ];
    valid[i] = new bool [ maxiy + 1 ];
    for ( int j = 0; j < maxiy + 1; j++ ) {
      data[i][j] = 0;
      valid[i][j] = false;
    }
  }
}

void S2DView::setData( int ix, int iy, double v )
{
  if (( ix >= 0 )&&( iy >= 0 )&&( ix < maxix )&&( iy < maxiy )) {
    data[ix][iy] = v;
    valid[ix][iy] = true;
    lastIx = ix;
    lastIy = iy;
    update();
  }
}

double S2DView::getData( int ix, int iy )
{
  if (( ix >= 0 )&&( iy >= 0 )&&( ix < maxix )&&( iy < maxiy )) {
    if ( valid[ix][iy] )
      return data[ix][iy];
  }
  return 0;
}

void S2DView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void S2DView::print( QPrinter *p )
{
  QPainter pp( p );
  Draw( &pp );
}
 
void S2DView::Draw( QPainter *p )
{
  QColor White( 255, 255, 255 );
  QColor GridC( 128, 128, 128 );

  int w = width();
  int h = height();
  
  int LM = w * 0.25;        // 画面を描くときの基準になる定数を幾つか決めておく
  if ( LM > 180 ) LM = 250;
  int RM = w * 0.05;
  if ( RM > 80 ) RM = 80;
  int HW = w - LM - RM;
  int TicL = RM / 10;
  if ( TicL < 2 )
    TicL = 2;
  if ( TicL > 5 )
    TicL = 5;

  int TM = h * 0.05;
  if ( TM > 40 ) TM = 40;
  int BM = h * 0.1;
  if ( BM > 80 ) BM = 80;
  int VW = h - TM - BM;
  //  double dLM = LM / 10;
  double dVW = VW / 20;     // 1行の高さ(文字の高さ)
  if ( dVW > TM )
    dVW = TM * 0.8;
  double dVW2 = dVW * 1.2;  // 行間

  double screen_w = HW;
  double screen_h = VW;
  double real_w = maxx - minx;
  double real_h = maxy - miny;
  switch ( rType ) {
  case REAL_RATIO:
    if ( ( screen_w / real_w * real_h ) > screen_h ) {
      // (HW x VW) のスクリーンに横を合わせると縦がはみ出るなら
      // 縦を元々の VW のままにして、横(HW)を定義し直す
      HW = screen_h / real_h * real_w;
      // LM = ( w - HW ) / 2;
    } else {
      // (HW x VW) のスクリーンに横を合わせると縦がはみ出ないなら
      // 横を元々の HW のままにして、縦(VM)を定義し直す
      VW = screen_w / real_w * real_h;
      // TM = ( h - VW ) / 2;
    }
    break;
  case AS_SCREEN:
  default:
    break;
  }
  RM = w - LM - HW;
  TM = h - BM - VW;
  cc.SetScreenCoord( LM, h-BM-VW, LM+HW, h-BM );
  if ( ! invXf )
    cc.SetRealX( minx, maxx );
  else
    cc.SetRealX( maxx, minx );
  if ( ! invYf )
    cc.SetRealY( miny, maxy );
  else
    cc.SetRealY( maxy, miny );
  
  p->fillRect( 0, 0, w, h, White );
  p->setPen( GridC );
  p->drawRect( cc.r2sx( minx ), cc.r2sy( maxy ),
	       cc.r2sdx( maxx - minx ), cc.r2sdy( maxy - miny ) );

  cc.ShowAButton( p, true, tr( "Int. MCA" ), 0, 100, height() );
  cc.ShowAButton( p, invXf, tr( "Inv. X" ), 0, 100, height() - 40 );
  cc.ShowAButton( p, invYf, tr( "Inv. Y" ), 0, 100, height() - 20 );
  
  if ( AutoScale ) {  // 表示色の範囲をオートで決めるための最大、最小値検索
    minz = 1e300;
    maxz = -1e300;
    for ( int ix = 0; ix < maxix; ix++ ) {
      for ( int iy = 0; iy < maxiy; iy++ ) {
	if ( valid[ix][iy] ) {
	  if ( data[ix][iy] < minz ) minz = data[ix][iy];
	  if ( data[ix][iy] > maxz ) maxz = data[ix][iy];
	}
      }
    }
  }
  emit newAutoZmax( maxz );
  emit newAutoZmin( minz );

  double rx1, rx2, ry1, ry2;
  double ssx, ssy, sdx, sdy;
  //  bool inRange = false;
  ssx = ssy = sdx = sdy = 0;
  int x0, y0, xd, yd;
  int x1, x2, y1, y2;
  for ( int ix = 0; ix < maxix; ix++ ) {
    for ( int iy = 0; iy < maxiy; iy++ ) {
      x1 = cc.r2sx( rx1 = ( sx + dx * ix ) );
      x2 = cc.r2sx( rx2 = ( sx + dx * ix + dx ) );
      y1 = cc.r2sy( ry1 = ( sy + dy * iy ) );
      y2 = cc.r2sy( ry2 = ( sy + dy * iy + dy ) );
      x0 = ( x1 < x2 ) ? x1 : x2;
      y0 = ( y1 < y2 ) ? y1 : y2;
      xd = abs( x1 - x2 );
      yd = abs( y1 - y2 );
      if ( valid[ix][iy] ) 
	p->fillRect( x0, y0, xd, yd, *(CBar->c( data[ix][iy] )) );
      else 
	p->fillRect( x0, y0, xd, yd, Grey );
    }
  }
  
  p->setPen( QColor( 0, 0, 0 ) );
  // 格子
  for ( int ix = 0; ix <= maxix; ix++ ) {
    p->drawLine( cc.r2sx( sx + dx * ix ), cc.r2sy( maxy ),
		 cc.r2sx( sx + dx * ix ), cc.r2sy( miny ) );
  }
  for ( int iy = 0; iy <= maxiy; iy++ ) {
    p->drawLine( cc.r2sx( minx ), cc.r2sy( sy + dy * iy ),
		 cc.r2sx( maxx ), cc.r2sy( sy + dy * iy ) );
  }

  QPen p1;
  //  p1.setWidth( 2 );
  p1.setColor( Pink );
  p->setPen( p1 );
  p->drawEllipse( cc.r2sx( nowRx ) - 3, cc.r2sy( nowRy ) - 3,
		  7, 7 );
  p1.setWidth( 1 );
  p1.setColor( QColor( 0, 0, 0 ) );
  p->setPen( p1 );
  
  // 軸反転した時のレイアウトの向きの補正用
  int xdir = ( invXf ) ? -1 : 1;
  int ydir = ( invYf ) ? -1 : 1;
  // 格子の中央点からのヒゲ
  for ( int ix = 0; ix < maxix; ix++ ) {
    p->drawLine( cc.r2sx( sx + dx * ( ix + 0.5 ) ), cc.r2sy( maxy ),
		 cc.r2sx( sx + dx * ( ix + 0.5 ) ), cc.r2sy( maxy )-TicL * ydir );
    p->drawLine( cc.r2sx( sx + dx * ( ix + 0.5 ) ), cc.r2sy( miny ),
		 cc.r2sx( sx + dx * ( ix + 0.5 ) ), cc.r2sy( miny )+TicL * ydir);
  }
  for ( int iy = 0; iy < maxiy; iy++ ) {
    p->drawLine( cc.r2sx( minx ), cc.r2sy( sy + dy * ( iy + 0.5 ) ),
		 cc.r2sx( minx )-TicL * xdir, cc.r2sy( sy + dy * ( iy + 0.5 ) ) );
    p->drawLine( cc.r2sx( maxx ), cc.r2sy( sy + dy * ( iy + 0.5 ) ),
		 cc.r2sx( maxx )+TicL * xdir, cc.r2sy( sy + dy * ( iy + 0.5 ) ) );
  }

  // 軸反転した時のレイアウトの補正用
  int xbase = cc.r2sx( ( invXf ) ? minx : maxx ) + 5;
  int ybase = cc.r2sy( ( invYf ) ? maxy : miny ) + 5;
  // メモリ数字
  QRectF rec;
  QFont F1;
  F1.setPointSizeF( 10 );
  // 横軸
  rec = QRectF( cc.r2sx( sx+dx*0.5 )-HW/4, ybase, HW/2, dVW );
  cc.DrawText( p, rec, F1, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE,
	       QString::number( sx + dx * 0.5 ) );
  rec = QRectF( cc.r2sx( sx+dx*(maxix-0.5) )-HW/4, ybase, HW/2, dVW );
  cc.DrawText( p, rec, F1, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE,
	       QString::number( sx + dx * (maxix - 0.5) ) );
  // 縦軸
  rec = QRectF( xbase, cc.r2sy( sy+dx*0.5 )-dVW2/2, RM - 5, dVW );
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
	       QString::number( sy + dy * 0.5 ) );
  rec = QRectF( xbase, cc.r2sy( sy+dx*(maxiy-0.5) )-dVW2/2, RM - 5, dVW );
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
	       QString::number( sy + dy * ( maxiy - 0.5 ) ) );

  QStringList Infos;
  Infos << tr( "Measured : (%1, %2)" )    // 現在の測定位置表示
    .arg( sx + dx * ( lastIx + 0.5 ) ).arg( sy + dy * lastIy );
  Infos << tr( "Intensity : %1" )         // 現在の測定値表示
    .arg( valid[lastIx][lastIy] ?
	  QString::number( data[lastIx][lastIy] ) : QString( "--" ) );
  Infos << tr( "Pinted : (%1, %2)" )      // カーソルがある位置の表示
    .arg( sx + dx * ( showIx + 0.5 ) ).arg( sy + dy * showIy );
  Infos << tr( "Intensity : %1" )         // カーソルがある位置の測定値
    .arg( valid[showIx][showIy] ?
	  QString::number( data[showIx][showIy] ) : QString( "--" ) );
  Infos << tr( "Max Int. : %1" )          // 最大値
    .arg( ( maxz > -0.9e300 ) ?
	  QString::number( maxz ) : QString( "--" ) );
  Infos << tr( "Min Int. : %1" )          // 最小値
    .arg( ( minz < 0.9e300 ) ?
	  QString::number( minz ) : QString( "--" ) );

  rec = QRectF( 10, 10, LM-20, dVW );
  cc.DrawTexts( p, rec, 0, dVW2, F1, Qt::AlignLeft | Qt::AlignVCenter, FIXSIZE, Infos );
}

void S2DView::mouseMoveEvent( QMouseEvent *e )
{
  rangeoutf = false;

  m.Moved( e );

  if (( cc.s2rx( m.x() ) < sx )||( cc.s2ry( m.y() ) < sy )) { rangeoutf = true; }
  showIx = ( int )( ( cc.s2rx( m.x() ) - sx ) / dx );
  showIy = ( int )( ( cc.s2ry( m.y() ) - sy ) / dy );

  if ( showIx < 0 ) { showIx = 0; rangeoutf = true; }
  if ( showIx >= maxix ) { showIx = maxix - 1; rangeoutf = true; }
  if ( showIy < 0 ) { showIy = 0; rangeoutf = true; }
  if ( showIy >= maxiy ) { showIy = maxiy - 1; rangeoutf = true; }

  if ((( showIx != showIx0 )||( showIy != showIy0 ))&&(!rangeoutf)) {
    emit PointerMovedToNewPosition( showIx, showIy );
  }

  if ( rangeoutf ) {
    showIx0 = showIy0 = -1;
  } else {
    showIx0 = showIx;
    showIy0 = showIy;
  }

  update();
}

void S2DView::mousePressEvent( QMouseEvent * )
{
  if ( !rangeoutf )
    emit AskMoveToPointedPosition( showIx, showIy );
}

void S2DView::CheckOnIntMCA( QMouseEvent *e )
{
  if ( m.CheckABPosition( e, 0, height() ) ) {
    emit PointerMovedOnIntMCA();
#if 0
    for ( int iy = maxiy-1; iy >= 0; iy-- ) {     // 最後の1点を見つける
      for ( int ix = maxix-1; ix >= 0; ix-- ) {
	if ( valid[ix][iy] ) {
	  emit PointerMovedOnIntMCA( ix, iy );
	  return;
	}
      }
    }
#endif
  }
}

void S2DView::CheckIXBPush( QMouseEvent *e )
{
  if ( m.CheckABPosition( e, 0, height() - 40 ) )
    invXf = !invXf;
  
  update();
}

void S2DView::CheckIYBPush( QMouseEvent *e )
{
  if ( m.CheckABPosition( e, 0, height() - 20 ) )
    invYf = !invYf;

  update();
}


void S2DView::mouseReleaseEvent( QMouseEvent *e )
{
  m.Released( e );

  CheckOnIntMCA( e );
  CheckIXBPush( e );
  CheckIYBPush( e );
}

void S2DView::mouseDoubleClickEvent( QMouseEvent * )
{
}

void S2DView::wheelEvent( QWheelEvent *e )
{
  int step = ( e->delta() / 8. ) / 15.;    // deg := e->delta / 8.
  emit AskToChangeMCACh( step );
}

void S2DView::setNowPosition( int ax, double pos )
{
  switch( ax ) {
  case 0: nowRx = pos; break;
  case 1: nowRy = pos; break;
  default: break;
  }
}
