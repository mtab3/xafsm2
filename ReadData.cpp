
#include "MainWindow.h"

void MainWindow::setupReadDataArea( void )
{
  Datas << Data001 << Data002 << Data003 << Data004 << Data005
        << Data006 << Data007 << Data008 << Data009 << Data010;

  for ( int i = 0; i < Datas.count(); i++ ) {
    connect( Datas.at(i), SIGNAL( AskToGetNewView( DATATYPE ) ),
	     this, SLOT( TryToGiveNewView( DATATYPE ) ) );
    connect( this, SIGNAL( GiveNewView( QObject *, ViewCTRL * ) ),
	     Datas.at(i), SLOT( GotNewView( QObject *, ViewCTRL * ) ) );
    connect( Datas.at(i), SIGNAL( showMessage( QString, int ) ),
	     this, SLOT( ShowMessageOnSBar( QString, int ) ) );
  }
}

void MainWindow::TryToGiveNewView( DATATYPE dtype )
{
  QObject *from = sender();
  ViewCTRL *view;

  switch( dtype ) {
  case MEASDATA:  // MEASDATA と SCANDATA は今表示されてるのが同タイプだったら重ね書き
    view = ViewCtrls[ ViewTab->currentIndex() ];
    if ( view->getNowDType() != dtype ) {
      view = SetUpNewView( XYVIEW );
      ClearXViewScreenForMeas( (XYView*)(view->getView()) );
    }
    break;
  case SCANDATA:
    view = ViewCtrls[ ViewTab->currentIndex() ];
    if ( view->getNowDType() != dtype ) {
      view = SetUpNewView( XYVIEW );
      ClearXViewScreenForScan( (XYView*)(view->getView()) );
    }
    break;
  case MONDATA:   // MONDATA と MCADATA は重ね書きは諦める。
    view = SetUpNewView( TYVIEW );
    break;
  case MCADATA:
    view = SetUpNewView( MCAVIEW );
    break;
  default:
    view = NULL;
    break;
  }

  emit( GiveNewView( from, view ) );
}
