
#include "MainWindow.h"
#include "ViewCtrl.h"

static int vcount = 0;

ViewCTRL::ViewCTRL( void )
{
  ViewBase = NULL;
  nowView = NULL;
  nowVType = NONVIEW;
  nowDType = NONDATA;
  deletable = true;
  gsbStat = NULL;
  VC = vcount++;
}

ViewCTRL::~ViewCTRL( void )
{
  deleteView();
}

bool ViewCTRL::setView( void *view, VTYPE vtype, DATATYPE dtype )
{
  if ( nowView != NULL ) {
    if ( ( nowVType != vtype ) || ( nowDType != dtype ) ){
      return false;
    }
    if ( ! deleteView() )
      return false;
  }
  nowView = view;
  nowVType = vtype;
  nowDType = dtype;
  ViewBase->layout()->addWidget( (QWidget *)nowView );

  deletable = true;
  gsbStat = new GSBStats;
  
  return true;
}

bool ViewCTRL::deleteView( void )
{
  if ( nowView == NULL ) 
    return true;                    // the view is already cleaned up
  if ( deletable ) {
    ViewBase->layout()->removeWidget( (QWidget *)nowView );
    switch( nowVType ) {
    case XYVIEW:
      delete (XYView *)nowView; break;
    case TYVIEW:
      delete (TYView *)nowView; break;
    case MCAVIEW:
      delete (MCAView *)nowView; break;
    case S2DVIEW:
      delete (S2DView *)nowView; break;
    default:
      break;
    }
    nowView = (void *)NULL;
    nowVType = NONVIEW;
    nowDType = NONDATA;
    gsbStat->clear();
    delete gsbStat;
    gsbStat = NULL;
    // deletable = true;
    return true;                    // the view is cleaned up
  }
  return false;                     // the view can not be cleaned up
}

void ViewCTRL::addAGSBStat( QString label, bool f )
{
  if ( gsbStat != NULL ) {
    gsbStat->addAStat( label, f );
  }
}

void ViewCTRL::setGSBStats( QVector<aGSBS> GSBSs )
{
  for ( int i = 0; i < GSBSs.count(); i++ ) {
    if ( i < gsbStat->count() ) {
      gsbStat->setAStat( i, GSBSs[i].label, GSBSs[i].stat );
    } else {
      gsbStat->addAStat( GSBSs[i].label, GSBSs[i].stat );
    }
  }
}

QString ViewCTRL::getAGSBSLabel( int n )
{
  if ( gsbStat != NULL )
    return gsbStat->getALabel( n );

  return "";
}

bool ViewCTRL::getAGSBSStat( int n )
{
  if ( gsbStat != NULL )
    return gsbStat->getAStat( n );

  return PBFalse;
}

QStringList ViewCTRL::getGSBLabels( void )
{
  if ( gsbStat != NULL ) {
    return gsbStat->labels();
  }

  QStringList rv;
  return rv;
}

QVector<bool> ViewCTRL::getGSBFlags( void )
{
  if ( gsbStat != NULL )
    return gsbStat->flags();

  QVector<bool> rv;
  return rv;
}

void ViewCTRL::setGSBStat( int i, bool f ) 
{
  if ( gsbStat == NULL )
    return;

  gsbStat->setAStat( i, f );
}

// 以下は MainWindow の関数だが、ViewCTRL を確保するには必ずこれを使うはず

/*******************
Data.cpp:  theViewC = viewC;
MwMeas.cpp:    if ( ( MeasViewC = SetUpNewView( XYVIEW ) ) == NULL ) {
MwSSDSetUp.cpp:    if ( ( cMCAViewC = SetUpNewView( MCAVIEW ) ) == NULL ) 
MwSetup.cpp:    if ( ( ScanViewC = SetUpNewView( XYVIEW ) ) == NULL ) {
MwSetup.cpp:    if ( ( MonitorViewC = SetUpNewView( TYVIEW ) ) == NULL ) {
ReadData.cpp:      viewC = SetUpNewView( XYVIEW );
ReadData.cpp:    viewC = SetUpNewView( TYVIEW );
ReadData.cpp:      viewC = SetUpNewView( XYVIEW );
ReadData.cpp:    viewC = SetUpNewView( MCAVIEW );
ReadData.cpp:    viewC = SetUpNewView( S2DVIEW );

ReadData.cpp:    viewC = ViewCtrls[ ViewTab->currentIndex() ];
ReadData.cpp:    viewC = ViewCtrls[ ViewTab->currentIndex() ];
*******************/


