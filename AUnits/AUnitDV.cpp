
#include "AUnitDV.h"

void AUnitDV::init0( void )
{
  connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  init00();
}

void AUnitDV::init00( void )
{
  connect( s, SIGNAL( AnsQInitialize( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsQGetData( SMsg ) ), this, SLOT( RcvQGetData( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsQFinalize( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
}

void AUnitDV2::init00( void )
{
  connect( s, SIGNAL( AnsSetAperture( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetAutoZero( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
}

void AUnitDV2::AskIsBusy( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}

/* DV/DV2 は DV の方が QXAFS 用で、DV2 が NORMAL 用 */

bool AUnitDV::QStart( void )
{
  IsBusy2On( Dev, "Start" );
  s->SendCMD2( Uid, DevCh, "qInitialize", QString::number( setTime ) );
  return false;
}

bool AUnitDV::QRead( void )
{
  IsBusy2On( Dev, "Read" );
  s->SendCMD2( Uid, DevCh, "qGetData" );

  return false;
}

bool AUnitDV::QEnd( void )
{
  IsBusy2On( Dev, "End" );
  s->SendCMD2( Uid, DevCh, "qFinalize" );

  return false;
}
