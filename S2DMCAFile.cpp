
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
  QFileInfo f2 = QFileInfo( f1.absoluteDir().absolutePath(), f1.baseName() );
  // f2 : path と basename の結合を Qt に任せる
  QString bfname = f2.filePath();

  for ( int y = 0; y <= S2DI.ps[1]; y++ ) {
    for ( int x = 0; x < S2DI.ps[0] + (( S2DI.ScanMode == STEP ) ? 0 : 1 ); x++ ) {
      qDebug() << "try to saving " << y << x;
      aMCASet *set = S2DBase->mapAPoint( x, y );
      if ( ( set != NULL ) && ( set->isValid() ) ) {
	qDebug() << "saving " << y << x;
	QString fname = QString( "%1-MCA-%2-%3.dat" )
	  .arg( bfname )
	  .arg( y, 4, 10, QChar( '0' ) ).arg( x, 4, 10, QChar( '0' ) );

	set->save( fname, S2DMCAFILETITLE );
      }
    }
  }
}
