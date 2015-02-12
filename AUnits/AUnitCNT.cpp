
#include "AUnitCNT.h"

void AUnitCNT::init0( void )
{
  connect( s, SIGNAL( AnsSetStopMode( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetTimerPreset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsCounterReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsCountStart( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsStop( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  s->SendCMD2( "Init", Dev, "SetStopMode", "T" );
  
  init00();
}

void AUnitCNT::init00( void )
{
  s->SendCMD2( "Init", Dev, "IsBusy" );
}

void AUnitCNT2::init00( void )
{
  s->SendCMD2( "Init", Dev, "IsBusy" );

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

void AUnitCNT::AskIsBusy( void )
{
  _AskIsBusy();   // CNT だけ反応して CNT2 は反応しない
}

void AUnitCNT::_AskIsBusy( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}


/* 連続スキャン対応 */
// 連続スキャンの後にノーマルモードに戻す

bool AUnitCNT::Close( void )
{
  bool rv = false;

  switch( LocalStage ) {
  case 0:
    IsBusy2On( Dev, "Close0" );
    s->SendCMD2( Uid, Dev, "Stop" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    IsBusy2On( Dev, "Close1" );
    s->SendCMD2( Uid, Dev, "SetStopMode", "T" );
    LocalStage++;
    rv = false;
    break;
  }

  return rv;
}


bool AUnitCNT2::GetRange( void ) // CNT2, OTC2
{
  QString Type2 = The2ndDev->type();
  IsBusy2On( Dev2, "GetRange" );
  if ( Type2 == "PAM" )
    s->SendCMD2( Uid, DevCh2, QString( "GetRange" ) );
  if ( Type2 == "PAM2" )
    s->SendCMD2( Uid, Dev2, QString( "GetRange " ) + Ch2 );

  return false;
}

void AUnitCNT2::ReactGetRange( SMsg msg )  // CNT2, OTC2
{
  double range = RangeL;
  if ( ( msg.From() == DevCh2 ) || ( msg.From() == Dev2 ) ) {
    QString Type2 = The2ndDev->type();
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
    
    IsBusy2Off( Dev2 );
    if ( range > RangeU ) range = RangeU;
    if ( range < RangeL ) range = RangeL;
    emit AskedNowRange( (int)range );
  }
}

