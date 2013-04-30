#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupSetupArea( void )   /* 設定エリア */
{
  GoUnit << GoUnit1 << GoUnit2 << GoUnit3 << GoUnit4;
  GoPosEdit << GoPos1 << GoPos2 << GoPos3 << GoPos4;

  double Eg = ManTEkeV->text().toDouble();

  //  inMove = 0;
  inMMove = 0;
  inMonitor = 0;
  inSPSing = 0;

  ScanView = NULL;
  MonitorView = NULL;

  GoMRelAbs = REL;
  SPSRelAbs = REL;
  ShowGoMRelAbs();
  ShowSPSRelAbs();

  GoMSpeed = MIDDLE;

  setupMDispFirstTime = true;
  monRecF = false;

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
  GoUnit0->setCurrentIndex( DefaultUnit );
  for ( int i = 0; i < GOS; i++ ) {
    GoUnit[i]->setCurrentIndex( DefaultUnit );
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
  SPSUnit->addItem( "Pulse" );
  SPSUnit->addItem( AMotors.value( MotorN->currentIndex() )->getUnit() );

  for ( int i = 0; i < ASensors.count(); i++ ) {
    SelectD1->addItem( ASensors.value(i)->getName() );
    SelectD10->addItem( ASensors.value(i)->getName() );
    SelectD20->addItem( ASensors.value(i)->getName() );
    SelectD21->addItem( ASensors.value(i)->getName() );
    SelectD22->addItem( ASensors.value(i)->getName() );
    SelectD3->addItem( ASensors.value(i)->getName() );
    connect( ASensors.value(i), SIGNAL( newDark( double ) ),
	     this, SLOT( ShowNewDark( double ) ) );
    if ( ASensors.at(i)->isRangeSelectable() ) {
      SelSensToSetRange->addItem( ASensors.at(i)->getName() );
      SensWithRange << ASensors.at(i);
      ASensors.at(i)->setRange( ASensors.at(i)->getRangeU() );
      connect( ASensors.at(i), SIGNAL( AskedNowRange( int ) ),
	       this, SLOT( GotNowRange( int ) ) );
    }
  }

  if ( SensWithRange.count() > 0 ) {
    RangeSelect->setRange( SensWithRange.at(0)->getRangeL(),
			   SensWithRange.at(0)->getRangeU() );
    RangeSelect->setValue( SensWithRange.at(0)->getRange() );
    SetAutoRangeMode( 0 );
  }

  connect( SelectAutoRange, SIGNAL( toggled( bool ) ),
	   this, SLOT( SelAutoRange( bool ) ) );
  connect( SelSensToSetRange, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( newSensSelected( int ) ) );
  connect( RangeSelect, SIGNAL( valueChanged( int ) ),
	   this, SLOT( newRangeSelected( int ) ) );
  connect( GetRange, SIGNAL( clicked() ), this, SLOT( askNowRange() ) );
  connect( GetAllRange, SIGNAL( clicked() ), this, SLOT( askNowRanges() ) );

  InputDark
    ->setText( QString::number( ASensors.at( SelectD3->currentIndex() )->getDark() ) );
  connect( SelectD3, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( NewDarkChSelected( int ) ) );
  connect( SetDark, SIGNAL( clicked() ), this, SLOT( AskedToSetDark() ) );

  for ( int i = 0; i < MSCALES; i++ ) {
    SelectScale->addItem( MScales[i].MSName );
  }
  MonStage = 0;

  scanFSel = new QFileDialog;
  scanFSel->setAcceptMode( QFileDialog::AcceptSave );
  scanFSel->setDirectory( QDir::currentPath() );
  scanFSel->setFilter( "*.dat" );

  monFSel = new QFileDialog;
  monFSel->setAcceptMode( QFileDialog::AcceptSave );
  monFSel->setDirectory( QDir::currentPath() );
  monFSel->setFilter( "*.dat" );

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

  connect( SelMonRecFile, SIGNAL( clicked() ), monFSel, SLOT( show() ) );
  connect( monFSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( setSelectedMonFName( const QString & ) ) );

  connect( SelScanRecFile, SIGNAL( clicked() ), scanFSel, SLOT( show() ) );
  connect( scanFSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( setSelectedScanFName( const QString & ) ) );
  connect( ScanRec, SIGNAL( clicked() ), this, SLOT( saveScanData() ) );
  connect( SaveMonData, SIGNAL( clicked() ), this, SLOT( saveMonData() ) );
}

void MainWindow::newSensSelected( int i )
{
  RangeSelect->setRange( SensWithRange.at(i)->getRangeL(),
			 SensWithRange.at(i)->getRangeU() );
  RangeSelect->setValue( SensWithRange.at(i)->getRange() );
  SetAutoRangeMode( i );
}

void MainWindow::SetAutoRangeMode( int i )
{
  if ( SensWithRange.at(i)->isAutoRangeAvailable() ) {
    SelectAutoRange->setEnabled( true );
    if ( SensWithRange.at(i)->isAutoRange() ) {
      RangeSelect->setEnabled( false );
      SelectAutoRange->setChecked( true );
    } else {
      RangeSelect->setEnabled( true );
      SelectAutoRange->setChecked( false );
    }
  } else {
    SelectAutoRange->setEnabled( false );
  }
}

void MainWindow::SelAutoRange( bool Auto )
{
  AUnit *as = SensWithRange.at( SelSensToSetRange->currentIndex() );
  as->setAutoRange( Auto );
  RangeSelect->setEnabled( !Auto );
  if ( I0Sensors[ SelectI0->currentIndex() ] == as )
    I0Range->setEnabled( !Auto );
  if ( I1Sensors[ SelectI1->currentIndex() ] == as )
    I1Range->setEnabled( !Auto );
  if ( A1Sensors[ SelectAux1->currentIndex() ] == as )
    A1Range->setEnabled( !Auto );
  if ( A2Sensors[ SelectAux2->currentIndex() ] == as )
    A2Range->setEnabled( !Auto );
}

void MainWindow::newRangeSelected( int i )
{
  AUnit *as = SensWithRange[ SelSensToSetRange->currentIndex() ];
  as->setRange( i );
  if ( as == I0Sensors[ SelectI0->currentIndex() ] )
    I0Range->setValue( i );
  if ( as == I1Sensors[ SelectI1->currentIndex() ] )
    I1Range->setValue( i );
  if ( as == A1Sensors[ SelectAux1->currentIndex() ] )
    A1Range->setValue( i );
  if ( as == A1Sensors[ SelectAux2->currentIndex() ] )
    A2Range->setValue( i );
}

void MainWindow::askNowRange( void )
{
  SensWithRange.at( SelSensToSetRange->currentIndex() )->GetRange();
}

void MainWindow::askNowRanges( void )
{
  for ( int i = 0; i < SensWithRange.count(); i++ ) {
    SensWithRange.at( i )->GetRange();
  }
}

void MainWindow::GotNowRange( int r ) // This function is pure SLOT as it used 'sender()'
{
  for ( int i = 0; i < SensWithRange.count(); i++ ) {
    if ( SensWithRange.at( i ) == sender() ) {
      SensWithRange.at( i )->setRange( r );
      if ( i == SelSensToSetRange->currentIndex() )
	RangeSelect->setValue( r );
    }
  }
}

void MainWindow::saveScanData( void )
{
  if ( ScanView == NULL ) {
    statusbar->showMessage( tr( "Scan data is not valid" ), 2000 );
    return;
  }
  if ( ScanRecFile->text().isEmpty() ) {
    statusbar->showMessage( tr( "Save file name is not selected" ), 2000 );
    return;
  }

  QFile f( ScanRecFile->text() );
  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
    statusbar->showMessage( tr( "The file [%1] can not open to record the data" ),
			    2000 );
    return;
  }
  ScanDataStat = OLD;
  ScanNameStat = OLD;
  ScanRecFile->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
  ScanRecFile->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );

  QTextStream out( &f );

  AUnit *am = AMotors.value( ScanMotor );

  out << "# XafsM2 Scan Data\n";
  out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" ) << "\n";
  out << "#\t";
  for ( int i = 0; i < mUnits.count(); i++ )
    out << mUnits.at(i)->getName() << "\t";
  out << am->getName() << "\t";
  out << SPSUnit->itemText( SPSSelU ) << "\t";
  out << SPSUPP << "\t";
  out << am->getCenter() << "\n";

  int points = ScanView->GetPoints( 1 );

  for ( int i = 0; i < points; i++ ) {
    out << ScanView->GetX( 0, i )
	<< "\t" << ScanView->GetY( 0, i ) << "\t" << ScanView->GetY( 1, i ) << "\n";
  }

  f.close();
}

