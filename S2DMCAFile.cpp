
#include "MainWindow.h"

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

  for ( int y = 0; y < S2DI.ps[1]; y++ ) {
    for ( int x = 0; x < S2DI.ps[0]; x++ ) {
      aMCASet *set = S2DMCAMap.aPoint( x, y );
      if ( ( set != NULL ) && ( set->isValid() ) ) {
	QString fname = QString( "%1-MCA-%2-%3.dat" )
	  .arg( bfname )
	  .arg( y, 4, 10, QChar( '0' ) ).arg( x, 4, 10, QChar( '0' ) );
	QFile f( fname );
	if ( f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
	  QTextStream out( &f );
	  out << "# XafsM2 MCA Data measured by 2D Scan\n";
	  out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" )
	      << "\n";
	  out << "# Ring Current : " << set->RINGCurrent << "\n";
	  out << "# I0           : " << set->I0 << "\n";
	  out << "# Channel Status Length RealTime LiveTime ICR\n";
	  for ( int i = 0; i < MaxSSDs; i++ ) {
	    MCAHead head = set->Heads[i];
	    out << "# " << head.ch << "\t" << head.stat << "\t" << head.len << "\t"
                << head.realTime << "\t" << head.liveTime << "\t" << head.icr << "\n";
	  }

	  if ( cMCAView != NULL ) {
	    if ( ShowAlwaysSelElm->isChecked() ) {
	      out << "# Selected elements list\n";
	      QStringList Elms = cMCAView->getSelectedElms();
	      for ( int i = 0; i < Elms.count(); i++ ) {
		out << "# " << Elms[i] << "\n";
	      }
	    }
	  }

	  for ( int i = 0; i < MCALength; i++ ) {
	    out << i;
	    for ( int j = 0; j < MaxSSDs; j++ ) {
	      out << "\t" << set->Ch[ j ].E[ i ];
	      out << "\t" << set->Ch[ j ].cnt[ i ];
	    }
	    out << "\n";
	  }
	  f.close();
	}
      }
    }
  }
}
