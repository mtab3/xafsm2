#include <QtGui>

#include "SMsg.h"
#include "MainWindow.h"
#include "MCAView.h"

void MainWindow::setupSetupSSDArea( void )   /* 測定エリア */
{
  hasConnected = false;

  SSDbs << SSDE01 << SSDE02 << SSDE03 << SSDE04 << SSDE05
        << SSDE06 << SSDE07 << SSDE08 << SSDE09 << SSDE10
        << SSDE11 << SSDE12 << SSDE13 << SSDE14 << SSDE15
        << SSDE16 << SSDE17 << SSDE18 << SSDE19;
  SSDbs2 << SSDE01_2 << SSDE02_2 << SSDE03_2 << SSDE04_2 << SSDE05_2
	 << SSDE06_2 << SSDE07_2 << SSDE08_2 << SSDE09_2 << SSDE10_2
	 << SSDE11_2 << SSDE12_2 << SSDE13_2 << SSDE14_2 << SSDE15_2
	 << SSDE16_2 << SSDE17_2 << SSDE18_2 << SSDE19_2;
  
  for ( int i = 0; i < SSDbs.count(); i++ ) {
    SSDbs[i]->setStyleSheet( SSDActive );
    SSDbs[i]->setToolTip( tr( "Active" ) );
    connect( SSDbs[i],  SIGNAL( clicked() ), this, SLOT( SelSSDs0() ) );
  }

  for ( int i = 0; i < SSDbs2.count(); i++ ) {
    SSDbs2[i]->setStyleSheet( SSDActive );
    SSDbs2[i]->setToolTip( tr( "Active" ) );
    SSDbs2[i]->setChecked( PBTrue );
    connect( SSDbs2[i], SIGNAL( clicked() ), this, SLOT( SelSSDs20() ) );
  }

  StartResume = MCA_START;
  
  connect( s, SIGNAL( AnsGetPeakingTime( SMsg ) ),
	   this, SLOT( showPeakingTime( SMsg ) ) );
  connect( s, SIGNAL( AnsGetThreshold( SMsg ) ), this, SLOT( showThreshold( SMsg ) ) );
  connect( s, SIGNAL( AnsGetCalibration( SMsg ) ),
	   this, SLOT( showCalibration( SMsg ) ) );
  connect( s, SIGNAL( AnsGetDynamicRange( SMsg ) ),
	   this, SLOT( showDynamicRange( SMsg ) ) );
  connect( s, SIGNAL( AnsGetPreAMPGain( SMsg ) ), this, SLOT( showPreAMPGain( SMsg ) ) );
  connect( s, SIGNAL( AnsGetMCALength( SMsg ) ), this, SLOT( getMCALen( SMsg ) ) );
  
  connect( MCAStart, SIGNAL( clicked() ), this, SLOT( StartMCA() ) );
  connect( MCACh, SIGNAL( valueChanged( int ) ), this, SLOT( MCAChSelected( int ) ) );
  connect( ROIStartInput, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( newROIStart( const QString & ) ) );
  connect( ROIEndInput, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( newROIEnd( const QString & ) ) );
  connect( MCAClear, SIGNAL( clicked() ), this, SLOT( clearMCA() ) );
  
  connect( SelRealTime, SIGNAL( clicked() ), this, SLOT( RealTimeIsSelected() ) );
  connect( SelLiveTime, SIGNAL( clicked() ), this, SLOT( LiveTimeIsSelected() ) );

  MCAFSel = new QFileDialog;
  MCAFSel->setAcceptMode( QFileDialog::AcceptSave );
  MCAFSel->setDirectory( QDir::currentPath() );
  MCAFSel->setFilter( "*.dat" );

  connect( SelMCARecFile, SIGNAL( clicked() ), MCAFSel, SLOT( show() ) );
  connect( MCAFSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( setSelectedMCAFName( const QString & ) ) );
  connect( MCARec, SIGNAL( clicked() ), this, SLOT( saveMCAData() ) );
  connect( SFluo, SIGNAL( ReceivedNewMCAValue() ), this, SLOT( ShowNewMCAStat() ) );
  connect( SFluo, SIGNAL( ReceivedNewMCARealTime( int ) ),
	   this, SLOT( ShowNewMCARealTime( int ) ) );
  connect( SFluo, SIGNAL( ReceivedNewMCALiveTime( int ) ),
	   this, SLOT( ShowNewMCALiveTime( int ) ) );

  connect( ROIsetAll, SIGNAL( clicked() ), this, SLOT( setAllROIs() ) );

  inMCAMeas = false;
  validMCAData = false;
  MCAData = NULL;
  cMCAView = NULL;
  cMCACh = 0;
  //  oldMCACh = -1;

  SelSSDs( 0 );

  NonSelC = "#ccccaa";
  SelectC = "#ffffdd";
  PT2 = new PeriodicTable;
  PT2->SetActionOnSelected( PT_STAY );
  PT2->SetActionOnClosed( PT_CLOSE );
  PT2->SetCheckable( true );
  PT2->ShowAllNoneBs( true );
  for ( int i = 0; i < PT->Atoms(); i++ ) {
    PT2->SetAColor( i, NonSelC );
  }
  connect( SelectElmNames, SIGNAL( clicked() ), PT2, SLOT( show() ) );
  PT2->setAll();
  connect( PT2, SIGNAL( AtomToggled( bool, int ) ),
	   fdbase, SLOT( ElementSelected( bool, int ) ) );

  connect( SetDisplayLog, SIGNAL( clicked( bool ) ),
	   this, SLOT( NoticeMCAViewSetDisplayLog( bool ) ) );
  connect( DispElmNames, SIGNAL( toggled( bool ) ),
	   this, SLOT( NoticeMCAViewSetShowElements( bool ) ) );
  connect( ShowAlwaysSelElm, SIGNAL( toggled( bool ) ),
	   this, SLOT( NoticeMCAViewShowAlwaysSelElm( bool ) ) );
  connect( ShowElmEnergy, SIGNAL( toggled( bool ) ),
	   this, SLOT( NoticeMCAViewShowElmEnergy( bool ) ) );
  connect( PeakFitB, SIGNAL( clicked() ), this, SLOT( doPeakFit() ) );
}

