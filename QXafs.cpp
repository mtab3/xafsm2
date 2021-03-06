
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

void MainWindow::saveNSelections( void )
{
  NSaveSelectedI0 = SelectI0->currentIndex();
  NSaveSelectedI1 = SelectI1->currentIndex();
  NSaveSelectedAux1 = SelectAux1->currentIndex();
  NSaveSelectedAux2 = SelectAux2->currentIndex();

  NSaveUseI1 = UseI1->isChecked();
  NSaveUse19ChSSD = Use19chSSD->isChecked();
  NSaveUseAux1 = UseAux1->isChecked();
  NSaveUseAux2 = UseAux2->isChecked();

  NSaveModeSelAux1 = ModeA1->currentIndex();
  NSaveModeSelAux2 = ModeA2->currentIndex();
}

void MainWindow::recoverNSelections( void )
{
  SelectI0->setCurrentIndex( NSaveSelectedI0 );
  SelectI1->setCurrentIndex( NSaveSelectedI1 );
  SelectAux1->setCurrentIndex( NSaveSelectedAux1 );
  SelectAux2->setCurrentIndex( NSaveSelectedAux2 );

  UseI1->setChecked( NSaveUseI1 );
  Use19chSSD->setChecked( NSaveUse19ChSSD );
  UseAux1->setChecked( NSaveUseAux1 );
  UseAux2->setChecked( NSaveUseAux2 );

  ModeA1->setCurrentIndex( NSaveModeSelAux1 );
  ModeA2->setCurrentIndex( NSaveModeSelAux2 );
  
  Use19chSSD->setEnabled( true );
  UseAux1->setEnabled( true );
  UseAux2->setEnabled( true );
}

void MainWindow::saveQSelections( void )
{
  QSaveSelectedI0 = SelectI0->currentIndex();
  QSaveSelectedI1 = SelectI1->currentIndex();
  QSaveSelectedAux1 = SelectAux1->currentIndex();
  QSaveSelectedAux2 = SelectAux2->currentIndex();

  QSaveUseI1 = UseI1->isChecked();
  QSaveUse19ChSSD = Use19chSSD->isChecked();
  QSaveUseAux1 = UseAux1->isChecked();
  QSaveUseAux2 = UseAux2->isChecked();

  QSaveModeSelAux1 = ModeA1->currentIndex();
  QSaveModeSelAux2 = ModeA2->currentIndex();
}

void MainWindow::recoverQSelections( void )
{
  SelectI0->setCurrentIndex( QSaveSelectedI0 );
  SelectI1->setCurrentIndex( QSaveSelectedI1 );
  SelectAux1->setCurrentIndex( QSaveSelectedAux1 );
  SelectAux2->setCurrentIndex( QSaveSelectedAux2 );

  UseI1->setChecked( QSaveUseI1 );
  Use19chSSD->setChecked( QSaveUse19ChSSD );
  UseAux1->setChecked( QSaveUseAux1 );
  UseAux2->setChecked( QSaveUseAux2 );

  ModeA1->setCurrentIndex( QSaveModeSelAux1 );
  ModeA2->setCurrentIndex( QSaveModeSelAux2 );

  Use19chSSD->setEnabled( false );
  UseAux1->setEnabled( true );
  UseAux2->setEnabled( true );
}

