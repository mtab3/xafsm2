
#include "S2DB.h"

S2DB::S2DB( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  connect( S2DPopUp, SIGNAL( clicked() ), this, SIGNAL( popup() ), Qt::UniqueConnection );
  connect( S2DPrintB, SIGNAL( clicked() ), this, SIGNAL( print() ), Qt::UniqueConnection );
}
