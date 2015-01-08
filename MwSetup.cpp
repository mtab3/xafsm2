
#include "MainWindow.h"

void MainWindow::setupSetupArea( void )   /* 設定エリア */
{
  // モニタの 0 番は常に on 
  MonSel0->setEnabled( false );

  GoUnit << GoUnit1 << GoUnit2 << GoUnit3 << GoUnit4;
  GoPosEdit << GoPos1 << GoPos2 << GoPos3 << GoPos4;
  GoTos << GoTo1 << GoTo2 << GoTo3 << GoTo4;

  monSels << MonSel0 << MonSel1 << MonSel2 << MonSel3 << MonSel4;
  monDevs << MonDev0 << MonDev1 << MonDev2 << MonDev3 << MonDev4;
  monVals << MonV0 << MonV1 << MonV2 << MonV3 << MonV4;

  for ( int i = 0; i < monSels.count(); i++ ) {
    MonLine *aml = new MonLine( monSels[i], monDevs[i], monVals[i] );
    monLines << aml;
  }
  QGridLayout *L = (QGridLayout*)MonLinesGrid->layout();
  if ( monLines.count() < TYView::maxMon() ) {
    for ( int i = monLines.count(); i < TYView::maxMon(); i++ ) {
      MonLine *aml = new MonLine;
      monLines << aml;
      L->addWidget( aml->sel(), i+1, 0 );
      L->addWidget( aml->dev(), i+1, 1 );
      L->addWidget( aml->val(), i+1, 2 );
      aml->copyStyles( *monLines[0] );
    }
  }

  MonLinesDialog = new QDialog;
  MonLinesDialog->resize( 350, 510 );
  QGridLayout *bl = new QGridLayout;
  MonLinesDialog->setLayout( bl );
  connect( MonLinesDialog, SIGNAL( finished(int) ), this, SLOT( PopDownMonLines() ),
	   Qt::UniqueConnection );
  connect( MonLinesPop, SIGNAL( toggled( bool ) ), this, SLOT( PopChangeMonLines( bool ) ), 
	   Qt::UniqueConnection );

  double Eg = ManTEkeV->text().toDouble();

  //  inMove = 0;
  inMMove = false;
  inMonitor = false;
  MinPause = false;
  inSPSing = false;

  ScanView = NULL;
  MonitorView = NULL;

  GoMSpeed = MIDDLE;

  setupMDispFirstTime = true;
  monRecF = false;
  LastInIsPulsV = true;

  GoPosKeV[0] = Eg - 0.50;
  GoPosKeV[1] = Eg - 0.05;
  GoPosKeV[2] = Eg + 0.10;
  GoPosKeV[3] = Eg + 1.10;
  ScanStage = 0;

  SInfo.valid = false;

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
	   this, SLOT( SetAllGoUnits( int ) ),
	   Qt::UniqueConnection );
  for ( int i = 0; i < GOS; i++ ) {
    connect( GoUnit[i], SIGNAL( currentIndexChanged( int ) ),
	     this, SLOT( ShowAllGos() ),
	     Qt::UniqueConnection );
  }
  for ( int i = 0; i < GOS; i++ ) {
    connect( GoPosEdit[i], SIGNAL( editingFinished() ),
	     this, SLOT( GetNewGos() ),
	     Qt::UniqueConnection );
  }

  for ( int i = 0; i < AMotors.count(); i++ ) {
    SPSMotorSelect->addItem( AMotors.value(i)->getName() );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( ShowCurMotorPos( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( ShowCurMotorPos( SMsg ) ),
	     Qt::UniqueConnection );

    ScanPSet *aScanP = new ScanPSet;
    aScanP->Unit = ( UseDefUReal( AMotors[i] ) ) ? 1 : 0;
    ScanPs << aScanP;
  }
  for ( int i = 0; i < MSPEEDS; i++ ) {
    GoMotorS->addItem( MSpeeds[i].MSName );
    SPSMotorSpeed->addItem( MSpeeds[i].MSName );
  }
  GoMotorS->setCurrentIndex( 1 );

  connect( MMRelAbs, SIGNAL( statChanged( RELABS ) ),
	   this, SLOT( NewMMRelAbs( RELABS ) ),
	   Qt::UniqueConnection );

  SPSLastSelectedM = SPSMotorSelect->currentIndex();
  SPSMotorSpeed->setCurrentIndex( 1 );
  AMotors.at( SPSMotorSelect->currentIndex() )->GetValue();
  GoMotorUnit->setText( AMotors.value( SPSMotorSelect->currentIndex() )->getUnit() );
  SPSUnitSelect->addItem( "Pulse" );
  SPSUnitSelect->addItem( AMotors.value( SPSMotorSelect->currentIndex() )->getUnit() );
  PutScanPSet( ScanPs[ SPSMotorSelect->currentIndex() ] );

  for ( int i = 0; i < ASensors.count(); i++ ) {
    SPSSelectD1->addItem( ASensors.value(i)->getName() );
    SPSSelectD10->addItem( ASensors.value(i)->getName() );
    for ( int j = 0; j < monLines.count(); j++ ) {
      monLines[j]->addItem( ASensors.value(i)->getName() );
    }
    SelectD3->addItem( ASensors.value(i)->getName() );
    connect( ASensors.value(i), SIGNAL( newDark( double ) ),
	     this, SLOT( ShowNewDark( double ) ),
	     Qt::UniqueConnection );
    if ( ASensors.at(i)->isRangeSelectable() ) {
      SelSensToSetRange->addItem( ASensors.at(i)->getName() );
      SensWithRange << ASensors.at(i);
      ASensors.at(i)->setRange( ASensors.at(i)->getRangeU() );
      connect( ASensors.at(i), SIGNAL( AskedNowRange( int ) ),
	       this, SLOT( GotNowRange( int ) ),
	       Qt::UniqueConnection );
    }
  }

  if ( SensWithRange.count() > 0 ) {
    RangeSelect->setRange( SensWithRange.at(0)->getRangeL(),
			   SensWithRange.at(0)->getRangeU() );
    RangeSelect->setValue( SensWithRange.at(0)->getRange() );
    SetAutoRangeMode( 0 );
  }

  connect( SelectAutoRange, SIGNAL( toggled( bool ) ),
	   this, SLOT( SelAutoRange( bool ) ),
	   Qt::UniqueConnection );
  connect( SelSensToSetRange, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( newSensSelected( int ) ),
	   Qt::UniqueConnection );
  connect( RangeSelect, SIGNAL( valueChanged( int ) ),
	   this, SLOT( newRangeSelected( int ) ),
	   Qt::UniqueConnection );
  connect( GetRange, SIGNAL( clicked() ), this, SLOT( askNowRange() ),
	   Qt::UniqueConnection );
  connect( GetAllRange, SIGNAL( clicked() ), this, SLOT( askNowRanges() ),
	   Qt::UniqueConnection );
  connect( SetRange, SIGNAL( clicked() ), this, SLOT( askSetRange() ),
	   Qt::UniqueConnection );

  InputDark
    ->setText( QString::number( ASensors.at( SelectD3->currentIndex() )->getDark() ) );
  connect( SelectD3, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( NewDarkChSelected( int ) ),
	   Qt::UniqueConnection );
  connect( SetDark, SIGNAL( clicked() ), this, SLOT( AskedToSetDark() ),
	   Qt::UniqueConnection );

  for ( int i = 0; i < MSCALES; i++ ) {
    SelectScale->addItem( MScales[i].MSName );
  }
  MonStage = 0;

  scanFSel = new QFileDialog;
  scanFSel->setAcceptMode( QFileDialog::AcceptSave );
  scanFSel->setDirectory( QDir::currentPath() );
  scanFSel->setNameFilter( "*.dat" );

  monFSel = new QFileDialog;
  monFSel->setAcceptMode( QFileDialog::AcceptSave );
  monFSel->setDirectory( QDir::currentPath() );
  monFSel->setNameFilter( "*.dat" );

  MPause->setEnabled( false );

  connect( GoMSpeedH, SIGNAL( clicked() ), this, SLOT( SetGoMSpeedH() ),
	   Qt::UniqueConnection );
  connect( GoMSpeedM, SIGNAL( clicked() ), this, SLOT( SetGoMSpeedM() ),
	   Qt::UniqueConnection );
  connect( GoMSpeedL, SIGNAL( clicked() ), this, SLOT( SetGoMSpeedL() ),
	   Qt::UniqueConnection );

  for ( int i = 0; i < GoTos.count(); i++ ) {
    connect( GoTos[i], SIGNAL( clicked() ), this, SLOT( GoToPosKeV() ),
	     Qt::UniqueConnection );
  }

  connect( SPSMotorSelect, SIGNAL( currentIndexChanged( int ) ), this, SLOT( NewMotor() ),
	   Qt::UniqueConnection );
  connect( GoMotor, SIGNAL( clicked() ), this, SLOT( GoMAtP() ),
	   Qt::UniqueConnection );
  connect( GoMotorPosPuls, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( NewGoMotorPosPuls( const QString & ) ),
	   Qt::UniqueConnection );
  connect( GoMotorPosUnit, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( NewGoMotorPosUnit( const QString & ) ),
	   Qt::UniqueConnection );

  connect( SPSScan, SIGNAL( clicked() ), this, SLOT( ScanStart() ),
	   Qt::UniqueConnection );
  connect( MStart, SIGNAL( clicked() ), this, SLOT( Monitor() ),
	   Qt::UniqueConnection );
  connect( MPause, SIGNAL( clicked() ), this, SLOT( PauseMonitor() ),
	   Qt::UniqueConnection );

  connect( SelMonRecFile, SIGNAL( clicked() ), monFSel, SLOT( show() ),
	   Qt::UniqueConnection );
  connect( monFSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( setSelectedMonFName( const QString & ) ),
	   Qt::UniqueConnection );

  connect( SelScanRecFile, SIGNAL( clicked() ), scanFSel, SLOT( show() ),
	   Qt::UniqueConnection );
  connect( scanFSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( setSelectedScanFName( const QString & ) ),
	   Qt::UniqueConnection );
  connect( ScanRec, SIGNAL( clicked() ), this, SLOT( saveScanData() ),
	   Qt::UniqueConnection );
  connect( SaveMonData, SIGNAL( clicked() ), this, SLOT( saveMonData() ),
	   Qt::UniqueConnection );
  connect( MMainTh, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( ToggleGoToButtons( QString ) ),
	   Qt::UniqueConnection );

  connect( MovingAvr, SIGNAL( edigingFinished() ), this, SLOT( newMovingAvr() ),
	   Qt::UniqueConnection );
}

