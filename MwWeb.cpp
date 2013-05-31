
#include <QUrl>

#include "MainWindow.h"

void MainWindow::setupWebView( void )
{
  QUrl url( "http://www.astf-kha.jp/synchrotron/status/kougen/status.html" );
  webView->setZoomFactor( 0.43 );
  webView->load( url );
}
