
#include "MainWindow.h"

void MainWindow::setupView( void )
{
  ViewCount = 0;

  // タブを動かせるようにすると ViewCTRL と ViewTab の関係がずれて管理が面倒
  ViewTab->setMovable( false );
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
    ViewTab->removeTab( i );
    ViewCtrls[i]->deleteView();
    ViewCtrls.removeAt( i );
  }
}

void *MainWindow::findAView( DATATYPE dtype )
{
  void *rv = NULL;
  int vcnt = -1;
  
  for ( int i = 0; i < ViewCtrls.count(); i++ ) {
    ViewCTRL *vc = ViewCtrls[i];
    if ( vc->getDType() == dtype ) {
      if ( vc->vcnt() > vcnt ) {
	rv = vc->getView();
	vcnt = vc->vcnt();
      }
    }
  }

  return rv;
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
    SetUpSFluo *ssfluo = (SetUpSFluo*)view->parent();
    ssfluo->setViewStats();
  }
}