void MainWindow::setupDataRoot( void )
{
  DataRootSelect = new QFileDialog;
  DataRootSelect->setFileMode( QFileDialog::Directory );

  connect( DataRoot, SIGNAL( textChanged( const QString & ) ),
	   this, SLOT( newDataRoot( const QString & ) ),
	   Qt::UniqueConnection );

  if ( DataRoot0 != "" ) {          // DataRoot の指定があった
    QFileInfo droot( DataRoot0 );
    if ( droot.exists() ) {   // 指定された名前があった
      if ( !droot.isDir() ) { // 指定された名前がファイルだった (ディレクトリではなかった)
	DataRoot0 = "";      // 指定は無かったことにする
      }
    } else {                  // 指定された名前が無のディレクトリを作るように努力する
      QDir newdir;
      if ( ! newdir.mkpath( DataRoot0 ) ) { // 作れなかったら
	DataRoot0 = "";       // 指定は無かったことにする
      }
    }
  }
  if ( DataRoot0 == "" ) { // 上の if の中で DataRoot0="" にする可能性があるので改めて訊く
    DataRoot0 = QDir::currentPath(); 
  }

  DataRootSelect->setDirectory( DataRoot0 );
  DataRoot->setText( DataRoot0 );  // この signal で newDataRoot を呼ぶ
  EditDFName->setText( QFileInfo( DataRoot0, EditDFName->text() ).filePath() );

  connect( SelDataRoot, SIGNAL( clicked() ), DataRootSelect, SLOT( show() ),
	   Qt::UniqueConnection );
  connect( DataRootSelect, SIGNAL( directoryEntered( const QString & ) ),
	   DataRoot, SLOT( setText( const QString & ) ),
	   Qt::UniqueConnection );
}