ViewCTRL *MainWindow::SetUpNewView( VTYPE vtype, DATATYPE dtype )
{
  // まずは View の方を先に作っておいて
  void *newView = NULL;
  switch( vtype ) {
  case XYVIEW:
    newView = (void *)(new XYView);
    ((XYView*)newView)->setParent( this );
    ((XYView*)newView)->setDiffType1( conds->Diff1Type() );
    ((XYView*)newView)->setDiffType2( conds->Diff2Type() );
    break;
  case TYVIEW:
    newView = (void *)(new TYView);
    ((TYView*)newView)->setParent( this );
    ((TYView*)newView)->setMovingAvr( MovingAvr->text().toInt() );
    break;
  case MCAVIEW:
    newView = (void *)(new MCAView( this ));
    ((MCAView *)newView)->setKeV2Pix( XMAPk2p );
    ((MCAView *)newView)->setFDBase( fdbase );
    ((MCAView *)newView)->setShowElements( DispElmNames->isChecked() );
    ((MCAView *)newView)->setShowElementsAlways( ShowAlwaysSelElm->isChecked() );
    ((MCAView *)newView)->setShowElementsEnergy( ShowElmEnergy->isChecked() );
    ((MCAView *)newView)->setLimitPSEnergy( LimitPSEnergy->isChecked() );
    ((MCAView *)newView)->setShowDiff( ShowDiff->isChecked() );
    ((MCAView *)newView)->setShowSmoothed( ShowSmoothed->isChecked() );
    ((MCAView *)newView)->setPeakSearch( MCAPeakSearch->isChecked() );
    ((MCAView *)newView)->setFitToRaw( FitToRaw->isChecked() );
    ((MCAView *)newView)->setLog( SetDisplayLog->isChecked() );
    ((MCAView *)newView)->setNewPSSens( PeakSearchSensitivity->text() );
    ((MCAView *)newView)->setMaxEnergy( MaxMCAEnergy );
    ((MCAView *)newView)->setMaxLoop( MaxLoop->text().toInt() );
    ((MCAView *)newView)->setDampFact( DampFact->text().toDouble() );
    ((MCAView *)newView)->setPrec1( Prec1->text().toDouble() );
    ((MCAView *)newView)->setPrec2( Prec2->text().toDouble() );
    break;
  case S2DVIEW:
    newView = (void *)(new S2DB( this ) );
    //    "Not Set Parent in MainWindow";
    //    ((S2DB*)newView)->setParent( this );
    ((S2DB*)newView)->setK2P( XMAPk2p );
    break;
  default:
    break;
  }
  // newView は ViewCTRL の中で作れば良さそうなものだが、
  // 上の操作にいっぱい MainWindow の持ち物が出てくるのでめんどくさい
  
  if ( newView == NULL ) {
    qDebug() << "Can't setup new View";
    return NULL;
  }

  // ViewCTRL(ViewTab と一対一対応) に登録する
  // 現在の ViewTab に対応する ViewCTRL が使えたらそれで OK
  if ( ! ViewCtrls[ ViewTab->currentIndex() ]->setView( newView, vtype, dtype ) ) {
    // 使えなかったら、若い番号の ViewTab から順番に登録できないか試してみる
    int i;
    for ( i = 0; i < ViewTab->count(); i++ ) {
      if ( ViewCtrls[ i ]->setView( newView, vtype, dtype ) ) {
	break;
      }
    }
    if ( i < ViewTab->count() ) {
      // 登録できたら OK、その ViewTab を表に出す
      ViewTab->setCurrentIndex( i );
    } else {
      // 登録できなかったら今作った View を消して NULL を返して終了
      statusbar->showMessage( tr( "No View is available!" ), 2000 );
      switch( vtype ) {
      case XYVIEW:
	delete (XYView *)newView; break;
      case TYVIEW:
	delete (TYView *)newView; break;
      case MCAVIEW:
	delete (MCAView *)newView; break;
      case S2DVIEW:
	delete (S2DB *)newView; break;
      default:
	qDebug() << "Unknow vewType was passed to SetUpNewView";
      }
      newView = NULL;
      return NULL;
    }
  }

  // ViewTab まで確定した後の後始末
  switch( vtype ) {
  case MCAVIEW:
    ((MCAView*)newView)->setLayout( ViewTab->widget( ViewTab->currentIndex() )->layout() );
    break;
  case S2DVIEW:
    ((S2DB*)newView)->setLayout( ViewTab->widget( ViewTab->currentIndex() )->layout() );
    break;
  default:
    break;
  }

  // 次に使える ViewTab が残っていなかったら一つ追加しておく
  int i;
  for ( i = 0; i < ViewCtrls.count(); i++ ) {
    if ( ViewCtrls[i]->getVType() == NONVIEW )
      break;
  }
  if ( i >= ViewCtrls.count() ) {
    addAView();
  }
  // 正常時には確保した ViewCTRL を返して終了
  ViewTab->setTabText( ViewTab->currentIndex(),
		       QString( "%1(%2)" )
		       .arg( ViewTypeNames[dtype] ).arg( ++viewCounts[dtype] ) );
  return ViewCtrls[ ViewTab->currentIndex() ];
}

// これも MainWindow の関数だが MCAView の確保に使われる

/******************************************************
// MCAView の確保には特別な手続きがある?
// なんでだったっけ ??

MwMeas.cpp:      getNewMCAView();
MwS2D.cpp:      getNewMCAView();
MwSSDSetUp.cpp:    getNewMCAView();
ReadData.cpp:  getNewMCAView();
********************************************************/

void MainWindow::getNewMCAView( void )
{
  if ( cMCAViewTabNo >= 0 )
    if ( ViewCtrls[ cMCAViewTabNo ]->getDType() == MCADATA )
      return;

  if ( ( cMCAViewC = SetUpNewView( MCAVIEW, MCADATA ) ) == NULL ) 
    return;
  // 他のタイプの View は ViewCTRL 内で付けた名前を使ってる
  ViewTab->setTabText( ViewTab->currentIndex(), tr( "MCA" ) );
  // cMCAViewC->setNowDType( MCADATA );
  cMCAView = (MCAView*)(cMCAViewC->getView());
  cMCAView->setSelectedAtoms( PT2->getSelectedAtoms() );
  
  MCAData = cMCAView->setMCAdataPointer( SFluo->length() );
  validMCAData = true;
  cMCAViewTabNo = ViewTab->currentIndex();
  cMCAView->setLog( SetDisplayLog->isChecked() );
  cMCAView->SetMCACh( cMCACh );
  cMCAView->makeValid( true );
  
  cMCAView->setROI( ROIStartInput->text().toInt(), ROIEndInput->text().toInt() );
  if ( StartResume == MCA_START )
    for ( int i = 0; i < SFluo->length(); i++ ) MCAData[i] = 0;
}