void MainWindow::doPeakFit( void )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->doPeakFit();
    }
  }
}

void MainWindow::NoticeMCAViewSetDisplayLog( bool f )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setLog( f );
    }
  }
}

void MainWindow::NoticeMCAViewSetShowElements( bool f )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setShowElements( f );
    }
  }
}

void MainWindow::NoticeMCAViewShowAlwaysSelElm( bool f )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setShowElementsAlways( f );
    }
  }
}

void MainWindow::NoticeMCAViewShowElmEnergy( bool f )
{
  MCAView *view;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      view->setShowElementsEnergy( f );
    }
  }
}

void MainWindow::setAllROIs( void )
{
  int ch = MCACh->text().toInt();
  double startE = kev2pix->p2E( ch, ROIStart[ ch ].toDouble() );
  double endE = kev2pix->p2E( ch, ROIEnd[ MCACh->text().toInt() ].toDouble() );

  for ( int i = 0; i < MaxSSDs; i++ ) {
    ROIStart[ i ] = QString::number( kev2pix->E2p( i, startE ) );
    ROIEnd[ i ] = QString::number( kev2pix->E2p( i, endE ) );
  }
}

void MainWindow::saveMCAData( void )
{
  if ( !validMCAData ) {
    statusbar->showMessage( tr( "MCA data is not valid" ), 2000 );
    return;
  }
  if ( MCARecFile->text().isEmpty() ) {
    statusbar->showMessage( tr( "Save file name is not selected" ), 2000 );
    return;
  }

  QFile f( MCARecFile->text() );
  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
    statusbar->showMessage( tr( "The file [%1] can not open to record the data" ),
			    2000 );
    return;
  }
  QTextStream out( &f );

  out << "# XafsM2 MCA Data\n";
  out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" ) << "\n";
  out << "# " << "MCALength " << "MCA Ch. " << "RealTime " << "LiveTime "
              << "ROIs " << "ROIe" << "\n";
  out << "# " << MCALength << " " << cMCACh << " "
              << cMCAView->getRealTime() << " " << cMCAView->getLiveTime() << " "
              << ROIStartInput->text().toInt() << " " << ROIEndInput->text().toInt()
              << "\n";
  for ( int i = 0; i < MCALength; i++ ) {
    out << i << "\t" << MCAData[i] << "\n";
  }

  f.close();
}

void MainWindow::setSelectedMCAFName( const QString &fname )
{
  MCARecFile->setText( fname );
}

void MainWindow::RealTimeIsSelected( void )
{
  if ( SelRealTime->isChecked() ) {
    SelLiveTime->setChecked( false );
  }
}

void MainWindow::LiveTimeIsSelected( void )
{
  if ( SelLiveTime->isChecked() ) {
    SelRealTime->setChecked( false );
  }
}

