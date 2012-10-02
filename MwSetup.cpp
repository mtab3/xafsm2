#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupSetupArea( void )   /* 設定エリア */
{
  double Eg = ManTEkeV->text().toDouble();

  inMove = 0;
  inMMove = 0;
  inMonitor = 0;
  inSPSing = 0;

  GoPosKeV[0] = Eg - 0.50;
  GoPosKeV[1] = Eg - 0.05;
  GoPosKeV[2] = Eg + 0.10;
  GoPosKeV[3] = Eg + 1.10;
  ScanStage = 0;

  for ( int i = 0; i < UNITS; i++ ) {
    GoUnit0->addItem( QString( UnitName[i].name ) );
    for ( int j = 0; j < GOS; j++ ) {
      GoUnit[j]->addItem( QString( UnitName[i].name ) );
    }
  }

  GoUnit0->setCurrentIndex( KEV );
  for ( int i = 0; i < GOS; i++ ) {
    GoUnit[i]->setCurrentIndex( KEV );
  }
  ShowAllGos();

  connect( GoUnit0, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( SetAllGoUnits( int ) ) );
  for ( int i = 0; i < GOS; i++ ) {
    connect( GoUnit[i], SIGNAL( currentIndexChanged( int ) ),
	     this, SLOT( ShowAllGos() ) );
  }
  for ( int i = 0; i < GOS; i++ ) {
    connect( GoPosEdit[i], SIGNAL( editingFinished() ),
	     this, SLOT( GetNewGos() ) );
  }

  for ( int i = 1; i < MOTORS; i++ ) {       // Monochro は抜くので、
                                             // Index と i の対応はずれる
    MotorN->addItem( QString( tr( "%1: %2" ) )
			  .arg( Motors[i].Mid ).arg( Motors[i].MName ) );
    // なので、名前の先頭の Mid を見て判断するのがよい... けどそうはしてない。
  }
  for ( int i = 0; i < MSPEEDS; i++ ) {
    MotorS->addItem( MSpeeds[i].MSName );
  }
  GoMotorPos
    ->setText( sks->GetValue( Motors[ MotorN->currentIndex() + 1 ].devName ) );
  MCurPos
    ->setText( sks->GetValue( Motors[ MotorN->currentIndex() + 1 ].devName ) );
  GoMUpStep->setText( QString( "%1" )
		      .arg( Motors[ MotorN->currentIndex() + 1 ].upstep ) );
  GoMDownStep->setText( QString( "%1" )
			.arg( Motors[ MotorN->currentIndex() + 1 ].downstep ) );

  for ( int i = 0; i < SPSMODES; i++ ) {
    SorPS->addItem( SPSModes[i].SPSMName );
  }
  for ( int i = 0; i < MEASDEVICES; i++ ) {
    SelectD1->addItem( MDevs[i].MDName );
    SelectD2->addItem( MDevs[i].MDName );
  }

  SPSstep->setText( QString("%1").arg( Motors[ MotorN->currentIndex()+1 ].SPSstep ) );
  SPSdwell->setText( QString("%1").arg( Motors[ MotorN->currentIndex()+1 ].SPSdwell ) );
  SPSsP->setText( QString("%1").arg( Motors[ MotorN->currentIndex()+1 ].SPSstart ) );
  SPSeP->setText( QString("%1").arg( Motors[ MotorN->currentIndex()+1 ].SPSend ) );

  for ( int i = 0; i < MSCALES; i++ ) {
    SelectScale->addItem( MScales[i].MSName );
  }
  MonStage = 0;

  connect( GoTo1, SIGNAL( clicked() ), this, SLOT( GoToPosKeV1() ) );
  connect( GoTo2, SIGNAL( clicked() ), this, SLOT( GoToPosKeV2() ) );
  connect( GoTo3, SIGNAL( clicked() ), this, SLOT( GoToPosKeV3() ) );
  connect( GoTo4, SIGNAL( clicked() ), this, SLOT( GoToPosKeV4() ) );
  connect( GoMotor, SIGNAL( clicked() ), this, SLOT( GoMAt() ) );
  connect( GoMotorU, SIGNAL( clicked() ), this, SLOT( GoMUp() ) );
  connect( GoMotorD, SIGNAL( clicked() ), this, SLOT( GoMDown() ) );
  connect( MotorN, SIGNAL( currentIndexChanged( int ) ), this, SLOT( NewMotor() ) );
  connect( GoMUpStep, SIGNAL( editingFinished() ), this, SLOT( NewGMU() ) );
  connect( GoMDownStep, SIGNAL( editingFinished() ), this, SLOT( NewGMD() ) );
  connect( SPSAScan, SIGNAL( clicked() ), this, SLOT( SPSAS() ) );
  connect( SPSRScan, SIGNAL( clicked() ), this, SLOT( SPSRS() ) );
  connect( MStart, SIGNAL( clicked() ), this, SLOT( Monitor() ) );
}

