#include <QtGui>

#include "AUnit.h"

AUnit::AUnit( QObject *parent ) : QObject( parent )
{
  Enable = false;
  Type = "";
  ID = "";
  Uid = "";
  Name = "";
  Driver = "";
  Ch = "";
  DevCh = "";
  Unit = "";
  UPP = 0;
  IsInt = false;
  HasParent = false;
  theParent = NULL;
  for ( int i = 0; i < 20; i++ ) {
    MCARealTime[i] = MCALiveTime[i] = 0;
    SSDUsingCh[i] = true;
  }
  
  Center = 0;        // only for PM

  MaxV = 0;          // only for PZ
  MinV = 0;          // only for PZ

  SSDPresetType = "REAL";

  IsBusy = false;    // 相手に尋ねる isBusy
  IsBusy2 = false;   // その他のコマンドを投げて返答が返ってくるまで isBusy2
  Value = "";

  lastSetV = 0;

  LocalStage = 0;
}

void AUnit::setEnable( bool enable )
{
  Enable = enable;
  IsBusy = IsBusy2 = false;
  emit Enabled( Driver, enable );
  emit ChangedIsBusy1( Driver );
  emit ChangedIsBusy2( Driver );
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
// (全部のユニットに必要な処理をしていて、必要でないことはしていないことを
//  なんとか頑張って明確にする)
{
  s = S;

  connect( s, SIGNAL( ReceiveError( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
  // 何らかのコマンドに対する応答がエラーだった場合の対処。
  // 単に、isBusy2 をクリアしているだけ。
  // こんなに単純でいいかどうかは難しいところだけれど、
  // enable をちゃんと管理するようにしたので、変な処理に突入することはそちらで避けて
  // 変な処理に突入してしまった場合は、緊急避難的にこの方法で逃げることにする。

  // 現状ある unit は
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
    connect( s, SIGNAL( AnsSetROIs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsGetStatistics( SMsg ) ), this, SLOT( ReactGetStat( SMsg ) ) );
    connect( s, SIGNAL( AnsGetRealTime( SMsg ) ), this, SLOT( ReactGetRealTime( SMsg ) ) );
    connect( s, SIGNAL( AnsGetLiveTime( SMsg ) ), this, SLOT( ReactGetLiveTime( SMsg ) ) );
    connect( s, SIGNAL( AnsGetMCA( SMsg ) ), this, SLOT( ReactGetMCA( SMsg ) ) );
    s->SendCMD2( "Init", "System", "flgon", Driver );
    s->SendCMD2( "Init", Driver, "RunStop" );
  }
  //            PM  PZ CNT PAM ENC SSD SSDP      // SSDP(Xmap)だけ
  if ( TypeCHK(  0,  0,  0,  0,  0,  0, 1 ) ) {
    connect( s, SIGNAL( AnsIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ) );
    connect( s, SIGNAL( EvIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ) );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ) );
    connect( s, SIGNAL( AnsSetPresetValue( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsSetROIs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ) );
    connect( s, SIGNAL( AnsGetMCA( SMsg ) ), this, SLOT( ReactGetMCA( SMsg ) ) );
    s->SendCMD2( "Init", "System", "flgon", DevCh );
  }

  // 以下 Unit タイプではなくて、特定のユニットに固有の処理
  // MMainTh   : "THETA"
  // SI0       : "I0"
  // SI1       : "I1"
  // SFluo     : "TotalF"
  // EncMainTh : "ENCTH"
  if ( ID == "THETA" ) {
    AskIsBusy();
    GetValue();
  }
  if ( ID == "TotalF" ) {
    s->SendCMD2( "SetUpMCA", getDriver(), "GetMCALength" );
  }
  if ( ID == "ENCTH" ) {
    GetValue();
  }

  emit ChangedIsBusy1( Driver );
  emit ChangedIsBusy2( Driver );
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
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, DevCh, QString( "GetValue" ) );
  }
  //            PM  PZ CNT PAM ENC SSD SSDP
  if ( TypeCHK(  0,  0,  0,  0,  0,  1,   0 ) ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, Driver, "GetValues" );
  }
  //            PM  PZ CNT PAM ENC SSD SSDP
  if ( TypeCHK(  0,  0,  0,  1,  0,  0,   0 ) ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, DevCh, "Read" );
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
      IsBusy2 = true;
      emit ChangedIsBusy2( Driver );
      s->SendCMD2( Uid, Driver, "CounterReset" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2 = true;
      IsBusy = true;
      emit ChangedIsBusy1( Driver );
      emit ChangedIsBusy2( Driver );
      s->SendCMD2( Uid, Driver, "CountStart" );
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
      IsBusy2 = true;
      emit ChangedIsBusy2( Driver );
      s->SendCMD2( Uid, Driver, "RunStop" );
      rv = true;
      LocalStage++;
      break;
    case 1:
      IsBusy2 = true;
      IsBusy = true;
      emit ChangedIsBusy1( Driver );
      emit ChangedIsBusy2( Driver );
      s->SendCMD2( Uid, Driver, "RunStart" );
      rv = false;
      LocalStage++;
      break;
    }
  }

  return rv;
}