void MainWindow::newDataRoot( const QString &dataRoot )
{
  NewLogMsg( QString( "Data root is set at %1" ).arg( dataRoot ) );

  if ( MCAFSel != NULL )
    MCAFSel->setDirectory( dataRoot );
  if ( scanFSel != NULL )
    scanFSel->setDirectory( dataRoot );
  if ( monFSel != NULL )
    monFSel->setDirectory( dataRoot );
  if ( S2DFileSel != NULL )
    S2DFileSel->setDirectory( dataRoot );
  if ( SelDFND != NULL )
    SelDFND->setDirectory( dataRoot );
  if ( SelWBFND != NULL )
    SelWBFND->setDirectory( dataRoot );
  if ( SelRBFND != NULL )
    SelRBFND->setDirectory( dataRoot );
  if ( SelLFND != NULL )
    SelLFND->setDirectory( dataRoot );

  for ( int i = 0; i < Datas.count(); i++ ) {
    Datas[i]->setDataRoot( dataRoot );
  }
}

void MainWindow::PutScanPSet( ScanPSet *set )
{
  SPSSelectD1->setCurrentIndex( set->Sensor );
  SPSSelectD10->setCurrentIndex( set->Sensor0 );
  SPSNormalize->setChecked( set->Sens0Selected );
  SPSUnitSelect->setCurrentIndex( set->Unit );
  SPSMotorSpeed->setCurrentIndex( set->Speed );
  SPSsP0->setText( set->Start );
  SPSeP0->setText( set->End );
  SPSstep0->setText( set->Step );
  SPSdwell0->setText( set->Dwell );
}

