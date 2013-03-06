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
    viewCtrl->setNowDType( NONDATA );
    viewCtrl->setIsDeletable( true );
    ViewCtrls << viewCtrl;
  }

  connect( ViewTab, SIGNAL( currentChanged( int ) ),
	   this, SLOT( ShowButtonsForATab( int ) ) );
}

void MainWindow::ShowButtonsForCurrentTab( void )
{
  ShowButtonsForATab( ViewTab->currentIndex() );
}


void MainWindow::ShowButtonsForATab( int i )
{
  QStringList lbls;

  clearGSBs();
  lbls = ViewCtrls[i]->getGSBLabels();
  SetGSBLabels( lbls );
  SetGSBFlags( ViewCtrls[i]->getGSBFlags() );
}
