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

  isBusy = false;    // 相手に尋ねる isBusy
  isBusy2 = false;   // その他のコマンドを投げて返答が返ってくるまで isBusy2
  Value = "";

  LocalStage = 0;
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
  if ( Type != "CNT" ) 
    s->SendCMD2( "Init", DevCh, "IsBusy" );
  else 
    s->SendCMD2( "Init", Driver, "IsBusy" );

  if ( Type != "PAM" )
    s->SendCMD2( "Init", DevCh, "GetValue" );
  if ( Type == "PAM" ) {   // Keithley 6485
    connect( s, SIGNAL( AnsRead( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ) );
    connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( RcvAns( SMsg ) ) );
    connect( s, SIGNAL( AnsSetAutoRange( SMsg ) ), this, SLOT( RcvAns( SMsg ) ) );
    connect( s, SIGNAL( AnsSetDataFormat( SMsg ) ), this, SLOT( RcvAns( SMsg ) ) );
    connect( s, SIGNAL( AnsSetZeroCheck( SMsg ) ), this, SLOT( RcvAns( SMsg ) ) );
    connect( s, SIGNAL( AnsSetNPLCycles( SMsg ) ), this, SLOT( RcvAns( SMsg ) ) );
  }
  if ( Type == "CNT" ) {   // nct08
    connect( s, SIGNAL( AnsSetStopMode( SMsg ) ), this, SLOT( RcvAns( SMsg ) ) );
    connect( s, SIGNAL( AnsSetTimerPreset( SMsg ) ), this, SLOT( RcvAns( SMsg ) ) );
    connect( s, SIGNAL( AnsCounterReset( SMsg ) ), this, SLOT( RcvAns( SMsg ) ) );
    connect( s, SIGNAL( AnsCountStart( SMsg ) ), this, SLOT( RcvAns( SMsg ) ) );
    isBusy = true;
    s->SendCMD2( "Init", Driver, "SetStopMode", "T" );
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

bool AUnit::GetValue( void )
{
  bool rv = true;

  // Motor
  if ( GType == "MOTOR" ) {
    if ( Type == "PM" ) {
      s->SendCMD2( UID, DevCh, "GetValue" );
      rv = false;
    }
  } 
  // Sensor
  if ( GType == "SENSOR" ) {
    if ( Type == "PAM" ) {    // Keithley
      isBusy2 = true;
      s->SendCMD2( UID, DevCh, "Read" );
      rv = false;
    }
    if ( Type == "CNT" ) {    // nct08
      switch( LocalStage ) {
      case 0:
	isBusy2 = true;
	s->SendCMD2( UID, Driver, "CounterReset" );
	LocalStage++;
	break;
      case 1:
	isBusy = true;
	s->SendCMD2( UID, Driver, "CountStart" );
	LocalStage++;
	rv = false;
	break;
      }
    }
  }

  return rv;
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

void AUnit::RcvAns( SMsg msg )
{
  if ( Type == "PAM" ) {
    if ( ( msg.From() == DevCh )
	 && ( ( msg.Msgt() == RESET ) || ( msg.Msgt() == SETAUTORANGE )
	      || ( msg.Msgt() == SETDATAFORMAT ) || ( msg.Msgt() == SETZEROCHECK )
	      || ( msg.Msgt() == SETNPLCYCLES ) ) ) {
      isBusy2 = false;
    }
  }

  if ( Type == "CNT" ) {
    if ( ( ( msg.From() == DevCh )||( msg.From() == Driver ) )
	 && ( ( msg.Msgt() == SETSTOPMODE ) || ( msg.Msgt() == SETTIMERPRESET )
	      || ( msg.Msgt() == COUNTERRESET ) || ( msg.Msgt() == COUNTSTART ) ) ) {
      isBusy2 = false;
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
    if ( time > 40 ) time = 40;
    s->SendCMD2( UID, DevCh, "SetNPLCycles", QString::number( time ) );
  }
  if ( Type == "CNT" ) {
    ltime = dtime * 1e6;
    s->SendCMD2( UID, Driver, "SetTimerPreset", QString::number( ltime ) );
  }
}

void AUnit::InitLocalStage( void )
{
  LocalStage = 0;
}

bool AUnit::InitSensor( void )
{
  bool rv = false;

  if ( Type == "PAM" ) {         // Keithley 6845
    switch( LocalStage ) {
    case 0:
      isBusy = true;
      s->SendCMD2( "Scan", DevCh, "Reset", "" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      isBusy = true;
      s->SendCMD2( "Scan", DevCh, "SetAutoRangeEnable", "1" );
      LocalStage++;
      rv = true;
      break;
    case 2:
      isBusy = true;
      s->SendCMD2( "Scan", DevCh, "SetDataFormatElements", "READ" );
      LocalStage++;
      rv = true;
      break;
    case 3:
      isBusy = true;
      s->SendCMD2( "Scan", DevCh, "SetZeroCheckEnable", "0" );
      rv = false;
      LocalStage++;
      break;
    default:
      rv = false;
    }
  }

  return rv;
}

