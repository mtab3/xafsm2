
#include "MainWindow.h"

void MainWindow::setupReadDataArea( void )
{
  // 仮の処置、データ読込タブを消してしまう --> 復活
  //  MainTab->removeTab( MainTab->indexOf( ReadDataTab ) );

  Datas << Data001 << Data002 << Data003 << Data004 << Data005
        << Data006 << Data007 << Data008 << Data009 << Data010;

  for ( int i = 0; i < Datas.count(); i++ ) {
    connect( Datas[i], SIGNAL( AskToGetNewView( D_TYPE, QString, QString ) ),
	     this, SLOT( TryToGiveNewView( D_TYPE, QString, QString ) ),
	     Qt::UniqueConnection );
    //    connect( this, SIGNAL( GiveNewView( QObject *, ViewCTRL * ) ),
    //	     Datas[i], SLOT( GotNewView( QObject *, ViewCTRL * ) ),
    //       Qt::UniqueConnection );
    connect( Datas[i], SIGNAL( showMessage( QString, int ) ),
	     this, SLOT( ShowMessageOnSBar( QString, int ) ),
	     Qt::UniqueConnection );
    connect( Datas[i], SIGNAL( GiveMeCurrentView( void ) ),
	     this, SLOT( TryToNoticeCurrentView( void ) ),
	     Qt::UniqueConnection );
    
#if 0
    connect( Datas[i], SIGNAL( setMCACh( int ) ), this, SLOT( MCAChSelected( int ) ),
	     Qt::UniqueConnection );
    connect( this, SIGNAL( NewMCACh( int ) ), Datas[i], SLOT( SelectedNewMCACh( int ) ),
	     Qt::UniqueConnection );
    //    Datas[i]->setSSFluos( SSFluos );
#endif
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

void MainWindow::TryToGiveNewView( D_TYPE dtype, QString dir, QString uid )
{
  QObject *from = sender();
  ViewCTRL *viewC = NULL;

  qDebug() << "1111"; 

  int sfn = 0;
  int vcn = 0;
  switch( dtype ) {
#if 0
  case MEASSHOW:  // MEASDATA と SCANDATA は今表示されてるのが同タイプだったら重ね書き
    // ここは復活させるなら要再検討
    vcn = ViewCtrls[ ViewTab->currentIndex() ];
    viewC = ( vcn < 0 ) ? NULL : ViewCtrls[ vcn ];
    if ( viewC->getNowDType() != dtype ) {
      viewC = SetUpNewView( XYVIEW );
      //      ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-XAFS" ) );
      ClearXViewScreenForMeas( (XYView*)(view->getView()) );
    }
    break;
#endif
  case MONDATA:
    vcn = SetUpNewView( TYVIEW, MONDATA, READD, NULL );
    viewC = ( vcn < 0 ) ? NULL : ViewCtrls[ vcn ];
    //    ViewTab->setTabText( ViewTab->currentIndex(), tr( "D-MON." ) );
    break;
  case SCANDATA:
    vcn = SetUpNewView( XYVIEW, SCANDATA, READD, NULL );
    viewC = ( vcn < 0 ) ? NULL : ViewCtrls[ vcn ];
    ClearXViewScreenForScan( (XYView*)(viewC->getView()) );
    break;
  case MCADATA:
    if ( SFluos.count() <= 0 )
      break;
    for ( sfn = 0; sfn < SFluos.count(); sfn++ ) {
      if ( SFluos[sfn]->uid() == uid )
	break;
    }
    if ( sfn >= SFluos.count() )
      sfn = 0;
    vcn = SetUpNewView( MCAVIEW, MCADATA, READD, new MCAView( NULL, SSFluos[sfn] ) );
    viewC = ( vcn < 0 ) ? NULL : ViewCtrls[ vcn ];
    break;
  case S2DDATA:
    vcn = SetUpNewView( S2DVIEW, S2DDATA, READD, NULL );
    viewC = ( vcn < 0 ) ? NULL : ViewCtrls[ vcn ];
#if 0
    connect( (S2DB*)(viewC->getView() ), SIGNAL( askToGetNewMCAView( S2DB*) ),
	     this, SLOT( ansToGetNewMCAView( S2DB* ) ) );
#endif
    ((S2DB*)(viewC->getView()))->setParent( this );
    ((S2DB*)(viewC->getView()))->setRead( true );
    ((S2DB*)(viewC->getView()))->setDataRoot( ( dir == "" ) ? conds->dataRoot() : dir );
    break;
  default:
    viewC = NULL;
    break;
  }

  if ( viewC != NULL )
    ((Data*)from)->GotNewView( viewC, AMotors, ASensors );
}

#if 0
void MainWindow::ansToGetNewMCAView( S2DB* s2db )
{
  getNewMCAView();    // S2DView は、MwMeas が使う MCAView (cMCAView) を共有する
  s2db->getNewMCAMap();
}
#endif