void MainWindow::GetScanPSet( ScanPSet *set )
{
  set->Sensor = SPSSelectD1->currentIndex();
  set->Sensor0 = SPSSelectD10->currentIndex();
  set->Sens0Selected = SPSNormalize->isChecked();
  set->Unit = SPSUnitSelect->currentIndex();
  set->Speed = SPSMotorSpeed->currentIndex();
  set->Start = SPSsP0->text();
  set->End = SPSeP0->text();
  set->Step = SPSstep0->text();
  set->Dwell = SPSdwell0->text();
}

bool MainWindow::UseDefUReal( AUnit *am )
{
  bool rv = false;

  for ( int i = 0; i < DefUReals.count(); i++ ) {
    if ( am->getUid() == DefUReals[i] ) {
      rv = true;
      break;
    }
  }

  return rv;
}

void MainWindow::ToggleGoToButtons( QString )
{
  if ( MMainTh->isBusy() ) {
    for ( int i = 0; i < GoTos.count(); i++ ) {
      GoTos[i]->setText( "Stop" );
      GoTos[i]->setStyleSheet( InActive );
    }
  } else {
    for ( int i = 0; i < GoTos.count(); i++ ) {
      GoTos[i]->setText( "Go" );
      GoTos[i]->setStyleSheet( NormalB );
    }
  }
}

void MainWindow::GoToPosKeV( void )
{
  if ( MMainTh->isBusy() ) {
    MMainTh->Stop();
  } else {
    QString User;
    if ( ( User = UUnits.isTheUnitInUse( MMainTh ) ) != "" ) {
      // 検出器が他のことに使われていたらダメ
      statusbar->showMessage( tr( "The Monochromator is used by the process %1!" )
			      .arg( User ), 2000 );
      return;
    }
    for ( int i = 0; i < GoTos.count(); i++ ) {
      if ( GoTos[i] == sender() )
	MoveCurThPosKeV( GoPosKeV[i] );
    }
  }
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

void MainWindow::askSetRange( void )
{
  SensWithRange.at( SelSensToSetRange->currentIndex() )
    ->SetRange( RangeSelect->text().toInt() );
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
  if ( ScanRecFileName->text().isEmpty() ) {
    statusbar->showMessage( tr( "Save file name is not selected" ), 2000 );
    return;
  }
  if ( ! SInfo.valid ) {
    statusbar->showMessage( tr( "Scan data is not valid" ), 2000 );
  }

  QFile f( ScanRecFileName->text() );
  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
    statusbar->showMessage( tr( "The file [%1] can not open to record the data" ),
			    2000 );
    return;
  }
  ScanDataStat = OLD;
  ScanNameStat = OLD;
  ScanRecFileName->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
  ScanRecFileName->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );

  QTextStream out( &f );

  AUnit *am = SInfo.am;

  out << "# XafsM2 Scan Data\n";
  out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" ) << "\n";
  out << "#\t";
  for ( int i = 0; i < mUnits.count(); i++ )
    out << mUnits.at(i)->getName() << "\t";
  out << am->getName() << "\t";
  out << ( ( SInfo.showUnit == 0 ) ? "Pulse" : am->getUnit() ) << "\t";
  out << ( ( SInfo.showUnit == 0 ) ? 1 : am->getUPP() ) << "\t";
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
  ScanRecFileName->setText( fname );
  ScanNameStat = NEW;
  ScanRecFileName->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
  ScanRecFileName->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );
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

  AUnit *am = AMotors.value( SPSMotorSelect->currentIndex() );

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
      if ( MMRelAbs->stat() == ABS ) {
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
  GoPosKeV[0] = newv - 0.30;
  GoPosKeV[1] = newv - 0.04;
  GoPosKeV[2] = newv + 0.05;
  GoPosKeV[3] = newv + 1.10;
  ShowAllGos();
}

