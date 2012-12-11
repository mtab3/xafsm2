#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupSetupArea( void )   /* 設定エリア */
{
  double Eg = ManTEkeV->text().toDouble();

  //  inMove = 0;
  inMMove = 0;
  inMonitor = 0;
  inSPSing = 0;

  RadioBOn = "background-color: rgb(255,255,000)";
  RadioBOff = "background-color: rgb(210,210,230)";
  GoMRelAbs = REL;
  SPSRelAbs = REL;
  ShowGoMRelAbs();
  ShowSPSRelAbs();

  GoMSpeed = MIDDLE;

  setupMDispFirstTime = true;

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

  for ( int i = 0; i < AMotors.count(); i++ ) {
    MotorN->addItem( AMotors.value(i)->getName() );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( ShowCurMotorPos( SMsg ) ) );
    connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( ShowCurMotorPos( SMsg ) ) );
  }

  for ( int i = 0; i < MSPEEDS; i++ ) {
    GoMotorS->addItem( MSpeeds[i].MSName );
    SPSMotorS->addItem( MSpeeds[i].MSName );
  }
  AMotors.at( MotorN->currentIndex() )->GetValue();
  GoMotorUnit->setText( AMotors.value( MotorN->currentIndex() )->getUnit() );
  SPSUnit->addItem( "Puls" );
  SPSUnit->addItem( AMotors.value( MotorN->currentIndex() )->getUnit() );

  for ( int i = 0; i < ASensors.count(); i++ ) {
    SelectD1->addItem( ASensors.value(i)->getName() );
    SelectD20->addItem( ASensors.value(i)->getName() );
    SelectD21->addItem( ASensors.value(i)->getName() );
    SelectD22->addItem( ASensors.value(i)->getName() );
  }

  for ( int i = 0; i < MSCALES; i++ ) {
    SelectScale->addItem( MScales[i].MSName );
  }
  MonStage = 0;

  connect( GoMSpeedH, SIGNAL( clicked() ), this, SLOT( SetGoMSpeedH() ) );
  connect( GoMSpeedM, SIGNAL( clicked() ), this, SLOT( SetGoMSpeedM() ) );
  connect( GoMSpeedL, SIGNAL( clicked() ), this, SLOT( SetGoMSpeedL() ) );

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

  connect( SPSScan, SIGNAL( clicked() ), this, SLOT( ScanStart() ) );
  connect( MStart, SIGNAL( clicked() ), this, SLOT( Monitor() ) );
  connect( SetUpMMRel, SIGNAL( clicked() ), this, SLOT( MMRel() ) );
  connect( SetUpMMAbs, SIGNAL( clicked() ), this, SLOT( MMAbs() ) );
  connect( SetUpSPSRel, SIGNAL( clicked() ), this, SLOT( SPSRel() ) );
  connect( SetUpSPSAbs, SIGNAL( clicked() ), this, SLOT( SPSAbs() ) );
}

void MainWindow::MMRel( void )
{
  GoMRelAbs = REL;
  ShowGoMRelAbs();
}

void MainWindow::MMAbs( void )
{
  GoMRelAbs = ABS;
  ShowGoMRelAbs();
}

void MainWindow::ShowGoMRelAbs( void )
{
  SetUpMMRel->setStyleSheet( ( GoMRelAbs == REL )? RadioBOn : RadioBOff );
  SetUpMMAbs->setStyleSheet( ( GoMRelAbs == ABS )? RadioBOn : RadioBOff );
}

void MainWindow::SPSRel( void )
{
  SPSRelAbs = REL;
  ShowSPSRelAbs();
}

void MainWindow::SPSAbs( void )
{
  SPSRelAbs = ABS;
  ShowSPSRelAbs();
}

void MainWindow::ShowSPSRelAbs( void )
{
  SetUpSPSRel->setStyleSheet( ( SPSRelAbs == REL )? RadioBOn : RadioBOff );
  SetUpSPSAbs->setStyleSheet( ( SPSRelAbs == ABS )? RadioBOn : RadioBOff );
}

void MainWindow::SetGoMSpeedH( void )
{
  GoMSpeed = HIGH;
  MMainTh->SetSpeed( HIGH );
  ShowGoMSpeed();
}

void MainWindow::SetGoMSpeedM( void )
{
  GoMSpeed = MIDDLE;
  MMainTh->SetSpeed( MIDDLE );
  ShowGoMSpeed();
}

