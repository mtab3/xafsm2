
#include "ViewCtrl.h"

ViewCTRL::ViewCTRL( void )
{
}

bool ViewCTRL::setView( void *view, VTYPE vtype )
{
  if ( nowView != NULL ) {
    if ( ! deleteView() ) {
      return false;                // new view is rejected to be set 
    }                              // since it can not be cleaned up.
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
    case XVIEW:
      delete (XView *)nowView; break;
    case MCAVIEW:
      delete (MCAView *)nowView; break;
    default:
      break;
    }
    nowView = (void *)NULL;
    return true;                    // the view is cleaned up
  }
  return false;                     // the view can not be cleaned up
}