void MainWindow::SetAllGoUnits( int i )
{
  for ( int j = 0; j < GOS; j++ ) {
    GoUnit[j]->setCurrentIndex( i );
  }
  ShowAllGos();
}

void MainWindow::NewMMRelAbs( RELABS /* stat */ )
{
  if ( LastInIsPulsV ) {
    NewGoMotorPosPuls( GoMotorPosPuls->text() );
  } else {
    NewGoMotorPosUnit( GoMotorPosUnit->text() );
  }
}

void MainWindow::NewGoMotorPosPuls( const QString &val )
{
  AUnit *am = AMotors.value( SPSMotorSelect->currentIndex() );

  LastInIsPulsV = true;
  if ( MMRelAbs->stat() == ABS ) {
    GoMotorPosUnit
      ->setText( QString::number( ( val.toDouble() - am->getCenter() ) * am->getUPP() ) );
  } else {
    GoMotorPosUnit->setText( QString::number( val.toDouble() * am->getUPP() ) );
  }
}

void MainWindow::NewGoMotorPosUnit( const QString &val )
{
  AUnit *am = AMotors.value( SPSMotorSelect->currentIndex() );

  LastInIsPulsV = false;
  if ( MMRelAbs->stat() == ABS ) {
    GoMotorPosPuls
      ->setText( QString::number( val.toDouble() / am->getUPP() + am->getCenter() ) );
  } else {
    GoMotorPosPuls->setText( QString::number( val.toDouble() / am->getUPP() ) );
  }
}

void MainWindow::GoMAtP( void )
{
  QString User;

  if ( !inMMove ) {
    GoMAtPuls( GoMotorPosPuls->text().toDouble() );
  } else {
    GoMStop();
  }
}

void MainWindow::GoMAtPuls( double Pos )
{
  AUnit *am = AMotors.value( SPSMotorSelect->currentIndex() );

  QString User;
  if ( ( User = UUnits.isTheUnitInUse( am ) ) != "" ) {
    statusbar->showMessage( tr( "The Motor [%1] is used by the process %2!" )
			    .arg( am->getName() ).arg( User ), 2000 );
    return;
  }

  inMMove = true;
  MovingM = SPSMotorSelect->currentIndex();
  //  MovingS = GoMotorS->currentIndex();

  if ( MMRelAbs->stat() == REL )
    Pos += am->value().toDouble();

  if ( am->getType() == "PM" ) {
    Pos = (double)((int)Pos);
  }

  GoMotor->setText( tr( "Stop" ) );
  GoMotor->setStyleSheet( InActive );

  SPSScan->setEnabled( false );

  am->SetSpeed( MSpeeds[ GoMotorS->currentIndex() ].MSid );
  am->SetValue( Pos );
  am->setIsBusy( true );

  UUnits.addUnit( GOMOTOR_ID, am );
  
  GoTimer->start( 100 );

  NewLogMsg( QString( tr( "Setup: %1 : GoTo %2 : Speed %3" ) )
	     .arg( am->getName() )
	     .arg( GoMotorPosPuls->text().toInt() )
	     .arg( MSpeeds[ GoMotorS->currentIndex() ].MSName ) );
  statusbar->showMessage( QString( tr( "Setup: %1 : GoTo %2 : Speed %3" ) )
			  .arg( am->getName() )
			  .arg( GoMotorPosPuls->text().toInt() )
			  .arg( MSpeeds[ GoMotorS->currentIndex() ].MSName ),
			  1000 );
}

void MainWindow::NewMotor( void )
{
  setupMDispFirstTime = true;
  GetScanPSet( ScanPs[ SPSLastSelectedM ] );
  SPSLastSelectedM = SPSMotorSelect->currentIndex();
  AMotors.value( SPSMotorSelect->currentIndex() )->GetValue();
  GoMotorUnit->setText( AMotors.value( SPSMotorSelect->currentIndex() )->getUnit() );
  SPSUnitSelect->removeItem( 1 );
  SPSUnitSelect->addItem( AMotors.value( SPSMotorSelect->currentIndex() )->getUnit() );
  PutScanPSet( ScanPs[ SPSLastSelectedM ] );
}

