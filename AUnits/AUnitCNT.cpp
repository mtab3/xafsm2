
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

bool AUnitCNT::InitSensor( void )
{
  return _InitSensor();
}

bool AUnitCNT::_InitSensor( void )
{
  return false;
}

bool AUnitCNT2::_InitSensor( void )
{
  if ( The2ndDev == NULL ) {
    qDebug() << "InitSensor: the 2nd Dev is not defined for" << Uid << Name;
    return false;
  }

  bool rv;
  // CNT2, OTC2 のとき カウンタの向こうにつながるのは
  // keithley なのでそれ用の処理をしておく
  QString Type2 = The2ndDev->type();
  switch( LocalStage ) {
  case 0:
    IsBusy2On( Dev2, "InitSensor-c0" );
    s->SendCMD2( "Scan", DevCh2, "Reset", "" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    IsBusy2On( Dev2, "InitSensor-c1" );
    if ( autoRange ) {
      if ( Type2 == "PAM" )
	s->SendCMD2( "Scan", DevCh2, "SetAutoRangeEnable", "1" );
      if ( Type2 == "PAM2" )
	s->SendCMD2( "Scan", Dev2, "SetAutoRangeEnable " + Ch2, "1" );
      LocalStage = 3;
    } else {
      if ( Type2 == "PAM" )
	s->SendCMD2( "Scan", DevCh2, "SetAutoRangeEnable", "0" );
      if ( Type2 == "PAM2" )
	s->SendCMD2( "Scan", Dev2, "SetAutoRangeEnable " + Ch2, "0" );
      LocalStage = 2;
    }
    rv = true;
    break;
  case 2:
    IsBusy2On( Dev2, "InitSensor-c2" );
    if ( Type2 == "PAM" ) {
      s->SendCMD2( "Scan", DevCh2, "SetRange", QString( "2E%1" ).arg( SelectedRange ) );
      LocalStage++;
      rv = true;
    }
    if ( Type2 == "PAM2" ) {
      s->SendCMD2( "Scan", Dev2, "SetRange " + Ch2,
		   QString( "2E%1" ).arg( SelectedRange ) );
      LocalStage+=2;     // PAM2 の時は、LocalStage == 3 をとばす
      rv = false;
    }
    break;
  case 3:
    IsBusy2On( Dev2, "InitSensor-c3" );
    s->SendCMD2( "Scan", DevCh2, "SetZeroCheckEnable", "0" );
    rv = false;
    LocalStage++;
    break;
  default:
    rv = false;
  }

  return rv;
}

void AUnitCNT::AskIsBusy( void )
{
  _AskIsBusy();   // CNT だけ反応して CNT2 は反応しない
}

void AUnitCNT::_AskIsBusy( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}

void AUnitCNT::SetIsBusyByMsg( SMsg msg )
{
  if ( ( msg.From() == Dev )   // Check !!!!! DevCh/Drv
       && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
    IsBusy = ( msg.Val().toInt() == 1 );
    if ( IsBusy )
      LastFunc = "SetIsBusyByMsg";
    else
      LastFunc = "";
    emit ChangedIsBusy1( Dev );
  }
}

bool AUnitCNT::GetValue0( void )  // 値読み出しコマンドの前に何か必要なタイプの場合
{
  bool rv = false;

  switch( LocalStage ) {
  case 0:
    IsBusy2On( Dev, "GetValue0c0" );
    s->SendCMD2( Uid, Dev, "CounterReset" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    IsBusy2On( Dev, "GetValue0c1" );
    IsBusy = true;
    LastFunc = "GetValue0c1";
    emit ChangedIsBusy1( Dev );
    s->SendCMD2( Uid, Dev, "CountStart" );
    LocalStage++;
    rv = false;
    break;
  }

  return rv;
}

// 値読み出しコマンドの前に何か必要なタイプの場合
// 別バージョン、presetTime 等の終了条件無しにしてある
// 連続スキャン (差分で値を見る)モード用
bool AUnitCNT::GetValue02( void )
{
  bool rv = false;

  switch( LocalStage ) {
  case 0:
    IsBusy2On( Dev, "GetValue0c0" );
    s->SendCMD2( Uid, Dev, "SetStopMode", "N" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    IsBusy2On( Dev, "GetValue0c1" );
    s->SendCMD2( Uid, Dev, "CounterReset" );
    LocalStage++;
    rv = true;
    break;
  case 2:
    IsBusy2On( Dev, "GetValue0c2" );
    IsBusy = true;
    LastFunc = "GetValue0c1";
    emit ChangedIsBusy1( Dev );
    s->SendCMD2( Uid, Dev, "CountStart" );
    LocalStage++;
    rv = false;
    break;
  }

  return rv;
}

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

double AUnitCNT::SetTime( double dtime ) // in sec // この関数は、複数ステップ化できない
{
  long int ltime;

  IsBusy2On( Dev, "SetTime" );
  ltime = dtime * 1e6;
  s->SendCMD2( Uid, Dev, "SetTimerPreset", QString::number( ltime ) );
  setTime = dtime;
  
  return setTime;
}

void AUnitCNT2::SetRange( int range )
{
  if ( The2ndDev == NULL ) {
    qDebug() << "SetRange: the 2nd Dev is not defined for" << Uid << Name;
    return;
  }

  IsBusy2On( Dev2, "SetRange" );
  // CNT2, OTC2 のとき カウンタの向こうにつながるのは
  // keithley ( PAM/PAM2 )なのでそれ用の処理をしておく
  QString Type2 = The2ndDev->type();
  if ( Type2 == "PAM" ) {
    s->SendCMD2( "Scan", DevCh2, "SetRange", QString( "2E%1" ).arg( range ) );
  }
  if ( Type2 == "PAM2" ) {
    s->SendCMD2( "Scan", Dev2, "SetRange " + Ch2,
		 QString( "2E%1" ).arg( range ) );
  }
  SelectedRange = range;
}
