
#include "MainWindow.h"

// QXafs 関連のイニシャライズ setupMwMeas より前に呼ばれてる
void MainWindow::setupQXafsMode( void )
{
  connect( QMaxSpeed, SIGNAL( toggled( bool ) ), this, SLOT( CheckQXafsParams() ),
	   Qt::UniqueConnection );
  connect( QMinTime, SIGNAL( toggled( bool ) ), this, SLOT( CheckQXafsParams() ),
	   Qt::UniqueConnection );
  connect( SelRPT, SIGNAL( valueChanged( int ) ), this, SLOT( CheckQXafsParams() ),
	   Qt::UniqueConnection );
  connect( QLimitedDisplay, SIGNAL( toggled( bool ) ), this, SLOT( SetNewRPTLimit() ),
	   Qt::UniqueConnection );

  connect( QIntervalCycle, SIGNAL( editingFinished() ),
	   this, SLOT( CheckQXafsParams() ),
	   Qt::UniqueConnection );
  connect( QIntervalHalf, SIGNAL( editingFinished() ),
	   this, SLOT( CheckQXafsParams() ),
	   Qt::UniqueConnection );

  OrigHSpeed = HSpeed = 6000;    // 6000 pps, 0.1arcsec/pulse = 2.777..x10-5 deg/pulse
  QConditionBox->setHidden( true );

  QIntervalTimer1 = new QTimer;
  QIntervalTimer1->setSingleShot( true );
  connect( QIntervalTimer1, SIGNAL( timeout() ), this, SLOT( QIntervalTimeout1() ),
	   Qt::UniqueConnection );

  QIntervalTimer2 = new QTimer;
  QIntervalTimer2->setSingleShot( true );
  connect( QIntervalTimer2, SIGNAL( timeout() ), this, SLOT( QIntervalTimeout2() ),
	   Qt::UniqueConnection );

  connect( QRefPoint, SIGNAL( editingFinished() ), this, SLOT( ShowDeltaAtRefPoint() ) );
}

void MainWindow::QIntervalTimeout1( void )
{
  QIntervalTimer1->stop();
  QIntervalBlock1 = false;
}

void MainWindow::QIntervalTimeout2( void )
{
  QIntervalTimer2->stop();
  QIntervalBlock2 = false;
}

void MainWindow::SetNewRPTLimit( void )
{
  if ( QXafsMode->isChecked() ) {
    if ( QLimitedDisplay->isChecked() ) {
      SelRPT->setMaximum( 999999 ); // 別に unlimit でもいい。事実上の unlimit のつもり。
    } else {
      if ( QMeasOnBackward->isChecked() ) {
	SelRPT->setMaximum( 4999 );
      } else {
	SelRPT->setMaximum( 9999 );
      }
    }
  } else {
    SelRPT->setMaximum( 99 );
  }
  if ( SelRPT->value() > SelRPT->maximum() ) {
    SelRPT->setValue( SelRPT->maximum() );
  }
}

