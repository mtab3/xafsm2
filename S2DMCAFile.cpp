
#include "MainWindow.h"

void MainWindow::SaveS2DMCAs( void )
{
  // S2D の事後セーブ
  // S2D の計測に使う View は View タブには無くメインの中に一個だけなので
  // 他の View の様にあるナシとか、どれが対象とかを気にしなくていい
  
  if ( S2DBase == NULL ) {
    statusbar->showMessage( tr( "S2D data is not valid" ), 2000 );
    return;
  }
  if ( inS2D ) {
    statusbar->showMessage( tr( "Can not save MCA spectra while scanning." ), 2000 );
    return;
  }

  S2DBase->SaveMCAs( S2DFileName0->text() );
}

