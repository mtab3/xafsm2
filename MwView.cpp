#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupView( void )
{
  QVector<QWidget*> ViewBases;
  ViewBases << VT1 << VT2 << VT3 << VT4 << VT5 << VT6 << VT7 << VT8 << VT9 << VT10;

  ViewCTRL *viewCtrl;

  for ( int i = 0; i < ViewBases.count(); i++ ) {
    viewCtrl = new ViewCTRL;
    viewCtrl->setViewBase( ViewBases.at(i) );
    viewCtrl->setNowView( (void *)NULL );
    viewCtrl->setNowVType( NONVIEW );
    viewCtrl->setIsDeletable( true );
    ViewCtrls << viewCtrl;
  }
}