void MainWindow::ToggleQXafsMode( bool )
{
  if ( QXafsMode->isChecked() ) {

    // QXAFS モード

    SetDXMPMC();   // 分光器リセット

    SaveSelectedI0 = SelectI0->currentIndex();
    SaveSelectedI1 = SelectI1->currentIndex();
    SetUpSensorComboBoxes();

    for ( int i = 0; i < I0Sensors.count(); i++ ) {
      if ( I0Sensors[i]->getID() == "QXAFS-I0" ) {
	SelectI0->setCurrentIndex( i );
	break;
      }
    }
    for ( int i = 0; i < I1Sensors.count(); i++ ) {
      if ( I1Sensors[i]->getID() == "QXAFS-I1" ) {
	SelectI1->setCurrentIndex( i );
	break;
      }
    }
    for ( int i = 0; i < I1Sensors.count(); i++ ) {
      if ( I1Sensors[i]->getID() == "QXAFS-I2" ) {
	SelectAux1->setCurrentIndex( i );
	break;
      }
    }
    // ブロックパラメータのセーブを行う前に、
    // 現在のパラメータの妥当性を確認しておく
    NBlockPisValid = ( CheckBlockRange() && ( ( TP > 0 ) && ( TT0 > 0 ) ) );

    // ノーマルのブロックパラメータのセーブ
    SaveNowBlocks = SelBLKs->value();
    NXAFSBInfo.Blocks = SaveNowBlocks;
    NXAFSBInfo.Unit = (UNIT)(SelBLKUnit->currentIndex());
    SelBLKUnit->setCurrentIndex( (int)(QXAFSBInfo.Unit) );
    int i;
    for ( i = 0; i < MaxBLKs; i++ ) {
      NXAFSBInfo.Block[i].start = BLKstart[i]->text();
      BLKstart[i]->setText( QXAFSBInfo.Block[i].start );
      NXAFSBInfo.Block[i].step = BLKstep[i]->text();
      BLKstep[i]->setText( QXAFSBInfo.Block[i].step );
      NXAFSBInfo.Block[i].points = BLKpoints[i]->text();
      BLKpoints[i]->setText( QXAFSBInfo.Block[i].points );
      NXAFSBInfo.Block[i].dwell = BLKdwell[i]->text();
      BLKdwell[i]->setText( QXAFSBInfo.Block[i].dwell );
    }
    NXAFSBInfo.Block[ i ].start = BLKstart[ i ]->text();
    BLKstart[ i ]->setText( QXAFSBInfo.Block[ i ].start );
    ShowTotal();    // ブロックパラメータを変えた後に、合計点数の再計算と表示

    CheckQXafsParams();
    ChangeBLKs( 1 );
    SelBLKs->setEnabled( false );
    HideBLKs( true );
    QConditionBox->setHidden( false );

    SaveUse19ChSSD = Use19chSSD->isChecked();
    SaveUseAux1 = UseAux1->isChecked();
    SaveUseAux2 = UseAux2->isChecked();
    Use19chSSD->setChecked( false );
    UseAux1->setChecked( false );
    UseAux2->setChecked( false );
    Use19chSSD->setEnabled( false );
    UseAux1->setEnabled( true );       // QXafs で使えることにする
    UseAux2->setEnabled( false );      // やっぱこっちは使えないまま

    SetNewRPTLimit();

  } else {

    // NORML XAFS モード

    SelBLKs->setEnabled( true );
    ChangeBLKs( SaveNowBlocks );
    QXAFSBInfo.Blocks = SelBLKs->value();
    QXAFSBInfo.Unit = (UNIT)(SelBLKUnit->currentIndex());
    SelBLKUnit->setCurrentIndex( (int)(NXAFSBInfo.Unit) );
    int i;
    for ( i = 0; i < MaxBLKs; i++ ) {
      QXAFSBInfo.Block[i].start = BLKstart[i]->text();
      BLKstart[i]->setText( NXAFSBInfo.Block[i].start );
      QXAFSBInfo.Block[i].step = BLKstep[i]->text();
      BLKstep[i]->setText( NXAFSBInfo.Block[i].step );
      QXAFSBInfo.Block[i].points = BLKpoints[i]->text();
      BLKpoints[i]->setText( NXAFSBInfo.Block[i].points );
      QXAFSBInfo.Block[i].dwell = BLKdwell[i]->text();
      BLKdwell[i]->setText( NXAFSBInfo.Block[i].dwell );
    }
    QXAFSBInfo.Block[ i ].start = BLKstart[ i ]->text();
    BLKstart[ i ]->setText( NXAFSBInfo.Block[ i ].start );
    ShowTotal();    // ブロックパラメータを変えた後に、合計点数の再計算と表示

    HideBLKs( false );
    QConditionBox->setHidden( true );
    SetUpSensorComboBoxes();

    SelectI0->setCurrentIndex( SaveSelectedI0 );
    SelectI1->setCurrentIndex( SaveSelectedI1 );
    Use19chSSD->setChecked( SaveUse19ChSSD );
    UseAux1->setChecked( SaveUseAux1 );
    UseAux2->setChecked( SaveUseAux1 );
    Use19chSSD->setEnabled( true );
    UseAux1->setEnabled( true );
    UseAux2->setEnabled( true );

    SetNewRPTLimit();
  }
}

void MainWindow::HideBLKs( bool f )
{
  for ( int i = 2; i < BLKstart.count(); i++ ) {
    BLKstart[i]->setHidden( f );
  }
  for ( int i = 1; i < BLKstep.count(); i++ ) {
    BLKstep[i]->setHidden( f );
    BLKdwell[i]->setHidden( f );
    BLKpoints[i]->setHidden( f );
  }
  DwellAll->setHidden( f );
  for ( int i = 2; i < BLKlabels.count(); i++ ) {
    BLKlabels[i]->setHidden( f );
  }
}

#define MIN_INTEGRAL  ( 1e-4 )     // この数字は q34410a の属性として .def ファイルに
                                   // 書かれているべき。後でそのように変える。
#define INTEGRAL_INTERVAL_RATIO  ( 0.9 )
#define MIN_INTERVAL  ( MIN_INTEGRAL / INTEGRAL_INTERVAL_RATIO )

