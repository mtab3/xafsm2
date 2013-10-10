#include "MainWindow.h"

// SSD を使った連続スキャンに限る
// nct08 も同じことができるはず
void MainWindow::S2DQuasiContinuousScanSequence( void )
{
  // 1番目の軸と、2, 3番目の軸は、ステップの考え方が違う。
  // 1番目の軸は、測定中に次の点に移動するように制御しているので
  // s: -10, e: -10, periods 10 (step 2) とすると、
  //    -10〜-8, -8〜-6, ... 8〜10 と 10回測定 + 移動する。
  // 2番目の軸は、同じく
  // s: -10, e: -10, periods 10 (step 2) とすると、
  //    -10, -8, -6, ... 8, 10 と 10回移動して11回測定する。
  // このため、1番目の軸と、2番目の軸を同じにしても、正方形の測定にならない。
  // もし同じにしたければ
  // 1番目の軸:: s: -11, e: -11, periods 11 (step 2) 
  // 2番目の軸:: s: -10, e: -10, periods 10 (step 2) 
  // の様に、1番目の軸の指定と2番目の軸の指定を、半ステップずらす必要がある。

  QString msg;

  // モータ駆動中は入ってこない (とりあえずステップのことだけ考える)
  for ( int i = 0; i < S2DI.motors; i++ ) {
    if ( S2DI.used[i] && S2DI.unit[i]->isBusy0() )
      return;
  }
  // センサー busy でも入ってこない 
  // 但し、一旦測定を始めてしまうと検出器はずっと busy なので、内部の busy2 だけチェック
  if ( S2DStage < 2 ) {
    if ( mUnits.isBusy() )
      return;
  } else {
    if ( mUnits.isBusy2() )
      return;
  }

#if 0
  qDebug() << "Stage " << S2DStage
	   << QDateTime::currentDateTime().toString("yy.MM.dd hh:mm:ss.zzz");
#endif

  int pps;
  switch( S2DStage ) {
  case 0:
    // 検出器初期化
    if ( mUnits.init() ) // true :: initializing
      break;
    mUnits.clearStage();
    S2DStage++;
    break;
  case 1:
    if ( mUnits.getValue02() )
      break;
    mUnits.clearStage();
    S2DStage++;
    break;
  case 2:
    // 全軸に対して
    for ( int i = 0; i < S2DI.motors; i++ ) {
      if ( S2DI.used[i] ) {
	S2DI.unit[i]->SetHighSpeed( S2DI.unit[i]->highestSpeed() ); // スピードマックス
	S2DI.unit[i]->SetSpeed( HIGH );                          // スピードマックス
	S2DI.unit[i]->SetValue( S2DI.unit[i]->u2p( S2DI.sx[i] ) ); // 始点に移動
	S2DI.i[i] = 0; // ステップコントロール変数初期化
      }
    }
    S2DStage++;
    break;
  case 3:
    // 1st Ax のみ、スキャン用のスピードにセット
    pps = (int)fabs( (double)S2DI.dx[0]
		     / S2DI.unit[0]->getUPP()
		     / S2DI.Dwell );
    if ( pps == 0 ) pps = 1;
    if ( pps > S2DI.unit[0]->highestSpeed() ) {
      msg = tr( "The scan speed %1 was limited to %2" )
	.arg( pps ).arg( S2DI.unit[0]->highestSpeed() );
      qDebug() << msg;
      statusbar->showMessage( msg, 2000 );
      pps = S2DI.unit[0]->highestSpeed();
    }
    S2DI.unit[0]->SetHighSpeed( pps );
    S2DStage++;
    // break しない
  case 4:     // リピートポイント
    // 計測開始準備
    mUnits.getValue();
    // 同時に次の点に移動開始
    if ( S2DI.i[0] < S2DI.ps[0] ) {
      if ( S2DScanDir == FORWARD ) {
	S2DI.unit[0]->SetValue( S2DI.unit[0]
				->u2p( S2DI.sx[0] + (S2DI.i[0]+1)*S2DI.dx[0] ) );
      } else {
	S2DI.unit[0]->SetValue( S2DI.unit[0]
				->u2p( S2DI.ex[0] - (S2DI.i[0]+1)*S2DI.dx[0] ) );
      }
    }
    S2DStage++;
    break;
  case 5:
    // 計測値読み取り
    // 連続測定なので、各ラインの最初の値は、それまでの積分値になる。
    // di0 = 0, 1, 2, ... 10 と 11 点の計測を行うとき、
    // di0 = 0 の点は、そこまでのモータの移動時間等を含んだ積分値
    // V(1)-V(0), V(2)-V(1), V(3)-V(2),... V(10)-V(9) の 10 の値が、
    // 表示、記録の対象
    mUnits.readValue( S2DVals, S2DCPSs, false );  // false : ダークの補正しない
    if ( S2DScanDir == FORWARD )
      S2DWriteBody2( S2DI.i[0], S2DI.i[1] );
    else
      S2DWriteBody2( S2DI.ps[0]-S2DI.i[0], S2DI.i[1] );

    if ( S2DI.i[0] > 0 ) {
      // ファイル記録
      S2DWriteBody( S2DVals[0] - S2DLastV );
      // 描画
      if ( S2DScanDir == FORWARD ) {
	S2DV->setData( S2DI.i[0] - 1, S2DI.i[1], S2DVals[0] - S2DLastV );
      } else {
	S2DV->setData( S2DI.ps[0] - S2DI.i[0], S2DI.i[1], S2DVals[0] - S2DLastV );
      }
    }
    S2DLastV = S2DVals[0];
    // ステップコントロール変数更新
    S2DI.i[0]++;
    // 上に書いた事情で、そのままでは意図したより一点少ない計測になる分、
    // 終点を一つ増やしておく
    if ( S2DI.i[0] <= S2DI.ps[0] ) { // 1st ax の端点でなければ
      S2DStage = 4;
      break;
    }
    // 1st ax の端点に到達していたら
    S2DWriteBlankLine();
    S2DI.i[0] = 0;
    S2DI.i[1]++;
    if ( S2DI.i[1] <= S2DI.ps[1] ) {       // 2nd ax の端点でなければ
      S2DI.unit[1]->SetValue( S2DI.unit[1]->u2p( S2DI.sx[1] + S2DI.i[1] * S2DI.dx[1] ) );
      // 2nd ax は次の点に移動
      if ( ! S2DI.ScanBothDir ) { 
	S2DI.unit[0]->SetHighSpeed( S2DI.unit[0]->highSpeed() ); // 1st ax を高速にして
	S2DI.unit[0]->SetValue( S2DI.unit[0]->u2p( S2DI.sx[0] ) ); // 1st ax は原点に戻し
	S2DStage = 3;    // 1st ax の速度をステップ用の速度に戻す
      } else {
	S2DScanDir = ReversedDir( S2DScanDir );
	S2DStage = 4;    // 速度変更は不要
      }
      break;
    }
    // 2nd ax の端点に達していたら
    if ( S2DI.Use3rdAx ) {  // 3軸スキャンする場合
      S2DI.i[1] = 0;
      S2DI.i[2]++;
      if ( S2DI.i[2] <= S2DI.ps[2] ) {  // 3rd ax の端点でなければ
	S2DI.unit[1]->SetValue( S2DI.unit[1]->u2p( S2DI.sx[1] ) ); // 2nd ax は原点に戻し
	S2DI.unit[2]->SetValue( S2DI.unit[2]
				->u2p( S2DI.sx[2] + S2DI.i[2] * S2DI.dx[2] ) );
	// 3rd ax は次の点に移動
	if ( ! S2DI.ScanBothDir ) { 
	  S2DI.unit[0]->SetHighSpeed( S2DI.unit[0]->highSpeed() ); // 1st ax を高速にして
	  S2DI.unit[0]->SetValue( S2DI.unit[0]->u2p( S2DI.sx[0] ) ); // 1st ax は原点に
	  S2DStage = 3;    // 1st ax の速度をステップ用の速度に戻す
	} else {
	  S2DScanDir = ReversedDir( S2DScanDir );
	  S2DStage = 4;    // 速度変更は不要
	}
	break;
      }
    }
    // 終了
    S2DStage = S2D_END_STAGE;
    break;
    
  case S2D_END_STAGE:
    S2DWriteTail();
    S2DI.unit[0]->SetHighSpeed( S2DI.unit[0]->highSpeed() ); // 1st ax を高速にして
    for ( int i = 0; i < S2DI.motors; i++ ) {          // 全軸原点に戻す
      if ( S2DI.used[i] )
	S2DI.unit[i]->SetValue( S2DI.now[i] );
    }
    // とりあえずスピードは「High」設定のママほっとく。
    mUnits.clearStage();
    S2DStage++;
    break;
  case S2D_END_STAGE+1:
    //    qDebug() << "before";
    if ( mUnits.Close() )
      break;
    //    qDebug() << "after";
    S2DStage++;
    break;
  case S2D_END_STAGE+2:
    inS2D = false;
    UUnits.clear( S2D_ID );
    NewLogMsg( QString( tr( "2D Scan Finished." ) ) );
    statusbar->showMessage( QString( tr( "2D Scan Finished." ) ), 2000 );
    S2DStart->setText( tr( "Start" ) );
    S2DStart->setStyleSheet( NormalB );
    S2DTimer->stop();
    break;
  }
}