void MainWindow::GetNewGos( void )
{
  for ( int i = 0; i < GOS; i++ ) {
    GetNewGo( i );
  }
}

void MainWindow::ShowAllGos( void )
{
  for ( int i = 0; i < GOS; i++ ) {
    ShowGo( i );
  }
}

void MainWindow::ShowGo( int i )
{
  QString buf;

  UNIT unit = (UNIT)GoUnit[i]->currentIndex();
  buf.sprintf( UnitName[unit].form, keV2any( unit, GoPosKeV[ i ] ) );
  GoPosEdit[i]->setText( buf );
}

void MainWindow::GetNewGo( int i )
{
  GoPosKeV[i] = any2keV( (UNIT)GoUnit[i]->currentIndex(),
			 GoPosEdit[i]->text().toDouble() );
}

void MainWindow::SetNewGos( void )
{
  //  double newv = Vic[SelectedA].AE[SelectedE];
  double newv = ManTEkeV->text().toDouble();
  GoPosKeV[0] = newv - 0.5;
  GoPosKeV[1] = newv - 0.1;
  GoPosKeV[2] = newv + 0.1;
  GoPosKeV[3] = newv + 1.0;
  ShowAllGos();
}

void MainWindow::SetAllGoUnits( int i )
{
  for ( int j = 0; j < GOS; j++ ) {
    GoUnit[j]->setCurrentIndex( i );
  }
  ShowAllGos();
}

void MainWindow::GoMAt( void )
{
  GoMB = GoMotor;
  if ( inMMove == 0 ) {
    GoMAt0( GoMotorPos->text().toInt() );
  } else {
    GoMStop();
  }
}

void MainWindow::GoMUp( void )
{
  GoMB = GoMotorU;
  if ( inMMove == 0 ) {
    GoMAt0( GoMotorPos->text().toInt() + abs( GoMUpStep->text().toInt() ) );
  } else {
    GoMStop();
  }
}

void MainWindow::GoMDown( void )
{
  GoMB = GoMotorD;
  if ( inMMove == 0 ) {
    GoMAt0( GoMotorPos->text().toInt() - abs( GoMUpStep->text().toInt() ) );
  } else {
    GoMStop();
  }
}

void MainWindow::GoMAt0( int Pos )
{
  inMMove = 1;
  MovingM = MotorN->currentIndex() + 1;
  MovingS = MotorS->currentIndex();

  GoMotor->setEnabled( false );
  GoMotorU->setEnabled( false );
  GoMotorD->setEnabled( false );
  SPSAScan->setEnabled( false );
  SPSRScan->setEnabled( false );
  GoMB->setEnabled( true );
  GoMB->setText( tr( "Stop" ) );
  GoMB->setStyleSheet( "background-color: yellow" );

  sks->SetSpeed( Motors[ MovingM ].devName,
		MSpeeds[ MovingS ].MSid );

  sks->SetValue( Motors[ MovingM ].devName, Pos );

  MoveID = startTimer( 500 );

  NewLogMsg( QString( tr( "Setup: %1 : GoTo %2 : Speed %3\n" ) )
	     .arg( Motors[ MovingM ].MName )
	     .arg( GoMotorPos->text().toInt() )
	     .arg( MSpeeds[ MovingS ].MSName ) );
  statusbar->showMessage( QString( tr( "Setup: %1 : GoTo %2 : Speed %3\n" ) )
			  .arg( Motors[ MovingM ].MName )
			  .arg( GoMotorPos->text().toInt() )
			  .arg( MSpeeds[ MovingS ].MSName ),
			  1000 );
}

