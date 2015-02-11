
#include "AUnitOTC.h"

AUnitOTC:AUnitOTC( void )
{
}

AUnitOTC2:AUnitOTC2( void )
{
}

void AUnitOTC::init0( Stars *s )
{
  connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetMode( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetCountPreset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsCounterReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
  connect( s, SIGNAL( AnsRun( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsStop( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  s->SendCMD2( "Init", Dev, "IsBusy" );
  s->SendCMD2( "Init", Driver, "Reset" );
  s->SendCMD2( "Init", Driver, "SetMode", "0" );

  init00( s );
}

void AUnitOTC::init00( Stars * )
{
}

void AUnitOTC2::init00( Stars * )
{
  connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetAutoRange( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetZeroCheck( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetRange( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetRange( SMsg ) ), this, SLOT( ReactGetRange( SMsg ) ),
	   Qt::UniqueConnection );
}

bool AUnitOTC2::GetRange( void ) // CNT2, OTC2
{
  QString Type2 = the2ndDriver->getType();
  IsBusy2On( Driver2, "GetRange" );
  if ( Type2 == "PAM" )
    s->SendCMD2( Uid, DevCh2, QString( "GetRange" ) );
  if ( Type2 == "PAM2" )
    s->SendCMD2( Uid, Driver2, QString( "GetRange " ) + Ch2 );

  return false;
}

void AUnitOTC2::ReactGetRange( SMsg msg )  // CNT2, OTC2
{
  double range = RangeL;
  if ( ( msg.From() == DevCh2 ) || ( msg.From() == Driver2 ) ) {
    QString Type2 = the2ndDriver->getType();
    if ( Type2 == "PAM" ) {
      range = log10( msg.Vals().at(0).toDouble() / 2.1 );
    }
    if ( Type2 == "PAM2" ) {
      if ( msg.Vals().at(0).toInt() == Ch2.toInt() ) {
	range = log10( msg.Vals().at(1).toDouble() / 2.1 );
      } else {
	return;
      }
    }
    
    IsBusy2Off( Driver2 );
    if ( range > RangeU ) range = RangeU;
    if ( range < RangeL ) range = RangeL;
    emit AskedNowRange( (int)range );
  }
}
