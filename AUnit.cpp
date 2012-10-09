#include <QtGui>

#include "AUnit.h"

AUnit::AUnit( QObject *parent ) : QObject( parent )
{
  Type = "";
  ID = "";
  UID = "";
  Name = "";
  Driver = "";
  Ch = "";
  DevCh = "";
  Unit = "";

  UPP = 0;           // only for PM
  Center = 0;        // only for PM

  MaxV = 0;          // only for PZ
  MinV = 0;          // only for PZ

  isBusy = false;
  Value = "";
}

void AUnit::Initialize( Stars *S )
{
  s = S;

  // PM, nct08 は OK
  connect( s, SIGNAL( AnsIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ) );
  connect( s, SIGNAL( EvIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ) );
  connect( s, SIGNAL( AnsGetValue( SMsg ) ),this, SLOT( SetCurPos( SMsg ) ) );
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ) );

  s->SendCMD2( "Init", "System", "flgon", Driver );
  s->SendCMD2( "Init", "System", "flgon", DevCh );
  s->SendCMD2( "Init", DevCh, "IsBusy" );
  s->SendCMD2( "Init", DevCh, "GetValue" );

  if ( Type == "PAM" ) {   // Keithley 6485
    connect( s, SIGNAL( AnsRead( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ) );
    // こんなにまとめてドカンとやっていいかどうかは後で検討
    s->SendCMD2( "Init", DevCh, "Reset" );
    sleep( 1 );
    s->SendCMD2( "Init", DevCh, "SetAutoRangeEnable", "1" );
    sleep( 1 );
    s->SendCMD2( "Init", DevCh, "SetDataFormatElements", "READ" );
    sleep( 1 );
    s->SendCMD2( "Init", DevCh, "SetZeroCheckEnable", "0" );
  }
  if ( Type == "CNT" ) {   // nct08
    s->SendCMD2( "Init", DevCh, "SetStopMode", "C" );
  }
}

void AUnit::show( void )
{
  qDebug() << tr( " TP[%1] ID[%2] NM[%3] DR[%4] Ch[%5] DC[%6] "
		" UT[%7] PU[%8] CT[%9] MaV[%10] MiV[%11] " )
    .arg( Type )
    .arg( ID ).arg( Name ).arg( Driver ).arg( Ch ).arg( DevCh ).arg( Unit )
    .arg( UPP ).arg( Center ).arg( MaxV ).arg( MinV );
}

void AUnit::GetValue( void )
{
  // Motor
  if ( Type == "PM" ) {
    s->SendCMD2( UID, DevCh, "GetValue" );
  }

  // Sensor
  if ( Type == "PAM" ) {    // Keithley
    s->SendCMD2( UID, DevCh, "Read" );
  }
  if ( Type == "CNT" ) {    // nct08
    s->SendCMD2( UID, DevCh, "GetValue" );
  }
}

void AUnit::SetValue( double v )
{
  if ( Type == "PM" ) {
    s->SendCMD2( UID, DevCh, "SetValue", QString::number( (int)v ) );
  }
}

void AUnit::SetSpeed( MSPEED i )
{
  QString cmd = "SpeedLow";

  if ( Type == "PM" ) {
    switch( i ) {
    case LOW: cmd = "SpeedLow"; break;
    case MIDIUM: cmd = "SpeedMiddle"; break;
    case HIGH: cmd = "SpeedHigh"; break;
    default: cmd = "SpeedLow"; break;
    }
    s->SendCMD2( UID, DevCh, cmd );
  }
}

void AUnit::AskIsBusy( void )
{
  if ( Type == "PM" ) {
    s->SendCMD2( UID, DevCh, "IsBusy" );
  }

  if ( Type == "CNT" ) {
    s->SendCMD2( UID, Driver, "IsBusy" );
  }
}

void AUnit::SetCurPos( SMsg msg )
{
  QString buf;

  if ( ( msg.From() == DevCh )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE ) 
	    || ( msg.Msgt() == READ ) ) ) {
    Value = msg.Val();

    if ( ( msg.Msgt() == READ )||( msg.Msgt() == GETVALUE ) ) {
      isBusy = false;
    }
  }
}

void AUnit::SetIsBusyByMsg( SMsg msg )
{
  if ( Type == "PM" ) {
    if ( ( msg.From() == DevCh )
	 && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
      isBusy = ( msg.Val().toInt() == 1 );
    }
  }

  if ( Type == "CNT" ) {
    if ( ( msg.From() == Driver )
	 && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
      isBusy = ( msg.Val().toInt() == 1 );
    }
  }
}

void AUnit::Stop( void )
{
  if ( Type == "PM" ) {
    s->SendCMD2( UID, DevCh, "Stop" );
  }
}

void AUnit::SetTime( double dtime )   // in sec
{
  double time;
  long int ltime;

  if ( Type == "SSD" ) {
  }
  if ( Type == "PAM" ) {
    // 1 sec -> 1/60 sec
    time = dtime * 60;
    if ( time < 1 ) time = 1;
    if ( time > 60 ) time = 60;
    s->SendCMD2( UID, DevCh, "SetNPLCycles", QString::number( time ) );
  }
  if ( Type == "CNT" ) {
    ltime = dtime * 1e6;
    s->SendCMD2( UID, DevCh, "SetTimerPreset", QString::number( ltime ) );
  }
}