void MainWindow::setSelectedMonFName( const QString &fname )
{
  MonRecFile->setText( fname );
  MonNameStat = NEW;
  MonRecFile->setStyleSheet( FSTATCOLORS[ MonDataStat ][ MonNameStat ] );
  MonRecFile->setToolTip( FSTATMsgs[ MonDataStat ][ MonNameStat ] );
}

void MainWindow::setSelectedScanFName( const QString &fname )
{
  ScanRecFile->setText( fname );
  ScanNameStat = NEW;
  ScanRecFile->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
  ScanRecFile->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );
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
  QString val0;
  QString val;

  AUnit *am = AMotors.value( MotorN->currentIndex() );

  if ( ( msg.From() == am->getDevCh() )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE ) ) ) {
    if ( ( am->getType() == "SC" ) && ( msg.Msgt() == GETVALUE ) ) {
      val0 = msg.Vals().at(1);
    } else {
      val0 = msg.Val();
    }
    MCurPosPuls->setText( val0 );
    val = QString::number
      ( ( val0.toDouble() - am->getCenter() ) * am->getUPP() );
    MCurPosUnit->setText( val );
    if ( setupMDispFirstTime == true ) {  // 最初の一回だけ
      if ( GoMRelAbs == ABS ) {
	GoMotorPosPuls->setText( val0 );
	GoMotorPosUnit->setText( val );
	setupMDispFirstTime = false;
      } else {
	GoMotorPosPuls->setText( 0 );
	GoMotorPosUnit->setText( 0 );
	setupMDispFirstTime = false;
      }
    }
    if ( am->checkNewVal() ) {
      NewLogMsg( tr( "Current Position of [%1] : [%2] %3" )
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
  buf.sprintf( UnitName[unit].form, u->keV2any( unit, GoPosKeV[ i ] ) );
  GoPosEdit[i]->setText( buf );
}

void MainWindow::GetNewGo( int i )
{
  GoPosKeV[i] = u->any2keV( (UNIT)GoUnit[i]->currentIndex(),
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
  GoMotor->setStyleSheet( InActive );

  SPSScan->setEnabled( false );

  am->SetSpeed( MSpeeds[ MovingS ].MSid );
  am->SetValue( Pos );
  am->setIsBusy( true );

  GoTimer->start( 100 );

  NewLogMsg( QString( tr( "Setup: %1 : GoTo %2 : Speed %3" ) )
	     .arg( am->getName() )
	     .arg( GoMotorPosPuls->text().toInt() )
	     .arg( MSpeeds[ MovingS ].MSName ) );
  statusbar->showMessage( QString( tr( "Setup: %1 : GoTo %2 : Speed %3" ) )
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

  NewLogMsg( QString( tr( "Setup: %1 : Stopped at %2" ) )
	     .arg( am->getName() )
	     .arg( am->value() ) );
  statusbar->showMessage( QString( tr( "Setup: %1 : Stopped at %2" ) )
			  .arg( am->getName() )
			  .arg( am->value() ), 1000 );
}

void MainWindow::GoMStop0( void )
{
  inMMove = 0;

  GoTimer->stop();
  GoMotor->setEnabled( true );
  SPSScan->setEnabled( true );
  GoMotor->setText( tr( "Go" ) );
  GoMotor->setStyleSheet( NormalB );
}

void MainWindow::ScanStart( void )
{
  AUnit *am, *as, *as1 = NULL;

  if ( inSPSing == 0 ) {
    if ( ( ScanViewC = SetUpNewView( XYVIEW ) ) == NULL ) {
      statusbar->showMessage( tr( "No drawing screen is available" ), 2000 );
      return;
    }
    ViewTab->setTabText( ViewTab->currentIndex(), "SCAN" );
    ScanViewC->setNowDType( SCANDATA );
    ScanView = (XYView*)(ScanViewC->getView());

    ScanMotor = MotorN->currentIndex();
    am = AMotors.value( ScanMotor );
    mUnits.clearUnits();
    mUnits.addUnit( as = ASensors.value( SelectD1->currentIndex() ) );
    mUnits.addUnit( as1 = ASensors.value( SelectD10->currentIndex() ) );
    mUnits.setDwellTimes( SPSdwell->text().toDouble() );
    mUnits.setDwellTime();

    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( ! CheckOkList( mUnits.at(i), NXafsOk ) ) {
	QString msg = tr( "The Sensor (%1) can use only in QXafs mode." )
	  .arg( mUnits.at(i)->getName() );
	statusbar->showMessage( msg, 2000 );
	return;
      }
    }
    if ( ! am->isEnable() ) {
      QString msg = tr( "Scan cannot Start : (%1) is disabled" )
	.arg( am->getName() );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
    }
    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( ! mUnits.at(i)->isEnable() ) {
	QString msg = QString( tr( "Scan cannot Start : (%1) is disabled" ) )
	  .arg( mUnits.at(i)->getName() );
	statusbar->showMessage( msg, 2000 );
	NewLogMsg( msg );
	return;
      }
    }
    MovingS = SPSMotorS->currentIndex();  // motor speed;

    SPSSelU = SPSUnit->currentIndex();
    SPSUPP = ( SPSSelU == 0 ) ? 1 : am->getUPP();
    ScanOrigin = am->value().toDouble();
    ScanSP = am->any2p( SPSsP->text().toDouble(), SPSSelU, SPSRelAbs );
    ScanEP = am->any2p( SPSeP->text().toDouble(), SPSSelU, SPSRelAbs );
    ScanSTP = SPSstep->text().toDouble() / SPSUPP;
    if ( ScanEP > ScanSP ) {
      ScanSTP = fabs( ScanSTP );
    } else {
      ScanSTP = - fabs( ScanSTP );
    }
    if ( ScanSTP == 0 ) {
      statusbar->showMessage( tr( "Error: Scan Step is 0." ), 2000 );
      return;
    }
    inSPSing = 1;

    NewLogMsg( QString( tr( "Scan Start (%1 %2)" ) )
	       .arg( am->getName() )
	       .arg( as->getName() ) );
    
    am->SetSpeed( MSpeeds[ MovingS ].MSid );

    SPSScan->setText( tr( "Stop" ) );
    SPSScan->setStyleSheet( InActive );
    GoMotor->setEnabled( false );

    ScanView->Clear();
    ScanView->SetLR( 0, LEFT_AX );   // 0 番目の線はグループ 0, 1 番目の線はグループ 1
    ScanView->SetLR( 1, RIGHT_AX );   // 0 番目の線はグループ 0, 1 番目の線はグループ 1
    ScanView->SetScaleType( 0, FULLSCALE ); // グループ 0 も 1 も FULLSCALE
    ScanView->SetScaleType( 1, FULLSCALE ); // グループ 0 も 1 も FULLSCALE
    ScanView->SetLeftName( " " );
    ScanView->SetRightName( " " );
    for ( int i = 0; i < mUnits.count(); i++ )
      ScanView->SetLineName( i, mUnits.at(i)->getName() );
    ScanView->SetXName( am->getName() );
    ScanView->SetXUnitName( SPSUnit->itemText( SPSSelU ) );
    ScanView->SetUpp( SPSUPP );
    ScanView->SetCenter( am->getCenter() );
    ScanView->SetAutoScale( true );
    ScanView->makeValid( true );

    ScanDataStat = NEW;
    ScanRecFile->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
    ScanRecFile->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );
    
    ScanStage = 0;
    ScanTimer->start( 100 );
    ScanViewC->setIsDeletable( false );
  } else {
    ScanViewC->setIsDeletable( true );
    ScanStop0();
  }
}

void MainWindow::ClearXViewScreenForScan( XYView *view )
{
  view->Clear();
  view->makeValid( true );
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
    if ( ! as0->isEnable() ) {
      QString msg = QString( tr( "Scan cannot Start : (%1) is disabled" ) )
	.arg( as0->getName() );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
    }
    if ( ( MonitorViewC = SetUpNewView( TYVIEW ) ) == NULL ) {
      statusbar->showMessage( tr( "No drawing area is avairable" ) );
      return;
    }
    ViewTab->setTabText( ViewTab->currentIndex(), "MON." );
    if ( IsMonRec->isChecked() ) {
      if ( MonRecFile->text().isEmpty() ) {
	statusbar->showMessage( tr ( "No Record file is selected" ) );
	return;
      } else {
	MonDataStat = OLD;
	MonNameStat = OLD;
	MonRecFile->setStyleSheet( FSTATCOLORS[ MonDataStat ][ MonNameStat ] );
	MonRecFile->setToolTip( FSTATMsgs[ MonDataStat ][ MonNameStat ] );

	monRecF = true;
	MonFile.setFileName( MonRecFile->text() );
	if ( !MonFile.open( QIODevice::Append | QIODevice::Text ) ) {
	  statusbar->showMessage( tr( "The file [%1] can not open to record the data" ),
				      2000 );
	  return;
	}
	MonOut.setDevice( &MonFile );

	MonOut << "# XafsM2 Monitor Data\n";
	MonOut << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" )
	       << "\n";
      }
    } else {
      monRecF = false;
    }

    MonitorViewC->setNowDType( MONDATA );
    MonitorView = (TYView*)(MonitorViewC->getView());
    
    inMonitor = 1;
    MonStage = 0;   // 計測のサイクル

    mUnits.clearUnits();
    mUnits.addUnit( as0 );
    MonSensF[0] = true;
    if ( SelectD21Sel->isChecked() ) {
      if ( ! as1->isEnable() ) {
	QString msg = QString( tr( "Scan cannot Start : (%1) is disabled" ) )
	  .arg( as1->getName() );
	statusbar->showMessage( msg, 2000 );
	NewLogMsg( msg );
	return;
      }
      mUnits.addUnit( as1 );
      MonSensF[1] = true;
    }
    if ( SelectD22Sel->isChecked() ) {
      if ( ! as2->isEnable() ) {
	QString msg = QString( tr( "Scan cannot Start : (%1) is disabled" ) )
	  .arg( as2->getName() );
	statusbar->showMessage( msg, 2000 );
	NewLogMsg( msg );
	return;
      }
      mUnits.addUnit( as2 );
      MonSensF[2] = true;
    }
    mUnits.setDwellTimes( DwellT20->text().toDouble() );
    mUnits.setDwellTime();

    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( ! CheckOkList( mUnits.at(i), NXafsOk ) ) {
	QString msg = tr( "The Sensor [%1] can use only in QXafs mode." )
	  .arg( mUnits.at(i)->getName() );
	statusbar->showMessage( msg, 2000 );
	return;
      }
    }

    if ( monRecF ) {
      MonOut << "#\tsec";
      for ( int i = 0; i < mUnits.count(); i++ ) {
	MonOut << QString( tr( "\t%1[%2]" )
			   .arg( mUnits.getName( i ) )
			   .arg( mUnits.getUnit( i ) ) );
      }
      MonOut << "\n";
    }

    MonitorView->ClearDataR();
    for ( int i = 0; i < mUnits.count(); i++ ) {
      MonitorView->SetLName( i, mUnits.getName( i ) );
    }
    MonitorView->SetLines( mUnits.count() );
    MonitorView->makeValid( true );

    MonitorView->SetMonScale( SelectScale->currentIndex() );
    connect( SelectScale, SIGNAL( currentIndexChanged( int ) ),
	     MonitorView, SLOT( SetMonScale( int ) ) );
    connect( as0, SIGNAL( newValue( QString ) ), this, SLOT( newVI0( QString ) ) );
    if ( MonSensF[1] )
      connect( as1, SIGNAL( newValue( QString ) ), this, SLOT( newVS1( QString ) ) );
    if ( MonSensF[2] )
      connect( as2, SIGNAL( newValue( QString ) ), this, SLOT( newVS2( QString ) ) );
		 
    MStart->setText( tr( "Stop" ) );
    MStart->setStyleSheet( InActive );

    if ( !monRecF ) {
      MonDataStat = NEW;
      MonRecFile->setStyleSheet( FSTATCOLORS[ MonDataStat ][ MonNameStat ] );
      MonRecFile->setToolTip( FSTATMsgs[ MonDataStat ][ MonNameStat ] );
    }

    MonitorViewC->setIsDeletable( false );
    MonTime.restart();
    MonTimer->start( 100 );
  } else {
    if ( monRecF ) {
      MonFile.close();
    }
    MonTimer->stop();
    inMonitor = 0;

    disconnect( SelectScale, SIGNAL( currentIndexChanged( int ) ),
	     MonitorView, SLOT( SetMonScale( int ) ) );
    disconnect( as0, SIGNAL( newValue( QString ) ), this, SLOT( newVI0( QString ) ) );
    if ( MonSensF[1] )
      disconnect( as1, SIGNAL( newValue( QString ) ), this, SLOT( newVS1( QString ) ) );
    if ( MonSensF[2] )
      disconnect( as2, SIGNAL( newValue( QString ) ), this, SLOT( newVS2( QString ) ) );

    MonitorViewC->setIsDeletable( true );
    MStart->setText( tr( "Mon. Start" ) );
    MStart->setStyleSheet( NormalB );
  }
}

