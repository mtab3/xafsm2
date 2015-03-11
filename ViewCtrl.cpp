
#include "MainWindow.h"
#include "ViewCtrl.h"

static int vcount = 0;

ViewCTRL::ViewCTRL( void )
{
  ViewBase = NULL;
  nowView = NULL;
  nowVType = NONVIEW;
  nowDataType = NODATA;
  nowDataOrig = NOORIG;
  deletable = true;
  gsbStat = NULL;
  VC = vcount++;
}

ViewCTRL::~ViewCTRL( void )
{
  deleteView();
}

bool ViewCTRL::setView( void *view, VTYPE vtype, D_TYPE dtype, D_ORIG dorig, bool overlap )
{
  if ( overlap ) {   // 上書きを許す場合
    if ( nowView != NULL ) {   // 現在の View が空でない場合でも
      // 表示しようとしているものと同タイプなら
      if ( ( nowVType != vtype ) || ( nowDataType != dtype ) ){
	qDebug() << "x"; 
	return false;
      }
      // 表示しようとしているものと同その View の内容を消してしまって上書きを許す
      if ( ! deleteView() ) {
	qDebug() << "w"; 
	return false;
      }
    }
  } else {
    if ( nowView != NULL ) {
      qDebug() << "z";
      return false;
    }
  }
  qDebug() << "dd"; 
  nowView = view;
  nowVType = vtype;
  nowDataType = dtype;
  nowDataOrig = dorig;
  ViewBase->layout()->addWidget( (QWidget *)nowView );

  qDebug() << "ee"; 

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
      // MCA の測定に使う View は SetUpSFluo が持っている固有の View で消してはいけない
      // データ表示のための作った View ( orig == READD ) は消していい
      if ( nowDataOrig == READD )
	delete (MCAView *)nowView;
      break;
    case S2DVIEW:
      delete (S2DView *)nowView; break;
    default:
      break;
    }
    nowView = (void *)NULL;
    nowVType = NONVIEW;
    nowDataType = NODATA;
    nowDataOrig = NOORIG;
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

int MainWindow::SetUpNewView( VTYPE vtype, D_TYPE dtype, D_ORIG dorig, void *newView, bool overlap )
{
  void *origView = newView;
  // まずは View の方を先に作っておいて
  if ( ( newView == NULL )
       || ( ( dtype == MCADATA )&&( dorig == READD ) ) ) {
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
      if ( dorig == MEASUREDD ) {
	// その場で測定する場合(データファイルの読み込みだと READD )
	// かならず newView をパラメータとして持ち込むはずなのでここは通らない
	// newView = SSFluo0->McaView();
      } else {
	qDebug() << "Here!";
	//newView = (void *)(new MCAView( this ));
	MCAView *view = (MCAView*)newView;
	SetUpSFluo *ssfluo = (SetUpSFluo*)(view->parent());
	ssfluo->setUpMCAView( view );
#if 0
	view->setKeV2Pix( ssfluo->K2P() );
	view->setFDBase( fdbase );
	view->setShowElements( ssfluo->DispElmNamesIsChecked() );
	view->setShowElementsAlways( ssfluo->ShowAlwaysSelElmIsChecked() );
	view->setShowElementsEnergy( ssfluo->ShowElmEnergyIsChecked() );
	//	view->setLimitPSEnergy( LimitPSEnergy->isChecked() );
	view->setShowDiff( ssfluo->ShowDiffIsChecked() );
	view->setShowSmoothed( ssfluo->ShowSmoothedIsChecked() );
	//	view->setPeakSearch( MCAPeakSearch->isChecked() );
	view->setFitToRaw( ssfluo->FitToRawIsChecked() );
	view->setLog( ssfluo->SetDisplayLogIsChecked() );
	//	view->setNewPSSens( PeakSearchSensitivity->text() );
	view->setMaxEnergy( ssfluo->maxMCAEnergy() );
	view->setMaxLoop( ssfluo->maxLoop() );
	view->setDampFact( ssfluo->dampFact() );
	view->setPrec1( ssfluo->prec1() );
	view->setPrec2( ssfluo->prec2() );
#endif
      }
      break;
    case S2DVIEW:
      newView = (void *)(new S2DB( this ) );
      //    "Not Set Parent in MainWindow";
      // // ((S2DB*)newView)->setParent( this );
      // ((S2DB*)newView)->setK2P( SSFluos[0]->K2P() );   // これもホントはだめ
      break;
    default:
      break;
    }
  }
  // newView は ViewCTRL の中で作れば良さそうなものだが、
  // 上の操作にいっぱい MainWindow の持ち物が出てくるのでめんどくさい

  qDebug() << "aa00";
  if ( newView == NULL ) {
    qDebug() << "Can't setup new View";
    return -1;
  }

  qDebug() << "a";
  // ViewCTRL(ViewTab と一対一対応) に登録する
  // 現在の ViewTab に対応する ViewCTRL が使えたらそれで OK
  if ( ! ViewCtrls[ ViewTab->currentIndex() ]
       ->setView( newView, vtype, dtype, dorig, overlap ) ) {
    // 使えなかったら、若い番号の ViewTab から順番に登録できないか試してみる
    int i;
    for ( i = 0; i < ViewTab->count(); i++ ) {
      if ( ViewCtrls[ i ]->setView( newView, vtype, dtype, dorig, overlap ) ) {
	break;
      }
    }
    if ( i < ViewTab->count() ) {
      // 登録できたら OK、その ViewTab を表に出す
      ViewTab->setCurrentIndex( i );
    } else {
      // 登録できなかったら今作った View を消して NULL を返して終了
      statusbar->showMessage( tr( "No View is available!" ), 2000 );
      if ( origView == NULL ) {
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
      }
      newView = origView;
      return -1;
    }
  }

  // ViewTab まで確定した後の後始末
  switch( vtype ) {
  case MCAVIEW:
    ((MCAView*)newView)
      ->setLayout( ViewTab->widget( ViewTab->currentIndex() )->layout() );
    break;
  case S2DVIEW:
    ((S2DB*)newView)
      ->setLayout( ViewTab->widget( ViewTab->currentIndex() )->layout() );
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
		       .arg( ViewTypeNames[dorig][dtype] ).arg( ++viewCounts[dtype] ) );

  return ViewTab->currentIndex();
}

// これも MainWindow の関数だが MCAView の確保に使われる

/******************************************************
// MCAView の確保には特別な手続きがある?
// なんでだったっけ ??

// MwMeas.cpp:      getNewMCAView();
// MwS2D.cpp:       getNewMCAView();
// MwSSDSetUp.cpp:  getNewMCAView();
// ReadData.cpp:    getNewMCAView();
********************************************************/

void MainWindow::showOnesMCAView( SetUpSFluo *ssfluo )
{
  int i = 0;
  // 与えられた ssfluo の view を持った tab が既にあるか
  for ( ; i < ViewCtrls.count(); i++ ) {
    if ( ViewCtrls[i]->getView() == ssfluo->McaView() )
      break;
  }
  // 無ければ ssfluo が持つ view を標準の手順で tab に登録する
  int vcn = i;
  if ( i >= ViewCtrls.count() ) {
    if ( ( vcn = SetUpNewView( MCAVIEW, MCADATA, MEASUREDD,
			       ssfluo->McaView(), false ) ) < 0 )
      return;
  }
  ViewTab->setTabText( vcn, ssfluo->sFluo()->name() );
  ssfluo->setViewStats();
  ssfluo->McaView()->makeValid( true );
}
