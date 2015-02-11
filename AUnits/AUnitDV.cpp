
#include "AUnitDV.h"

void AUnitDV::init0( Stars *s )
{
  connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  init00( s );
}

void AUnitDV::init00( Stars *s )
{
  connect( s, SIGNAL( AnsQInitialize( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsQGetData( SMsg ) ), this, SLOT( RcvQGetData( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsQFinalize( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
}

void AUnitDV2::init00( Stars *s )
{
  connect( s, SIGNAL( AnsSetAperture( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetAutoZero( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
}

/* DV/DV2 は DV の方が QXAFS 用で、DV2 が NORMAL 用 */

bool AUnitDV::QStart( void )
{
  IsBusy2On( Driver, "Start" );
  s->SendCMD2( Uid, DevCh, "qInitialize", QString::number( setTime ) );
  return false
}

bool AUnitDV::QRead( void )
{
  IsBusy2On( Driver, "Read" );
  s->SendCMD2( Uid, DevCh, "qGetData" );

  return false;
}

bool AUnit::QEnd( void )
{
  IsBusy2On( Driver, "End" );
  s->SendCMD2( Uid, DevCh, "qFinalize" );

  return false;
}
