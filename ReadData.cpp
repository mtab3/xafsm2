
#include "MainWindow.h"

void MainWindow::setupReadDataArea( void )
{
  // 仮の処置、データ読込タブを消してしまう --> 復活
  //  MainTab->removeTab( MainTab->indexOf( ReadDataTab ) );

  Datas << Data001 << Data002 << Data003 << Data004 << Data005
        << Data006 << Data007 << Data008 << Data009 << Data010;

  for ( int i = 0; i < Datas.count(); i++ ) {
    connect( Datas.at(i), SIGNAL( AskToGetNewView( DATATYPE ) ),
	     this, SLOT( TryToGiveNewView( DATATYPE ) ),
	     Qt::UniqueConnection );
    //    connect( this, SIGNAL( GiveNewView( QObject *, ViewCTRL * ) ),
    //	     Datas.at(i), SLOT( GotNewView( QObject *, ViewCTRL * ) ),
    //       Qt::UniqueConnection );
    connect( Datas.at(i), SIGNAL( showMessage( QString, int ) ),
	     this, SLOT( ShowMessageOnSBar( QString, int ) ),
	     Qt::UniqueConnection );
    connect( Datas.at(i), SIGNAL( GiveMeCurrentView( void ) ),
	     this, SLOT( TryToNoticeCurrentView( void ) ),
	     Qt::UniqueConnection );
    connect( Datas[i], SIGNAL( setMCACh( int ) ), this, SLOT( MCAChSelected( int ) ),
	     Qt::UniqueConnection );
    connect( this, SIGNAL( NewMCACh( int ) ), Datas[i], SLOT( SelectedNewMCACh( int ) ),
	     Qt::UniqueConnection );
  }
  connect( CloseView, SIGNAL( clicked() ), this, SLOT( DeleteTheView() ),
	   Qt::UniqueConnection );
}

void MainWindow::TryToNoticeCurrentView( void )
{
  ((Data*)sender())->GotCurrentView( ViewCtrls[ ViewTab->currentIndex() ]->getView() );
}

void MainWindow::DeleteTheView( void )
{
  ViewTab->setTabText( ViewTab->currentIndex(),
		       QString( "View%1" ).arg( ViewTab->currentIndex()+1 ) );
  ViewCtrls[ ViewTab->currentIndex() ]->deleteView();
}

void MainWindow::TryToGiveNewView( DATATYPE dtype )
{
  QObject *from = sender();
  ViewCTRL *viewC;

  switch( dtype ) {
#if 0
  case MEASDATA:  // MEASDATA と SCANDATA は今表示されてるのが同タイプだったら重ね書き
    viewC = ViewCtrls[ ViewTab->currentIndex() ];
    if ( viewC->getNowDType() != dtype ) {
      viewC = SetUpNewView( XYVIEW );
      ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-XAFS" ) );
      ClearXViewScreenForMeas( (XYView*)(view->getView()) );
    }
    break;
#endif
  case MONDATA:   // MONDATA と MCADATA は重ね書きは諦める。
    viewC = SetUpNewView( TYVIEW );
    ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-MON." ) );
    break;
  case SCANDATA:
    viewC = ViewCtrls[ ViewTab->currentIndex() ];
    if ( viewC->getNowDType() != dtype ) {
      viewC = SetUpNewView( XYVIEW );
      ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-SCAN" ) );
      ClearXViewScreenForScan( (XYView*)(viewC->getView()) );
    }
    break;
  case MCADATA:
    viewC = SetUpNewView( MCAVIEW );
    ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-MCA" ) );
    break;
  case S2DDATA:
    viewC = SetUpNewView( S2DVIEW );
    connect( (S2DB*)(viewC->getView() ), SIGNAL( askToGetNewMCAView( S2DB*) ), this, SLOT( ansToGetNewMCAView( S2DB* ) ) );
    ((S2DB*)(viewC->getView()))->setRead( true );
    ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-S2D" ) );
    break;
  default:
    viewC = NULL;
    break;
  }

  ((Data*)from)->GotNewView( viewC, AMotors );
}

void MainWindow::ansToGetNewMCAView( S2DB* s2db )
{
  getNewMCAView();    // S2DView は、MwMeas が使う MCAView (cMCAView) を共有する
  s2db->gotNewMCAView( cMCAView, MCALength, SAVEMCACh );
}