void MainWindow::GoMStop( void )
{
  // 動かしたモーターを憶えておいてそれを止めるより、
  // 今指定しているモーターを止めるほうが実用的と判断した。
  AUnit *am = AMotors.value( SPSMotorSelect->currentIndex() );

  am->Stop();   // なんでも止められる。
  if ( SPSMotorSelect->currentIndex() == MovingM ) // 今動かしたモーターだったら
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
  inMMove = false;

  UUnits.clear( GOMOTOR_ID );
  GoTimer->stop();
  GoMotor->setEnabled( true );
  SPSScan->setEnabled( true );
  GoMotor->setText( tr( "Go" ) );
  GoMotor->setStyleSheet( NormalB );
}

void MainWindow::SelectedAPointInScanArea( double x, double )
{
  if ( sender() == ScanView ) {
    // 受け取った x はパルス値
    SInfo.am->SetValue( x );
  }
}

void MainWindow::setSInfo( void )
{
  SInfo.am = AMotors.value( SPSMotorSelect->currentIndex() );
  SInfo.as = ASensors.value( SPSSelectD1->currentIndex() );
  SInfo.as0 = ASensors.value( SPSSelectD10->currentIndex() );
  SInfo.normalize = SPSNormalize->isChecked();
  SInfo.showUnit = SPSUnitSelect->currentIndex();
  SInfo.unitName = SPSUnitSelect->currentText();
  SInfo.upp = ( SInfo.showUnit == 0 ) ? 1 : SInfo.am->getUPP();
  SInfo.speed = SPSMotorSpeed->currentIndex();
  SInfo.relabs = SPSRelAbsSelect->stat();
  SInfo.origin = SInfo.am->value().toDouble();
  SInfo.offset = SInfo.am->getCenter();
  SInfo.sx0 = SPSsP0->text();
  SInfo.ex0 = SPSeP0->text();
  SInfo.dx0 = SPSstep0->text();
  SInfo.dwell0 = SPSdwell0->text();
  SInfo.sx = SInfo.am->any2p( SInfo.sx0.toDouble(), SInfo.showUnit, SInfo.relabs );
  SInfo.ex = SInfo.am->any2p( SInfo.ex0.toDouble(), SInfo.showUnit, SInfo.relabs );
  SInfo.dx = fabs( SInfo.dx0.toDouble() / SInfo.upp );
  if ( SInfo.sx > SInfo.ex )
    SInfo.dx = -SInfo.dx;
  SInfo.dwell = SInfo.dwell0.toDouble();
}