void MainWindow::CheckQXafsParams( void )
{
  if ( ! QXafsMode->isChecked() )
    return;

  QString buf;
  double sdeg = u->keV2deg( u->any2keV( BLKUnit, BLKstart[0]->text().toDouble() ) );
  double edeg = u->keV2deg( u->any2keV( BLKUnit, BLKstart[1]->text().toDouble() ) );
  double dtime = BLKdwell[0]->text().toDouble();

  HSpeed = fabs( ( edeg - sdeg ) / dtime / MMainTh->getUPP() );
  double MinTimeSpeed = sqrt( fabs( edeg - sdeg ) / MMainTh->getUPP()
			       / ( 2. * RunUpRate / 1000. ) );

  if ( QMinTime->isChecked() ) {
    if (( HSpeed > MinTimeSpeed )||( HSpeed < 0 ))
      HSpeed = MinTimeSpeed;
  }
  if (( HSpeed > MaxHSpeed )||( HSpeed < 0 ))
    HSpeed = MaxHSpeed;

  DispQSpeedInPPS->setText( QString::number( HSpeed ) );
  DispQSpeedInDPS->setText( QString::number( HSpeed * MMainTh->getUPP() ) );
  double WidthInPuls = fabs( edeg - sdeg ) / MMainTh->getUPP();

  if ( ( WidthInPuls / BLKpoints[0]->text().toInt() / HSpeed ) < MIN_INTERVAL ) {
    // PM16C が出す Trigger は 10us 幅にするので
    // Interval の時間は念の為 20us とる。
    // それよりも短くなるなら、インターバルを変更
    int Interval = MIN_INTERVAL * HSpeed;      
    BLKpoints[0]->setText( QString::number( (int)( WidthInPuls / Interval ) ) );
  }
  if ( BLKpoints[0]->text().toInt() > WidthInPuls ) {
    BLKpoints[0]->setText( QString::number( WidthInPuls ) );
    BLKstep[0]->setText( QString::number( fabs( edeg - sdeg ) / WidthInPuls ) );
  }
  dtime = WidthInPuls / HSpeed;
  BLKdwell[0]->setText( QString::number( dtime ) );
  ShowQTime( dtime, WidthInPuls );

  ShowDeltaAtRefPoint();
}

void MainWindow::ShowDeltaAtRefPoint( void )
{
  double rpEV = QRefPoint->text().toDouble() / 1000.;
  double sdeg = u->keV2deg( u->any2keV( BLKUnit, BLKs01->text().toDouble() ) );
  double edeg = u->keV2deg( u->any2keV( BLKUnit, BLKs02->text().toDouble() ) );
  double ddeg = fabs( edeg - sdeg ) / BLKpoints01->text().toInt();
  double dpEV2 = fabs( u->deg2keV( u->keV2deg( rpEV ) + ddeg ) - rpEV ) * 1000;
  QDeltaAtRefPoint->setText( QString( "%1" ).arg( dpEV2, 5, 'f', 3 ) );
}

void MainWindow::ShowQTime( double dtime, double WidthInPuls )
{
  // HSpeed までの加速にかかる時間
  double RunUpTime = ( HSpeed - LowSpeed ) * RunUpRate / 1000;
  // その時間で走るパルス数
  int RunUpPulses = ( HSpeed - LowSpeed ) * ( HSpeed + LowSpeed ) * RunUpRate / 2000.;

  int Th, Tm, Ts;
  double TT, TTF, TTB, TTT;
  // TT は一回のスキャンだけの時間。
  // TTF は往の時間、TTB は復の時間
  // TTT はインターバルを入れて、繰り返し回数をかけた時間
  if ( dtime > 0 ) {
    TTB = TTF = dtime + RunUpTime * 2 + 1.5;   // 1.5 は補正値
    if ( QMeasOnBackward->isChecked() ) {
      if ( TTF < QIntervalHalf->text().toDouble() )
	TTF = QIntervalHalf->text().toDouble();
    } else {
      TTB = ( RunUpPulses + WidthInPuls ) / MaxHSpeed + 1.5;  // 1.5 は補正値
    }
    TT = TTF + TTB;
    if ( TT < QIntervalCycle->text().toDouble() )
      TT = QIntervalCycle->text().toDouble();

    TTT = TT * SelRPT->value();
  } else {
    TTT = TT = 0;
  }
  EstimatedMeasurementTimeInSec = TTT;

  QString buf;

  Th = (int)( TT / 3600 );
  TT -= Th * 3600;
  Tm = (int)( TT / 60 );
  TT -= Tm * 60;
  Ts = (int)TT;
  TT -= Ts;
  buf.sprintf( "%02d:%02d:%02d.%02d", Th, Tm, Ts, (int)(TT*100) );
  TPoints->setText( tr( "Cycle: " ) + buf );

  Th = (int)( TTT / 3600 );
  TTT -= Th * 3600;
  Tm = (int)( TTT / 60 );
  TTT -= Tm * 60;
   Ts = (int)TTT;
  TTT -= Ts;
  buf.sprintf( "%02d:%02d:%02d.%02d", Th, Tm, Ts, (int)(TT*100) );
  TTime->setText( tr( "Total: " ) + buf );
}

