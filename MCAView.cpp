
#include <QtGui>

#include "XafsM.h"
#include "MCAView.h"

MCAView::MCAView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  valid = false;
}

void MCAView::setMCAdataPointer( double *mca, int len )
{
  MCA = mca;
  MCALen = len;
}

void MCAView::mouseMoveEvent( QMouseEvent *e ) {}
void MCAView::mousePressEvent( QMouseEvent *e ) {}
void MCAView::mouseReleaseEvent( QMouseEvent *e ) {}
void MCAView::mouseDoubleClickEvent( QMouseEvent *e ) {}