void AUnit::RunStart( void )
{
  if ( Type == "SSD" ) {
    s->SendCMD2( Uid, Driver, "RunStart" );
  }
}

void AUnit::RunStop( void )
{
  if ( Type == "SSD" ) {
    s->SendCMD2( Uid, Driver, "RunStop" );
  }
}

void AUnit::RunResume( void )
{
  if ( Type == "SSD" ) {
    s->SendCMD2( Uid, Driver, "Resume" );
  }
}

void AUnit::SetValue( double v )
{
  //  IsBusy2 = true;    // setvalue に対する応答は無視するので isBusy2 もセットしない
  if ( Type == "PM" ) {
    s->SendCMD2( Uid, DevCh, "SetValue", QString::number( lastSetV = (int)v ) );
  }
}

void AUnit::SetSpeed( MSPEED i )
{
  QString cmd = "SpeedLow";

  //  IsBusy2 = true;    // setvalue に対する応答は無視するので isBusy2 もセットしない

  if ( Type == "PM" ) {
    switch( i ) {
    case LOW: cmd = "SpeedLow"; break;
    case MIDDLE: cmd = "SpeedMiddle"; break;
    case HIGH: cmd = "SpeedHigh"; break;
    default: cmd = "SpeedLow"; break;
    }
    s->SendCMD2( Uid, DevCh, cmd );
  }
}

void AUnit::AskIsBusy( void )
{
  if ( Type == "PM" ) {
    s->SendCMD2( Uid, DevCh, "IsBusy" );
  }
  if ( Type == "CNT" ) {
    s->SendCMD2( Uid, Driver, "IsBusy" );
  }
  if ( Type == "SSD" ) {                 // SSDP では訊かない
    s->SendCMD2( Uid, Driver, "IsBusy" );
  }
}

void AUnit::ReceiveValues( SMsg msg )
{
  QString buf;

  if ( ( msg.From() == Driver ) && ( msg.Msgt() == GETVALUES ) ) {
    if ( Type == "SSD" ) {
      int sum = 0;
      for ( int i = 0; i < 19; i++ ) {
	if ( SSDUsingCh[i] ) {
	  sum += msg.Vals().at( i + 1 ).toInt();
	}
      }
      Value = QString::number( sum );
    } else {
      Value = msg.Vals().at(0);
    }
    Values = msg.Vals();
    emit newValue( Value );
    IsBusy2 = false;
    emit ChangedIsBusy2( Driver );
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
    IsBusy2 = false;
    emit ChangedIsBusy2( Driver );
  }
}

void AUnit::SetIsBusyByMsg( SMsg msg )
{
  if ( Type == "PM" ) {
    if ( ( msg.From() == DevCh )
	 && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
      IsBusy = ( msg.Val().toInt() == 1 );
      emit ChangedIsBusy1( Driver );
    }
  }

  if ( Type == "CNT" ) {
    if ( ( msg.From() == Driver )
	 && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
      IsBusy = ( msg.Val().toInt() == 1 );
      emit ChangedIsBusy1( Driver );
    }
  }

  if ( Type == "SSD" ) {
    if ( ( msg.From() == Driver )
	 && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
      IsBusy = ( msg.Val().toInt() == 1 );
      emit ChangedIsBusy1( Driver );
    }
  }
}

void AUnit::ClrBusy( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {
    IsBusy2 = false;
    emit ChangedIsBusy2( Driver );
  }
}

void AUnit::Stop( void )
{
  if ( Type == "PM" ) {
    s->SendCMD2( Uid, DevCh, "Stop" );
  }
}

