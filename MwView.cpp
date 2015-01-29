
#include "MainWindow.h"

void MainWindow::setupView( void )
{
  QVector<QWidget*> ViewBases;
  ViewBases << VT1 << VT2 << VT3 << VT4 << VT5 << VT6 << VT7 << VT8 << VT9 << VT10;

  ViewCTRL *viewCtrl;

  for ( int i = 0; i < ViewBases.count(); i++ ) {
    viewCtrl = new ViewCTRL;
    viewCtrl->setViewBase( ViewBases.at(i) );
    //    viewCtrl->setNowView( (void *)NULL );
    //    viewCtrl->setNowVType( NONVIEW );
    //    viewCtrl->setNowDType( NONDATA );
    //    viewCtrl->setIsDeletable( true );
    ViewCtrls << viewCtrl;
  }

  oldCurrentView = 0;

  connect( ViewTab, SIGNAL( currentChanged( int ) ),
	   this, SLOT( moveToATab( int ) ),
	   Qt::UniqueConnection );
}

void MainWindow::moveToATab( int tab ) 
{
  NoticeSelectedStats( tab );
  ShowButtonsForATab( tab );   // for XYView, oldCurrentView is set in the function
}

void MainWindow::ShowButtonsForCurrentTab( void )   // manage graph-select-button
{
  ShowButtonsForATab( ViewTab->currentIndex() );
}


void MainWindow::ShowButtonsForATab( int i )   // manage graph-select-button
{
  QStringList lbls;

  if ( i != oldCurrentView )
    saveGSBs( oldCurrentView );
  clearGSBs();
  loadGSBs( i );
  oldCurrentView = i;
}

void MainWindow::NoticeSelectedStats( int tab )
{
  if ( ViewCtrls[ tab ]->getVType() == MCAVIEW ) {
    MCAView *view = (MCAView*)(ViewCtrls[ tab ]->getView());
    view->setLog( SetDisplayLog->isChecked() );
    view->setShowElements( DispElmNames->isChecked() );
    view->setShowElementsAlways( ShowAlwaysSelElm->isChecked() );
    view->setShowElementsEnergy( ShowElmEnergy->isChecked() );
  }
}