void MainWindow::SetGoMSpeedL( void )
{
  GoMSpeed = LOW;
  MMainTh->SetSpeed( LOW );
  ShowGoMSpeed();
}

void MainWindow::ShowGoMSpeed( void )
{
  GoMSpeedH->setStyleSheet( ( GoMSpeed == HIGH )? RadioBOn : RadioBOff );
  GoMSpeedM->setStyleSheet( ( GoMSpeed == MIDDLE )? RadioBOn : RadioBOff );
  GoMSpeedL->setStyleSheet( ( GoMSpeed == LOW )? RadioBOn : RadioBOff );
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
      if ( GoMRelAbs == ABS ) {
	GoMotorPosPuls->setText( msg.Val() );
	GoMotorPosUnit->setText( val );
	setupMDispFirstTime = false;
      } else {
	GoMotorPosPuls->setText( 0 );
	GoMotorPosUnit->setText( 0 );
	setupMDispFirstTime = false;
      }
    }
    if ( am->checkNewVal() ) {
      NewLogMsg( tr( "Current Position of [%1] : [%2] %3\n" )
		 .arg( am->getName() )
		 .arg( val )
		 .arg( am->getUnit() ) );
    }
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

  if ( GoMRelAbs == ABS ) {
    GoMotorPosUnit
      ->setText( QString::number( ( val.toDouble() - am->getCenter() ) * am->getUPP() ) );
  } else {
    GoMotorPosUnit->setText( QString::number( val.toDouble() * am->getUPP() ) );
  }
}

