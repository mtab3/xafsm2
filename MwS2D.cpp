
#include "MainWindow.h"

#include "SMsg.h"

void MainWindow::setupScan2DArea( void )
{
  S2DFileSel = new QFileDialog;
  S2DFileSel->setAcceptMode( QFileDialog::AcceptSave );
  S2DFileSel->setDirectory( QDir::currentPath() );
  S2DFileSel->setFilter( "*.dat" );

  

  for ( int i = 0; i < ASensors.count(); i++ ) {
    SelectS2DSensor->addItem( ASensors[i]->getName() );
  }
  for ( int i = 0; i < AMotors.count(); i++ ) {
    S2DAx1->addItem( AMotors[i]->getName() );
    S2DAx2->addItem( AMotors[i]->getName() );
    S2DAx3->addItem( AMotors[i]->getName() );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( ShowS2DCurMotorPos( SMsg ) ) );
    connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( ShowS2DCurMotorPos( SMsg ) ) );
  }

  connect( S2DFileSelect, SIGNAL( clicked() ), S2DFileSel, SLOT( show() ) );
  connect( S2DFileSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( newS2DFileSelected( const QString & ) ) );
}

void MainWindow::newS2DFileSelected( const QString &fname )
{
  S2DFileName->setText( fname );
  S2DNameStat = NEW;
  S2DFileName->setStyleSheet( FSTATCOLORS[ S2DDataStat ][ S2DNameStat ] );
  S2DFileName->setToolTip( FSTATMsgs[ S2DDataStat ][ S2DNameStat ] );
}

void MainWindow::ShowS2DCurMotorPos( SMsg msg )
{
}
