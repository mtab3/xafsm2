
#include "global.h"
#include "MainWindow.h"

void MainWindow::setupSetupArea( void )   /* 設定エリア */
{
  // モニタの 0 番は常に on 
  // MonSel0->setEnabled( false );

  GoUnit << GoUnit1 << GoUnit2 << GoUnit3 << GoUnit4;
  GoPosEdit << GoPos1 << GoPos2 << GoPos3 << GoPos4;
  GoTos << GoTo1 << GoTo2 << GoTo3 << GoTo4;

  monSels << MonSel0 << MonSel1 << MonSel2 << MonSel3 << MonSel4;
  monDevs << MonDev0 << MonDev1 << MonDev2 << MonDev3 << MonDev4;
  monVals << MonV0 << MonV1 << MonV2 << MonV3 << MonV4;

  SPSScanB->setStyleSheet( NormalEXECB );
  MStart->setStyleSheet( NormalEXECB );
  
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
  for ( int i = 0; i < monLines.count(); i++ ) {
    //    monLines[i]->dev()
    //      ->setMinimumContentsLength( 10 );
    monLines[i]->dev()
      ->setSizeAdjustPolicy( QComboBox::AdjustToMinimumContentsLengthWithIcon );
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
  //  inMMove0 = false;
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
    SPSMotorSelect->addItem( AMotors.value(i)->name() );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( ShowCurMotorPos( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( ShowCurMotorPos( SMsg ) ),
	     Qt::UniqueConnection );

    ScanInfo *aSif = new ScanInfo;
    aSif->showUnit = ( UseDefUReal( AMotors[i] ) ) ? 1 : 0;
    aSif->am = AMotors[i];
    aSif->as = ASensors[0];
    aSif->as0 = ASensors[0];
    ScanIFs << aSif;
    //    inMMoves << false;
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
  GoMotorUnit->setText( AMotors.value( SPSMotorSelect->currentIndex() )->unit() );
  SPSUnitSelect->addItem( "Pulse" );
  SPSUnitSelect->addItem( AMotors.value( SPSMotorSelect->currentIndex() )->unit() );
  LoadScanInfo( ScanIFs[ SPSMotorSelect->currentIndex() ] );

  for ( int i = 0; i < ASensors.count(); i++ ) {
    SPSSelectD1->addItem( ASensors.value(i)->name() );
    SPSSelectD10->addItem( ASensors.value(i)->name() );
    for ( int j = 0; j < monLines.count(); j++ ) {
      monLines[j]->addItem( ASensors.value(i)->name() );
    }
    SelectD3->addItem( ASensors.value(i)->name() );
    connect( ASensors.value(i), SIGNAL( newDark( double ) ),
	     this, SLOT( ShowNewDark( double ) ),
	     Qt::UniqueConnection );
    if ( ASensors.at(i)->isRangeSelectable() ) {
      SelSensToSetRange->addItem( ASensors.at(i)->name() );
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
  connect( GoMotorB, SIGNAL( clicked() ), this, SLOT( GoMAtP() ),
	   Qt::UniqueConnection );
  connect( GoMotorPosPuls, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( NewGoMotorPosPuls( const QString & ) ),
	   Qt::UniqueConnection );
  connect( GoMotorPosUnit, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( NewGoMotorPosUnit( const QString & ) ),
	   Qt::UniqueConnection );

  connect( SPSScanB, SIGNAL( clicked() ), this, SLOT( ScanStart() ),
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

  connect( MovingAvr, SIGNAL( editingFinished() ), this, SLOT( newMovingAvr() ),
	   Qt::UniqueConnection );

  connect( conds, SIGNAL( newDataRoot( const QString & ) ),
	   this, SLOT( newDataRoot( const QString & ) ),
	   Qt::UniqueConnection );
}

void MainWindow::newDataRoot( const QString &dataRoot )
{
  NewLogMsg( QString( "Data root is set at %1" ).arg( dataRoot ) );

  if ( EditDFName != NULL )
    EditDFName->setText( QFileInfo( dataRoot, EditDFName->text() ).filePath() );
  if ( scanFSel != NULL )
    scanFSel->setDirectory( dataRoot );
  if ( monFSel != NULL )
    monFSel->setDirectory( dataRoot );
  if ( S2DFileSel != NULL )
    S2DFileSel->setDirectory( dataRoot );
  //  if ( S2DMCAsDirSel != NULL )
  //    S2DMCAsDirSel->setDirectory( dataRoot );
  if ( SelDFND != NULL )
    SelDFND->setDirectory( dataRoot );
  if ( SelWBFND != NULL )
    SelWBFND->setDirectory( dataRoot );
  if ( SelRBFND != NULL )
    SelRBFND->setDirectory( dataRoot );
  if ( SelLFND != NULL )
    SelLFND->setDirectory( dataRoot );
  for ( int i = 0; i < SSFluos.count(); i++ ) {
    SSFluos[i]->setDirectory( dataRoot );
  }
  for ( int i = 0; i < Datas.count(); i++ ) {
    Datas[i]->setDataRoot( dataRoot );
  }
}

void MainWindow::NewMotor( void )
{
  //  setupMDispFirstTime = true;
  SaveScanInfo( ScanIFs[ SPSLastSelectedM ], AMotors[ SPSLastSelectedM ] );

  SPSLastSelectedM = SPSMotorSelect->currentIndex();
  AMotor *am = AMotors[ SPSLastSelectedM ];
  am->GetValue();

  GoMotorUnit->setText( am->unit() );
  SPSUnitSelect->removeItem( 1 );
  SPSUnitSelect->addItem( am->unit() );

  LoadScanInfo( ScanIFs[ SPSLastSelectedM ] );
  if ( UUnits.user( am ) != "" ) {
    setGoBAsMoving();
  } else {
    setGoBAsNotMoving();
  }
}

bool MainWindow::LoadScanInfo( ScanInfo *set )
{
  if ( !AMotors.contains( set->am ) ) return false;
  if ( !ASensors.contains( set->as ) ) return false;
  if ( !ASensors.contains( set->as0 ) ) return false;

  if ( SPSMotorSelect->currentIndex() != AMotors.indexOf( set->am ) )
    SPSMotorSelect->setCurrentIndex( AMotors.indexOf( set->am ) );
  SPSSelectD1->setCurrentIndex( ASensors.indexOf( set->as ) );
  SPSSelectD10->setCurrentIndex( ASensors.indexOf( set->as0 ) );
  SPSNormalize->setChecked( set->normalize );
  SPSUnitSelect->setCurrentIndex( set->showUnit );
  SPSMotorSpeed->setCurrentIndex( set->speed );
  SPSsP0->setText( set->sx0 );
  SPSeP0->setText( set->ex0 );
  SPSstep0->setText( set->dx0 );
  SPSdwell0->setText( set->dt0 );
  SPSRelAbsSelect->setRelAbs( set->relabs );

  GoMotorPosUnit->setText( set->goingPinU );
  GoMotorPosPuls->setText( set->goingP );
  return true;
}

void MainWindow::SaveScanInfo( ScanInfo *set, AMotor *am )
{
  if ( am == NULL )
    set->am = AMotors[ SPSMotorSelect->currentIndex() ];
  else
    set->am = am;
  set->as = ASensors[ SPSSelectD1->currentIndex() ];
  set->as0 = ASensors[ SPSSelectD10->currentIndex() ];
  set->normalize = SPSNormalize->isChecked();
  set->showUnit = SPSUnitSelect->currentIndex();
  set->unitName = set->am->unit();
  set->upp = ( set->showUnit == 0 ) ? 1 : set->am->upp();
  set->speed = (MSPEED)(SPSMotorSpeed->currentIndex());
  set->sx0 = SPSsP0->text();
  set->ex0 = SPSeP0->text();
  set->dx0 = SPSstep0->text();
  set->dt0 = SPSdwell0->text();
  set->relabs = SPSRelAbsSelect->stat();

  set->sx = set->am->any2p( set->sx0.toDouble(), set->showUnit, set->relabs );
  set->ex = set->am->any2p( set->ex0.toDouble(), set->showUnit, set->relabs );
  set->dx = fabs( set->dx0.toDouble() / set->upp );
  if ( set->sx > set->ex )
    set->dx = -set->dx;
  set->dt = set->dt0.toDouble();

  set->origin = set->am->value().toDouble();
  set->offset = set->am->getCenter();

  set->UseMonitors = SPSUseAdditionalSs->isChecked();
  if ( set->UseMonitors ) {
    SaveMonInfo( &(set->mi) );
  }

  set->goingP = GoMotorPosPuls->text();
  set->goingPinU = GoMotorPosUnit->text();
}

void MainWindow::SaveMonInfo( MonInfo *set )
{
  ASensor *as;

  set->Sensors.clear();
  set->SensorNames.clear();
  set->SensorUnits.clear();
  for ( int i = 0; i < monLines.count(); i++ ) {
    if ( monLines[i]->isChecked() ) {
      as = ASensors[ monLines[i]->currentIndex() ];
      if ( as->isEnable() ) {
	set->Sensors << as;
	set->SensorNames << as->name();
	set->SensorUnits << as->unit();
      }
    }
  }
  set->MeasTime = DwellT20->text().toDouble();
  set->valid = true;
}


bool MainWindow::UseDefUReal( AMotor *am )
{
  bool rv = false;

  for ( int i = 0; i < DefUReals.count(); i++ ) {
    if ( am->uid() == DefUReals[i] ) {
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
    if ( ( User = UUnits.user( MMainTh ) ) != "" ) {
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
  ASensor *as = SensWithRange.at( SelSensToSetRange->currentIndex() );
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
  ASensor *as = SensWithRange[ SelSensToSetRange->currentIndex() ];
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
  XYView *view;   // 現在表示しているのがスキャン画面だったらその画面がセーブの対象
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getDType() == SCANDATA ) {
    view = (XYView*)ViewCtrls[ ViewTab->currentIndex() ]->getView();
  } else {        // 違ったら、一番最近のスキャン結果がセーブの対象
    view = (XYView*)findAView( SCANDATA );
  }
  ScanInfo si = view->getSInfo();

  if ( view == NULL ) {
    statusbar->showMessage( tr( "Scan data is not valid" ), 2000 );
    return;
  }
  if ( ScanRecFileName->text().isEmpty() ) {
    statusbar->showMessage( tr( "Save file name is not selected" ), 2000 );
    return;
  }
  if ( ! si.valid ) {
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

  out << FileIDs[ SCANDATA ] << "\n";
  out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" ) << "\n";

  view->getSInfo().save( out );

  int points = view->GetPoints( 0 );

  for ( int i = 0; i < points; i++ ) {
    out << view->GetX( 0, i );
    for ( int j = 0; j < mScanUnits.count(); j++ ) {
      out << "\t" << view->GetY( j, i );
    }
    out << "\n";
  }

  f.close();
}

void MainWindow::setSelectedMonFName( const QString &fname )
{
  MonRecFile->setText( CheckFNameExt( fname, "dat" ) );
  MonNameStat = NEW;
  MonRecFile->setStyleSheet( FSTATCOLORS[ MonDataStat ][ MonNameStat ] );
  MonRecFile->setToolTip( FSTATMsgs[ MonDataStat ][ MonNameStat ] );
}

void MainWindow::setSelectedScanFName( const QString &fname )
{
  ScanRecFileName->setText( CheckFNameExt( fname, "dat" ) );
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

void MainWindow::ShowCurMotorPos( SMsg msg )  // AUnit を通さずにつないでる !!
{
  QString buf;
  QString val0;
  QString val;

  AMotor *am = AMotors.value( SPSMotorSelect->currentIndex() );

  if ( ( msg.From() == am->devCh() )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE ) ) ) {
    if ( ( am->type() == "SC" ) && ( msg.Msgt() == GETVALUE ) ) {
      val0 = msg.Vals().at(1);
    } else {
      val0 = msg.Val();
    }
    MCurPosPuls->setText( val0 );
    val = QString::number
      ( ( val0.toDouble() - am->getCenter() ) * am->upp() );
    MCurPosUnit->setText( val );
    if ( setupMDispFirstTime == true ) {  // 最初の一回だけ
      if ( MMRelAbs->stat() == ABS ) {
	GoMotorPosPuls->setText( val0 );
	GoMotorPosUnit->setText( val );
	setupMDispFirstTime = false;
      } else {
	GoMotorPosPuls->setText( "0" );
	GoMotorPosUnit->setText( "0" );
	setupMDispFirstTime = false;
      }
    }
    if ( am->checkNewVal() ) {
      NewLogMsg( tr( "Current Position of [%1] : [%2] %3" )
		 .arg( am->name() )
		 .arg( val )
		 .arg( am->unit() ) );
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
  AMotor *am = AMotors.value( SPSMotorSelect->currentIndex() );

  LastInIsPulsV = true;
  if ( MMRelAbs->stat() == ABS ) {
    GoMotorPosUnit
      ->setText( QString::number( ( val.toDouble() - am->getCenter() ) * am->upp() ) );
  } else {
    GoMotorPosUnit->setText( QString::number( val.toDouble() * am->upp() ) );
  }
}

void MainWindow::NewGoMotorPosUnit( const QString &val )
{
  AMotor *am = AMotors.value( SPSMotorSelect->currentIndex() );

  LastInIsPulsV = false;
  if ( MMRelAbs->stat() == ABS ) {
    GoMotorPosPuls
      ->setText( QString::number( val.toDouble() / am->upp() + am->getCenter() ) );
  } else {
    GoMotorPosPuls->setText( QString::number( val.toDouble() / am->upp() ) );
  }
}

void MainWindow::GoMAtP( void )
{
  QString User;

  if ( UUnits.user( AMotors[ SPSMotorSelect->currentIndex() ] ) == "" ) {
    // 他のモータが動いていても、当該のモータが動いていなければ OK
    GoMAtPuls( GoMotorPosPuls->text().toDouble() );
  } else {
    GoMStop();
  }
}

void MainWindow::GoMAtPuls( double Pos )
{
  int mNo = SPSMotorSelect->currentIndex();
  AMotor *am = AMotors.value( mNo );

  QString User;
  if ( ( User = UUnits.user( am ) ) != "" ) {
    statusbar->showMessage( tr( "The Motor [%1] is used by the process %2!" )
			    .arg( am->name() ).arg( User ), 2000 );
    return;
  }
  //  MovingS = GoMotorS->currentIndex();
  UUnits.addAnUnit( GOMOTOR_ID, am );
  setGoBAsMoving();
  if ( !GoTimer->isActive() )
    GoTimer->start( 100 );

  if ( MMRelAbs->stat() == REL )
    Pos += am->value().toDouble();

  if ( am->type() == "PM" ) {
    Pos = (double)((int)Pos);
  }

  am->SetSpeed( MSpeeds[ GoMotorS->currentIndex() ].MSid );
  am->SetValue( Pos );
  am->setIsBusy( true );

  NewLogMsg( QString( tr( "Setup: %1 : GoTo %2 : Speed %3" ) )
	     .arg( am->name() )
	     .arg( GoMotorPosPuls->text().toInt() )
	     .arg( MSpeeds[ GoMotorS->currentIndex() ].MSName ) );
  statusbar->showMessage( QString( tr( "Setup: %1 : GoTo %2 : Speed %3" ) )
			  .arg( am->name() )
			  .arg( GoMotorPosPuls->text().toInt() )
			  .arg( MSpeeds[ GoMotorS->currentIndex() ].MSName ),
			  1000 );
}

void MainWindow::setGoBAsMoving( void )
{
  // ここは表示だけ、ステータスをいじってはダメ
#if 0
  inMMoves[ mNo ] = true;
  inMMove0 = true;            // これはどれか一つでも動けばとにかく true
  UUnits.addAnUnit( GOMOTOR_ID, am );
#endif

  GoMotorB->setText( tr( "Stop" ) );
  GoMotorB->setStyleSheet( InActive );
  //  GoMotorB->setEnabled( false );
  SPSScanB->setEnabled( false );
}

void MainWindow::setGoBAsNotMoving( void )
{
  // ここは表示だけ、ステータスをいじってはダメ
#if 0
  UUnits.clear( GOMOTOR_ID );
  inMMoves[ mNo ] = false;
#endif
  GoMotorB->setText( tr( "Go" ) );
  GoMotorB->setStyleSheet( NormalB );
  //  GoMotorB->setEnabled( true );
  SPSScanB->setEnabled( true );

  // これもここでやることではない
#if 0
  int i;
  for ( i = 0; i < inMMoves.count(); i++ ) {
    if ( inMMoves[i] )
      break;
  }
  if ( i >= inMMoves.count() ) {
    // この２つは全部止まってることを確認してから止める
    inMMove0 = false;
    GoTimer->stop();
  }
#endif
}

void MainWindow::GoMStop( void )
{
  // 動かしたモーターを憶えておいてそれを止めるより、
  // 今指定しているモーターを止めるほうが実用的と判断した。
  int mNo = SPSMotorSelect->currentIndex();
  AMotor *am = AMotors.value( mNo );

  // 当該のモータが使われてて、かつそれを使ってるのが GOMOTOR の場合のみ止める
  if ( UUnits.removeAnUnit( GOMOTOR_ID, am ) ) {
    am->Stop();
    setGoBAsNotMoving();
    
    NewLogMsg( QString( tr( "Setup: %1 : Stopped at %2" ) )
	       .arg( am->name() )
	       .arg( am->value() ) );
    statusbar->showMessage( QString( tr( "Setup: %1 : Stopped at %2" ) )
			  .arg( am->name() )
			    .arg( am->value() ), 1000 );
  }
}

#if 0
void MainWindow::GoMStop0( void )
{
  inMMove = false;

  UUnits.clear( GOMOTOR_ID );
  GoTimer->stop();
  GoMotorB->setEnabled( true );
  SPSScanB->setEnabled( true );
  GoMotorB->setText( tr( "Go" ) );
  GoMotorB->setStyleSheet( NormalB );
}
#endif

void MainWindow::SelectedAPointInScanArea( double x, double )
{
  for ( int i = 0; i < ViewCtrls.count(); i++ ) {
    if ( sender() == ViewCtrls[i]->getView() ) {
      if ( ViewCtrls[i]->getDType() == SCANDATA ) {
	ScanInfo si = ((XYView*)(ViewCtrls[i]->getView()))->getSInfo();
	si.am->SetValue( x );
      }
    }
  }
}

void MainWindow::ScanStart( void )
{
  //  AMotor *am;
  //  ASensor *as, *as0 = NULL;

  if ( !inSPSing ) {
    if ( SPSUseAdditionalSs->isChecked() && inSPSing ) {
      statusbar->showMessage( tr( "Although the 'Additional' button is checked, "
				  "the monitoring process is running." ) );
      return;
    }
    if ( ( ScanViewC = SetUpNewView( XYVIEW, SCANDATA, NULL ) ) == NULL ) {
      statusbar->showMessage( tr( "No drawing screen is available" ), 2000 );
      return;
    }
    ScanInfo si;
    SaveScanInfo( &si );

    //    ViewTab->setTabText( ViewTab->currentIndex(), tr( "SCAN" ) );
    //    ScanViewC->setNowDType( SCANDATA );
    ScanView = (XYView*)(ScanViewC->getView());

    // 古いコネクションがあったらとりあえず消しておく
    disconnect( this, SLOT( SelectedAPointInScanArea( double, double ) ) );
    connect( ScanView, SIGNAL( SelectAPoint( double, double ) ),
	     this, SLOT( SelectedAPointInScanArea( double, double ) ),
	     Qt::UniqueConnection );

    //    ScanMotor = MotorN->currentIndex();
    //    SInfo.am = AMotors.value( ScanMotor );
    mScanUnits.clearUnits();
    mScanUnits.addUnit( si.as );
    if ( si.normalize )
      mScanUnits.addUnit( si.as0 );
    if ( SPSUseAdditionalSs->isChecked() ) {
      for ( int i = 0; i < monLines.count(); i++ ) {
	if ( monLines[i]->isChecked() ) {
	  ASensor *as = ASensors[ monLines[i]->currentIndex() ];
	  if ( as->isEnable() ) {
	    int i;
	    for ( i = 0; i < mScanUnits.count(); i++ ) {
	      if ( as == mScanUnits.at(i) )
		break;
	    }
	    if ( i >= mScanUnits.count() ) {
	      mScanUnits.addUnit( as );
	    }
	  }
	}
      }
    }
    mScanUnits.setDwellTimes( si.dt );
    mScanUnits.setDwellTime();

    QString User;
    if ( ! si.am->isEnable() ) {  // 使用するモータに関するチェック
      QString msg = tr( "Scan cannot Start : (%1) is disabled" )
	.arg( si.am->name() );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
    }
    if ( ( User = UUnits.user( si.am ) ) != "" ) {
      // モーターがが他のことに使われたらダメ
      statusbar->showMessage( tr( "The Motor [%1] is used by the process %2!" )
			      .arg( si.am->name() ).arg( User ), 2000 );
      return;
    }

    for ( int i = 0; i < mScanUnits.count(); i++ ) {   // 使用する検出器に関するチェック
      ASensor *as = mScanUnits.at(i);
      if ( ! CheckOkList( as, NXafsOk ) ) {
	QString msg = tr( "The Sensor (%1) can use only in QXafs mode." )
	  .arg( as->name() );
	statusbar->showMessage( msg, 2000 );
	return;
      }

      if ( ! as->isEnable() ) {
	QString msg = QString( tr( "Scan cannot Start : (%1) is disabled" ) )
	  .arg( as->name() );
	statusbar->showMessage( msg, 2000 );
	NewLogMsg( msg );
	return;
      }

      if ( ( User = UUnits.user( as ) ) != "" ) {
	// 検出器が他のことに使われたらダメ
	statusbar->showMessage( tr( "The Sensor [%1] is used by the process %2!" )
				.arg( as->name() ).arg( User ), 2000 );
	return;
      }
    }
    if ( si.dx == 0 ) {
      statusbar->showMessage( tr( "Error: Scan Step is 0." ), 2000 );
      return;
    }

    si.valid = true;
    inSPSing = true;

    NewLogMsg( QString( tr( "Scan Start (%1 %2)" ) )
	       .arg( si.am->name() )
	       .arg( si.as->name() ) );
    
    si.am->SetSpeed( MSpeeds[ si.speed ].MSid );

    SPSScanB->setText( tr( "Stop" ) );
    SPSScanB->setStyleSheet( InActive );
    GoMotorB->setEnabled( false );

    ScanView->Clear();
    ScanView->SetLR( 0, LEFT_AX );   // 0 番目の線はグループ 0, 1 番目の線はグループ 1
    ScanView->SetScaleType( 0, FULLSCALE ); // グループ 0 も 1 も FULLSCALE
    ScanView->SetLeftName( " " );
    for ( int i = 1; i < mScanUnits.count(); i++ ) {
      ScanView->SetLR( i, RIGHT_AX );   // 0 番目の線はグループ 0, 1 番目の線はグループ 1
      ScanView->SetScaleType( i, FULLSCALE ); // グループ 0 も 1 も FULLSCALE
    }
    ScanView->SetRightName( " " );
    for ( int i = 0; i < mScanUnits.count(); i++ )
      ScanView->SetLineName( i, mScanUnits.at(i)->name() );
    ScanView->SetXName( si.am->name() );
    ScanView->SetXUnitName( si.unitName );

    ScanView->setDispRelAbsSw( true );            // Rel/Abs SW を表示するかどうか
    ScanView->setDispRel( si.relabs == REL );  // 初期の表示の仕方(スキャン条件自体の Rel/Abs に合わせる)
    // 以下の 4 つの情報があれば、XYView(ScanView) の内部で正しく表示できるはず
    ScanView->SetUnitType( si.showUnit );      // 表示単位 pulse/実単位
    ScanView->SetUpp( si.upp );                // Unit per pulse
    ScanView->SetOffset( si.offset );          // PM の 0 点
    ScanView->SetCenter( si.origin );          // 開始時点での PM 位置
    // 以上の 4 つの情報があれば、XYView(ScanView) の内部で正しく表示できるはず
    ScanView->SetAutoScale( true );
    ScanView->makeValid( true );

    ScanDataStat = NEW;
    ScanRecFileName->setStyleSheet( FSTATCOLORS[ ScanDataStat ][ ScanNameStat ] );
    ScanRecFileName->setToolTip( FSTATMsgs[ ScanDataStat ][ ScanNameStat ] );

    UUnits.addAnUnit( SCAN_ID, si.am );
    for ( int i = 0; i < mScanUnits.count(); i++ ) {
      UUnits.addAnUnit( SCAN_ID, mScanUnits.at(i) );
    }
    ScanView->setSInfo( si );

    ScanStage = 0;
    ScanTimer->start( 100 );
    ScanViewC->setDeletable( false );
  } else {
    ScanStop0();
    ScanViewC->setDeletable( true );
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
  QVector<ASensor*> ass;
  for ( int i = 0; i < monLines.count(); i++ )
    ass << ASensors.value( monLines[i]->currentIndex() );
  
  if ( !inMonitor ) {
#if 0
    if ( ! ass[0]->isEnable() ) {
      QString msg = QString( tr( "Scan cannot Start : (%1) is disabled" ) )
	.arg( ass[0]->name() );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
    }
#endif
    if ( SPSUseAdditionalSs->isChecked() && inSPSing ) {
      statusbar->showMessage( tr( "The monitoring sensors might used for scanning, "
				  "as 'Additonal' button is checked." ) );
      return;
    }
    if ( ( MonitorViewC = SetUpNewView( TYVIEW, MONDATA, NULL ) ) == NULL ) {
      statusbar->showMessage( tr( "No drawing area is avairable" ) );
      return;
    }
    //    ViewTab->setTabText( ViewTab->currentIndex(), tr( "MON." ) );
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
	MonOut << FileIDs[ MONDATA ] << "\n";
	MonOut << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" )
	       << "\n";
	MonFile.close();
      }
    } else {
      monRecF = false;
    }

    //    MonitorViewC->setNowDType( MONDATA );
    MonitorView = (TYView*)(MonitorViewC->getView());

    mMonUnits.clearUnits();
    for ( int i = 0; i < monLines.count(); i++ ) {
      if ( monLines[i]->isChecked() ) {
	if ( ! ass[i]->isEnable() ) {
	  QString msg = QString( tr( "Scan cannot Start : (%1) is disabled" ) )
	    .arg( ass[i]->name() );
	  statusbar->showMessage( msg, 2000 );
	  NewLogMsg( msg );
	  return;
	}
	mMonUnits.addUnit( ass[i] );
	// 注意 !!
	// mMonUnits の unit 番号と、ass, MonSels, MonDevs, MonVals の番号はずれる。
	// (選ばれていないものは、mMonUnits に登録されないため)
      }
    }
    if ( mMonUnits.count() < 1 ) {
      QString msg
	= QString( tr( "At least 1 sensor should be selected for the monitor" ) );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
    }
    mMonUnits.setDwellTimes( DwellT20->text().toDouble() );
    mMonUnits.setDwellTime();

    QString User;
    for ( int i = 0; i < mMonUnits.count(); i++ ) {
      ASensor *as = mMonUnits.at(i);
      if ( ! CheckOkList( as, NXafsOk ) ) {
	QString msg = tr( "The Sensor [%1] can use only in QXafs mode." )
	  .arg( as->name() );
	statusbar->showMessage( msg, 2000 );
	return;
      }

      if ( ( User = UUnits.user( as ) ) != "" ) {
	// 検出器が他のことに使われたらダメ(この時点ではまだ登録しない)
	statusbar->showMessage( tr( "The Sensor [%1] is used by the process %2!" )
				.arg( as->name() ).arg( User ), 2000 );
	return;
      }
    }

    MonInfo monInfo;
    SaveMonInfo( &monInfo );
    if ( monRecF ) {
      monInfo.save( MonOut );
#if 0
      MonOut << "#\tsec";
      for ( int i = 0; i < mMonUnits.count(); i++ ) {
	MonOut << QString( tr( "\t%1[%2]" )
			   .arg( mMonUnits.getName( i ) )
			   .arg( mMonUnits.getUnit( i ) ) );
      }
      MonOut << "\n";
#endif
    }

    MonitorView->ClearDataR();
    for ( int i = 0; i < mMonUnits.count(); i++ ) {
      MonitorView->SetLName( i, mMonUnits.getName( i ) );
    }
    MonitorView->SetLines( mMonUnits.count() );
    MonitorView->makeValid( true );

    MonitorView->SetMonScale( SelectScale->currentIndex() );
    connect( SelectScale, SIGNAL( currentIndexChanged( int ) ),
	     MonitorView, SLOT( SetMonScale( int ) ),
	     Qt::UniqueConnection );
    for ( int i = 0; i < monLines.count(); i++ ) {
      if ( monLines[i]->isChecked() ) {
	connect( ass[i], SIGNAL( NewValue( QString ) ), this, SLOT( newVs( QString ) ),
	     Qt::UniqueConnection );
      }
    }
    MonitorView->setMonInfo( monInfo );

    MStart->setText( tr( "Stop" ) );
    MStart->setStyleSheet( InActive );

    if ( !monRecF ) {
      MonDataStat = NEW;
      MonRecFile->setStyleSheet( FSTATCOLORS[ MonDataStat ][ MonNameStat ] );
      MonRecFile->setToolTip( FSTATMsgs[ MonDataStat ][ MonNameStat ] );
    }

    for ( int i = 0; i < mMonUnits.count(); i++ ) {
      UUnits.addAnUnit( MONITOR_ID, mMonUnits.at(i) );
    }

    inMonitor = true;
    MinPause = false;
    MonStage = 0;   // 計測のサイクル
    MonitorViewC->setDeletable( false );
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
	disconnect( ass[i], SIGNAL( NewValue( QString ) ),
		    this, SLOT( newVs( QString ) ) );
      }
    }

    UUnits.removeUnits( MONITOR_ID );
    MonitorViewC->setDeletable( true );
    MPause->setEnabled( false );
    MPause->setText( tr( "Pause" ) );
    MPause->setStyleSheet( NormalB );
    MStart->setText( tr( "Mon. Start" ) );
    MStart->setStyleSheet( NormalEXECB );
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
  TYView *view;   // 現在表示しているのがモニタ画面だったらその画面がセーブの対象
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getDType() == MONDATA ) {
    view = (TYView*)ViewCtrls[ ViewTab->currentIndex() ]->getView();
  } else {        // 違ったら、一番最近のモニタ結果がセーブの対象
    view = (TYView*)findAView( MONDATA );
  }

  if ( view == NULL ) {
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

  out << FileIDs[ MONDATA ] << "\n";
  out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" )
      << "\n";
  view->getMonInfo().save( out );
#if 0
  out << "#\tsec";
  for ( int i = 0; i < mMonUnits.count(); i++ ) {
    out << QString( tr( "\t%1[%2]" )
		    .arg( mMonUnits.name( i ) )
		    .arg( mMonUnits.unit( i ) ) );
  }
  out << "\n";
#endif

  int points = view->getDatas();
  int lines = view->GetLines();

  for ( int i = 0; i < points; i++ ) {
    out << (double)(view->getT( i ))/1000;
    for ( int j = 0; j < lines; j++ ) {
      out << "\t" << view->getY( j, i );
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
