#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupSetupArea( void )   /* 設定エリア */
{
  double Eg = ManTEkeV->text().toDouble();

  //  inMove = 0;
  inMMove = 0;
  inMonitor = 0;
  inSPSing = 0;

  RadioBOn = "background-color: rgb(255,255,200)";
  RadioBOff = "background-color: rgb(200,200,220)";
  RelAbs = REL;
  ShowRelAbs();

  setupMDispFirstTime = true;

  GoPosKeV[0] = Eg - 0.50;
  GoPosKeV[1] = Eg - 0.05;
  GoPosKeV[2] = Eg + 0.10;
  GoPosKeV[3] = Eg + 1.10;
  ScanStage = 0;

  s->WatchS( "w1" );

  for ( int i = 0; i < UNITS; i++ ) {
    GoUnit0->addItem( QString( UnitName[i].name ) );
    for ( int j = 0; j < GOS; j++ ) {
      GoUnit[j]->addItem( QString( UnitName[i].name ) );
    }
  }

    s->WatchS( "w2" );

  GoUnit0->setCurrentIndex( KEV );
  for ( int i = 0; i < GOS; i++ ) {
    GoUnit[i]->setCurrentIndex( KEV );
  }
  ShowAllGos();

    s->WatchS( "w3" );

  connect( GoUnit0, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( SetAllGoUnits( int ) ) );
  for ( int i = 0; i < GOS; i++ ) {
    connect( GoUnit[i], SIGNAL( currentIndexChanged( int ) ),
	     this, SLOT( ShowAllGos() ) );
  }

    s->WatchS( "w4" );

  for ( int i = 0; i < GOS; i++ ) {
    connect( GoPosEdit[i], SIGNAL( editingFinished() ),
	     this, SLOT( GetNewGos() ) );
  }

    s->WatchS( "w5" );

  for ( int i = 0; i < AMotors.count(); i++ ) {
    MotorN->addItem( AMotors.value(i)->getName() );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( ShowCurMotorPos( SMsg ) ) );
    connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( ShowCurMotorPos( SMsg ) ) );
  }

    s->WatchS( "w6" );

  for ( int i = 0; i < MSPEEDS; i++ ) {
    MotorS->addItem( MSpeeds[i].MSName );
  }
    s->WatchS( "w7" );
    qDebug() << "ffv1" << MotorN->currentIndex();
    AMotors.at( MotorN->currentIndex() )->GetValue();
    s->WatchS( "w8" );
  GoMotorUnit->setText( AMotors.value( MotorN->currentIndex() )->getUnit() );
    qDebug() << "ffv2";
  for ( int i = 0; i < SPSMODES; i++ ) {
    SorPS->addItem( SPSModes[i].SPSMName );
  }
  for ( int i = 0; i < ASensors.count(); i++ ) {
    SelectD1->addItem( ASensors.value(i)->getName() );
    SelectD21->addItem( ASensors.value(i)->getName() );
    SelectD22->addItem( ASensors.value(i)->getName() );
  }

  for ( int i = 0; i < MSCALES; i++ ) {
    SelectScale->addItem( MScales[i].MSName );
  }
  MonStage1 = MonStage2 = 0;

  connect( GoTo1, SIGNAL( clicked() ), this, SLOT( GoToPosKeV1() ) );
  connect( GoTo2, SIGNAL( clicked() ), this, SLOT( GoToPosKeV2() ) );
  connect( GoTo3, SIGNAL( clicked() ), this, SLOT( GoToPosKeV3() ) );
  connect( GoTo4, SIGNAL( clicked() ), this, SLOT( GoToPosKeV4() ) );

  connect( MotorN, SIGNAL( currentIndexChanged( int ) ), this, SLOT( NewMotor() ) );
  connect( GoMotor, SIGNAL( clicked() ), this, SLOT( GoMAtP() ) );
  connect( GoMotorPosPuls, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( NewGoMotorPosPuls( const QString & ) ) );
  connect( GoMotorPosUnit, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( NewGoMotorPosUnit( const QString & ) ) );

  connect( SPSAScan, SIGNAL( clicked() ), this, SLOT( SPSAS() ) );
  connect( SPSRScan, SIGNAL( clicked() ), this, SLOT( SPSRS() ) );
  connect( MStart, SIGNAL( clicked() ), this, SLOT( Monitor() ) );
  connect( SetUpMMRel, SIGNAL( clicked() ), this, SLOT( MMRel() ) );
  connect( SetUpMMAbs, SIGNAL( clicked() ), this, SLOT( MMAbs() ) );
}

void MainWindow::MMRel( void )
{
  RelAbs = REL;
  ShowRelAbs();
}

void MainWindow::MMAbs( void )
{
  RelAbs = ABS;
  ShowRelAbs();
}

void MainWindow::ShowRelAbs( void )
{
  SetUpMMRel->setStyleSheet( ( RelAbs == REL )? RadioBOn : RadioBOff );
  SetUpMMAbs->setStyleSheet( ( RelAbs == ABS )? RadioBOn : RadioBOff );
}