void MainWindow::saveMonData( void )
{
  if ( MonitorView == NULL ) {
    statusbar->showMessage( tr( "Monitor data is not valid" ), 2000 );
    return;
  }
  if ( MonRecFile->text().isEmpty() ) {
    statusbar->showMessage( tr( "Save file name is not selected" ), 2000 );
    return;
  }

  QFile f( MonRecFile->text() );
  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
    statusbar->showMessage( tr( "The file [%1] can not open to record the data" ),
			    2000 );
    return;
  }
  MonDataStat = OLD;
  MonNameStat = OLD;
  MonRecFile->setStyleSheet( FSTATCOLORS[ MonDataStat ][ MonNameStat ] );
  MonRecFile->setToolTip( FSTATMsgs[ MonDataStat ][ MonNameStat ] );

  QTextStream out( &f );

  out << "# XafsM2 Monitor Data\n";
  out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" )
      << "\n";
  out << "#\tsec";
  for ( int i = 0; i < mUnits.count(); i++ ) {
    out << QString( tr( "\t%1[%2]" )
		    .arg( mUnits.getName( i ) )
		    .arg( mUnits.getUnit( i ) ) );
  }
  out << "\n";

  int points = MonitorView->getDatas();
  int lines = MonitorView->GetLines();

  for ( int i = 0; i < points; i++ ) {
    out << (double)(MonitorView->getT( i ))/1000;
    for ( int j = 0; j < lines; j++ ) {
      out << "\t" << MonitorView->getY( j, i );
    }
    out << "\n";
  }

  f.close();
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
