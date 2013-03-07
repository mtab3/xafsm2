
#include "ViewCtrl.h"

ViewCTRL::ViewCTRL( void )
{
  ViewBase = NULL;
  nowView = NULL;
  nowVType = NONVIEW;
  nowDType = NONDATA;
  deletable = true;
  gsbStat = NULL;
}

bool ViewCTRL::setView( void *view, VTYPE vtype )
{
  if ( nowView != NULL ) {           // 現在の View は使用中 ?
    if ( vtype == nowVType ) {       // 表示しようとしているのと同じ種類のグラフなら
      if ( ! deleteView() ) {        // 乗っ取りを企てる。
	return false;                // ダメなら諦める。
      }
    } else {
      return false;                  // 種類の違うグラフなら諦める
    }
  }
  nowView = view;
  nowVType = vtype;
  deletable = true;
  ViewBase->layout()->addWidget( (QWidget *)view );
  gsbStat = new GSBStats;
  return true;                   // new view is set.
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
    default:
      break;
    }
    nowView = (void *)NULL;
    nowVType = NONVIEW;
    nowDType = NONDATA;
    gsbStat->clear();
    delete gsbStat;
    gsbStat = NULL;
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