void MainWindow::SelSSDs0( void )
{
  for ( int i = 0; i < SSDbs.count(); i++ ) {
    if ( sender() == SSDbs.at(i) ) {
      SelSSDs( i );
      break;
    }
  }
}

void MainWindow::SelSSDs20( void )
{
  for ( int i = 0; i < SSDbs2.count(); i++ ) {
    if ( sender() == SSDbs2.at(i) ) {
      if ( SSDbs2.at(i)->isChecked() == PBTrue ) {
	SSDbs2.at(i)->setStyleSheet( SSDActive );
	SSDbs2.at(i)->setToolTip( tr( "Active" ) );
	SFluo->setSSDUsingCh( i, true );
	emit SelectedSSD( i, true );
      } else {
	SSDbs2.at(i)->setStyleSheet( SSDnotActive );
	SSDbs2.at(i)->setToolTip( tr( "Inactive" ) );
	SFluo->setSSDUsingCh( i, false );
	emit SelectedSSD( i, false );
      }
    }
  }
}

void MainWindow::SelSSDs( int ch )
{
  for ( int i = 0; i < SSDbs.count(); i++ ) {
    if ( i == ch ) {
      SSDbs[i]->setChecked( PBTrue );
      SSDbs[i]->setStyleSheet( SSDActive );
      SSDbs[i]->setToolTip( tr( "Active" ) );
      MCACh->setValue( i );
      MCAChSelected( i );
    } else {
      SSDbs[i]->setChecked( PBFalse );
      SSDbs[i]->setStyleSheet( SSDnotActive );
      SSDbs[i]->setToolTip( tr( "Inactive" ) );
    }
  }
}

void MainWindow::getMCASettings( int ch )
{
  s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetPeakingTime", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetThreshold", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetCalibration", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetDynamicRange", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetPreAMPGain", QString::number( ch ) );
  SFluo->GetMCA( ch );
}