void AUnit::SetTime( double dtime )   // in sec
{
  double time;
  long int ltime;
  
  if (( Type == "SSD" )||( Type == "SSDP" )) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, DevCh, "SetPresetValue", QString::number( dtime ) );
  }
  if ( Type == "PAM" ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    // 1 sec -> 1/60 sec
    time = dtime * 60;
    if ( time < 1 ) time = 1;
    if ( time > 40 ) time = 40;
    s->SendCMD2( Uid, DevCh, "SetNPLCycles", QString::number( time ) );
  }
  if ( Type == "CNT" ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    ltime = dtime * 1e6;
    s->SendCMD2( Uid, Driver, "SetTimerPreset", QString::number( ltime ) );
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
      IsBusy2 = true;
      emit ChangedIsBusy2( Driver );
      s->SendCMD2( "Scan", DevCh, "Reset", "" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2 = true;
      emit ChangedIsBusy2( Driver );
      s->SendCMD2( "Scan", DevCh, "SetAutoRangeEnable", "1" );
      LocalStage++;
      rv = true;
      break;
    case 2:
      IsBusy2 = true;
      emit ChangedIsBusy2( Driver );
      s->SendCMD2( "Scan", DevCh, "SetDataFormatElements", "READ" );
      LocalStage++;
      rv = true;
      break;
    case 3:
      IsBusy2 = true;
      emit ChangedIsBusy2( Driver );
      s->SendCMD2( "Scan", DevCh, "SetZeroCheckEnable", "0" );
      rv = false;
      LocalStage++;
      break;
    default:
      rv = false;
    }
  }
  if ( Type == "SSD" ) {
    QString ROIs = "";
    switch( LocalStage ) {
    case 0:
      IsBusy2 = true;
      emit ChangedIsBusy2( Driver );
      s->SendCMD2( "Init", Driver, "RunStop" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2 = true;
      emit ChangedIsBusy2( Driver );
      s->SendCMD2( "Init", Driver, "SetPresetType", SSDPresetType );
      LocalStage++;
      rv = true;
      break;
    case 2:
      IsBusy2 = true;
      emit ChangedIsBusy2( Driver );
      ROIs = ROIStart[0] + " " + ROIEnd[0];
      for ( int i = 1; i < 19; i++ ) {
	ROIs += " " + ROIStart[i] + " " + ROIEnd[i];
      }
      s->SendCMD2( "Init", Driver, "SetROIs", ROIs );
      LocalStage++;
      rv = false;
      break;
    }
  }

  return rv;
}

bool AUnit::GetMCA( int ch )
{
  bool rv;

  if ( Type == "SSD" ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, Driver, "GetMCA", QString::number( ch ) );
    rv = false;
  }

  return rv;
}

void AUnit::ReactGetMCA( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {
    IsBusy2 = false;
    emit ChangedIsBusy2( Driver );
    MCAValues = msg.Vals();
  }
}

bool AUnit::GetStat( void )
{
  bool rv;

  if ( Type == "SSD" ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, Driver, "GetStatistics" );
    rv = false;
  }

  return rv;
}

void AUnit::ReactGetStat( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {
    IsBusy2 = false;
    emit ChangedIsBusy2( Driver );
    MCAStats = msg.Vals();
  }
}

double AUnit::stat( int ch, STATELM i )
{
  double rv = 0;

  if ( Type == "SSD" ) {
    if ( MCAStats.count() >= 140 ) {
      rv = MCAStats.at( ch * 7 + (int)i ).toDouble();
    }
  }

  return rv;
}

double AUnit::stat( STATELM i )
{
  double rv = 0;

  if ( Type == "SSDP" ) {
    rv = theParent->stat( Ch.toInt(), i );
  }

  return rv;
}

bool AUnit::SetRealTime( double val )
{
  bool rv;

  if ( Type == "SSDP" ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, DevCh, "SetRealTime", QString::number( val ) );
    rv = false;
  }

  return rv;
}

bool AUnit::SetRealTime( int ch, double val )
{
  bool rv;

  if ( Type == "SSD" ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, Driver, "SetRealTime",
		 QString::number( ch ) + " " + QString::number( val ) );
    rv = false;
  }

  return rv;
}

bool AUnit::GetRealTime( int ch )
{
  bool rv;

  if ( Type == "SSD" ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, Driver, "GetRealTime", QString::number( ch ) );
    rv = false;
  }

  return rv;
}

void AUnit::ReactGetRealTime( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {
    IsBusy2 = false;
    emit ChangedIsBusy2( Driver );
    MCARealTime[ msg.Vals().at(0).toInt() ] = msg.Vals().at(1).toDouble();
  }
}

double AUnit::realTime( int ch )
{
  double rv = 0;

  if ( Type == "SSD" ) {
    rv = MCARealTime[ ch ];
  }

  return rv;
}

bool AUnit::SetLiveTime( double val )
{
  bool rv;

  if ( Type == "SSDP" ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, DevCh, "SetLiveTime", QString::number( val ) );
    rv = false;
  }

  return rv;
}

bool AUnit::SetLiveTime( int ch, double val )
{
  bool rv;

  if ( Type == "SSD" ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, Driver, "SetLiveTime",
		 QString::number( ch ) + " " + QString::number( val ) );
    rv = false;
  }

  return rv;
}

bool AUnit::GetLiveTime( int ch )
{
  bool rv;

  if ( Type == "SSD" ) {
    IsBusy2 = true;
    emit ChangedIsBusy2( Driver );
    s->SendCMD2( Uid, Driver, "GetLiveTime", QString::number( ch ) );
    rv = false;
  }

  return rv;
}

void AUnit::ReactGetLiveTime( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {
    IsBusy2 = false;
    emit ChangedIsBusy2( Driver );
    MCALiveTime[ msg.Vals().at(0).toInt() ] = msg.Vals().at(1).toDouble();
  }
}

double AUnit::liveTime( int ch )
{
  double rv = 0;

  if ( Type == "SSD" ) {
    rv = MCALiveTime[ ch ];
  }

  return rv;
}
