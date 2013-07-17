
#include "MainWindow.h"

// QXafs 関連のイニシャライズ setupMwMeas より前に呼ばれてる
void MainWindow::setupQXafsMode( void )
{
  connect( QMaxSpeed, SIGNAL( toggled( bool ) ), this, SLOT( CheckQXafsParams() ) );
  connect( QMinTime, SIGNAL( toggled( bool ) ), this, SLOT( CheckQXafsParams() ) );
  connect( SelRPT, SIGNAL( valueChanged( int ) ), this, SLOT( CheckQXafsParams() ) );
  connect( QLimitedDisplay, SIGNAL( toggled( bool ) ), this, SLOT( SetNewRPTLimit() ) );

  connect( QIntervalCycle, SIGNAL( editingFinished() ),
	   this, SLOT( CheckQXafsParams() ) );
  connect( QIntervalHalf, SIGNAL( editingFinished() ),
	   this, SLOT( CheckQXafsParams() ) );

  OrigHSpeed = HSpeed = 6000;    // 6000 pps, 0.1arcsec/pulse = 2.777..x10-5 deg/pulse
  QConditionBox->setHidden( true );

  QIntervalTimer1 = new QTimer;
  QIntervalTimer1->setSingleShot( true );
  connect( QIntervalTimer1, SIGNAL( timeout() ), this, SLOT( QIntervalTimeout1() ) );

  QIntervalTimer2 = new QTimer;
  QIntervalTimer2->setSingleShot( true );
  connect( QIntervalTimer2, SIGNAL( timeout() ), this, SLOT( QIntervalTimeout2() ) );

#if 0
  QMeasOnBackward->setHidden( true );
  QMinMaxBox->setHidden( true );
  QMaxSpeed->setHidden( true );
  QMinTime->setHidden( true );
  QSepLine->setHidden( true );
  QLimitedDisplay->setHidden( true );
#endif
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

    SaveNowBlocks = SelBLKs->value();
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
    UseAux2->setEnabled( true );       // QXafs で使えることにする

    SetNewRPTLimit();

  } else {

    // NORML XAFS モード

    SelBLKs->setEnabled( true );
    ChangeBLKs( SaveNowBlocks );
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

  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Interval の定義が変わった

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
}

