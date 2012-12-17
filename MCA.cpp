
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
    inMCAMeas = true;
    cMCACh = MCACh->text().toInt();
    cMCAV = new MCAView;
    
    int cTab = ViewTab->currentIndex();
    if ( nowViews[ cTab ] != NULL ) {
      ViewBases.at( cTab )->layout()->removeWidget( (QWidget *)nowViews[ cTab ] );
      delete nowViews[ cTab ];
      nowViews[ cTab ] = (void *)NULL;
    }
    ViewBases.at( cTab )->layout()->addWidget( cMCAV );
    nowViews[ cTab ] = (void *)cMCAV;
  } else {
    inMCAMeas = false;
  }
  

}
