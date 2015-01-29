
#include "MainWindow.h"

#define S2DMCAFILETITLE "measured by 2D Scan"

void MainWindow::SaveS2DMCAs( void )
{
  if (( ! S2DI.valid )||( inS2D )||( ! S2DI.isSFluo )) {
    statusbar->showMessage( tr( "Can not save MCA spectra for 2D scan." ), 2000 );
    return;
  }
  if ( S2DFileName0->text().simplified().isEmpty() ) {
    statusbar->showMessage( tr( "No file name was selected" ), 2000 );
    return;
  }
  QFileInfo f1( S2DFileName0->text() );
  QDir dir( f1.absoluteDir().absolutePath() );
  dir.mkdir( f1.baseName() );
  dir.cd( f1.baseName() );
  QFileInfo f2 = QFileInfo( dir.absolutePath(), f1.baseName() );
  // f2 : path と basename の結合を Qt に任せる
  QString bfname = f2.filePath();

  mcaWFList.clear();
  for ( int y = 0; y <= S2DI.ps[1]; y++ ) {
    for ( int x = 0; x < S2DI.ps[0] + (( S2DI.ScanMode == STEP ) ? 0 : 1 ); x++ ) {
      QString fname = QString( "%1-MCA-%2-%3" )
	.arg( bfname )
	.arg( y, 4, 10, QChar( '0' ) ).arg( x, 4, 10, QChar( '0' ) );
      mcaWFList << fname;
    }
  }
  mcaWriting = false;
  mcaTimer->setInterval( 10 );
  mcaTimer->start();
}

void MainWindow::S2DMCAWriteNext( void )
{
  if ( mcaWriting )
    return;

  QString wfname;
  if ( mcaWFList.count() > 0 ) {
    wfname = mcaWFList[0];
    mcaWFList.removeAt( 0 );
  } else {
    mcaTimer->stop();
  }

  mcaWriting = true;

  QStringList cmps = wfname.split( "-" );
  if ( cmps.count() > 1 ) {
    int x = cmps[ cmps.count() - 1 ].toInt();
    int y = cmps[ cmps.count() - 2 ].toInt();
    aMCASet *set = S2DBase->mapAPoint( x, y );
    if ( ( set != NULL ) && ( set->isValid() ) ) {
      ShowMessageOnSBar( tr( "Wriging MCA File : [%1]" ).arg( wfname + ".dat" ), 500 );
      set->save( wfname + ".dat", S2DMCAFILETITLE );
    }
  }
  mcaWriting = false;
}