void MainWindow::ScanStart( void )
{
  //  AUnit *am, *as, *as0 = NULL;

  if ( !inSPSing ) {
    if ( ( ScanViewC = SetUpNewView( XYVIEW ) ) == NULL ) {
      statusbar->showMessage( tr( "No drawing screen is available" ), 2000 );
      return;
    }
    setSInfo();

    ViewTab->setTabText( ViewTab->currentIndex(), "SCAN" );
    ScanViewC->setNowDType( SCANDATA );
    ScanView = (XYView*)(ScanViewC->getView());

    // 古いコネクションがあったらとりあえず消しておく
    disconnect( this, SLOT( SelectedAPointInScanArea( double, double ) ) );
    connect( ScanView, SIGNAL( SelectAPoint( double, double ) ),
	     this, SLOT( SelectedAPointInScanArea( double, double ) ),
	     Qt::UniqueConnection );

    //    ScanMotor = MotorN->currentIndex();
    //    SInfo.am = AMotors.value( ScanMotor );
    mUnits.clearUnits();
    mUnits.addUnit( SInfo.as );
    mUnits.addUnit( SInfo.as0 );
    mUnits.setDwellTimes( SInfo.dwell );
    mUnits.setDwellTime();

    QString User;
    if ( ! SInfo.am->isEnable() ) {  // 使用するモータに関するチェック
      QString msg = tr( "Scan cannot Start : (%1) is disabled" )
	.arg( SInfo.am->getName() );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
    }
    if ( ( User = UUnits.isTheUnitInUse( SInfo.am ) ) != "" ) {
      // モーターがが他のことに使われたらダメ
      statusbar->showMessage( tr( "The Motor [%1] is used by the process %2!" )
			      .arg( SInfo.am->getName() ).arg( User ), 2000 );
      return;
    }

    for ( int i = 0; i < mUnits.count(); i++ ) {   // 使用する検出器に関するチェック
      AUnit *as = mUnits.at(i);
      if ( ! CheckOkList( as, NXafsOk ) ) {
	QString msg = tr( "The Sensor (%1) can use only in QXafs mode." )
	  .arg( as->getName() );
	statusbar->showMessage( msg, 2000 );
	return;
      }

      if ( ! as->isEnable() ) {
	QString msg = QString( tr( "Scan cannot Start : (%1) is disabled" ) )
	  .arg( as->getName() );
	statusbar->showMessage( msg, 2000 );
	NewLogMsg( msg );
	return;
      }

      if ( ( User = UUnits.isTheUnitInUse( as ) ) != "" ) {
	// 検出器が他のことに使われたらダメ
	statusbar->showMessage( tr( "The Sensor [%1] is used by the process %2!" )
				.arg( as->getName() ).arg( User ), 2000 );
	return;
      }
    }
    if ( SInfo.dx == 0 ) {
      statusbar->showMessage( tr( "Error: Scan Step is 0." ), 2000 );
      return;
    }

    SInfo.valid = true;
    inSPSing = true;

    NewLogMsg( QString( tr( "Scan Start (%1 %2)" ) )
	       .arg( SInfo.am->getName() )
	       .arg( SInfo.as->getName() ) );
    
    SInfo.am->SetSpeed( MSpeeds[ SInfo.speed ].MSid );

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
    ScanView->SetXName( SInfo.am->getName() );
    ScanView->SetXUnitName( SInfo.unitName );

    ScanView->setDispRelAbsSw( true );            // Rel/Abs SW を表示するかどうか
    ScanView->setDispRel( SInfo.relabs == REL );  // 初期の表示の仕方(スキャン条件自体の Rel/Abs に合わせる)
    // 以下の 4 つの情報があれば、XYView(ScanView) の内部で正しく表示できるはず
    ScanView->SetUnitType( SInfo.showUnit );      // 表示単位 pulse/実単位
    ScanView->SetUpp( SInfo.upp );                // Unit per pulse
    ScanView->SetOffset( SInfo.offset );          // PM の 0 点
    ScanView->SetCenter( SInfo.origin );          // 開始時点での PM 位置
    // 以上の 4 つの情報があれば、XYView(ScanView) の内部で正しく表示できるはず
    ScanView->SetAutoScale( true );
    ScanView->makeValid( true );

    ScanDataStat = NEW;
    ScanRecFileName->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
    ScanRecFileName->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );

    UUnits.addUnit( SCAN_ID, SInfo.am );
    for ( int i = 0; i < mUnits.count(); i++ ) {
      UUnits.addUnit( SCAN_ID, mUnits.at(i) );
    }

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
  QVector<AUnit*> ass;
  for ( int i = 0; i < monLines.count(); i++ )
    ass << ASensors.value( monLines[i]->currentIndex() );

  if ( !inMonitor ) {
    if ( ! ass[0]->isEnable() ) {
      QString msg = QString( tr( "Scan cannot Start : (%1) is disabled" ) )
	.arg( ass[0]->getName() );
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
	MonFile.close();
      }
    } else {
      monRecF = false;
    }

    MonitorViewC->setNowDType( MONDATA );
    MonitorView = (TYView*)(MonitorViewC->getView());

    mUnits.clearUnits();
    for ( int i = 0; i < monLines.count(); i++ ) {
      if ( monLines[i]->isChecked() ) {
	if ( ! ass[i]->isEnable() ) {
	  QString msg = QString( tr( "Scan cannot Start : (%1) is disabled" ) )
	    .arg( ass[i]->getName() );
	  statusbar->showMessage( msg, 2000 );
	  NewLogMsg( msg );
	  return;
	}
	mUnits.addUnit( ass[i] );
	// 注意 !!
	// mUnits の unit 番号と、ass, MonSels, MonDevs, MonVals の番号はずれる。
	// (選ばれていないものは、mUnits に登録されないため)
      }
    }
    mUnits.setDwellTimes( DwellT20->text().toDouble() );
    mUnits.setDwellTime();

    QString User;
    for ( int i = 0; i < mUnits.count(); i++ ) {
      AUnit *as = mUnits.at(i);
      if ( ! CheckOkList( as, NXafsOk ) ) {
	QString msg = tr( "The Sensor [%1] can use only in QXafs mode." )
	  .arg( as->getName() );
	statusbar->showMessage( msg, 2000 );
	return;
      }

      if ( ( User = UUnits.isTheUnitInUse( as ) ) != "" ) {
	// 検出器が他のことに使われたらダメ
	statusbar->showMessage( tr( "The Sensor [%1] is used by the process %2!" )
				.arg( as->getName() ).arg( User ), 2000 );
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
	     MonitorView, SLOT( SetMonScale( int ) ),
	     Qt::UniqueConnection );
    for ( int i = 0; i < monLines.count(); i++ ) {
      if ( monLines[i]->isChecked() ) {
	connect( ass[i], SIGNAL( newValue( QString ) ), this, SLOT( newVs( QString ) ),
	     Qt::UniqueConnection );
      }
    }
		 
    MStart->setText( tr( "Stop" ) );
    MStart->setStyleSheet( InActive );

    if ( !monRecF ) {
      MonDataStat = NEW;
      MonRecFile->setStyleSheet( FSTATCOLORS[ MonDataStat ][ MonNameStat ] );
      MonRecFile->setToolTip( FSTATMsgs[ MonDataStat ][ MonNameStat ] );
    }

    for ( int i = 0; i < mUnits.count(); i++ ) {
      UUnits.addUnit( MONITOR_ID, mUnits.at(i) );
    }

    inMonitor = true;
    MinPause = false;
    MonStage = 0;   // 計測のサイクル
    MonitorViewC->setIsDeletable( false );
    MPause->setEnabled( true );
    MonTime.restart();
    MonTimer->start( 100 );
  } else {
    if ( monRecF ) {
      MonFile.close();
    }
    MonTimer->stop();
    inMonitor = false;

    disconnect( SelectScale, SIGNAL( currentIndexChanged( int ) ),
	     MonitorView, SLOT( SetMonScale( int ) ) );
    for ( int i = 0; i < monLines.count(); i++ ) {
      if ( monLines[i]->isChecked() ) {
	disconnect( ass[i], SIGNAL( newValue( QString ) ),
		    this, SLOT( newVs( QString ) ) );
      }
    }

    UUnits.clear( MONITOR_ID );
    MonitorViewC->setIsDeletable( true );
    MPause->setEnabled( false );
    MPause->setText( tr( "Pause" ) );
    MPause->setStyleSheet( NormalB );
    MStart->setText( tr( "Mon. Start" ) );
    MStart->setStyleSheet( NormalB );
  }
}