void MainWindow::NewGoMotorPosUnit( const QString &val )
{
  AUnit *am = AMotors.value( MotorN->currentIndex() );

  if ( GoMRelAbs == ABS ) {
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
  MovingS = GoMotorS->currentIndex();

  if ( GoMRelAbs == REL )
    Pos += am->value().toDouble();

  if ( am->getType() == "PM" ) {
    Pos = (double)((int)Pos);
  }

  GoMotor->setText( tr( "Stop" ) );
  GoMotor->setStyleSheet( "background-color: yellow" );

  SPSScan->setEnabled( false );

  am->SetSpeed( MSpeeds[ MovingS ].MSid );
  am->SetValue( Pos );
  am->setIsBusy( true );

  MoveID = startTimer( 100 );

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
  SPSUnit->removeItem( 1 );
  SPSUnit->addItem( AMotors.value( MotorN->currentIndex() )->getUnit() );
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

  killTimer( MoveID );
  GoMotor->setEnabled( true );
  SPSScan->setEnabled( true );
  GoMotor->setText( tr( "Go" ) );
  GoMotor->setStyleSheet( "" );
  
}

void MainWindow::ScanStart( void )
{
  AUnit *am, *as;

  if ( inSPSing == 0 ) {
    ScanMotor = MotorN->currentIndex();
    ScanSensor = SelectD1->currentIndex();
    am = AMotors.value( ScanMotor );
    as = ASensors.value( ScanSensor );

    MovingS = SPSMotorS->currentIndex();  // motor speed;

    SPSSelU = SPSUnit->currentIndex();
    SPSUPP = ( SPSSelU == 0 ) ? 1 : am->getUPP();
    ScanOrigin = am->value().toDouble();
    ScanSP = am->any2p( SPSsP->text().toDouble(), SPSSelU, SPSRelAbs );
    ScanEP = am->any2p( SPSeP->text().toDouble(), SPSSelU, SPSRelAbs );
    ScanSTP = SPSstep->text().toDouble() / SPSUPP;
    if ( ScanEP > ScanSP ) {
      ScanSTP = abs( ScanSTP );
    } else {
      ScanSTP = - abs( ScanSTP );
    }
    ScanDT = SPSdwell->text().toDouble();
    if ( ScanSTP == 0 ) {
      statusbar->showMessage( tr( "Error: Scan Step is 0." ), 2000 );
      return;
    }
    inSPSing = 1;

    NewLogMsg( QString( tr( "Scan Start (%1 %2)\n" ) )
	       .arg( am->getName() )
	       .arg( as->getName() ) );
    
    am->SetSpeed( MSpeeds[ MovingS ].MSid );
    as->SetTime( ScanDT );

    SPSScan->setText( tr( "Stop" ) );
    SPSScan->setStyleSheet( "background-color: yellow" );
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
    SPSID = startTimer( 100 );
  } else {
    ScanStop0();
  }
}

void MainWindow::ScanStop0( void )
{
  ScanStage = 10;
}

void MainWindow::Monitor( void )
{
  AUnit *as0 = ASensors.value( SelectD20->currentIndex() );
  AUnit *as1 = ASensors.value( SelectD21->currentIndex() );
  AUnit *as2 = ASensors.value( SelectD22->currentIndex() );

  if ( inMonitor == 0 ) {
    inMonitor = 1;
    MonStage = 0;   // 計測のサイクル

    for ( int i = 0; i < MCHANNELS; i++ )
      MeasSensF[i] = false;
    MeasSens[0] = as0;   MeasSensF[0] = true;
    MeasSens[1] = as1;   MeasSensF[1] = SelectD21Sel->isChecked();
    MeasSens[2] = as2;   MeasSensF[2] = SelectD22Sel->isChecked();

    MeasSensDT[0] = DwellT20->text().toDouble();
    MeasSensDT[1] = DwellT21->text().toDouble();
    MeasSensDT[2] = DwellT22->text().toDouble();

    OneOfTheSensorIsCounter = false;
    OneOfTheSensorIsSSD = false;
    for ( int i = 0; i < MCHANNELS; i++ ) {
      if ( MeasSensF[i] ) {
	if ( MeasSens[i]->getType() == "CNT" ) {
	  OneOfTheSensorIsCounter = true;
	  TheCounter = MeasSens[i];
	}
	if ( MeasSens[i]->getType() == "SSD" || MeasSens[i]->getType() == "SSDP") {
	  OneOfTheSensorIsSSD = true;
	}
      }
    }

    MonView = XViews[ ViewTab->currentIndex() ];
    MonView->ClearDataR();
    MonView->SetLineF( RIGHT, LEFT, LEFT );   // 現状意味なし
    //    MonView->SetScaleT( I0TYPE, FULLSCALE, FULLSCALE );   // 現状意味なし
    MonView->SetDrawF( MeasSensF );
    //    MonView->SetLName( 0, tr( "I0" ) );
    int LineCount = 0;
    for ( int i = 0; i < 3; i++ ) {
      if ( MeasSensF[i] ) {
	MonView->SetLName( LineCount, MeasSens[i]->getName() );
	LineCount++;
      }
    }
    MonView->SetGType( MONITOR );                            // 確認
    MonView->makeValid( true );                              // 確認

    MonView->SetMonScale( SelectScale->currentIndex() );
    connect( SelectScale, SIGNAL( currentIndexChanged( int ) ),
	     MonView, SLOT( SetMonScale( int ) ) );
    connect( as0, SIGNAL( newValue( QString ) ), this, SLOT( newVI0( QString ) ) );
    if ( MeasSensF[1] )
      connect( as1, SIGNAL( newValue( QString ) ), this, SLOT( newVS1( QString ) ) );
    if ( MeasSensF[2] )
      connect( as2, SIGNAL( newValue( QString ) ), this, SLOT( newVS2( QString ) ) );
		 
    MStart->setText( tr( "Stop" ) );
    MStart->setStyleSheet( "background-color: yellow" );

    MonTime.restart();
    MonID = startTimer( 50 );    /* 50msタイマーセット */
  } else {
    killTimer( MonID );
    inMonitor = 0;

    disconnect( SelectScale, SIGNAL( currentIndexChanged( int ) ),
	     MonView, SLOT( SetMonScale( int ) ) );
    disconnect( as0, SIGNAL( newValue( QString ) ), this, SLOT( newVI0( QString ) ) );
    if ( MeasSensF[1] )
      disconnect( as1, SIGNAL( newValue( QString ) ), this, SLOT( newVS1( QString ) ) );
    if ( MeasSensF[2] )
      disconnect( as2, SIGNAL( newValue( QString ) ), this, SLOT( newVS2( QString ) ) );

    MStart->setText( tr( "Mon. Start" ) );
    MStart->setStyleSheet( "" );
  }
}

void MainWindow::newVI0( QString v )
{
  DevCurV20->setText( QString("%1").arg( v.toDouble(), 8, 'g' ) );
}

void MainWindow::newVS1( QString v )
{
  DevCurV21->setText( QString("%1").arg( v.toDouble(), 8, 'g' ) );
}

void MainWindow::newVS2( QString v )
{
  DevCurV22->setText( QString("%1").arg( v.toDouble(), 8, 'g' ) );
}