void MainWindow::GetPM16CParamsForQXAFS( void )
// 本当は入力時にも計算できる内容だが、
// 確実を期すため測定直前に "Saved" パラメータを使って計算する
{
  double sdeg = SBlockStartInDeg[0];
  double edeg = SBlockStartInDeg[1];
  int points = abs( SBlockPoints[0] );
  double dtime = SBlockDwell[0];

  double WidthInPuls = fabs( edeg - sdeg ) / MMainTh->getUPP();
  HSpeed = WidthInPuls / dtime;
  if (( HSpeed > MaxHSpeed )||( HSpeed < 0 )) {
    HSpeed = MaxHSpeed;                           // PM16C に設定する H のスピード
  }
  DispQSpeedInPPS->setText( QString::number( HSpeed ) );
  DispQSpeedInDPS->setText( QString::number( HSpeed * MMainTh->getUPP() ) );

  QXafsSP0 = sdeg / MMainTh->getUPP() + MMainTh->getCenter();  // 測定範囲の始点
  QXafsEP0 = edeg / MMainTh->getUPP() + MMainTh->getCenter();  // 測定範囲の終点
  if ( abs( QXafsSP0 - QXafsEP0 ) > points )
    QXafsInterval = (int)(abs( QXafsSP0 - QXafsEP0 ) / points);
  // Trigger パルスを出す間隔
  else 
    QXafsInterval = 1;

//  if ( (double)QXafsInterval / HSpeed < 2e-5 ) {
  if ( ( (double)QXafsInterval / HSpeed ) < MIN_INTERVAL ) {
    // PM16C が出す Trigger は 10us 幅にするので
    // Interval の時間は念の為 20us とる。
    // ----> 20us ではだめ。a34410a の、最短積分時間は 100us
    //       Interval の 90% を積分時間に設定するので
    //       Interval は最低 100/0.9 = 111.1111.... 必要
    QXafsInterval = MIN_INTERVAL * HSpeed;   // これより短くなるなら、インターバルを変更
    SBlockPoints[0] = (int)(abs( QXafsSP0 - QXafsEP0 ) / QXafsInterval);
    statusbar
      ->showMessage( tr( "Selected Points were too many!  It was changed to be %1" )
		     .arg( SBlockPoints[0] ), 3000 );
  }

  QXafsPoints = abs( QXafsSP0 - QXafsEP0 ) / QXafsInterval;    // 測定ステップ数を再計算
  QXafsDwellTime = ( (double)QXafsInterval / HSpeed ) * 0.9;
  // 1点の積分時間を Trigger パルス間隔の 90% にする

  RunUpTime = ( HSpeed - LowSpeed ) * RunUpRate / 1000;  // HSpeed までの加速にかかる時間

  int RunUpPulses = ( HSpeed - LowSpeed ) * ( HSpeed + LowSpeed ) * RunUpRate / 2000.;
  if ( RunUpPulses < 0 ) // High Speed の方が Low Speed より遅いと RunUpPulses は負になる
    RunUpPulses *= -1;   // 減速にも同じ時間(とパルス)が必要だと仮定。
  // HSpeed までの加速に必要なパルス数
  if ( QXafsSP0 > QXafsEP0 )
    RunUpPulses *= -1;
  qDebug() << "RunUpPulses "
	   << HSpeed << LowSpeed << RunUpRate << RunUpPulses << RunUpTime;

  QXafsSP = QXafsSP0 - RunUpPulses;   // 測定範囲を加速パルス分広げた範囲の
  QXafsEP = QXafsEP0 + RunUpPulses;   // 始点と終点

  QString DebugBuf;
  DebugBuf = QString( "QXafs Measure Range [ %1 [ %2 %3 ] %4 ], RunUpRate %5" )
    .arg( QXafsSP ).arg( QXafsSP0 ).arg( QXafsEP0 ).arg( QXafsEP ).arg( RunUpRate )
    + QString( " Interval and Points %1 %2" )
    .arg( QXafsInterval ).arg( QXafsPoints )
    + QString( " Dwell Time %1" ).arg( QXafsDwellTime );
  NewLogMsg( DebugBuf );

  ShowQTime( dtime, WidthInPuls );
}