void MainWindow::ShowQTime( double dtime, double WidthInPuls )
{
  // HSpeed までの加速にかかる時間
  double RunUpTime = ( HSpeed - LowSpeed ) * RunUpRate / 1000;
  // その時間で走るパルス数
  int RunUpPulses = ( HSpeed - LowSpeed ) * ( HSpeed + LowSpeed ) * RunUpRate / 2000.;

  int Th, Tm, Ts;
  double TT, TTT;
  // TT は一回のスキャンだけの時間。
  // TTT はインターバルを入れて、繰り返し回数をかけた時間
  if ( dtime > 0 ) {
    if ( QMeasOnBackward->isChecked() ) {
      TT = ( dtime + RunUpTime * 2 ) * 2 + 3;      // +1 は実測の補正値
    } else {
      TT = dtime + RunUpTime * 2 + ( RunUpPulses + WidthInPuls ) / MaxHSpeed + 3;
    }
    TTT = TT * SelRPT->value();
#if 0
      + QIntervalAtStart->text().toDouble() * ( SelRPT->value() - 1 )
      + QIntervalAtEnd->text().toDouble() * SelRPT->value();
#endif
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
  MMainTh->SetTimingOutMode( 3 );          // 10um puls at every interval
  MMainTh->SetTimingOutStart( sp );
  MMainTh->SetTimingOutEnd( ep );
  MMainTh->SetTimingOutInterval( QXafsInterval );
  MMainTh->SetTimingOutReady( 1 );
}

void MainWindow::QXafsMeasSequence( void )
{
  int g;

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
    EncMainTh->GetValue();
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
    // 読み取り命令を発行しておいたエンコーダ(nd287)の現在値の取得
    // EIB741 が使えるなら、その現在値も取得
    // 1点の計測時間の設定
    // 計測器類の内部ループカウンタクリア
    EncValue0 = EncMainTh->value();
    if ( Enc2 != NULL ) Enc2Value0 = Enc2->value();
    mUnits.setDwellTimes( QXafsDwellTime );  
    mUnits.setDwellTime();
    mUnits.clearStage();
    MeasStage++;
    break;
  case 3:
    // 分光器をスタート地点に向けて移動開始
    MMainTh->SetValue( QXafsSP );   // 助走距離を含めたスタート地点へ
    // break しない
  case 4:      // Repeat Point 1
    // R++ (1回目の測定に入る前に R=1 になることに注意)
    // 終了判定 ---> 終了してれば stage = 99
    MeasR++;
    if ( MeasR >= SelRPT->value() ) { // 規定回数回り終わってれば終了処理に入る!!
      MeasStage = 99;
      break;
    }
    // break しない
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
    if ( QIntervalBlock1 )
      break;
    // 測定開始！ ( = 分光器を終了地点へ移動)
    MMainTh->SetValue( QXafsEP );   // 減速距離を含めた終了地点へ
    QIntervalBlock1 = QIntervalBlock2 = true;
    QIntervalTimer1->start( fabs( QIntervalCycle->text().toDouble() * 1000 ) );
    QIntervalTimer2->start( fabs( QIntervalHalf->text().toDouble() * 1000 ) ); 
    // mUnits.clearDoneF() : QRead を一台ずつ行うためのしかけ
    // OneByOne == true だと働くが、今は true にしていないので現状不要のはず。
    mUnits.clearDoneF();
    MeasStage++;
    break;
  case 7:
    // 計測器にデータ読み出し命令発行(完了するまでループ)
    if ( mUnits.QRead() )
      break;
    mUnits.clearStage();
    MeasStage++;
    break;
  case 8:
    // スキャン終了時の情報をヘッダに追加書き込み
    // データ本体の書き出し
    // 何番目のスキャンになるかを g にセット
    // グラフ表示
    WriteQHeader2( MeasR, FORWARD );
    WriteQBody();
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
      MeasStage = 11;
      break;
    }
    // 戻りも測定する場合は break しない
  case 10:   // 分岐飛び込み点 1  : 戻りも測定 and Intrval 指定有り
    if ( QIntervalBlock2 )
      break;
    // 計測器にデータ読み出し命令発行(完了するまでループ)
    if ( mUnits.QStart() )
      break;
    MeasStage++;
    // break; しない
  case 11:   // 分岐飛び込み点 2  : 戻り測定なし
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
  case 12:
    // 計測器に測定結果を読み出す指示(完了するまでループ)
    if ( mUnits.QRead() )
      break;
    mUnits.clearStage();
    MeasStage++;
    break;
  case 13:
    // スキャン終了時の情報をヘッダに追加書き込み
    // データ本体の書き出し
    // 何番目のスキャンになるかを g にセット
    // グラフ表示
    WriteQHeader2( MeasR, BACKWARD );
    WriteQBody();
    g = ( MeasR - 1 ) * 2 + 1;
    DispQSpectrum( g );
    MeasStage++;
    // break; しない
  case 14:
    // 計測器の測定終了処理(完了するまでループ)
    // 始点でインターバルがあるかないかで分岐
    if ( mUnits.QEnd() )
      break;
    mUnits.clearStage();
    MeasStage = 4;
    break;

  case 99:
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
  int Us = mUnits.count();
  QStringList vals0 = mUnits.at(0)->values();
  QStringList vals1 = mUnits.at(1)->values();
  QStringList vals2;
  if ( Us > 3 ) 
    vals2 = mUnits.at(2)->values();
  QStringList valsEnc;
  if ( Enc2 != NULL )
    valsEnc = Enc2->values();
  else 
    valsEnc.clear();
  int num = findMini( vals0, vals1, valsEnc );

  double dark0 = mUnits.at(0)->getDark() * QXafsDwellTime;
  double dark1 = mUnits.at(1)->getDark() * QXafsDwellTime;
  double dark2 = 0;
  if ( Us > 3 )
    dark2 = mUnits.at(2)->getDark() * QXafsDwellTime;

  int p = QXafsSP0;
  int d = QXafsInterval;
  int c = MMainTh->getCenter();
  double upp = MMainTh->getUPP();
  double deg2;
  double upp2 = 0;
  double E, I0, I1, I2;
  if ( Enc2 != NULL ) {
    upp2 = Enc2->getUPP();
  }
  //  qDebug() << "upp2 " << upp2 << EncValue0.toDouble() << Enc2Value0.toInt();

  if ( QLimitedDisplay->isChecked() ) {
    g = g % QLastLines->value();
  }

  int Ls = ( Us - 2 ) * 2 + 1;
  MeasView->SetLR( g*Ls, RIGHT_AX );                    // I0 
  MeasView->SetScaleType( g*Ls, I0TYPE );
  MeasView->SetLineName( g*Ls, "I0" );
  MeasView->SetDispF( g*Ls, true );
  MeasView->SetPoints( g*Ls, 0 );
  MeasView->SetDG( g*Ls, 0 );

  MeasView->SetLR( g*Ls+1, LEFT_AX );                   // I1
  MeasView->SetScaleType( g*Ls+1, FULLSCALE );
  MeasView->SetLineName( g*Ls+1, "I1" );
  MeasView->SetDispF( g*Ls+1, false );
  MeasView->SetPoints( g*Ls+1, 0 );
  MeasView->SetDG( g*Ls+1, 1 );

  MeasView->SetLR( g*Ls+2, LEFT_AX );                   // mu
  MeasView->SetScaleType( g*Ls+2, FULLSCALE );
  MeasView->SetLineName( g*Ls+2, tr( "mu" ) );
  MeasView->SetDispF( g*Ls+2, true );
  MeasView->SetPoints( g*Ls+2, 0 );
  MeasView->SetDG( g*Ls+2, 2 );

  if ( Us > 3 ) {
    MeasView->SetLR( g*Ls+3, LEFT_AX );                   // I1
    MeasView->SetScaleType( g*Ls+3, FULLSCALE );
    MeasView->SetLineName( g*Ls+3, "I2" );
    MeasView->SetDispF( g*Ls+3, false );
    MeasView->SetPoints( g*Ls+3, 0 );
    MeasView->SetDG( g*Ls+3, 3 );
    
    MeasView->SetLR( g*Ls+4, LEFT_AX );                   // mu
    MeasView->SetScaleType( g*Ls+4, FULLSCALE );
    MeasView->SetLineName( g*Ls+4, tr( "mu2" ) );
    MeasView->SetDispF( g*Ls+4, true );
    MeasView->SetPoints( g*Ls+4, 0 );
    MeasView->SetDG( g*Ls+4, 4 );
  }

  for ( int i = 0; i < num; i++ ) {
    if ( Enc2 == NULL ) {
      deg2 = ( p - c ) * upp + d * i;
    } else {
      deg2 = EncValue0.toDouble() + ( valsEnc[i+1].toInt() - Enc2Value0.toInt() ) * upp2;
    }

    E = u->deg2keV( deg2 );
    I0 = vals0[i+1].toDouble() - dark0;
    I1 = vals1[i+1].toDouble() - dark1;
    MeasView->NewPoint( g*3 + 0, E, I0 );
    MeasView->NewPoint( g*3 + 1, E, I1 );
    if ( ( I1 != 0 ) && ( ( I0 / I1 ) > 0 ) ) {
      MeasView->NewPoint( g*3 + 2, E, log( I0/I1 ) );
    } else {
      MeasView->NewPoint( g*3 + 2, E, 0 );
    }

    if ( Us > 3 ) {
      I2 = vals2[i+1].toDouble() - dark2;
      MeasView->NewPoint( g*3 + 3, E, I2 );
      if ( ( I2 != 0 ) && ( ( I0 / I2 ) > 0 ) ) {
	MeasView->NewPoint( g*3 + 4, E, log( I0/I2 ) );
      } else {
	MeasView->NewPoint( g*3 + 4, E, 0 );
      }
    }

  }
  MeasView->update();
}


  
