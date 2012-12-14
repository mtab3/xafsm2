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

  lastSetV = 0;

  LocalStage = 0;
}

//                       PM      PZ      CNT      PAM      ENC      SSD      SSDP
bool AUnit::TypeCHK( int pm, int pz, int cnt, int pam, int enc, int ssd, int ssdp )
{
  bool rv = false;

  if ( ( Type == "PM"   ) && ( pm   == 1 ) ) rv = true;
  if ( ( Type == "PZ"   ) && ( pz   == 1 ) ) rv = true;
  if ( ( Type == "CNT"  ) && ( cnt  == 1 ) ) rv = true;
  if ( ( Type == "PAM"  ) && ( pam  == 1 ) ) rv = true;
  if ( ( Type == "ENC"  ) && ( enc  == 1 ) ) rv = true;
  if ( ( Type == "SSD"  ) && ( ssd  == 1 ) ) rv = true;
  if ( ( Type == "SSDP" ) && ( ssdp == 1 ) ) rv = true;

  return rv;
}

void AUnit::Initialize( Stars *S )
// ここは重くてもいいので分かりやすく書くべき
// (全部のユニットに必要な処理をしていて、必要でないことはしていないことを明確にする)
{
  s = S;
  // 現状ある unit は
  //            PM  PZ CNT PAM ENC SSD SSDP

  //            PM  PZ CNT PAM ENC SSD SSDP        // SSDP を除く全ユニット
  if ( TypeCHK(  1,  1,  1,  1,  1,  0,   0 ) ) {
    connect( s, SIGNAL( AnsIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ) );
    connect( s, SIGNAL( EvIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ) );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ),this, SLOT( SetCurPos( SMsg ) ) );
    s->SendCMD2( "Init", "System", "flgon", DevCh );
    s->SendCMD2( "Init", "System", "flgon", Driver );
    // flgon を DevCh と Driver  両方にかけると二重になるかもしれないが
    // 悪いことは起こらないはずなので安全側に振っておく
  }

  //            PM  PZ CNT PAM ENC SSD SSDP        // 駆動系だけ
  if ( TypeCHK(  1,  1,  0,  0,  1,  0,   0 ) ) {
    connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ) );
    s->SendCMD2( "Init", DevCh, "GetValue" );
  }
  if ( TypeCHK(  0,  0,  1,  1,  1,  1,   1 ) ) {
  }

  //            PM  PZ CNT PAM ENC SSD SSDP        // CNT は Ch に IsBusy を訊くとエラー
  if ( TypeCHK(  1,  1,  0,  1,  1,  1,   0 ) ) {
    s->SendCMD2( "Init", DevCh, "IsBusy" );
  }
  if ( TypeCHK(  0,  0,  1,  0,  0,  0,   0 ) ) {
    s->SendCMD2( "Init", Driver, "IsBusy" );
  }

  // 以下個別処理
  //            PM  PZ CNT PAM ENC SSD SSDP       // カウンタ(nct08)だけ
  if ( TypeCHK(  0,  0,  1,  0,  0,  0,   0 ) ) {
    connect( s, SIGNAL( AnsSetStopMode( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsSetTimerPreset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsCounterReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsCountStart( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    s->SendCMD2( "Init", Driver, "SetStopMode", "T" );
  }
  //            PM  PZ CNT PAM ENC SSD SSDP      // PAM(Keithley)だけ
  if ( TypeCHK(  0,  0,  0,  1,  0,  0,   0 ) ) {
    connect( s, SIGNAL( AnsRead( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ) );
    connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsSetAutoRange( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsSetDataFormat( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsSetZeroCheck( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsSetNPLCycles( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
  }
  //            PM  PZ CNT PAM ENC SSD SSDP      // SSD(Xmap)だけ
  if ( TypeCHK(  0,  0,  0,  0,  0,  1,   0 ) ) {
    connect( s, SIGNAL( AnsIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ) );
    connect( s, SIGNAL( EvIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ) );
    connect( s, SIGNAL( AnsSetPresetType( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsSetPresetValue( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsRunStart( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsRunStop( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsGetValues( SMsg ) ), this, SLOT( ReceiveValues( SMsg ) ) );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ) );
    s->SendCMD2( "Init", "System", "flgon", Driver );
    connect( s, SIGNAL( AnsGetMCA( SMsg ) ), this, SLOT( ReactGetMCA( SMsg ) ) );
  }
  //            PM  PZ CNT PAM ENC SSD SSDP      // SSDP(Xmap)だけ
  if ( TypeCHK(  0,  0,  0,  0,  0,  0, 1 ) ) {
    connect( s, SIGNAL( AnsIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ) );
    connect( s, SIGNAL( EvIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ) );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ) );
    connect( s, SIGNAL( AnsSetPresetValue( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    s->SendCMD2( "Init", "System", "flgon", DevCh );
    connect( s, SIGNAL( AnsGetMCA( SMsg ) ), this, SLOT( ReactGetMCA( SMsg ) ) );
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
  bool rv = false;

  //                PZ                          // この2つの GetValue まだ対応してない
  //            PM  PZ CNT PAM ENC SSD SSDP
  if ( TypeCHK(  1,  0,  1,  0,  1,  0,   1 ) ) {
    isBusy2 = true;
    s->SendCMD2( UID, DevCh, QString( "GetValue" ) );
  }
  //            PM  PZ CNT PAM ENC SSD SSDP
  if ( TypeCHK(  0,  0,  0,  0,  0,  1,   0 ) ) {
    isBusy2 = true;
    s->SendCMD2( UID, Driver, "GetValues" );
  }
  //            PM  PZ CNT PAM ENC SSD SSDP
  if ( TypeCHK(  0,  0,  0,  1,  0,  0,   0 ) ) {
    isBusy2 = true;
    s->SendCMD2( UID, DevCh, "Read" );
    rv = false;
  }

  return rv;
}

bool AUnit::GetValue0( void )
{
  bool rv = false;

  //            PM  PZ CNT PAM ENC SSD SSDP
  if ( TypeCHK(  0,  0,  1,  0,  0,  0,   0 ) ) {
    switch( LocalStage ) {
    case 0:
      isBusy2 = true;
      s->SendCMD2( UID, Driver, "CounterReset" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      isBusy2 = true;
      isBusy = true;
      s->SendCMD2( UID, Driver, "CountStart" );
      LocalStage++;
      rv = false;
      break;
    }
  }
  //            PM  PZ CNT PAM ENC SSD SSDP     SSDP ではなにもしない
  // 　　　　　　                               (SSDP の時も代表して SSD を呼ぶ)
  if ( TypeCHK(  0,  0,  0,  0,  0,  1,   0 ) ) {
    switch( LocalStage ) {
    case 0:
      isBusy2 = true;
      isBusy = true;
      s->SendCMD2( UID, Driver, "RunStart" );
      rv = true;
      LocalStage++;
      break;
    case 1:
      isBusy2 = true;
      s->SendCMD2( UID, Driver, "RunStop" );
      rv = false;
      LocalStage++;
      break;
    }
  }

  return rv;
}

void AUnit::SetValue( double v )
{
  //  isBusy2 = true;    // setvalue に対する応答は無視するので isBusy2 もセットしない
  if ( Type == "PM" ) {
    s->SendCMD2( UID, DevCh, "SetValue", QString::number( lastSetV = (int)v ) );
  }
}

void AUnit::SetSpeed( MSPEED i )
{
  QString cmd = "SpeedLow";

  //  isBusy2 = true;    // setvalue に対する応答は無視するので isBusy2 もセットしない

  if ( Type == "PM" ) {
    switch( i ) {
    case LOW: cmd = "SpeedLow"; break;
    case MIDDLE: cmd = "SpeedMiddle"; break;
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
  if ( Type == "SSD" ) {                 // SSDP では訊かない
    s->SendCMD2( UID, Driver, "IsBusy" );
  }
}

void AUnit::ReceiveValues( SMsg msg )
{
  QString buf;

  if ( ( msg.From() == Driver ) && ( msg.Msgt() == GETVALUES ) ) {
    Value = msg.Vals().at(0);
    Values = msg.Vals();
    emit newValue( Value );
    isBusy2 = false;
  }
}

void AUnit::SetCurPos( SMsg msg )
{
  QString buf;

  if ( ( msg.From() == DevCh )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE ) 
	    || ( msg.Msgt() == READ ) ) ) {
    Value = msg.Val();
    Values = msg.Vals();
    emit newValue( Value );
    isBusy2 = false;
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

  if ( Type == "SSD" ) {
    if ( ( msg.From() == Driver )
	 && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
      isBusy = ( msg.Val().toInt() == 1 );
    }
  }
}

void AUnit::ClrBusy( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {
    isBusy2 = false;
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
  
  if (( Type == "SSD" )||( Type == "SSDP" )) {
    isBusy2 = true;
    s->SendCMD2( UID, DevCh, "SetPresetValue", QString::number( dtime ) );
  }
  if ( Type == "PAM" ) {
    isBusy2 = true;
    // 1 sec -> 1/60 sec
    time = dtime * 60;
    if ( time < 1 ) time = 1;
    if ( time > 40 ) time = 40;
    s->SendCMD2( UID, DevCh, "SetNPLCycles", QString::number( time ) );
  }
  if ( Type == "CNT" ) {
    isBusy2 = true;
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
      isBusy2 = true;
      s->SendCMD2( "Scan", DevCh, "Reset", "" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      isBusy2 = true;
      s->SendCMD2( "Scan", DevCh, "SetAutoRangeEnable", "1" );
      LocalStage++;
      rv = true;
      break;
    case 2:
      isBusy2 = true;
      s->SendCMD2( "Scan", DevCh, "SetDataFormatElements", "READ" );
      LocalStage++;
      rv = true;
      break;
    case 3:
      isBusy2 = true;
      s->SendCMD2( "Scan", DevCh, "SetZeroCheckEnable", "0" );
      rv = false;
      LocalStage++;
      break;
    default:
      rv = false;
    }
  }
  if ( Type == "SSD" ) {
    qDebug() << "LocalStage SSD " << LocalStage;
    switch( LocalStage ) {
    case 0:
      isBusy2 = true;
      s->SendCMD2( "Init", Driver, "RunStop" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      isBusy2 = true;
      s->SendCMD2( "Init", Driver, "SetPresetType", "REAL" );
      LocalStage++;
      rv = false;
      break;
    }
  }

  return rv;
}

void AUnit::ReactGetMCA( SMsg msg )
{
  qDebug() << msg.Msg();
}
