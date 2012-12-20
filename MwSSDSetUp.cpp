#include <QtGui>

#include "SMsg.h"
#include "MainWindow.h"
#include "MCAView.h"

void MainWindow::setupSetupSSDArea( void )   /* 測定エリア */
{
  SSDbs << SSDE01 << SSDE02 << SSDE03 << SSDE04 << SSDE05
        << SSDE06 << SSDE07 << SSDE08 << SSDE09 << SSDE10
        << SSDE11 << SSDE12 << SSDE13 << SSDE14 << SSDE15
        << SSDE16 << SSDE17 << SSDE18 << SSDE19;

  for ( int i = 0; i < MaxSSDs; i++ ) {
    SSDbs[i]->setStyleSheet( "background-color: #eeffee" );
    SSDbs[i]->setToolTip( tr( "Active" ) );
  }

  connect( SSDbs[0],  SIGNAL( clicked() ), this, SLOT( SelSSDs00() ) );
  connect( SSDbs[1],  SIGNAL( clicked() ), this, SLOT( SelSSDs01() ) );
  connect( SSDbs[2],  SIGNAL( clicked() ), this, SLOT( SelSSDs02() ) );
  connect( SSDbs[3],  SIGNAL( clicked() ), this, SLOT( SelSSDs03() ) );
  connect( SSDbs[4],  SIGNAL( clicked() ), this, SLOT( SelSSDs04() ) );
  connect( SSDbs[5],  SIGNAL( clicked() ), this, SLOT( SelSSDs05() ) );
  connect( SSDbs[6],  SIGNAL( clicked() ), this, SLOT( SelSSDs06() ) );
  connect( SSDbs[7],  SIGNAL( clicked() ), this, SLOT( SelSSDs07() ) );
  connect( SSDbs[8],  SIGNAL( clicked() ), this, SLOT( SelSSDs08() ) );
  connect( SSDbs[9],  SIGNAL( clicked() ), this, SLOT( SelSSDs09() ) );
  connect( SSDbs[10], SIGNAL( clicked() ), this, SLOT( SelSSDs10() ) );
  connect( SSDbs[11], SIGNAL( clicked() ), this, SLOT( SelSSDs11() ) );
  connect( SSDbs[12], SIGNAL( clicked() ), this, SLOT( SelSSDs12() ) );
  connect( SSDbs[13], SIGNAL( clicked() ), this, SLOT( SelSSDs13() ) );
  connect( SSDbs[14], SIGNAL( clicked() ), this, SLOT( SelSSDs14() ) );
  connect( SSDbs[15], SIGNAL( clicked() ), this, SLOT( SelSSDs15() ) );
  connect( SSDbs[16], SIGNAL( clicked() ), this, SLOT( SelSSDs16() ) );
  connect( SSDbs[17], SIGNAL( clicked() ), this, SLOT( SelSSDs17() ) );
  connect( SSDbs[18], SIGNAL( clicked() ), this, SLOT( SelSSDs18() ) );

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
  inMCAMeas = false;
  MCAData = NULL;
  cMCAV = NULL;

  SelSSDs00();
}

void MainWindow::SelSSDs00( void ) { SelSSDs(  0 ); }
void MainWindow::SelSSDs01( void ) { SelSSDs(  1 ); }
void MainWindow::SelSSDs02( void ) { SelSSDs(  2 ); }
void MainWindow::SelSSDs03( void ) { SelSSDs(  3 ); }
void MainWindow::SelSSDs04( void ) { SelSSDs(  4 ); }
void MainWindow::SelSSDs05( void ) { SelSSDs(  5 ); }
void MainWindow::SelSSDs06( void ) { SelSSDs(  6 ); }
void MainWindow::SelSSDs07( void ) { SelSSDs(  7 ); }
void MainWindow::SelSSDs08( void ) { SelSSDs(  8 ); }
void MainWindow::SelSSDs09( void ) { SelSSDs(  9 ); }
void MainWindow::SelSSDs10( void ) { SelSSDs( 10 ); }
void MainWindow::SelSSDs11( void ) { SelSSDs( 11 ); }
void MainWindow::SelSSDs12( void ) { SelSSDs( 12 ); }
void MainWindow::SelSSDs13( void ) { SelSSDs( 13 ); }
void MainWindow::SelSSDs14( void ) { SelSSDs( 14 ); }
void MainWindow::SelSSDs15( void ) { SelSSDs( 15 ); }
void MainWindow::SelSSDs16( void ) { SelSSDs( 16 ); }
void MainWindow::SelSSDs17( void ) { SelSSDs( 17 ); }
void MainWindow::SelSSDs18( void ) { SelSSDs( 18 ); }

