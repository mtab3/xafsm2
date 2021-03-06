#include "MainWindow.h"

void MainWindow::Print( QPrinter *p )
{
  void *nv = ViewCtrls[ ViewTab->currentIndex() ]->getView();
  if ( nv == NULL )
    return;

  p->setOrientation( QPrinter::Landscape );

  switch( ViewCtrls[ ViewTab->currentIndex() ]->getVType() ) {
  case XYVIEW:
    ((XYView*)nv)->print( p ); break;
  case TYVIEW:
    ((TYView*)nv)->print( p ); break;
  case MCAVIEW:
     ((MCAView*)nv)->print( p ); break;
  default: break;
  }
}
