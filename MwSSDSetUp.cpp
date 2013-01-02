#include <QtGui>

#include "SMsg.h"
#include "MainWindow.h"
#include "MCAView.h"

void MainWindow::setupSetupSSDArea( void )   /* 測定エリア */
{
  SSDActive = "background-color: #ccffcc";
  SSDnotActive = "background-color: #aabbaa";

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
    SSDbs2[i]->setChecked( true );
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

  inMCAMeas = false;
  validMCAData = false;
  MCAData = NULL;
  cMCAView = NULL;
  cMCACh = 0;
  oldMCACh = -1;

  SelSSDs( 0 );
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


  out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" ) << "\n";
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
      if ( SSDbs2.at(i)->isChecked() ) {
	SSDbs2.at(i)->setStyleSheet( SSDActive );
	SSDbs2.at(i)->setToolTip( tr( "Active" ) );
	SFluo->setSSDUsingCh( i, true );
      } else {
	SSDbs2.at(i)->setStyleSheet( SSDnotActive );
	SSDbs2.at(i)->setToolTip( tr( "Inactive" ) );
	SFluo->setSSDUsingCh( i, false );
      }
    }
  }
}

void MainWindow::SelSSDs( int ch )
{
  for ( int i = 0; i < SSDbs.count(); i++ ) {
    if ( i == ch ) {
      SSDbs[i]->setChecked( true );
      SSDbs[i]->setStyleSheet( SSDActive );
      SSDbs[i]->setToolTip( tr( "Active" ) );
      MCACh->setValue( i );
    } else {
      SSDbs[i]->setChecked( false );
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
}

void MainWindow::getMCALen( SMsg msg )  // 初期化の時に一回しか呼ばれないと信じる
{
  if ( ( msg.From() == SFluo->getDriver() )&&( msg.ToCh() == "SetUpMCA" ) ) {
    MCALength = msg.Val().toInt();
  }
  for ( int i = 0; i < 19; i++ ) {
    ROIStart[i] = "0";
    ROIEnd[i] = QString::number( MCALength - 1 );
  }
  ROIStartInput->setText( ROIStart[ MCACh->text().toInt() ] );
  ROIEndInput->setText( ROIEnd[ MCACh->text().toInt() ] );
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
  if ( i < 0 ) { MCACh->setValue( 18 ); i = 18; }
  if ( i > 18 ) { MCACh->setValue( 0 ); i = 0; }
  getMCASettings( i );
  ROIStartInput->setText( ROIStart[ i ] );
  ROIEndInput->setText( ROIEnd[ i ] );
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
      NewLogMsg( msg + "\n" );
      return;
    }

    MCAStart->setText( tr( "Stop" ) );
    MCAStart->setStyleSheet( "background-color: yellow" );

    inMCAMeas = true;

    oldMCACh = cMCACh;
    cMCACh = MCACh->text().toInt();

    if (( StartResume == MCA_START )||( cMCACh != oldMCACh )) {
      if ( cMCAView != NULL ) {
	disconnect( cMCAView, SIGNAL( CurrentValues( int, int ) ),
		    this, SLOT( showCurrentValues( int, int ) ) );
	disconnect( cMCAView, SIGNAL( newROI( int, int ) ),
		    this, SLOT( setNewROI( int, int ) ) );
	disconnect( SetDisplayLog, SIGNAL( clicked( bool ) ),
		    cMCAView, SLOT( setLog( bool ) ) );
	cMCAViewC->setIsDeletable( true );
      }
      
      if ( ( cMCAViewC = SetUpNewView( MCAVIEW ) ) == NULL ) 
	return;
      cMCAView = (MCAView*)(cMCAViewC->getView());
      
      MCAData = cMCAView->setMCAdataPointer( MCALength );
      validMCAData = true;
      cMCAView->setLog( SetDisplayLog->isChecked() );
      cMCAView->SetMCACh( cMCACh );
      
      connect( cMCAView, SIGNAL( CurrentValues( int, int ) ),
	       this, SLOT( showCurrentValues( int, int ) ) );
      connect( cMCAView, SIGNAL( newROI( int, int ) ),
	       this, SLOT( setNewROI( int, int ) ) );
      connect( SetDisplayLog, SIGNAL( clicked( bool ) ),
	       cMCAView, SLOT( setLog( bool ) ) );

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
    MCAStart->setStyleSheet( "background-color: "
			     "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0"
			     " rgba(225, 235, 225, 255), stop:1"
			     " rgba(255, 255, 255, 255));" );
  }
}

void MainWindow::showCurrentValues( int atCur, int inROI )
{
  ValAtCurDisp->setText( QString::number( atCur ) );
  ValInROIDisp->setText( QString::number( inROI ) );
}

void MainWindow::setNewROI( int s, int e )
{
  ROIStartInput->setText( ROIStart[ MCACh->text().toInt() ] = QString::number( s ) );
  ROIEndInput->setText( ROIEnd[ MCACh->text().toInt() ] = QString::number( e ) );
}

void MainWindow::MCASequence( void )
{
  QStringList MCA;
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
      MCAStage = 5;
    }
    break;
  case 5:
    MCA = SFluo->MCAvalues();
    for ( int i = 0; i < MCA.count() && i < MCALength; i++ ) {
      MCAData[i] = MCA.at(i).toInt();
    }
    cMCAView->SetRealTime( SFluo->realTime( cMCACh ) );
    cMCAView->SetLiveTime( SFluo->liveTime( cMCACh ) );
    cMCAView->update();
    MCAStage = 4;
    break;
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