void MainWindow::PauseMonitor( void )
{
  if ( MinPause ) {
    MinPause = false;
    MPause->setText( tr( "Pause" ) );
    MPause->setStyleSheet( NormalB );
  } else {
    MinPause = true;
    MPause->setText( tr( "Resume" ) );
    MPause->setStyleSheet( InActive );
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


void MainWindow::newVs( QString v )
{
  for ( int i = 0; i < monLines.count(); i++ ) {
    if ( sender() == ASensors.value( monLines[i]->currentIndex() ) ) {
      monLines[i]->setText( QString("%1").arg( v.toDouble(), 8, 'g' ) );
    }
  }
}

void MainWindow::TYVUpScale( void )
{
  int i = SelectScale->currentIndex();
  if ( i < MSCALES - 1 ) {
    SelectScale->setCurrentIndex( i+1 );
    if ( sender() != NULL ) {
      ((TYView*)sender())->SetMonScale( i+1 );
    }
  }
}

void MainWindow::TYVDownScale( void )
{
  int i = SelectScale->currentIndex();
  if ( i > 0 ) {
    SelectScale->setCurrentIndex( i-1 );
    if ( sender() != NULL ) {
      ((TYView*)sender())->SetMonScale( i-1 );
    }
  }
}

void MainWindow::PopDownMonLines( void )
{
  MonLinesPop->setChecked( false );
}

void MainWindow::PopChangeMonLines( bool f )
{
  if ( f ) {
    MonLinesDialog->layout()->addWidget( MonLinesBase );
    MonLinesDialog->show();
  } else {
    MonLinesFrame->layout()->addWidget( MonLinesBase );
    MonLinesDialog->hide();
  }
}

void MainWindow::newMovingAvr( void )
{
  int MA = MovingAvr->text().toInt();
  if ( MA < 1 )  {
    MA = 1;
  }
  MovingAvr->setText( QString::number( MA ) );

  TYView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == TYVIEW ) {
    if ( ( view = (TYView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setMovingAvr( MA );
      view->update();
    }
  }
}
