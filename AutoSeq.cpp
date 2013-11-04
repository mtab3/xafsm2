
#include "MainWindow.h"

/**********************************************************************/

#define DTAUTOSEQFNAME  "DTAutoCalib.seq"  

void MainWindow::DTAutoCalibStart( void )
{
  if ( ! inDTAutoCalib ) {
    QString fname = DTAUTOSEQFNAME;
    
    QFileInfo F( fname );
    if ( ! F.exists() ) {
      fname = ":" DTAUTOSEQFNAME;
    }

    inDTAutoCalib = true;
    DTAutoCalib->setText( tr( "Stop" ) );
    DTAutoCalib->setStyleSheet( InActive );

    connect( this, SIGNAL( AutoSequenceFinished() ), this, SLOT( DTAutoCalibFinished() ),
	     Qt::UniqueConnection );
    AutoSequence( fname );
  } else {
    AutoSequenceEnd();
  }
}

void MainWindow::DTAutoCalibFinished( void )
{
  disconnect( this, SIGNAL( AutoSequenceFinished() ),
	      this, SLOT( DTAutoCalibFinished() ) );
  DTAutoCalib->setText( tr( "Auto Calib." ) );
  DTAutoCalib->setStyleSheet( NormalB );
}

/*********************************************************************/

void MainWindow::setupAutoSequence( void )
{
  inDTAutoCalib = false;
  ASTimer = new QTimer;
  connect( ASTimer, SIGNAL( timeout() ), this, SLOT( AutoSequence0() ),
	   Qt::UniqueConnection );
}

void MainWindow::AutoSequence( QString fname )
{
  ASFile.setFileName( fname );
  if ( ! ASFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  ASin.setDevice( &ASFile );

  ASTimer->start( 10 );
}

bool MainWindow::AutoSequenceShouldBeLocked( void )
{
  return false;
}

void MainWindow::AutoSequence0( void )
{
  if ( AutoSequenceShouldBeLocked() )
    return;

  QStringList items;
  if ( ! ASReadNextLine( items ) ) {
    AutoSequenceEnd();
    return;
  }
  qDebug() << items;
}

void MainWindow::AutoSequenceEnd( void )
{
  ASTimer->stop();
  ASFile.close();

  emit AutoSequenceFinished();
}

bool MainWindow::ASReadNextLine( QStringList &items )
{
  while( ! ASin.atEnd() ) {
    items = ASin.readLine().simplified().split( QRegExp( "\\s+" ) );
    if ( items.count() <= 0 )
      continue;
    if ( items[0] == "" )
      continue;
    if ( items[0][0] == '#' )
      continue;
    return true;
  }

  return false;
}
