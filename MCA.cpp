
#include "SMsg.h"
#include "MainWindow.h"
#include "MCAView.h"

void MainWindow::getMCASettings( int ch )
{
  s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetPeakingTime", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetThreshold", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetCalibration", QString::number( ch ) );
  s->SendCMD2( "SetUpMCA", SFluo->getDriver(), "GetDynamicRange", QString::number( ch ) );
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

    cMCAV->setLog( SetDisplayLog->isChecked() );
    connect( cMCAV, SIGNAL( CurrentValues( int, int ) ),
	     this, SLOT( showCurrentValues( int, int ) ) );
    connect( cMCAV, SIGNAL( newROI( int, int ) ), this, SLOT( setNewROI( int, int ) ) );
    connect( SetDisplayLog, SIGNAL( clicked( bool ) ), cMCAV, SLOT( setLog( bool ) ) );

    ViewBases.at( cTab )->layout()->addWidget( cMCAV );
    nowViews[ cTab ] = (void *)cMCAV;
    nowVTypes[ cTab ] = MCAVIEW;
    if ( MCAData != NULL )
      delete MCAData;
    MCAData = new int[ MCALength ];
    for ( int i = 0; i < MCALength; i++ ) MCAData[i] = 0;
    cMCAV->setMCAdataPointer( MCAData, MCALength );
    SFluo->setSSDPresetType( "NONE" );
    SFluo->RunStop();
    MCAStage = 0;
    MCATimer->start( 100 );
  } else {
    inMCAMeas = false;
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
      SFluo->RunStart();
      SFluo->InitLocalStage();
      MCAStage = 2;
    }
    break;
  case 2:
    if ( SFluo->GetMCA( cMCACh ) == false ) {
      SFluo->InitLocalStage();
      MCAStage = 3;
    }
    break;
  case 3:
    MCA = SFluo->MCAvalues();
    for ( int i = 0; i < MCA.count() && i < MCALength; i++ ) {
      MCAData[i] = MCA.at(i).toInt();
    }
    cMCAV->update();
    MCAStage = 2;
    break;
  }
}