void MainWindow::NewMotor( void )
{
  GoMotorPos
    ->setText( sks->GetValue( Motors[ MotorN->currentIndex() + 1 ].devName ) );
  MCurPos
    ->setText( sks->GetValue( Motors[ MotorN->currentIndex() + 1 ].devName ) );
  GoMUpStep->setText( QString( "%1" )
		      .arg( Motors[ MotorN->currentIndex() + 1 ].upstep ) );
  GoMDownStep->setText( QString( "%1" )
			.arg( Motors[ MotorN->currentIndex() + 1 ].downstep ) );
}

void MainWindow::GoMStop( void )
{
  char *now = NULL;

  sks->Stop( Motors[ MovingM ].devName );

  GoMStop0( now );

  NewLogMsg( QString( tr( "Setup: %1 : Stopped at %2\n" ) )
	     .arg( Motors[ MovingM ].MName )
	     .arg( now ) );
  statusbar->showMessage( QString( tr( "Setup: %1 : Stopped at %2\n" ) )
			  .arg( Motors[ MovingM ].MName )
			  .arg( now ), 1000 );
}

void MainWindow::GoMStop0( char *now )
{
  inMMove = 0;
  MCurPos
    ->setText( now = sks->GetValue( Motors[ MotorN->currentIndex() + 1 ].devName ) );
  killTimer( MoveID );

  GoMotor->setEnabled( true );
  GoMotorU->setEnabled( true );
  GoMotorD->setEnabled( true );
  SPSAScan->setEnabled( true );
  SPSRScan->setEnabled( true );
  if ( GoMB == GoMotor )
    GoMB->setText( tr( "Go" ) );
  if ( GoMB == GoMotorU )
    GoMB->setText( tr( "Go Up" ) );
  if ( GoMB == GoMotorD )
    GoMB->setText( tr( "Go Down" ) );
  GoMB->setStyleSheet( "" );
}


void MainWindow::NewGMU( void )
{
  Motors[ MotorN->currentIndex() + 1 ].upstep
    = GoMUpStep->text().toInt();
}

void MainWindow::NewGMD( void )
{
  Motors[ MotorN->currentIndex() + 1 ].downstep
    = GoMDownStep->text().toInt();
}

void MainWindow::SPSAS( void )
{
  SPSStart( 0 );
}

void MainWindow::SPSRS( void )
{
  SPSStart( 1 );
}

void MainWindow::SPSStart( int AbsRel )
{
  QPushButton *Go, *Go2;

  if ( inSPSing == 0 ) {
    MovingM = MotorN->currentIndex() + 1;
    MovingS = MotorS->currentIndex();
    SPSMon = SelectD1->currentIndex();

    if ( AbsRel == 0 ) {
      ScanSP = SPSsP->text().toInt();
      ScanEP = SPSeP->text().toInt();
    } else {
      ScanSP = atoi( sks->GetValue( Motors[ MovingM ].devName ) )
	+ SPSsP->text().toInt();
      ScanEP = atoi( sks->GetValue( Motors[ MovingM ].devName ) )
	+ SPSeP->text().toInt();
    }
    ScanSTP = SPSstep->text().toInt();
    if ( ScanEP > ScanSP ) {
      ScanSTP = abs( ScanSTP );
    } else {
      ScanSTP = - abs( ScanSTP );
    }
    ScanDW = SPSdwell->text().toDouble();
    if ( ScanSTP == 0 ) {
      statusbar->showMessage( tr( "Error: Scan Step is 0." ), 2000 );
      return;
    }
    inSPSing = 1;
    
    if ( MDevs[ SPSMon ].devName == NULL ) {
      statusbar->showMessage( tr( "Error: No available monitor device." ), 2000 );
      return;
    }

    NewLogMsg( QString( tr( "S/PS: Start (%1 %2)\n" ) )
	       .arg( Motors[ MovingM ].MName )
	       .arg( MDevs[ SPSMon ].MDName ) );
    
    sks->SetSpeed( Motors[ MovingM ].devName,
		  MSpeeds[ MovingS ].MSid );

    Go  = ( AbsRel == 0 ) ? SPSAScan : SPSRScan;
    Go2 = ( AbsRel == 1 ) ? SPSAScan : SPSRScan;

    Go->setText( tr( "Stop" ) );
    Go->setStyleSheet( "background-color: yellow" );
    Go2->setEnabled( false );
    GoMotor->setEnabled( false );
    GoMotorU->setEnabled( false );
    GoMotorD->setEnabled( false );

    SPSView = XViews[ ViewTab->currentIndex() ];
    SPSView->Clear();
    SPSView->SetSLines( 0, 1 );
    SPSView->SetLineF( RIGHT, LEFT );
    SPSView->SetScaleT( I0TYPE, FULLSCALE );
    SPSView->SetLName( 0, tr( "I0" ) );
    SPSView->SetLName( 1, MDevs[ SPSMon ].MDName );
    SPSView->SetXName( Motors[ MovingM ].MName );
    SPSView->SetGType( XYPLOT );
    SPSView->makeValid( true );

    ScanStage = 0;
    SPSID = startTimer( 200 );
  } else {
    SPSStop0();
  }
}