void MainWindow::SelSSDs( int ch )
{
  for ( int i = 0; i < 19; i++ ) {
    if ( ch == i ) {
      SSDbs[i]->setChecked( true );
      SSDbs[i]->setStyleSheet( "background-color: #eeffee" );
      SSDbs[i]->setToolTip( tr( "Active" ) );
      MCACh->setValue( i );
    } else {
      SSDbs[i]->setChecked( false );
      SSDbs[i]->setStyleSheet( "background-color: #aabbaa" );
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
    qDebug() << "MCA Length " << MCALength;
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
}

void MainWindow::newROIEnd( const QString &newv )
{
  ROIEnd[ MCACh->text().toInt() ] = newv;
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
    MCAStart->setText( tr( "Stop" ) );
    MCAStart->setStyleSheet( "background-color: yellow" );

    inMCAMeas = true;

    int cTab = ViewTab->currentIndex();
    cMCACh = MCACh->text().toInt();

    if ( cMCAV != NULL ) {
      disconnect( cMCAV, SIGNAL( CurrentValues( int, int ) ),
		  this, SLOT( showCurrentValues( int, int ) ) );
      disconnect( cMCAV, SIGNAL( newROI( int, int ) ),
		  this, SLOT( setNewROI( int, int ) ) );
      disconnect( SetDisplayLog, SIGNAL( clicked( bool ) ),
		  cMCAV, SLOT( setLog( bool ) ) );
    }
    deleteView( cTab );
    cMCAV = new MCAView;
    MCAData = cMCAV->setMCAdataPointer( MCALength );
    cMCAV->setLog( SetDisplayLog->isChecked() );
    cMCAV->SetMCACh( cMCACh );

    connect( cMCAV, SIGNAL( CurrentValues( int, int ) ),
	     this, SLOT( showCurrentValues( int, int ) ) );
    connect( cMCAV, SIGNAL( newROI( int, int ) ), this, SLOT( setNewROI( int, int ) ) );
    connect( SetDisplayLog, SIGNAL( clicked( bool ) ), cMCAV, SLOT( setLog( bool ) ) );

    ViewBases.at( cTab )->layout()->addWidget( cMCAV );
    nowViews[ cTab ] = (void *)cMCAV;
    nowVTypes[ cTab ] = MCAVIEW;
    for ( int i = 0; i < MCALength; i++ ) MCAData[i] = 0;
    SFluo->setSSDPresetType( "NONE" );
    SFluo->RunStop();
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

  switch( MCAStage ) {
  case 0:
    SFluo->InitLocalStage();
    MCAStage = 1;
  case 1:
    if ( SFluo->InitSensor() == false ) {  // true :: initializing
      if ( StartResume == MCA_START ) {
	SFluo->RunStart();
	StartResume = MCA_RESUME;
      } else {
	SFluo->RunResume();
      }
      SFluo->InitLocalStage();
      MCAStage = 2;
    }
    break;
  case 2:
    if ( SFluo->GetMCA( cMCACh ) == false ) {
      SFluo->GetRealTime( cMCACh );
      SFluo->GetLiveTime( cMCACh );
      SFluo->InitLocalStage();
      MCAStage = 3;
    }
    break;
  case 3:
    MCA = SFluo->MCAvalues();
    for ( int i = 0; i < MCA.count() && i < MCALength; i++ ) {
      MCAData[i] = MCA.at(i).toInt();
    }
    cMCAV->SetRealTime( SFluo->realTime( cMCACh ) );
    cMCAV->SetLiveTime( SFluo->liveTime( cMCACh ) );
    cMCAV->update();
    MCAStage = 2;
    break;
  }
}