void MainWindow::SetUpMainThToGenerageTriggerSignal( int sp, int ep )
{
  MMainTh->AssignDispCh( 0 );              // QXafs のために PM16C からトリガパルスをとる
                                           // 配線は、チャンネル A ( 0 ) に繋いでる。
  MMainTh->SetTimingOutMode( 3 );          // 10um puls at every interval
  MMainTh->SetTimingOutStart( sp );
  MMainTh->SetTimingOutEnd( ep );
  MMainTh->SetTimingOutInterval( QXafsInterval );
  MMainTh->SetTimingOutReady( 1 );
}

void MainWindow::QXafsMeasSequence( void )
{
  int g;
  double t1, t2;

  switch( MeasStage ) {
  case 0:
    // PM16C のパラメータ決定
    // 代表ファイル作成
    // 推定測定時間の表示
    // 計測器類の内部ループカウンタクリア
    // グラフ表示の準備 <<---- !! I0, I のみを想定している
    // 分光器を制御する PM16C のチャンネルに「高速」を指示
    // 「高速」の値を分光器に許される最高速にセット
    // エンコーダ(nd287)の現在値読取り命令発行
    QIntervalBlock1 = QIntervalBlock2 = false;
    statusbar->showMessage( tr( "Start QXAFS Measurement!" ) );
    GetPM16CParamsForQXAFS();
    MakeDelegateFile();
    if ( AutoModeButton->isChecked() ) {
      CurrentRpt->setText( QString( "%1 - %2" ).arg( MeasA+1 ).arg( 1 ) );
    } else {
      CurrentRpt->setText( QString::number( 1 ) );
    }
    //    WriteInfoFile();
    mUnits.clearStage();
    MeasView->SetRLine( 0 );            // まず、0 番目のラインを右軸に表示
    MeasView->SetLLine( 2 );            //       2 番目のラインを左軸に表示
    MeasView->SetWindow0( u->deg2keV( SBlockStartInDeg[0] ), 0,
			  u->deg2keV( SBlockStartInDeg[ SBlocks ] ), 0 );
    MMainTh->SetSpeed( HIGH );
    MMainTh->SetHighSpeed( OrigHSpeed );
    //    EncMainTh->GetValue();
    MeasStage++;
    break;
  case 1:
    // 計測器類の初期化ループ (「終了」ステータスになるまで、break して 再入 でループ)
    // 計測器類の内部ループカウンタクリア
    // R = 0 (repeat = 0)
    if ( mUnits.init() ) // D.V. は Reset だけ, ENC2 は GetValue だけ
      break;
    mUnits.clearStage();
    MeasR = 0;    // Measurement Repeat count
    MeasStage++;
    break;
  case 2:
    // 1点の計測時間の設定
    // 計測器類の内部ループカウンタクリア
    mUnits.setDwellTimes( QXafsDwellTime );  
    mUnits.setDwellTime();
    mUnits.clearStage();
    // 読み取り命令を発行しておいたエンコーダ(nd287)の現在値の取得
    // EIB741 が使えるなら、その現在値も取得
    //          --> ここだと初回だけになるので毎回になるように case 41 に移した
    //    EncValue0 = EncMainTh->value();
    //    if ( Enc2 != NULL ) Enc2Value0 = Enc2->value();
    MeasStage++;
    break;
  case 3:
    // 分光器をスタート地点に向けて移動開始
    MMainTh->SetValue( QXafsSP );   // 助走距離を含めたスタート地点へ
    statusbar->showMessage( tr( "Moving DXM to the start point" ) );
    MeasStage++;
    break;     // break 必要。分光器が止まっていないとトリガの設定ができない
  case 4:      // Repeat Point 1
    // R++ (1回目の測定に入る前に R=1 になることに注意)
    // 終了判定 ---> 終了してれば stage = 99
    MeasR++;
    if ( MeasR > SelRPT->value() ) { // 規定回数回り終わってれば終了処理に入る!!
      MeasStage = 99;
      break;
    }
    if ( inPause ) {
      MeasStage = 90;
      break;
    }
    mUnits.clearStage();  
    MeasStage = 41;
    break;
  case 41:
    qDebug() << "Big Tune";
    if ( MStabOk && MPSet.TuneAtEachStep ) {
      if ( MPSet.TuneESAbs ) {
	s->SendCMD2( "TuneAtEP", MStabDrv,
		     QString( "GoMaxAbs %1 %2 %3 %4 %5" )
		     .arg( ( MPSet.TuneESQuick ) ? 1 : 0 )
		     .arg( MPSet.TuneESStart )
		     .arg( MPSet.TuneESEnd )
		     .arg( MPSet.TuneESSteps )
		     .arg( ( MPSet.TuneESQuick )
			   ? QString::number( MPSet.TuneESQuickTime ) : "" ) );
      } else {
	s->SendCMD2( "TuneAtEP", MStabDrv,
		     QString( "GoMaxRel %1 %2 %3 %4" )
		     .arg( ( MPSet.TuneESQuick ) ? 1 : 0 )
		     .arg( MPSet.TuneESStart )
		     .arg( MPSet.TuneESSteps )
		     .arg( ( MPSet.TuneESQuick )
			   ? QString::number( MPSet.TuneESQuickTime ) : "" ) );
      }
    }
    EncMainTh->GetValue();
    if ( Enc2 != NULL ) Enc2->GetValue();
    MeasStage = 42;
    break;
  case 42:
    EncValue0 = EncMainTh->value();
    if ( Enc2 != NULL ) Enc2Value0 = Enc2->value();
    MeasStage = 5;
    // break; // しない
  case 5:
    // 計測器を計測開始(Trigger待ち)状態にする(「待ち状態」ready になるまでループ)
    // 計測器類の内部ループカウンタクリア
    // 記録ファイルのヘッダ部分書き出し
    // 現在のリーピート番号表示 (Auto モードの進行変数 A と R)
    // スキャン方向の表示 (forward)
    // 「高速」の値を QXafs スキャンのために計算された値にセット
    // 分光器を制御する PM16C のチャンネルが適切なトリガを出すように設定
    if ( mUnits.QStart() )
      break;
    mUnits.clearStage();  
    WriteQHeader( MeasR, FORWARD );
    if ( AutoModeButton->isChecked() ) {
      CurrentRpt->setText( QString( "%1 - %2" ).arg( MeasA+1 ).arg( MeasR + 1 ) );
    } else {
      CurrentRpt->setText( QString::number( MeasR ) );
    }
    CurrentPnt->setText( tr( "Fwd" ) );
    MMainTh->SetHighSpeed( HSpeed );
    SetUpMainThToGenerageTriggerSignal( QXafsSP0, QXafsEP0 );
    MeasStage++;
    break;
  case 6:
    // 前の測定から、既定のインターバル時間が経ってなければループ
    statusbar->showMessage( tr( "Waiting Interval at start position" ) );
    if ( QIntervalBlock1 )
      break;
    // 測定開始！ ( = 分光器を終了地点へ移動)
    MeasView->ShowProgressBar( true );
    statusbar->showMessage( tr( "Forward scan" ) );
    MMainTh->SetValue( QXafsEP );   // 減速距離を含めた終了地点へ
    QIntervalBlock1 = QIntervalBlock2 = true;
    t1 = fabs( QIntervalCycle->text().toDouble() * 1000 );
    t2 = fabs( QIntervalHalf->text().toDouble() * 1000 );
    QIntervalTimer1->start( t1 );
    QIntervalTimer2->start( t2 ); 
    // mUnits.clearDoneF() : QRead を一台ずつ行うためのしかけ
    // OneByOne == true だと働くが、今は true にしていないので現状不要のはず。
    mUnits.clearDoneF();
    MeasStage++;
    break;
  case 7:
    // 計測器にデータ読み出し命令発行(完了するまでループ)
    if ( mUnits.QRead() )
      break;
    MeasView->ShowProgressBar( false );
    mUnits.clearStage();
    MeasStage++;
    break;
  case 8:
    // スキャン終了時の情報をヘッダに追加書き込み
    // データ本体の書き出し
    // 何番目のスキャンになるかを g にセット
    // グラフ表示
    WriteQHeader2( MeasR, FORWARD );
    WriteQBody( MeasR, FORWARD );
    g = ( QMeasOnBackward->isChecked() ) ? ( ( MeasR - 1 ) * 2 ) : ( MeasR - 1 );
    DispQSpectrum( g );
    MeasStage++;
    break;
  case 9:
    // 計測器の測定終了処理(完了するまでループ)
    // 戻りも測定するかどうかで分岐
    //   測定する場合
    //     戻りの測定用に PM16C にトリガ信号発生の設定
    //     "Bwd"(Backward) の表示
    //     インターバル有りなしで分岐して次のステージへ
    //   測定しない場合
    //     分光器の速度を既定最高速度に設定する
    //     PM16C がトリガを発生しないように設定
    if ( mUnits.QEnd() )
      break;
    mUnits.clearStage();
    if ( QMeasOnBackward->isChecked() ) {   // 戻りも測定する
      SetUpMainThToGenerageTriggerSignal( QXafsEP0, QXafsSP0 );
      CurrentPnt->setText( tr( "Bwd" ) );
      MeasStage = 10;
    } else {
      MMainTh->SetHighSpeed( MaxHSpeed );
      MMainTh->SetTimingOutMode( 0 );
      MMainTh->SetTimingOutReady( 0 );
      statusbar->showMessage( tr( "Return back to the start position" ) );
      MeasStage = 12;
      break;
    }
    // 戻りも測定する場合は break しない
  case 10:   // 分岐飛び込み点 1  : 戻りも測定 and Intrval 指定有り
    statusbar->showMessage( tr( "Waiting Interval at end position" ) );
    if ( QIntervalBlock2 )
      break;
    if ( inPause ) {
      MeasStage = 91;
      break;
    }
    statusbar->showMessage( tr( "Backward scan" ) );
    MeasView->ShowProgressBar( true );
    mUnits.clearStage();  
    MeasStage++;
#if 0
  case 101:
    // この辺に Tuning と ENC, ENC2 の読み出しがあってもいいかもしれないが
    // かえりのばっくらっしゅがどうせあるので、ほっておく
    break;
#endif
  case 11:
    // 計測器に計測開始命令発行(完了するまでループ)
    if ( mUnits.QStart() )
      break;
    mUnits.clearStage();  
    MeasStage++;
    // break; しない
    break;  // してみる。
  case 12:   // 分岐飛び込み点 2  : 戻り測定なし
    // 分光器をスタート地点に戻す
    // 「戻りも測定」の場合、戻り測定記録ファイルのヘッダ書き出し
    MMainTh->SetValue( QXafsSP );   // 測定開始!!! ( = 助走距離を含めたスタート地点へ)
    if ( QMeasOnBackward->isChecked() ) {   // 戻りも測定する
      WriteQHeader( MeasR, BACKWARD );
      mUnits.clearDoneF();      // QRead を一台ずつ行うため  // 現状不要のはず
      MeasStage++;
    } else {
      MeasStage = 4; // 戻りで測定しない場合 : Repeat Point 1
    }
    break;
  case 13:
    // 計測器に測定結果を読み出す指示(完了するまでループ)
    if ( mUnits.QRead() )
      break;
    MeasView->ShowProgressBar( false );
    mUnits.clearStage();
    MeasStage++;
    break;
  case 14:
    // スキャン終了時の情報をヘッダに追加書き込み
    // データ本体の書き出し
    // 何番目のスキャンになるかを g にセット
    // グラフ表示
    WriteQHeader2( MeasR, BACKWARD );
    WriteQBody( MeasR, BACKWARD );
    g = ( MeasR - 1 ) * 2 + 1;
    DispQSpectrum( g );
    MeasStage++;
    // break; しない
  case 15:
    // 計測器の測定終了処理(完了するまでループ)
    // 始点でインターバルがあるかないかで分岐
    if ( mUnits.QEnd() )
      break;
    mUnits.clearStage();
    MeasStage = 4;
    break;

  case 90:             // MeasStage = 4 で pause
    statusbar->showMessage( tr( "Pausing at start position" ) );
    if ( !inPause )
      MeasStage = 5;
    break;
  case 91:             // MeasStage = 10 で pause
    statusbar->showMessage( tr( "Pausing at end position" ) );
    if ( !inPause )
      MeasStage = 11;
    break;

  case 99:
    MPSet.normallyFinished = true;
    PlayEndingSound();
    QXafsFinish();
    break;
  }
}