void MainWindow::ShowCurMotorPos( SMsg msg )
{
  QString buf;
  QString val;

  AUnit *am = AMotors.value( MotorN->currentIndex() );

  if ( ( msg.From() == am->getDevCh() )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE ) ) ) {
    MCurPosPuls->setText( msg.Val() );
    val = QString::number
      ( ( msg.Val().toDouble() - am->getCenter() ) * am->getUPP() );
    MCurPosUnit->setText( val );
    if ( setupMDispFirstTime == true ) {  // 最初の一回だけ
      if ( RelAbs == ABS ) {
	GoMotorPosPuls->setText( msg.Val() );
	GoMotorPosUnit->setText( val );
	setupMDispFirstTime = false;
      } else {
	GoMotorPosPuls->setText( 0 );
	GoMotorPosUnit->setText( 0 );
	setupMDispFirstTime = false;
      }
    }
    NewLogMsg( tr( "Current Position of [%1] : [%2] %3" )
	       .arg( am->getName() )
	       .arg( val )
	       .arg( am->getUnit() ) );
  }
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

void MainWindow::NewGoMotorPosPuls( const QString &val )
{
  AUnit *am = AMotors.value( MotorN->currentIndex() );

  if ( RelAbs == ABS ) {
    GoMotorPosUnit
      ->setText( QString::number( ( val.toDouble() - am->getCenter() ) * am->getUPP() ) );
  } else {
    GoMotorPosUnit->setText( QString::number( val.toDouble() * am->getUPP() ) );
  }
}

void MainWindow::NewGoMotorPosUnit( const QString &val )
{
  AUnit *am = AMotors.value( MotorN->currentIndex() );

  if ( RelAbs == ABS ) {
    GoMotorPosPuls
      ->setText( QString::number( val.toDouble() / am->getUPP() + am->getCenter() ) );
  } else {
    GoMotorPosPuls->setText( QString::number( val.toDouble() / am->getUPP() ) );
  }
}

void MainWindow::GoMAtP( void )
{
  if ( inMMove == 0 ) {
    GoMAtPuls( GoMotorPosPuls->text().toDouble() );
  } else {
    GoMStop();
  }
}

void MainWindow::GoMAtPuls( double Pos )
{
  inMMove = 1;
  AUnit *am = AMotors.value( MotorN->currentIndex() );
  MovingS = MotorS->currentIndex();

  if ( RelAbs == REL )
    Pos += am->value().toDouble();

  if ( am->getType() == "PM" ) {
    Pos = (double)((int)Pos);
  }

  GoMotor->setText( tr( "Stop" ) );
  GoMotor->setStyleSheet( "background-color: yellow" );

  SPSAScan->setEnabled( false );
  SPSRScan->setEnabled( false );

  am->SetSpeed( MSpeeds[ MovingS ].MSid );
  am->SetValue( Pos );
  am->setIsBusy( true );

  MoveID = startTimer( 500 );

  NewLogMsg( QString( tr( "Setup: %1 : GoTo %2 : Speed %3\n" ) )
	     .arg( am->getName() )
	     .arg( GoMotorPosPuls->text().toInt() )
	     .arg( MSpeeds[ MovingS ].MSName ) );
  statusbar->showMessage( QString( tr( "Setup: %1 : GoTo %2 : Speed %3\n" ) )
			  .arg( am->getName() )
			  .arg( GoMotorPosPuls->text().toInt() )
			  .arg( MSpeeds[ MovingS ].MSName ),
			  1000 );
}

void MainWindow::NewMotor( void )
{
  setupMDispFirstTime = true;
  AMotors.value( MotorN->currentIndex() )->GetValue();
  GoMotorUnit->setText( AMotors.value( MotorN->currentIndex() )->getUnit() );
}

void MainWindow::GoMStop( void )
{
  AUnit *am = AMotors.value( MotorN->currentIndex() );

  am->Stop();
  GoMStop0();

  NewLogMsg( QString( tr( "Setup: %1 : Stopped at %2\n" ) )
	     .arg( am->getName() )
	     .arg( am->value() ) );
  statusbar->showMessage( QString( tr( "Setup: %1 : Stopped at %2\n" ) )
			  .arg( am->getName() )
			  .arg( am->value() ), 1000 );
}