void MainWindow::ToggleQXafsMode( bool )
{
  if ( QXafsMode->isChecked() ) {     // QXAFS モード

    SetDXMPMC();   // 分光器リセット

    saveNSelections();
    SetUpSensorComboBoxes();
    recoverQSelections();

    // ブロックパラメータのセーブを行う前に、
    // 現在のパラメータの妥当性を確認しておく
    NBlockPisValid = ( CheckBlockRange() && ( ( TP > 0 ) && ( TT0 > 0 ) ) );

    // ノーマルのブロックパラメータのセーブ
    NSaveNowBlocks = SelBLKs->value();
    NXAFSBInfo.Blocks = NSaveNowBlocks;
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

    SetNewRPTLimit();

  } else {      // NORML XAFS モード

    saveQSelections();
    SetUpSensorComboBoxes();
    recoverNSelections();
    
    SelBLKs->setEnabled( true );
    ChangeBLKs( NSaveNowBlocks );
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

    SetNewRPTLimit();
  }
  showMeasModes();
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

#if 0
#define MIN_INTERVAL  ( 2e-5 )
#define MIN_INTEGRAL  ( 1e-4 )     // この数字は q34410a の属性として .def ファイルに
                                   // 書かれているべき。後でそのように変える。
#define MIN_INTERVAL  ( MIN_INTEGRAL / INTEGRAL_INTERVAL_RATIO )
#endif

#define INTEGRAL_INTERVAL_RATIO  ( 0.9 )

void MainWindow::CheckQXafsParams( void )
{
  if ( ! QXafsMode->isChecked() )
    return;

  QString buf;
  // sdeg:始点(角度)、edeg:終点(角度)、dtime:測定時間 を GUI から取得
  double sP = BLKstart[0]->text().toDouble();
  double eP = BLKstart[1]->text().toDouble();
  double sKeV = u->any2keV( BLKUnit, sP );
  double eKeV = u->any2keV( BLKUnit, eP );
  double sdeg = u->keV2deg( sKeV );
  double edeg = u->keV2deg( eKeV );
  double dtime = BLKdwell[0]->text().toDouble();

  if ( dtime <= 0 ) // dtime は 0 あるいは 負であってはならない。もしそうなってたら
    dtime = 1e-3;   // 十分小さな数字(1msec)に置き換えておく
  // HSpeed:上記を達成するのに必要なθの速度 (pulse/s) // global
  HSpeed = fabs( ( edeg - sdeg ) / dtime / MMainTh->upp() );
  // PM16C に設定できるモータのスピード は整数なので HSpeed も整数
  // 従って、上記の計算結果のスピードが 1pps を切る場合には HSpeed が 0 になってしまう。
  if ( HSpeed < 1 )
    HSpeed = 1;

  // MinTimeSpeed:加減速時間を考慮に入れて、同じ範囲を最小時間でスキャンする為の速度 (pulse/s)
  double MinTimeSpeed = sqrt( fabs( edeg - sdeg ) / MMainTh->upp()
			       / ( 2. * RunUpRate / 1000. ) );
  // 「時間最小」が指定されていて HSpeed が MinTimeSpeed を超えていた場合は
  //  HSpeed を MinTimeSpeed に改める
  //  (最高速度が早すぎると、加減速時間が余計にかかってかえって全体の時間が長くなる時がある)
  if ( QMinTime->isChecked() ) {
    if (( HSpeed > MinTimeSpeed )||( HSpeed < 0 ))
      HSpeed = MinTimeSpeed;
  }
  //  HSpeed が MaxHSpeed(出して良い最高速度)より大きい場合は
  //  HSpeed を MaxHSpeed に改める
  if (( HSpeed > MaxHSpeed )||( HSpeed < 0 ))
    HSpeed = MaxHSpeed;
  qDebug() << "QXafs calced HSpeed" << HSpeed;

  // 上記の手続きで決めたスキャン速度(HSpeed)を GUI に表示しておく
  DispQSpeedInPPS->setText( QString::number( HSpeed ) );    // pulse/sec
  DispQSpeedInDPS->setText( QString::number( HSpeed * MMainTh->upp() ) );  // deg/sec

  // WidthInPulse : スキャンする角度範囲は、パルス数で言うと何パルス分になるか
  double WidthInPulse = fabs( edeg - sdeg ) / MMainTh->upp();

  // 極めて特殊ケース
  // 測定しようとしているエネルギー範囲(の幅)をパルスで表した(なんパルス分のスキャンか)数字より
  // 指定された測定点数の方が多い場合 WidthInPulse / Points < 1 になるので、
  // パルス単位でトリガをかけて測定を行うことができなくなる。
  // その場合には、測定点数を測定範囲のパルス数まで減らす(全パルスでトリガがかかる)
  QPoints = BLKpoints[0]->text().toInt();
  if (  QPoints > WidthInPulse ) {
    QPoints = WidthInPulse;
  }
  
  // 指定された点数分の測定を行うとき、一点あたりのパルス送りと時間
  QIntervalInPulse = ceil( WidthInPulse / QPoints );
  QIntervalInSec = (double)QIntervalInPulse / HSpeed;

  // 一点あたりの時間が、可能な最低時間より短くなるなら
  // 一点あたりの時間を最低値にとり直し、対応する一点あたりのパルス数と、総点数を再計算
  if ( QIntervalInSec < QMinIntervalTime ) {
    // PM16C が出す Trigger は 10us 幅にするので
    // これが制限になる場合、QMinIntervalTime は念の為 20us に設定されているはず。
    QIntervalInSec = QMinIntervalTime;
    QIntervalInPulse = QIntervalInSec * HSpeed;
  }
  // 計測トリガを出すインターバルが 1pulse より小さくなってしまうようなら
  // 1 pulse にしておく。
  if ( QIntervalInPulse < 1 ) {
    QIntervalInPulse = 1;
  }
  qDebug() << "QXafs calced Interval" << QIntervalInPulse;

  // 実際の点数を表示し直しておく
  if ( QIntervalInPulse != 0 ) {
    QPoints = ceil( WidthInPulse / QIntervalInPulse );
  } else {
    QPoints = 0;
  }
  BLKpoints[0]->setText( QString::number( QPoints ) );
  double width = fabs( BLKstart[1]->text().toDouble() - BLKstart[0]->text().toDouble() );
  if ( QPoints > 0 ) 
    BLKstep[0]->setText( QString::number( width / QPoints ) );
  else 
    BLKstep[0]->setText( QString::number( 0 ) );

  // 計測にかかる時間の再計算と表示
  dtime = WidthInPulse / HSpeed;
  // dtime は測定しようとする範囲をスキャンするのに必要な時間
  // 計測のトータルでは、これに RunUp, RunDown など前後の時間が必要になってくる
  BLKdwell[0]->setText( QString::number( dtime ) );
  ShowQTime( dtime, WidthInPulse );

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
// QXafs 測定開始時に CheckQXafsParams を通るので
// そこで計算されるようなパラメータはここでは信じて良い
{
  double sdeg = SBlockStartInDeg[0];   // 角度単位での始点
  double edeg = SBlockStartInDeg[1];   // 確度単位での終点
#if 0
  int points = abs( SBlockPoints[0] ); // 測定点数 (CheckQXafsParams で補正済みのはず)
  double dtime = SBlockDwell[0];
#endif
  
#if 0  // HSpeed をここで計算してはダメ (CheckQXafsParamas で計算済)
  double WidthInPuls = fabs( edeg - sdeg ) / MMainTh->upp();　// パルス単位での測定範囲幅
  HSpeed = WidthInPuls / dtime;
  if (( HSpeed > MaxHSpeed )||( HSpeed < 0 )) {
    HSpeed = MaxHSpeed;                           // PM16C に設定する H のスピード
  }
  DispQSpeedInPPS->setText( QString::number( HSpeed ) );
  DispQSpeedInDPS->setText( QString::number( HSpeed * MMainTh->upp() ) );
#endif
  qDebug() << "QXafs exec HSpeed" << HSpeed;
  
  QXafsSP0 = sdeg / MMainTh->upp() + MMainTh->getCenter();  // 測定範囲の始点(pulse)
  QXafsEP0 = edeg / MMainTh->upp() + MMainTh->getCenter();  // 測定範囲の終点(pulse)
#if 0  // QInterval は再計算しなくていい
  if ( abs( QXafsSP0 - QXafsEP0 ) > points )
    QXafsInterval = (int)(abs( QXafsSP0 - QXafsEP0 ) / points);
  // Trigger パルスを出す間隔
  else 
    QXafsInterval = 1;

  // if ( (double)QXafsInterval / HSpeed < 2e-5 ) {
  if ( ( (double)QXafsInterval / HSpeed ) < QMinIntervalTime ) {
    // PM16C が出す Trigger は 10us 幅にするので
    // Interval の時間は念の為 20us とる。
    // ----> 20us ではだめ。a34410a の、最短積分時間は 100us
    //       Interval の 90% を積分時間に設定するので
    //       Interval は最低 100/0.9 = 111.1111.... 必要
    QXafsInterval = QMinIntervalTime * HSpeed;   // これより短くなるなら、インターバルを変更
    SBlockPoints[0] = (int)(abs( QXafsSP0 - QXafsEP0 ) / QXafsInterval);
    statusbar
      ->showMessage( tr( "Selected Points were too many!  It was changed to be %1" )
		     .arg( SBlockPoints[0] ), 3000 );
  }
#endif
  qDebug() << "QXafs exec Interval" << QIntervalInPulse;

#if 0  // QPoints も再計算しなくていい
  QXafsPoints = abs( QXafsSP0 - QXafsEP0 ) / QXafsInterval;    // 測定ステップ数を再計算
#endif
  QXafsDwellTime = QIntervalInSec * INTEGRAL_INTERVAL_RATIO;
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
    .arg( QIntervalInPulse ).arg( QPoints )
    + QString( " Dwell Time %1" ).arg( QXafsDwellTime );
  NewLogMsg( DebugBuf );

  double WidthInPuls = fabs( edeg - sdeg ) / MMainTh->upp(); // パルス単位での測定範囲幅
  ShowQTime( SBlockDwell[0], WidthInPuls );
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
    mMeasUnits.clearStage();
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
    // D.V. は Reset だけ, ENC2 は GetValue だけ
    if ( mMeasUnits.init() )
      break;
    mMeasUnits.clearStage();
    MeasR = 0;    // Measurement Repeat count
    MeasStage++;
    break;
  case 2:
    // 1点の計測時間の設定
    // 計測器類の内部ループカウンタクリア
    mMeasUnits.setDwellTimes( QXafsDwellTime );  
    mMeasUnits.setDwellTime();
    mMeasUnits.clearStage();
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
    if ( MeasBlockB->isChecked() )
      break;
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
    mMeasUnits.clearStage();  
    MeasStage = 41;
    break;
  case 41:
    qDebug() << "Big Tune";
    if ( MStabOk && MPSet.TuneAtEachStep ) {
      if ( MPSet.TuneESAbs ) {
	if ( MPSet.TuneESQuick ) {
	  MMStab->GoMaxAbsQ( MPSet.TuneESStart, MPSet.TuneESEnd, MPSet.TuneESSteps,
			     MPSet.TuneESQuickTime );
	} else {
	  MMStab->GoMaxAbs( MPSet.TuneESStart, MPSet.TuneESEnd, MPSet.TuneESSteps );
	}
#if 0
	s->SendCMD2( "TuneAtEP", MStabDrv,   // not be used
		     QString( "GoMaxAbs %1 %2 %3 %4 %5" )
		     .arg( ( MPSet.TuneESQuick ) ? 1 : 0 )
		     .arg( MPSet.TuneESStart )
		     .arg( MPSet.TuneESEnd )
		     .arg( MPSet.TuneESSteps )
		     .arg( ( MPSet.TuneESQuick )
			   ? QString::number( MPSet.TuneESQuickTime ) : "" ) );
#endif
      } else {
	if ( MPSet.TuneESQuick ) {
	  MMStab->GoMaxRelQ( MPSet.TuneESStart, MPSet.TuneESSteps, MPSet.TuneESQuickTime );
	} else {
	  MMStab->GoMaxRel( MPSet.TuneESStart, MPSet.TuneESSteps );
	}
#if 0
	s->SendCMD2( "TuneAtEP", MStabDrv,   // not be used
		     QString( "GoMaxRel %1 %2 %3 %4" )
		     .arg( ( MPSet.TuneESQuick ) ? 1 : 0 )
		     .arg( MPSet.TuneESStart )
		     .arg( MPSet.TuneESSteps )
		     .arg( ( MPSet.TuneESQuick )
			   ? QString::number( MPSet.TuneESQuickTime ) : "" ) );
#endif
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
    if ( mMeasUnits.QStart() )
      break;
    mMeasUnits.clearStage();  
    WriteQHeader( MeasR, FORWARD );
    if ( AutoModeButton->isChecked() ) {
      CurrentRpt->setText( QString( "%1 - %2" ).arg( MeasA+1 ).arg( MeasR + 1 ) );
    } else {
      CurrentRpt->setText( QString::number( MeasR ) );
    }
    CurrentPnt->setText( tr( "Fwd" ) );
    MMainTh->SetHighSpeed( HSpeed );
    MMainTh->SetUpToGenerageTriggerSignal( QXafsSP0, QXafsEP0, QIntervalInPulse );
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
    // mMeasUnits.clearDoneF() : QRead を一台ずつ行うためのしかけ
    // OneByOne == true だと働くが、今は true にしていないので現状不要のはず。
    mMeasUnits.clearDoneF();
    MeasStage++;
    break;
  case 7:
    // 計測器にデータ読み出し命令発行(完了するまでループ)
    if ( mMeasUnits.QRead() )
      break;
    MeasView->ShowProgressBar( false );
    mMeasUnits.clearStage();
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
    if ( mMeasUnits.QEnd() )
      break;
    mMeasUnits.clearStage();
    if ( QMeasOnBackward->isChecked() ) {   // 戻りも測定する
      MMainTh->SetUpToGenerageTriggerSignal( QXafsEP0, QXafsSP0, QIntervalInPulse );
      CurrentPnt->setText( tr( "Bwd" ) );
      MeasStage = 10;
    } else {
      MMainTh->SetHighSpeed( MaxHSpeed );
      MMainTh->TriggerOff();
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
    mMeasUnits.clearStage();  
    MeasStage++;
#if 0
  case 101:
    // この辺に Tuning と ENC, ENC2 の読み出しがあってもいいかもしれないが
    // かえりのばっくらっしゅがどうせあるので、ほっておく
    break;
#endif
  case 11:
    // 計測器に計測開始命令発行(完了するまでループ)
    if ( mMeasUnits.QStart() )
      break;
    mMeasUnits.clearStage();  
    MeasStage++;
    // break; しない
    break;  // してみる。
  case 12:   // 分岐飛び込み点 2  : 戻り測定なし
    // 分光器をスタート地点に戻す
    // 「戻りも測定」の場合、戻り測定記録ファイルのヘッダ書き出し
    MMainTh->SetValue( QXafsSP );   // 測定開始!!! ( = 助走距離を含めたスタート地点へ)
    if ( QMeasOnBackward->isChecked() ) {   // 戻りも測定する
      WriteQHeader( MeasR, BACKWARD );
      mMeasUnits.clearDoneF();      // QRead を一台ずつ行うため  // 現状不要のはず
      MeasStage++;
    } else {
      MeasStage = 4; // 戻りで測定しない場合 : Repeat Point 1
    }
    break;
  case 13:
    // 計測器に測定結果を読み出す指示(完了するまでループ)
    if ( mMeasUnits.QRead() )
      break;
    MeasView->ShowProgressBar( false );
    mMeasUnits.clearStage();
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
    if ( mMeasUnits.QEnd() )
      break;
    mMeasUnits.clearStage();
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
  UUnits.removeUnits( MEAS_ID );
  CheckNewMeasFileName();
  mMeasUnits.setOneByOne( false );           // 「ユニット一つずつ順番に」モードやめる
  MMainTh->SetHighSpeed( OrigHSpeed );   // H のスピードを標準に戻す
  MMainTh->SetSpeed( GoMSpeed );         // 選択されていたスピードに戻す
}

void MainWindow::QXafsFinish( void )
{
  statusbar->showMessage( tr( "The Measurement has Finished" ), 4000 );
  NewLogMsg( QString( tr( "Meas: QXafs Finished" ) ) );
  MeasTimer->stop();
  SignalToStars( XAFS_M_END );
  SignalToStars( QXAFS_M_END );
  inMeas = false;
  MeasStart->setText( tr( "Start" ) );
  MeasStart->setStyleSheet( NormalEXECB );
  MeasPause->setEnabled( false );
  MeasPause->setHidden( true );
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

  int Us = mMeasUnits.count();

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
    vals << mMeasUnits.at(i)->values();
    dark << mMeasUnits.at(i)->GetDark( QXafsDwellTime );
    /* q34410a だけならこれでいいが、他の計測器を使う時はダメ */
    // 現在は OK (なぜ大丈夫かは QFio の同一箇所を参照)
    if ( vals[i].count() > 0 ) {
      if ( num > vals[i][0].toInt() )
	num = vals[i][0].toInt();
    } else {
      num = 0;
    }
  }
  if ( Enc2 != NULL ) {
    if ( num > valsEnc[0].toInt() )
      num = valsEnc[0].toInt();
  }
  if ( num <= 0 )
    return;

  int p = QXafsSP0;
  int d = QIntervalInPulse;
  int c = MMainTh->getCenter();
  double upp = MMainTh->upp();
  double deg2;
  double upp2 = 0;
  double E, I0, I00, I1, I2, I3;
  I3 = I2 = 0;

  if ( Enc2 != NULL ) {
    upp2 = Enc2->upp();
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
    if ( Us > 3 )
      I3 = vals[3][i+1].toDouble() - dark[3];
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

    // Aux2
    if ( Us > 3 ) {
      int dMode = ModeA2->currentIndex();
      MeasView->NewPoint( L+5, E, I3 );
      if ( dMode == 0 ) {
	if ( fabs( I0 ) < 1e-10 ) I0 = 1e-10;
	MeasView->NewPoint( L+6, E, log( fabs( I3/I0 ) ) );
      } else {
	switch( dMode ) {
	case 1: I00 = I0; break;
	case 2: I00 = I0; I3*=-1; break;
	case 3: I00 = I1; break;
	case 4: I00 = I1; I3*=-1; break;
	case 5: I00 = I2; break;
	case 6: I00 = I2; I3*=-1; break;
	default: I00 = I0;
	}
	if ( fabs( I3 ) < 1e-10 ) I3 = 1e-10;
	if ( I00/I3 > 0 )
	  MeasView->NewPoint( L+6, E, log( fabs( I00/I3 ) ) );
	else 
	  MeasView->NewPoint( L+6, E, 0 );
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
