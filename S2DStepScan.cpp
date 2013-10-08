#include "MainWindow.h"

// 連続スキャンのコードは消して、ステップスキャンだけに特化する
// S2DStepF とかのチェックはこれの外で終わってるものとする

void MainWindow::S2DStepScanSequence( void )
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

  // モータ駆動中は入ってこない (とりあえずステップのことだけ考える)

  for ( int i = 0; i < S2DMotors.count(); i++ ) {
    if ( S2DMotorUse[i] && S2DMotors[i]->isBusy0() )
      return;
  }

#if 0
  // センサー busy でも入ってこない
  if (( isS2DSFluo )&&( S2DStage >= 3 )) {
    if ( SFluo->isBusy2() )
      return;
  } else {
#endif
  if ( mUnits.isBusy() ) {
    return;
  }
#if 0
  }
#endif
  
  int pps;
  switch( S2DStage ) {
  case 0:
    // 検出器初期化
    if ( mUnits.init() == false ) {  // true :: initializing
      mUnits.clearStage();
      S2DStage++;
    }
    break;
  case 1:
    // 検出器の計測時間セット
    mUnits.setDwellTimes( S2DTime1->text().toDouble() );
    mUnits.setDwellTime();
    S2DStage++;
    break;
  case 2:
    // 全軸に対して
    for ( int i = 0; i < S2DMotors.count(); i++ ) {
      S2DMotors[i]->SetSpeed( HIGH );                          // スピードマックス
      S2DMotors[i]->SetValue( S2DMotors[i]->u2p( S2Dsx[i] ) ); // 始点に移動
      S2Di[i] = 0; // ステップコントロール変数初期化
    }
    S2DStage++;
    break;
  case 3:     // もうひとつのリピートポイント
    // 1st Ax のみ、スキャン用のスピードにセット
    pps = (int)fabs( (double)S2Ddx[0]
		     / S2DMotors[0]->getUPP()
		     / S2DTime1->text().toDouble() );
    if ( pps == 0 ) pps = 1;
    if ( pps > S2DMotors[0]->highSpeed() )
      pps = S2DMotors[0]->highSpeed();
    S2DMotors[0]->SetHighSpeed( pps );
    S2DStage++;
    // break しない
  case 4:     // リピートポイント
    // 計測開始準備
    mUnits.clearStage();
    if ( ! mUnits.isParent() )  // 親がいなければ次のステップはとばす
      S2DStage++;
    S2DStage++;
    break;
  case 5:
    if ( mUnits.getValue0() == false ) { // 親ユニットの準備
      mUnits.clearStage();
      S2DStage++;
    }
    break;
  case 6:
    if ( mUnits.getValue() == false ) {  // 計測開始
      mUnits.clearStage();
      // 同時に次の点に移動開始
      S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] + (S2Di[0]+1)*S2Ddx[0] ) );
      S2DStage++;
    }
    break;
  case 7:
    // 計測値読み取り
    mUnits.readValue( S2DVals, S2DCPSs, false );  // false : ダークの補正しない
    // ファイル記録
    S2DWriteBody( S2DVals[0] );
    S2DV->setData( S2Di[0], S2Di[1], S2DVals[0] ); // 描画
    // ステップコントロール変数更新
    S2Di[0]++;
    if ( S2Di[0] < S2Dps[0] ) { // 1st ax の端点でなければ
      S2DStage = 4;
      break;
    }
    // 1st ax の端点に到達していたら
    S2DWriteBlankLine();
    S2Di[0] = 0;
    S2Di[1]++;
    if ( S2Di[1] <= S2Dps[1] ) {       // 2nd ax の端点でなければ
      S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax を高速にして
      S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] ) ); // 1st ax は原点に戻し
      S2DMotors[1]->SetValue( S2DMotors[1]->u2p( S2Dsx[1] + S2Di[1] * S2Ddx[1] ) );
                                                               // 2nd ax は次の点に移動
      S2DStage = 3;    // 1st ax の速度をステップ用の速度に戻す
      break;
    }
    // 2nd ax の端点に達していたら
    if ( S2DUse3rdAx->isChecked() ) {  // 3軸スキャンする場合
      S2Di[1] = 0;
      S2Di[2]++;
      if ( S2Di[2] <= S2Dps[2] ) {  // 3rd ax の端点でなければ
	S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax を高速にして
	S2DMotors[0]->SetValue( S2DMotors[0]->u2p( S2Dsx[0] ) ); // 1st ax は原点に戻し
	S2DMotors[1]->SetValue( S2DMotors[1]->u2p( S2Dsx[1] ) ); // 2nd ax も原点に戻し
	S2DMotors[2]->SetValue( S2DMotors[2]->u2p( S2Dsx[2] + S2Di[2] * S2Ddx[2] ) );
	                                                         // 3rd ax は次の点に移動
	S2DStage = 3;    // 1st ax の速度をステップ用の速度に戻す
	break;
      }
    }
    // 終了
    S2DStage = S2D_END_STAGE;
    break;

  case S2D_END_STAGE:
    S2DWriteTail();
    S2DMotors[0]->SetHighSpeed( S2DMotors[0]->highSpeed() ); // 1st ax を高速にして
    for ( int i = 0; i < S2DMotors.count(); i++ ) {          // 全軸原点に戻す
      S2DMotors[i]->SetValue( S2Dnow[i] );
    }
    // とりあえずスピードは「High」設定のママほっとく。
    mUnits.clearStage();
    S2DStage++;
    break;
  case S2D_END_STAGE+1:
     if ( mUnits.Close() )
      break;
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

