
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
    mUnits.clearUnits();
    mUnits.addUnit( SFluo, 0.1 );

    cMCACh = MCACh->text().toInt();
    cMCAV = new MCAView;
    int cTab = ViewTab->currentIndex();
    deleteView( cTab );
    ViewBases.at( cTab )->layout()->addWidget( cMCAV );
    nowViews[ cTab ] = (void *)cMCAV;
    nowVTypes[ cTab ] = MCAVIEW;

    MCAStage = 0;
    MCATimer->start( 100 );
  } else {
    inMCAMeas = false;
    MCATimer->stop();
    MCAStart->setText( tr( "Start" ) );
    MCAStart->setStyleSheet( "background-color: "
			     "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0"
			     " rgba(225, 235, 225, 255), stop:1"
			     " rgba(255, 255, 255, 255));" );
  }
}

void MainWindow::MCASequence( void )
{
  QStringList MCA;

  if ( mUnits.isBusy() )
    return;

  switch( MCAStage ) {
  case 0:
    mUnits.clearStage();
    MCAStage = 1;
  case 1:
    if ( mUnits.init() == false ) {  // true :: initializing
      mUnits.clearStage();
      MCAStage = 2;
    }
    break;
  case 2: 
    mUnits.setDwellTime();
    MCAStage = 3;
    break;
  case 3:
    if ( mUnits.getMCA( cMCACh ) == false ) {
      mUnits.clearStage();
      MCAStage = 4;
    }
    break;
  case 4:
    MCA = SFluo->MCAvalues();
    qDebug() << "MCA" << MCA;
    MCAStage = 3;
    break;
  }
}
