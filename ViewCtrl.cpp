
#include "ViewCtrl.h"

ViewCTRL::ViewCTRL( void )
{
}

bool ViewCTRL::setView( void *view, VTYPE vtype )
{
  if ( nowView != NULL ) {           // 現在の View は使用中
    if ( vtype == nowVType ) {       // 表示しようとしているのと同じ種類のグラフなら
      if ( ! deleteView() ) {        // 乗っ取りを企てる。
	return false;                // ダメなら諦める。
      }
    } else {
      return false;                  // 違う種類のグラフなら諦める
    }
  }
  nowView = view;
  nowVType = vtype;
  deletable = true;
  ViewBase->layout()->addWidget( (QWidget *)view );
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
    return true;                    // the view is cleaned up
  }
  return false;                     // the view can not be cleaned up
}