void MainWindow::QXafsFinish0( void )
{
  UUnits.clear( MEAS_ID );
  CheckNewMeasFileName();
  mUnits.setOneByOne( false );           // 「ユニット一つずつ順番に」モードやめる
  MMainTh->SetHighSpeed( OrigHSpeed );   // H のスピードを標準に戻す
  MMainTh->SetSpeed( GoMSpeed );         // 選択されていたスピードに戻す
}

void MainWindow::QXafsFinish( void )
{
  statusbar->showMessage( tr( "The Measurement has Finished" ), 4000 );
  NewLogMsg( QString( tr( "Meas: QXafs Finished" ) ) );
  MeasTimer->stop();
  inMeas = false;
  MeasStart->setText( tr( "Start" ) );
  MeasStart->setStyleSheet( NormalB );
  MeasPause->setEnabled( false );
  QXafsFinish0();
  onMeasFinishWorks();
}

void MainWindow::DispQSpectrum( int g )  // ダーク補正どうする？
{
  QVector<QString> Labels;
  Labels << tr( "I0" )
    << tr( "I1" ) << tr( "mu" )
    << tr( "A1" ) << tr( "mu2" )
    << tr( "A2" ) << tr( "mu3" );

  int Us = mUnits.count();

  QStringList valsEnc;
  if ( Enc2 != NULL ) {
    valsEnc = Enc2->values();
    Us -= 1;
  } else {
    valsEnc.clear();
  }

  QVector<QStringList> vals;
  QVector<double> dark;
  int num = 100000000;
  for ( int i = 0; i < Us; i++ ) {
    vals << mUnits.at(i)->values();

    dark << mUnits.at(i)->getDark();   //  * QXafsDwellTime;  // 即席
    /* q34410a だけならこれでいいが、他の計測器を使う時はダメ */

    if ( num > vals[i][0].toInt() )
      num = vals[i][0].toInt();
  }
  if ( num > valsEnc[0].toInt() )
    num = valsEnc[0].toInt();

  int p = QXafsSP0;
  int d = QXafsInterval;
  int c = MMainTh->getCenter();
  double upp = MMainTh->getUPP();
  double deg2;
  double upp2 = 0;
  double E, I0, I00, I1, I2;
  I2 = 0;

  if ( Enc2 != NULL ) {
    upp2 = Enc2->getUPP();
  }
  if ( QLimitedDisplay->isChecked() ) {
    g = g % QLastLines->value();
  }

  int Ls = Us * 2 - 1;
  MeasView->SetLR( g*Ls, RIGHT_AX );                    // I0 
  MeasView->SetScaleType( g*Ls, I0TYPE );
  MeasView->SetLineName( g*Ls, Labels[0] );
  MeasView->SetDispF( g*Ls, true );
  MeasView->SetPoints( g*Ls, 0 );
  MeasView->SetDG( g*Ls, 0 );

  int L;
  for ( int i = 1; i < Ls; i++ ) {
    L = g*Ls+i;
    MeasView->SetLR( L, LEFT_AX );
    MeasView->SetScaleType( L, FULLSCALE );
    MeasView->SetLineName( L, Labels[ i ] );
    MeasView->SetDispF( L, ( ( i % 2 ) == 1 ) ? false : true );
    MeasView->SetPoints( L, 0 );
    MeasView->SetDG( L, i );
  }

  for ( int i = 0; i < num; i++ ) {
    if ( Enc2 == NULL ) {
      deg2 = ( p - c ) * upp + d * i;
    } else {
      deg2 = EncValue0.toDouble() + ( valsEnc[i+1].toInt() - Enc2Value0.toInt() ) * upp2;
    }
    E = u->deg2keV( deg2 );
    I0 = vals[0][i+1].toDouble() - dark[0];
    I1 = vals[1][i+1].toDouble() - dark[1];
    if ( Us > 2 )
      I2 = vals[2][i+1].toDouble() - dark[2];
    L = g * Ls;

    // I0
    MeasView->NewPoint( L, E, I0 );

    // I1
    MeasView->NewPoint( L+1, E, I1 );
    if ( fabs( I1 ) < 1e-10 ) I1 = 1e-10;
    MeasView->NewPoint( L+2, E, log( fabs( I0/I1 ) ) );

    // Aux1
    if ( Us > 2 ) {
      int dMode = ModeA1->currentIndex();
      MeasView->NewPoint( L+3, E, I2 );
      if ( dMode == 0 ) {
	if ( fabs( I0 ) < 1e-10 ) I0 = 1e-10;
	MeasView->NewPoint( L+4, E, log( fabs( I2/I0 ) ) );
      } else {
	I00 = (( dMode == 3 )||( dMode == 4 )) ? I1 : I0;
	if (( dMode == 2 )||( dMode == 4 )) I2 *= -1;
	if ( fabs( I2 ) < 1e-10 ) I2 = 1e-10;
	if ( I00/I2 > 0 )
	  MeasView->NewPoint( L+4, E, log( fabs( I00/I2 ) ) );
	else 
	  MeasView->NewPoint( L+4, E, 0 );
      }
    }
  }

  MeasView->update();
}

void MainWindow::ShowQXafsProgress( void )
{
  if ( QXafsSP != QXafsEP ) {
    MeasView
      ->SetProgress( fabs( ( MMainTh->value().toDouble() - QXafsSP )
			   / ( QXafsEP - QXafsSP ) ) );
    MeasView->update();
  }
}