void MainWindow::GoMStop0( void )
{
  inMMove = 0;

  GoMotor->setEnabled( true );
  SPSAScan->setEnabled( true );
  SPSRScan->setEnabled( true );
  GoMotor->setText( tr( "Go" ) );
  GoMotor->setStyleSheet( "" );
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
  AUnit *am = AMotors.value( MotorN->currentIndex() );
  AUnit *as = ASensors.value( SelectD1->currentIndex() );

  if ( inSPSing == 0 ) {
    MovingS = MotorS->currentIndex();

    if ( AbsRel == 0 ) {
      ScanSP = SPSsP->text().toInt();
      ScanEP = SPSeP->text().toInt();
    } else {
      ScanSP = am->value().toInt() + SPSsP->text().toInt();
      ScanEP = am->value().toInt() + SPSeP->text().toInt();
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

    NewLogMsg( QString( tr( "S/PS: Start (%1 %2)\n" ) )
	       .arg( am->getName() )
	       .arg( as->getName() ) );
    
    am->SetSpeed( MSpeeds[ MovingS ].MSid );

    Go  = ( AbsRel == 0 ) ? SPSAScan : SPSRScan;
    Go2 = ( AbsRel == 1 ) ? SPSAScan : SPSRScan;

    Go->setText( tr( "Stop" ) );
    Go->setStyleSheet( "background-color: yellow" );
    Go2->setEnabled( false );
    GoMotor->setEnabled( false );

    SPSView = XViews[ ViewTab->currentIndex() ];
    SPSView->Clear();
    SPSView->SetSLines( 0, 1 );
    SPSView->SetLineF( RIGHT, LEFT );
    SPSView->SetScaleT( I0TYPE, FULLSCALE );
    SPSView->SetLName( 0, tr( "I0" ) );
    SPSView->SetLName( 1, as->getName() );
    SPSView->SetXName( am->getName() );
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
  AUnit *am = AMotors.value( MotorN->currentIndex() );

  killTimer( SPSID );
  inSPSing = 0;
  am->Stop();

  SPSAScan->setText( tr( "Abs. Scn" ) );
  SPSAScan->setStyleSheet( "" );
  SPSAScan->setEnabled( true );
  SPSRScan->setText( tr( "Rel. Scn" ) );
  SPSRScan->setStyleSheet( "" );
  SPSRScan->setEnabled( true );
  GoMotor->setEnabled( true );
}

void MainWindow::ReadOutScanData( void )    // ( int NowP )
{
  AUnit *as = ASensors.value( SelectD1->currentIndex() ); // D1 でいいのか???????

  SI0->GetValue();
  as->GetValue();

#if 0           // ここではトリガをかけるだけ。値の読み出しは別のところでね。
  SPSView->NewPoint( 0, NowP, I0 );
  SPSView->NewPoint( 1, NowP, I );
#endif
}

void MainWindow::Monitor( void )
{
  AUnit *as1 = ASensors.value( SelectD21->currentIndex() );
  AUnit *as2 = ASensors.value( SelectD22->currentIndex() );

  if ( inMonitor == 0 ) {
    inMonitor = 1;
    MonStage1 = 0;   // 表示のサイクル
    MonStage2 = 0;   // 計測のサイクル

    for ( int i = 0; i < MCHANNELS; i++ )
      MeasSensF[i] = false;
    MeasSens[0] = SI0;   MeasSensF[0] = true;
    MeasSens[1] = as1;   MeasSensF[1] = true;
    MeasSens[2] = as2;   MeasSensF[2] = SelectD22Sel->isChecked();
    OneOfTheSensorIsCounter = false;
    for ( int i = 0; i < MCHANNELS; i++ ) {
      if ( MeasSens[i]->getType() == "CNT" ) {
	OneOfTheSensorIsCounter = true;
	TheCounter = MeasSens[i];
	break;
      }
    }

    /* 計測時間 [s] (MomMeasTime)はプロット間隔の 1/4 */
    MonMeasTime = MScales[ SelectScale->currentIndex() ].div / TicPDiv / 4.;
    if ( MonMeasTime > 10 )
      MonMeasTime = 10;

    MonView = XViews[ ViewTab->currentIndex() ];
    MonView->Clear();
    MonView->SetSLines( 0, 1 );
    MonView->SetLineF( RIGHT, LEFT );
    MonView->SetScaleT( I0TYPE, FULLSCALE );
    MonView->SetLName( 0, tr( "I0" ) );
    int LineCount = 1;
    for ( int i = 1; i < 3; i++ ) {
      if ( MeasSensF[i] == true ) {
	MonView->SetLName( LineCount, MeasSens[i]->getName() );
	LineCount++;
      }
    }
    MonView->SetGType( MONITOR );                            // 確認
    MonView->makeValid( true );                              // 確認
    MonView->SetMonScale( SelectScale->currentIndex() );
    connect( SelectScale, SIGNAL( currentIndexChanged( int ) ),
	     MonView, SLOT( SetMonScale( int ) ) );

    MStart->setText( tr( "Stop" ) );
    MStart->setStyleSheet( "background-color: yellow" );

    /* 指定プロット間隔の 1/8 でタイマーセット [ms] */
    MonID = startTimer( MScales[ SelectScale->currentIndex() ].div / TicPDiv * 1000 / 8. );

  } else {
    killTimer( MonID );
    inMonitor = 0;

    disconnect( SelectScale, SIGNAL( currentIndexChanged( int ) ),
	     MonView, SLOT( SetMonScale( int ) ) );

    MStart->setText( tr( "Mon. Start" ) );
    MStart->setStyleSheet( "" );
  }
}
