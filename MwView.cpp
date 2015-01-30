
#include "MainWindow.h"

void MainWindow::setupView( void )
{
  ViewCount = 0;


  ViewTab->setTabsClosable( true );
  for ( int i = 0; i < 1; i++ ) {
    addAView();
  }

  oldCurrentView = 0;

  connect( ViewTab, SIGNAL( currentChanged( int ) ),
	   this, SLOT( moveToATab( int ) ),
	   Qt::UniqueConnection );
  connect( ViewTab, SIGNAL( tabCloseRequested( int ) ),
	   this, SLOT( onViewTabClosed( int ) ),
	   Qt::UniqueConnection );
}

void MainWindow::addAView( void )
{
  QWidget *tab = new QWidget;
  tab->setLayout( new QGridLayout );
  ViewTab->addTab( tab, tr( "View %1" ).arg( ++ViewCount ) );

  ViewCTRL *viewCtrl = new ViewCTRL;
  viewCtrl->setViewBase( tab );

  ViewCtrls << viewCtrl;
}

void MainWindow::onViewTabClosed( int i )
{
  if ( ( ViewTab->count() > 1 )
       && ( ViewCtrls[i]->getVType() != NONVIEW )
       && ( ViewCtrls[i]->isDeletable() ) ) {

    if ( ViewCtrls[i]->getDType() == MCADATA )
      cMCAViewTabNo = -1;
    ViewTab->removeTab( i );
    ViewCtrls[i]->deleteView();
    ViewCtrls.removeAt( i );
  }
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

