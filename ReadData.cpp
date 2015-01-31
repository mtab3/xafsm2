
#include "MainWindow.h"

void MainWindow::setupReadDataArea( void )
{
  // 仮の処置、データ読込タブを消してしまう --> 復活
  //  MainTab->removeTab( MainTab->indexOf( ReadDataTab ) );

  Datas << Data001 << Data002 << Data003 << Data004 << Data005
        << Data006 << Data007 << Data008 << Data009 << Data010;

  for ( int i = 0; i < Datas.count(); i++ ) {
    connect( Datas.at(i), SIGNAL( AskToGetNewView( DATATYPE, QString ) ),
	     this, SLOT( TryToGiveNewView( DATATYPE, QString ) ),
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
#if 0
  connect( CloseView, SIGNAL( clicked() ), this, SLOT( DeleteTheView() ),
	   Qt::UniqueConnection );
#endif
}

void MainWindow::TryToNoticeCurrentView( void )
{
  ((Data*)sender())->GotCurrentView( ViewCtrls[ ViewTab->currentIndex() ]->getView() );
}

#if 0
void MainWindow::DeleteTheView( void )
{
  ViewTab->setTabText( ViewTab->currentIndex(),
		       QString( "View%1" ).arg( ViewTab->currentIndex()+1 ) );
  ViewCtrls[ ViewTab->currentIndex() ]->deleteView();
}
#endif

void MainWindow::TryToGiveNewView( DATATYPE dtype, QString dir )
{
  QObject *from = sender();
  ViewCTRL *viewC;

  switch( dtype ) {
#if 0
  case MEASSHOW:  // MEASDATA と SCANDATA は今表示されてるのが同タイプだったら重ね書き
    // ここは復活させるなら要再検討
    viewC = ViewCtrls[ ViewTab->currentIndex() ];
    if ( viewC->getNowDType() != dtype ) {
      viewC = SetUpNewView( XYVIEW );
      //      ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-XAFS" ) );
      ClearXViewScreenForMeas( (XYView*)(view->getView()) );
    }
    break;
#endif
  case MONSHOW:
    viewC = SetUpNewView( TYVIEW, MONSHOW );
    //    ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-MON." ) );
    break;
  case SCANSHOW:
    qDebug() << "getScanShow";
    viewC = SetUpNewView( XYVIEW, SCANSHOW );
    //    ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-SCAN" ) );
    ClearXViewScreenForScan( (XYView*)(viewC->getView()) );
    break;
  case MCASHOW:
    viewC = SetUpNewView( MCAVIEW, MCASHOW );
    //    ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-MCA" ) );
    break;
  case S2DSHOW:
    viewC = SetUpNewView( S2DVIEW, S2DSHOW );
    connect( (S2DB*)(viewC->getView() ), SIGNAL( askToGetNewMCAView( S2DB*) ),
	     this, SLOT( ansToGetNewMCAView( S2DB* ) ) );
    ((S2DB*)(viewC->getView()))->setParent( this );
    ((S2DB*)(viewC->getView()))->setRead( true );
    ((S2DB*)(viewC->getView()))->setDataRoot( ( dir == "" ) ? conds->dataRoot() : dir );
    //    ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-S2D" ) );
    break;
  default:
    viewC = NULL;
    break;
  }

  if ( viewC != NULL )
    ((Data*)from)->GotNewView( viewC, AMotors );
}

void MainWindow::ansToGetNewMCAView( S2DB* s2db )
{
  //  if ( cMCAView == NULL ) {
  getNewMCAView();    // S2DView は、MwMeas が使う MCAView (cMCAView) を共有する
  //  }
  s2db->getNewMCAMap( MCALength, SAVEMCACh );
}
