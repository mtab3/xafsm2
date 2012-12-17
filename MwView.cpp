#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupView( void )
{
  ViewBases << VT1 << VT2 << VT3 << VT4 << VT5 << VT6 << VT7 << VT8 << VT9 << VT10;
  for ( int i = 0; i < ViewBases.count(); i++ ) {
    nowViews << (void *)NULL;
    nowVTypes << NONVIEW;
  }
}

void MainWindow::deleteView( int tab )
{
  if ( nowViews[ tab ] != NULL ) {
    ViewBases.at( tab )->layout()->removeWidget( (QWidget *)nowViews[ tab ] );
    switch( nowVTypes[ tab ] ) {
    case XVIEW:
      delete (XView *)nowViews[ tab ]; break;
    case MCAVIEW:
      delete (MCAView *)nowViews[ tab ]; break;
    default:
      break;
    }
    nowViews[ tab ] = (void *)NULL;
  }
}