void MainWindow::SPSStop0( void )
{
  killTimer( SPSID );
  inSPSing = 0;
  sks->Stop( Motors[ MovingM ].devName );

  SPSAScan->setText( tr( "Abs. Scn" ) );
  SPSAScan->setStyleSheet( "" );
  SPSAScan->setEnabled( true );
  SPSRScan->setText( tr( "Rel. Scn" ) );
  SPSRScan->setStyleSheet( "" );
  SPSRScan->setEnabled( true );
  GoMotor->setEnabled( true );
  GoMotorU->setEnabled( true );
  GoMotorD->setEnabled( true );
}

void MainWindow::ReadOutScanData( int NowP ) // 透過法の測定結果を得て記録する
{
  double I0 = atof( sks->GetValue( "ion-ch", IONCH0 - IONCH0 ) );
  double I = atof( sks->GetValue( MDevs[ SPSMon ].devName, MDevs[ SPSMon ].devCh ) );
  SPSView->NewPoint( 0, NowP, I0 );
  SPSView->NewPoint( 1, NowP, I );
}

void MainWindow::Monitor( void )
{
  if ( inMonitor == 0 ) {
    inMonitor = 1;
    MonDev = SelectD2->currentIndex();
    MonStage = 0;
    MonUsedOldV = 0;
    NewMonV = OldMonV = 0;
    NewMonV0 = OldMonV0 = 0;
#if 0
    MonMeasTime = MScales[ MonScale ].div / TicPDiv * 1000 / 4.;
    if ( MonMeasTime > 10 )
      MonMeasTime = 10;
#else
    MonMeasTime = 1.0 / 2. / 2.;
#endif

    MonView = XViews[ ViewTab->currentIndex() ];
    MonView->Clear();
    MonView->SetSLines( 0, 1 );
    MonView->SetLineF( RIGHT, LEFT );
    MonView->SetScaleT( I0TYPE, FULLSCALE );
    MonView->SetLName( 0, tr( "I0" ) );
    MonView->SetLName( 1, MDevs[ MonDev ].MDName );
    MonView->SetGType( MONITOR );
    MonView->makeValid( true );
    MonView->SetMonScale( SelectScale->currentIndex() );
    connect( SelectScale, SIGNAL( currentIndexChanged( int ) ),
	     MonView, SLOT( SetMonScale( int ) ) );

    MStart->setText( tr( "Stop" ) );
    MStart->setStyleSheet( "background-color: yellow" );
#if 0
    MonID = startTimer( MScales[ MonScale ].div / TicPDiv * 1000 / 2.);
#else
    MonID = startTimer( 1.0 / 2. * 1000 );
#endif
    /* プロット間隔の2倍周期でタイマーセット -> タイマーは 1/2秒 に固定 */
    /* 計測スタートと計測結果記録の 2動作 1セット */
    /* 計測時間はプロット間隔の 1/4 (タイマー周期のさらに半分) */
    /*   -> 計測時間も 1/4秒 に固定*/

  } else {
    killTimer( MonID );
    inMonitor = 0;

    disconnect( SelectScale, SIGNAL( currentIndexChanged( int ) ),
	     MonView, SLOT( SetMonScale( int ) ) );

    MStart->setText( tr( "Mon. Start" ) );
    MStart->setStyleSheet( "" );
  }
}