void MainWindow::getMCALen( SMsg msg )  // 初期化の時に一回しか呼ばれないと信じる
{
  if ( ( msg.From() == SFluo->getDriver() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    MCALength = msg.Val().toInt();
  }
  for ( int i = 0; i < MaxSSDs; i++ ) {
    ROIStart[i] = "0";
    ROIEnd[i] = QString::number( MCALength - 1 );
  }
  int ch = MCACh->text().toInt();
  ROIStartInput->setText( ROIStart[ ch ] );
  ROIEndInput->setText( ROIEnd[ ch ] );
}

void MainWindow::newROIStart( const QString &newv )
{
  ROIStart[ MCACh->text().toInt() ] = newv;
  if ( cMCAView != NULL ) {
    cMCAView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
    cMCAView->update();
  }
}

void MainWindow::newROIEnd( const QString &newv )
{
  ROIEnd[ MCACh->text().toInt() ] = newv;
  if ( cMCAView != NULL ) {
    cMCAView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
    cMCAView->update();
  }
}

void MainWindow::MCAChSelected( int i )
{
  if ( i == cMCACh )
    return;

  if ( i < 0 ) { MCACh->setValue( MaxSSDs - 1 ); i = MaxSSDs - 1; }
  if ( i >= MaxSSDs ) { MCACh->setValue( 0 ); i = 0; }
  cMCACh = i;
  getMCASettings( cMCACh );
  ROIStartInput->setText( ROIStart[ cMCACh ] );
  ROIEndInput->setText( ROIEnd[ cMCACh ] );
  if ( cMCAView != NULL ) {
    cMCAView->SetMCACh( cMCACh );
    cMCAView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
    SFluo->GetRealTime( cMCACh );
    SFluo->GetLiveTime( cMCACh );
  }
  SelSSDs( cMCACh );
}

void MainWindow::showPeakingTime( SMsg msg )
{
  if ( ( msg.From() == SFluo->getDriver() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    PeakingTimeInput->setText( msg.Val() );
  }
}

void MainWindow::showThreshold( SMsg msg )
{
  if ( ( msg.From() == SFluo->getDriver() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    ThresholdInput->setText( msg.Val() );
  }
}

void MainWindow::showCalibration( SMsg msg )
{
  if ( ( msg.From() == SFluo->getDriver() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    CalibrationInput->setText( msg.Val() );
  }
}

void MainWindow::showDynamicRange( SMsg msg )
{
  if ( ( msg.From() == SFluo->getDriver() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    DynamicRangeInput->setText( msg.Val() );
  }
}

void MainWindow::showPreAMPGain( SMsg msg )
{
  if ( ( msg.From() == SFluo->getDriver() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    GainInput->setText( msg.Val() );
  }
}

void MainWindow::StartMCA( void )
{
  if ( !inMCAMeas ) {
    if ( ! SFluo->isEnable() ) {
      QString msg = QString( tr( "Scan cannot Start : SSD is disabled" ) );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
      return;
    }

    MCAStart->setText( tr( "Stop" ) );
    MCAStart->setStyleSheet( InActive );

    inMCAMeas = true;

    //    oldMCACh = cMCACh;
    //    cMCACh = MCACh->text().toInt();

    //    if (( StartResume == MCA_START )||( cMCACh != oldMCACh )) {
    if ( ( cMCAViewTabNo != ViewTab->currentIndex() )
	 || ( StartResume == MCA_START ) ) {
      if ( cMCAView != NULL ) {
	MCAViewDisconnects();
	cMCAViewC->setIsDeletable( true );
      }
      
      if ( ( cMCAViewC = SetUpNewView( MCAVIEW ) ) == NULL ) 
	return;
      ViewTab->setTabText( ViewTab->currentIndex(), "MCA" );
      cMCAViewC->setNowDType( MCADATA );
      cMCAView = (MCAView*)(cMCAViewC->getView());
      cMCAView->setSelectedAtoms( PT2->getSelectedAtoms() );
      
      MCAData = cMCAView->setMCAdataPointer( MCALength );
      validMCAData = true;
      cMCAViewTabNo = ViewTab->currentIndex();
      cMCAView->setLog( SetDisplayLog->isChecked() );
      cMCAView->SetMCACh( cMCACh );
      cMCAView->makeValid( true );
      MCAViewConnects();

      cMCAView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
      if ( StartResume == MCA_START )
	for ( int i = 0; i < MCALength; i++ ) MCAData[i] = 0;
    }
    MCAClearRequest = false;
    SFluo->RunStop();
    cMCAViewC->setIsDeletable( false );
    MCAStage = 0;
    MCATimer->start( 100 );
  } else {
    inMCAMeas = false;
    SFluo->RunStop();
    SFluo->setSSDPresetType( "REAL" );
    MCATimer->stop();
    MCAStart->setText( tr( "Start" ) );
    MCAStart->setStyleSheet( NormalB );
    cMCAViewC->setIsDeletable( false );
  }
}

void MainWindow::MCAViewDisconnects( void )
{
#if 0
  // MCAView -> MainWindow への connect は MCAView 内で繋ぐ
  // data-disp で表示した時、にマズイかも
  disconnect( cMCAView, SIGNAL( CurrentValues( int, int ) ),
	      this, SLOT( showCurrentValues( int, int ) ) );
  disconnect( cMCAView, SIGNAL( newROI( int, int ) ),
	      this, SLOT( setNewROI( int, int ) ) );
  // MainWindow -> MCAView への connect はなくす (直接の関数呼び出しにする)
  // こっちは OK
  disconnect( SetDisplayLog, SIGNAL( clicked( bool ) ),
	      cMCAView, SLOT( setLog( bool ) ) );
  disconnect( DispElmNames, SIGNAL( toggled( bool ) ),
	      cMCAView, SLOT( setShowElements( bool ) ) );
#endif
  //  disconnect( SetDisplayLog, SIGNAL( clicked( bool ) ),
  //	      this, SLOT( NoticeMCAViewSetDisplayLog( bool ) ) );
}

void MainWindow::MCAViewConnects( void )
{
#if 0
  // MCAView -> MainWindow への connect は MCAView 内で繋ぐ
  // data-disp で表示した時、にマズイかも
  connect( cMCAView, SIGNAL( CurrentValues( int, int ) ),
	   this, SLOT( showCurrentValues( int, int ) ) );
  connect( cMCAView, SIGNAL( newROI( int, int ) ),
	   this, SLOT( setNewROI( int, int ) ) );
  // MainWindow -> MCAView への connect はなくす (直接の関数呼び出しにする)
  // こっちは OK
  connect( SetDisplayLog, SIGNAL( clicked( bool ) ),
	   cMCAView, SLOT( setLog( bool ) ) );
  connect( DispElmNames, SIGNAL( toggled( bool ) ),
	   cMCAView, SLOT( setShowElements( bool ) ) );
#endif
}

void MainWindow::showCurrentValues( int atCur, int inROI )
{
  if ( sender() == cMCAView ) {
    ValAtCurDisp->setText( QString::number( atCur ) );
    ValInROIDisp->setText( QString::number( inROI ) );
  }
}

void MainWindow::setNewROI( int s, int e )
{
  if ( sender() == cMCAView ) {
    ROIStartInput->setText( ROIStart[ MCACh->text().toInt() ] = QString::number( s ) );
    ROIEndInput->setText( ROIEnd[ MCACh->text().toInt() ] = QString::number( e ) );
  }
}

void MainWindow::MCASequence( void )
{
  if ( MCAStage < 2 )         // MCA に RunStart をかけてしまうと、ずっと isBusy
    if ( SFluo->isBusy() || SFluo->isBusy2() )
      return;
  if ( SFluo->isBusy2() )
    return;

  if ( MCAClearRequest == true ) {
    MCAClearRequest = false;
    for ( int i = 0; i < MCALength; i++ ) MCAData[i] = 0;
    StartResume = MCA_START;
    MCAStage = 0;
  }

  switch( MCAStage ) {
  case 0:
    SFluo->InitLocalStage();
    MCAStage = 1;
  case 1:
    if ( SelRealTime->isChecked() ) {
      SFluo->setSSDPresetType( "REAL" );
      MCAStage = 2;
    } else if ( SelLiveTime->isChecked() ) {
      SFluo->setSSDPresetType( "LIVE" );
      MCAStage = 2;
    } else {
      SFluo->setSSDPresetType( "NONE" );
      MCAStage = 3;
    }
    break;
  case 2:
    if ( SelRealTime->isChecked() || SelLiveTime->isChecked() ) {
      SFluo->SetTime( PresetTime->text().toDouble() );
    }
    MCAStage = 3;
    break;
  case 3:
    if ( SFluo->InitSensor() == false ) {  // true :: initializing
      if ( StartResume == MCA_START ) {
	SFluo->RunStart();
	StartResume = MCA_RESUME;
      } else {
	SFluo->RunResume();
      }
      SFluo->InitLocalStage();
      MCAStage = 4;
    }
    break;
  case 4:
    if ( SFluo->GetMCA( cMCACh ) == false ) {
      SFluo->GetRealTime( cMCACh );
      SFluo->GetLiveTime( cMCACh );
      SFluo->InitLocalStage();
      MCAStage = 4;
    }
    break;
  }
}

void MainWindow::ShowNewMCAStat( void )
{
  QStringList MCA;

  if ( cMCAView != NULL ) {
    MCA = SFluo->MCAvalues();
    for ( int i = 0; i < MCA.count() && i < MCALength; i++ ) {
      MCAData[i] = MCA.at(i).toInt();
    }
    cMCAView->SetRealTime( SFluo->realTime( cMCACh ) );
    cMCAView->SetLiveTime( SFluo->liveTime( cMCACh ) );
    cMCAView->update();
  }
}

void MainWindow::ShowNewMCARealTime( int ch )
{
  if ( cMCAView != NULL ) {
    if ( ch == cMCACh ) {
      cMCAView->SetRealTime( SFluo->realTime( cMCACh ) );
    }
  }
}

void MainWindow::ShowNewMCALiveTime( int ch )
{
  if ( cMCAView != NULL ) {
    if ( ch == cMCACh )
      cMCAView->SetLiveTime( SFluo->liveTime( cMCACh ) );
  }
}

void MainWindow::clearMCA( void )
{
  if ( inMCAMeas ) {
    MCAClearRequest = true;
  } else {
    StartResume = MCA_START;
    if ( cMCAView != NULL ) {
      for ( int i = 0; i < MCALength; i++ ) {
	MCAData[i] = 0;
      }
      MCAStage = 0;
      cMCAView->update();
    }
  }
}

void MainWindow::ConnectToDataLinkServer( QString host, qint16 port )
{
  if ( !hasConnected ) {
    hasConnected = true;
    qDebug() << "data link server" << host << port;
    dLink = new QTcpSocket;
    dLinkStream = new QDataStream( dLink );
    connect( dLink, SIGNAL( readyRead() ), this, SLOT( receiveMCAs() ) );
    dLink->connectToHost( host, port );
  }
}

void MainWindow::receiveMCAs( void )
{
  int ch, len, size, bytes;
  unsigned long *mca;

  dLink->waitForBytesWritten( 10 );
  qDebug() << "receive data via dLink";
  for ( int i = 0; i < 19; i++ ) {
    while ( dLink->bytesAvailable() < 12 );
    *dLinkStream >> ch >> len >> size;
    qDebug() << "receiced data info " << ch << len << size;
    while ( dLink->bytesAvailable() < len * size );
    mca = new unsigned long[ len ];
    bytes = dLinkStream->readRawData( (char *)mca, len * size );
    qDebug() << "receiced data bytes ?" << bytes;
    delete mca;
  }
}
