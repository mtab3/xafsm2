
#include "math.h"

#include "AUnit.h"

AUnit::AUnit( QObject *parent ) : QObject( parent )
{
  Enable = false;
  autoRange = false;
  Type = "";
  ID = "";
  Uid = "";
  Name = "";
  Driver = "";
  Ch = "";
  DevCh = "";
  Unit = "";
  Dark = 0;
  UPP = 0;
  points = 0;
  IsInt = false;
  HasParent = false;
  theParent = NULL;
  for ( int i = 0; i < MaxSSDs; i++ ) {
    MCARealTime[i] = MCALiveTime[i] = 0;
    SSDUsingCh[i] = true;
  }
  Has2ndDriver = false;
  RangeSelectable = false;
  SelectedRange = 0;
  setTime = 1;
  HasMaxIntTime = false;
  MaxIntTime = 1000000;   // 十分大きい
  HaveSetMaxS = false;
  
  Center = 0;        // only for PM

  MaxV = 0;          // only for PZ
  MinV = 0;          // only for PZ

  SSDPresetType = "REAL";   // for MCA/SSD
  connectingDLink = false;
  dLink = NULL;
  dLinkStream = NULL;
  MCAs0 = NULL;
  MCAs = NULL;

  HasSpeedsLine = false;
  MaxS = 1000;      // max speed (pps) 最初に設定されていたオリジナルのスピード
  MaxMaxS = 3000;   // 許される最高のスピード
  AccRate = 100;    // 加減速レート ( AccRage ms/1000pps )
  AccRateNo = 24;   // 対応する加減速レートの PM16C のテーブル番号

  CountsInROI.clear();
  CountsAll.clear();
  TotalEvents.clear();
  ICRs.clear();
  DarkCountsInROI.clear();
  DarkCountsAll.clear();
  DarkTotalEvents.clear();
  DarkICRs.clear();
  for ( int i = 0; i < MaxSSDs; i++ ) {
    CountsInROI << 0;
    CountsAll << 0;
    TotalEvents << 0;
    ICRs << 0;
    DarkCountsInROI << 0;
    DarkCountsAll << 0;
    DarkTotalEvents << 0;
    DarkICRs << 0;
  }
  DataLinkHostName = "";
  DataLinkHostPort = 0;

  LastFunc = "";
  LastFunc2 = "";
  IsBusy = false;         // 相手に尋ねる isBusy
  IsBusy2 = false;
  IsBusy2Off( "" );   // その他のコマンドを投げて返答が返ってくるまで isBusy2
  Value = "";

  ilastSetV = 0;
  dlastSetV = 0;

  LocalStage = 0;
}

void AUnit::setEnable( bool enable )
{
  Enable = enable;
  IsBusy = false;
  LastFunc = "";
  //  ConnectedToSSDServer = false;
  emit Enabled( Driver, enable );
  emit ChangedIsBusy1( Driver );
  IsBusy2Off( "" );
}

bool AUnit::
TypeCHK( int pm, int pz, int cnt, int pam, int enc, int ssd, int ssdp,
	 int cnt2, int sc, int otc, int otc2, int lsr, int dv, int dv2, int enc2,
	 int pam2, int ccg, int aioi, int aioo, int fp23, int epic )
{
  bool rv = false;

  if ( ( Type == "PM"   ) && ( pm   == 1 ) ) rv = true;
  if ( ( Type == "PZ"   ) && ( pz   == 1 ) ) rv = true;
  if ( ( Type == "CNT"  ) && ( cnt  == 1 ) ) rv = true;
  if ( ( Type == "PAM"  ) && ( pam  == 1 ) ) rv = true;
  if ( ( Type == "ENC"  ) && ( enc  == 1 ) ) rv = true;
  if ( ( Type == "SSD"  ) && ( ssd  == 1 ) ) rv = true;
  if ( ( Type == "SSDP" ) && ( ssdp == 1 ) ) rv = true;
  if ( ( Type == "CNT2" ) && ( cnt2 == 1 ) ) rv = true;
  if ( ( Type == "SC"   ) && ( sc   == 1 ) ) rv = true;
  if ( ( Type == "OTC"  ) && ( otc  == 1 ) ) rv = true;
  if ( ( Type == "OTC2" ) && ( otc2 == 1 ) ) rv = true;
  if ( ( Type == "LSR"  ) && ( lsr  == 1 ) ) rv = true;
  if ( ( Type == "DV"   ) && ( dv   == 1 ) ) rv = true;
  if ( ( Type == "DV2"  ) && ( dv2  == 1 ) ) rv = true;
  if ( ( Type == "ENC2" ) && ( enc2 == 1 ) ) rv = true;
  if ( ( Type == "PAM2" ) && ( pam2 == 1 ) ) rv = true;
  if ( ( Type == "CCG"  ) && ( ccg  == 1 ) ) rv = true; 
  if ( ( Type == "AIOi"  ) && ( aioi == 1 ) ) rv = true;
  if ( ( Type == "AIOo"  ) && ( aioo == 1 ) ) rv = true;
  if ( ( Type == "FP23"  ) && ( fp23 == 1 ) ) rv = true;
  if ( ( Type == "EPIC"  ) && ( epic == 1 ) ) rv = true;

  return rv;
}

void AUnit::Initialize( Stars *S )
// ここは重くてもいいので分かりやすく書くべき
// (全部のユニットに必要な処理をしていて、必要でないことはしていないことを
//  なんとか頑張って明確にする)
{
  s = S;

  connect( s, SIGNAL( ReceiveError( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  // 何らかのコマンドに対する応答がエラーだった場合の対処。
  // 単に、isBusy2 をクリアしているだけ。
  // こんなに単純でいいかどうかは難しいところだけれど、
  // enable をちゃんと管理するようにしたので、変な処理に突入することはそちらで避けて
  // 変な処理に突入してしまった場合は、緊急避難的にこの方法で逃げることにする。

  // 現状ある unit は
  //            PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  // SSDP を除く全て
  // (PAM2 は実際には SetCurPos は使えない (GetValue を Driver で呼ぶので))
  if ( TypeCHK(  1,  1,  1,  1,  1,  0,  0,   1,  1,  1,  1,  0,  1, 1,   1,  1,   1,   1,   1,   1,   1 ) ) {
    connect( s, SIGNAL( AnsIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( EvIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ),this, SLOT( SetCurPos( SMsg ) ),
	     Qt::UniqueConnection );
    s->SendCMD2( "Init", "System", "flgon", DevCh );
    s->SendCMD2( "Init", "System", "flgon", Driver );
    // flgon を DevCh と Driver  両方にかけると二重になるかもしれないが
    // 悪いことは起こらないはずなので安全側に振っておく
  }

  //            PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  // 駆動系だけ(PZ は外した)
  if ( TypeCHK(  1,  0,  0,  0,  1,  0,  0,   0,  1,  0,  0,  0,  0,  0,  0,  0,   0,   0,   1,   0,   0 ) ) {
    connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	     Qt::UniqueConnection );
    // SetValue は Ok: でも Er: でも無視する。
    // connect( s, SIGNAL( AnsSetValue( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
    //          Qt::UniqueConnection );
    s->SendCMD2( "Init", DevCh, "GetValue" );
  }                                                               // 駆動系以外
  if ( TypeCHK(  0,  0,  1,  1,  1,  1,  1,   0,  0,  1,  1,  0,  0,  0,  0,  0,   0,   1,   0,   1,   1 ) ) {
  }

  //            PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                         CNT, OTC は Ch に IsBusy を訊くとエラー
  if ( TypeCHK(  1,  0,  0,  1,  1,  1,  0,   0,  1,  0,  0,  0,  0,  0,  0,  0,   1,   0,   0,   1,   1 ) ) {
    s->SendCMD2( "Init", DevCh, "IsBusy" );
  }
  if ( TypeCHK(  0,  0,  1,  0,  0,  0,  0,   1,  0,  1,  1,  0,  0,  0,  0,  1,   0,   1,   1,   0,   0 ) ) {
    s->SendCMD2( "Init", Driver, "IsBusy" );
  }

  // 以下個別処理
  // 個別処理の判定に TypeCHK を使うのはやめる 2014.12
  //
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                                     カウンタ(nct08)だけ
  //if ( TypeCHK(  0,  0,  1,  0,  0,  0,  0,   1,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0, 0 ) ) {
  if (( Type == "CNT" )||( Type == "CNT2" )) {
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
    s->SendCMD2( "Init", Driver, "SetStopMode", "T" );
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                                     カウンタ(ortec974)だけ
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  1,  1,  0,  0, 0,  0,   0,   0,   0,   0,    0 ) ) {
  if (( Type == "OTC" )||( Type == "OTC2" )) {
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
    s->SendCMD2( "Init", Driver, "Reset" );
    s->SendCMD2( "Init", Driver, "SetMode", "0" );
  }

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                                      PAM, PAM2(Keithley)だけ
  //if ( TypeCHK(  0,  0,  0,  1,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  0,   1,   0,   0,   0,    0 ) ) {
  if (( Type == "PAM2" )||( Type == "PAM" )) {
    connect( s, SIGNAL( AnsRead( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetAutoRange( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetDataFormat( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetZeroCheck( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetNPLCycles( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                                             PAM2(Keithley)だけ
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  0,   1,   0,   0,   0,    0 ) ) {
  if ( Type == "PAM2" ) {
    connect( s, SIGNAL( AnsRead( SMsg ) ),this, SLOT( RcvAnsGetValueOfDriver( SMsg ) ),
	     Qt::UniqueConnection );
  }

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                         CNT2, OCT2 だけ。Keithley対応
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   1,  0,  0,  1,  0,  0, 0,  0,   0,   0,   0,   0,   0 ) ) {
  if (( Type == "CNT2" )||( Type == "OCT2" )) {
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
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                                                  DV2(34410)だけ
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0, 1,  0,   0,   0,   0,   0, 0 ) ) {
  if ( Type == "DV2" ) {
    connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetAperture( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetAutoZero( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    //    s->SendCMD2( "Init", Driver, "Reset" );  // Reset に対する返答は無視
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                                             DV (34410)だけ
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  1, 0,  0,   0,   0,   0,   0, 0 ) ) {
  if ( Type == "DV" ) {
    connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsQInitialize( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsQGetData( SMsg ) ), this, SLOT( RcvQGetData( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsQFinalize( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    //    s->SendCMD2( "Init", Driver, "Reset" );  // Reset に対する返答は無視
  }

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                                           SSD(Xmap)だけ
  //if ( TypeCHK(  0,  0,  0,  0,  0,  1,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0, 0 ) ) {
  if ( Type == "SSD" ) {
    connect( s, SIGNAL( AnsIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( EvIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetPresetType( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetPresetValue( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsRunStart( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsRunStop( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsGetValues( SMsg ) ), this, SLOT( ReceiveValues( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetROIs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsGetStatistics( SMsg ) ), this, SLOT( ReactGetStat( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsGetRealTime( SMsg )), this, SLOT( ReactGetRealTime( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsGetLiveTime( SMsg )), this, SLOT( ReactGetLiveTime( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsGetDataLinkCh( SMsg ) ),
	     this, SLOT( ReactGetDataLinkCh( SMsg ) ),
	     Qt::UniqueConnection );
    s->SendCMD2( "Init", "System", "flgon", Driver );
    s->SendCMD2( "Init", Driver, "RunStop" );
    connectingDLink = false;
    s->SendCMD2( "Init", Driver, "GetDataLinkCh" );
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                                          SSDP(Xmap)だけ
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  1,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0, 0 ) ) {
  if ( Type == "SSDP" ) {
    connect( s, SIGNAL( AnsIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( EvIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsGetValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetPresetValue( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsSetROIs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    s->SendCMD2( "Init", "System", "flgon", DevCh );
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
                                                             // LSR だけ
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  1,  0, 0,  0,   0,   0,   0,   0,  0 ) ) {
  if ( Type == "LSR" ) {
    connect( s, SIGNAL(EvReportCurrent( SMsg )), this, SLOT(OnReportCurrent( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(EvReportInjection( SMsg )), this, SLOT(OnReportInjection( SMsg )),
	     Qt::UniqueConnection );
    s->SendCMD2( "Init", Driver, "flgon", Ch );
    s->SendCMD2( "Init", "System", "flgon", Driver );
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
                                                              // PM だけ
  //if ( TypeCHK(  1,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0, 0 ) ) {
  if ( Type == "PM" ) {
    connect( s, SIGNAL(AnsSetHighSpeed( SMsg )), this, SLOT(ClrBusy( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsSetMiddleSpeed( SMsg )), this, SLOT(ClrBusy( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsSetLowSpeed( SMsg )), this, SLOT(ClrBusy( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsSetTimingOutMode( SMsg )), this, SLOT(ClrBusy( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsSetTimingOutStart( SMsg )), this, SLOT(ClrBusy( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsSetTimingOutEnd( SMsg )), this, SLOT(ClrBusy( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsSetTimingOutInterval( SMsg )), this, SLOT(ClrBusy( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsSetTimingOutReady( SMsg )), this, SLOT(ClrBusy( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsSelect( SMsg )), this, SLOT(ClrBusy( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsGetHighSpeed( SMsg )), this, SLOT(RcvHighSpeed( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsGetMiddleSpeed( SMsg )), this, SLOT(RcvMiddleSpeed( SMsg )),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsGetLowSpeed( SMsg )), this, SLOT(RcvLowSpeed( SMsg )),
	     Qt::UniqueConnection );
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                                                  ENC2 だけ
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  1,   0,   0,   0,  0, 0 ) ) {
  if ( Type == "ENC2" ) {
    connect( s, SIGNAL(AnsGetStat( SMsg )), this, SLOT( RcvStat( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsTrigger( SMsg )), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsStandBy( SMsg )), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL(AnsGetData( SMsg )), this, SLOT( RcvQGetData( SMsg ) ),
	     Qt::UniqueConnection );
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //                                                                             PZだけ
  //if ( TypeCHK(  0,  1,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,  0,   0, 0 ) ) {
  if ( Type == "PZ" ) {
    connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	     Qt::UniqueConnection );
    s->SendCMD2( "Init", DevCh, "Init" );
  }

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //                                                                             PZ, AIOo だけ
  //if ( TypeCHK(  0,  1,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,  0,   1,   0 ) ) {
  if (( Type == "PZ" )||( Type == "AIOo" )) {
    connect( s, SIGNAL( AnsGoMaxAbs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsGoMaxRel( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    connect( s, SIGNAL( AnsShutterOff( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
    s->SendCMD2( "Init", DevCh, "Init" );
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
                                                             // FP23 だけ
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,  1 ) ) {
  if ( Type == "FP23" ) {
    connect( s, SIGNAL( EvReportValue(SMsg) ), this, SLOT( OnReportValue(SMsg) ),
             Qt::UniqueConnection );
    s->SendCMD2( "Init", Driver, "flgon", Ch );
    s->SendCMD2( "Init", "System", "flgon", Driver );
  }

#if 0
  if ( Type == "AIOi" ) {
    connect( s, SIGNAL( EvRangeMax( SMsg ) ), this, SLOT( RcvAlarmEvent( SMsg ) ), Qt::UniqueConnection );
    connect( s, SIGNAL( EvRangeMaxOff( SMsg ) ), this, SLOT( RcvAlarmEvent( SMsg ) ), Qt::UniqueConnection );
  }
#endif
  
  // 以下 Unit タイプではなくて、特定の用途に固定された特定のユニットに固有の処理
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
    connect( s, SIGNAL( AnsGetMCALength( SMsg ) ), this, SLOT( getMCALength( SMsg ) ),
	     Qt::UniqueConnection );
    s->SendCMD2( "SetUpMCA", getDriver(), "GetMCALength" );
  }
  if ( ID == "ENCTH" ) {
    GetValue();
  }

  emit ChangedIsBusy1( Driver );    // ここの3つのエミットは念の為
  emit ChangedIsBusy2( Driver );
  emit ChangedBusy2Count( Driver );
}

void AUnit::show( void )
{
  qDebug() << tr( " TP[%1] ID[%2] NM[%3] DR[%4] Ch[%5] DC[%6] "
		" UT[%7] PU[%8] CT[%9] MaV[%10] MiV[%11] " )
    .arg( Type )
    .arg( ID ).arg( Name ).arg( Driver ).arg( Ch ).arg( DevCh ).arg( Unit )
    .arg( UPP ).arg( Center ).arg( MaxV ).arg( MinV );
}

bool AUnit::QStart( void )
{
  bool rv = false;

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  1, 0,  0,   0,   0,  0,   0,   0 ) ) {
  if ( Type == "DV" ) {
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "Start" );
      s->SendCMD2( Uid, DevCh, "qInitialize", QString::number( setTime ) );
      LocalStage++;
      rv = false;
      break;
    }
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  1,   0,   0,   0,   0,   0 ) ) {
  if ( Type == "ENC2" ) {
    IsBusy2On( Driver, "Start" );
    s->SendCMD2( Uid, DevCh, "StandBy" );
    rv = false;
  }

  return rv;
}

bool AUnit::QRead( void )
{
  bool rv = false;
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  1, 0,  0,   0,   0,   0,   0,   0 ) ) {
  if ( Type == "DV" ) {
    IsBusy2On( Driver, "Read" );
    s->SendCMD2( Uid, DevCh, "qGetData" );
    rv = false;
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  1,   0,   0,   0,   0,   0 ) ) {
  if ( Type == "ENC2" ) {
    IsBusy2On( Driver, "Read" );
    s->SendCMD2( Uid, DevCh, "GetData" );
    rv = false;
  }

  return rv;
}

bool AUnit::QEnd( void )
{
  bool rv = false;

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  1, 0,  0,   0,   0,   0,   0,  0 ) ) {
  if ( Type == "DV" ) {
    IsBusy2On( Driver, "End" );
    s->SendCMD2( Uid, DevCh, "qFinalize" );
    rv = false;
  }

  return rv;
}

bool AUnit::GetValue( void )
{
  bool rv = false;

  //            PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  if ( TypeCHK(  1,  1,  1,  0,  1,  0,  1,   1,  0,  1,  1,  0,  1, 1,  1,   0,   1,   1,   1,   1,   1 ) ) {
    IsBusy2On( Driver, "GetValue" );
    s->SendCMD2( Uid, DevCh, "GetValue" );
  }
  //               PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  // if ( TypeCHK(  0,  0,  0,  0,  0,  1,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,   0,   0 ) ) {
  if ( Type == "SSD" ) {
    IsBusy2On( Driver, "GetValue" );
    // 変則 : この IsBusy2 は @GetMCAs Ok: を受けても消さない
    //        data-link 経由で完全なデータをもらった時に消す
    //    s->SendCMD2( Uid, Driver, "GetValues" );    // new mcas
    s->SendCMD2( Uid, Driver, "GetMCAs" );
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //if ( TypeCHK(  0,  0,  0,  1,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,   0,   0 ) ) {
  if ( Type == "PAM" ) {
    IsBusy2On( Driver, "GetValue" );
    s->SendCMD2( Uid, DevCh, "Read" );
    rv = false;
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0, 0,  0,   1,   0,   0,   0,   0,   0 ) ) {
  if ( Type == "PAM2" ) {
    IsBusy2On( Driver, "GetValue" );
    s->SendCMD2( Uid, Driver, "Read" );
    rv = false;
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  1,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,   0,   0 ) ) {
  if ( Type == "SC" ) {
    IsBusy2On( Driver, "GetValue" );
    s->SendCMD2( Uid, DevCh, "GetValue 0" );
    rv = false;
  }

  return rv;
}

bool AUnit::GetValue0( void )  // 値読み出しコマンドの前に何か必要なタイプの場合
{
  bool rv = false;

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //if ( TypeCHK(  0,  0,  1,  0,  0,  0,  0,   1,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,   0,   0 ) ) {
  if (( Type == "CNT" )||( Type == "CNT2" )) {
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "GetValue0c0" );
      s->SendCMD2( Uid, Driver, "CounterReset" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2On( Driver, "GetValue0c1" );
      IsBusy = true;
      LastFunc = "GetValue0c1";
      emit ChangedIsBusy1( Driver );
      s->SendCMD2( Uid, Driver, "CountStart" );
      LocalStage++;
      rv = false;
      break;
    }
  }

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //if ( TypeCHK(  0,  0,  0,  0,  0,  0,  0,   0,  0,  1,  1,  0,  0, 0,  0,   0,   0,   0,   0,   0 ) ) {
  if (( Type == "OTC" )||( Type == "OTC2" )) {
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "GetValue0c0" );
      s->SendCMD2( Uid, Driver, "CounterReset" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2On( Driver, "GetValue0c1" );
      IsBusy = true;
      LastFunc = "GetValue0c1";
      emit ChangedIsBusy1( Driver );
      s->SendCMD2( Uid, Driver, "Run" );
      LocalStage++;
      rv = false;
      break;
    }
  }

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //                       SSDP ではなにもしない (SSDP の時も代表して SSD を呼ぶ)
  //if ( TypeCHK(  0,  0,  0,  0,  0,  1,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,  0 ) ) {
  if ( Type == "SSD" ) {
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "GetValue0c0" );
      s->SendCMD2( Uid, Driver, "RunStop" );
      rv = true;
      LocalStage++;
      break;
    case 1:
      IsBusy2On( Driver, "GetValue0c1" );
      IsBusy = true;
      LastFunc = "GetValue0c1";
      emit ChangedIsBusy1( Driver );
      s->SendCMD2( Uid, Driver, "RunStart" );
      rv = false;
      LocalStage++;
      break;
    }
  }

  return rv;
}


// 値読み出しコマンドの前に何か必要なタイプの場合
// 別バージョン、presetTime 等の終了条件無しにしてある
// 連続スキャン (差分で値を見る)モード用
bool AUnit::GetValue02( void )
{
  bool rv = false;

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //if ( TypeCHK(  0,  0,  1,  0,  0,  0,  0,   1,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,  0 ) ) {
  if (( Type == "CNT" )||( Type == "CNT2" )) {
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "GetValue0c0" );
      s->SendCMD2( Uid, Driver, "SetStopMode", "N" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2On( Driver, "GetValue0c1" );
      s->SendCMD2( Uid, Driver, "CounterReset" );
      LocalStage++;
      rv = true;
      break;
    case 2:
      IsBusy2On( Driver, "GetValue0c2" );
      IsBusy = true;
      LastFunc = "GetValue0c1";
      emit ChangedIsBusy1( Driver );
      s->SendCMD2( Uid, Driver, "CountStart" );
      LocalStage++;
      rv = false;
      break;
    }
  }

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //                       SSDP ではなにもしない (SSDP の時も代表して SSD を呼ぶ)
  //if ( TypeCHK(  0,  0,  0,  0,  0,  1,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,   0 ) ) {
  if ( Type == "SSD" ) {
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "GetValue0c0" );
      s->SendCMD2( Uid, Driver, "SetPresetType", "NONE" );
      rv = true;
      LocalStage++;
      break;
    case 1:
      IsBusy2On( Driver, "GetValue0c1" );
      s->SendCMD2( Uid, Driver, "RunStop" );
      rv = true;
      LocalStage++;
      break;
    case 2:
      IsBusy2On( Driver, "GetValue0c2" );
      IsBusy = true;
      LastFunc = "GetValue0c1";
      emit ChangedIsBusy1( Driver );
      s->SendCMD2( Uid, Driver, "RunStart" );
      rv = false;
      LocalStage++;
      break;
    }
  }

  return rv;
}

/* 連続スキャンの後にノーマルモードに戻す */
bool AUnit::Close( void )
{
  bool rv = false;

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //if ( TypeCHK(  0,  0,  1,  0,  0,  0,  0,   1,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,   0 ) ) {
  if (( Type == "CNT" )||( Type == "CNT2" )) {
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "Close0" );
      s->SendCMD2( Uid, Driver, "Stop" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2On( Driver, "Close1" );
      s->SendCMD2( Uid, Driver, "SetStopMode", "T" );
      LocalStage++;
      rv = false;
      break;
    }
  }

  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //                       SSDP ではなにもしない (SSDP の時も代表して SSD を呼ぶ)
  //if ( TypeCHK(  0,  0,  0,  0,  0,  1,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,   0 ) ) {
  if ( Type == "SSD" ) {
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "GetValue0c0" );
      s->SendCMD2( Uid, Driver, "RunStop" );
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
    IsBusy = true;
    emit ChangedIsBusy1( DevCh );
    s->SendCMD2( Uid, DevCh, "SetValue", QString::number( ilastSetV = (int)v ) );
  }
  if ( ( Type == "ENC" ) || ( Type == "AIOo" ) ){
    s->SendCMD2( Uid, DevCh, "SetValue", QString::number( dlastSetV = v ) );
  }
  if ( Type == "SC" ) {
    s->SendCMD2( Uid, DevCh, "SetValue", QString( "%1 1 0 0 0 0" )
		 .arg( ilastSetV = (int)v ) );
  }
}

void AUnit::SetSpeed( MSPEED i )
{
  QString cmd = "SpeedLow";

  //  IsBusy2 = true;    // setspeed に対する応答は無視するので isBusy2 もセットしない

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

void AUnit::SetHighSpeed( int speed )
{
  if ( Type == "PM" ) {
    IsBusy2On( Driver, "SetHighSpeed" );
    QString cmd = QString( "SetHighSpeed %1" ).arg( speed );
    s->SendCMD2( Uid, DevCh, cmd );
  }
}

void AUnit::SetMiddleSpeed( int speed )
{
  if ( Type == "PM" ) {
    IsBusy2On( Driver, "SetMiddleSpeed" );
    QString cmd = QString( "SetMiddleSpeed %1" ).arg( speed );
    s->SendCMD2( Uid, DevCh, cmd );
  }
}

void AUnit::SetLowSpeed( int speed )
{
  if ( Type == "PM" ) {
    IsBusy2On( Driver, "SetLowSpeed" );
    QString cmd = QString( "SetLowSpeed %1" ).arg( speed );
    s->SendCMD2( Uid, DevCh, cmd );
  }
}

void AUnit::AssignDispCh( int ch )
{
  if ( Type == "PM" ) {
    IsBusy2On( Driver, "AssignDispCh" );
    int num = Ch.toInt();
    if ( Ch.left( 2 ).toUpper() == "CH" )
      num = Ch.mid( 2 ).toInt();
    QString cmd = QString( "Select %1 %2" ).arg( QChar( 'A' + ch ) ).arg( num );
    s->SendCMD2( Uid, Driver, cmd );
  }
}

void AUnit::SetTimingOutMode( int mode )
{
  if ( Type == "PM" ) {
    IsBusy2On( Driver, "SetTimingOutMode" );
    QString cmd = QString( "SetTimingOutMode %1" ).arg( mode );
    s->SendCMD2( Uid, DevCh, cmd );
  }
}

void AUnit::SetTimingOutStart( int startP )
{
  if ( Type == "PM" ) {
    IsBusy2On( Driver, "SetTimingOutStart" );
    QString cmd = QString( "SetTimingOutStart %1" ).arg( startP );
    s->SendCMD2( Uid, DevCh, cmd );
  }
}

void AUnit::SetTimingOutEnd( int endP )
{
  if ( Type == "PM" ) {
    IsBusy2On( Driver, "SetTimingOutEnd" );
    QString cmd = QString( "SetTimingOutEnd %1" ).arg( endP );
    s->SendCMD2( Uid, DevCh, cmd );
  }
}

void AUnit::SetTimingOutInterval( int interval )
{
  if ( Type == "PM" ) {
    IsBusy2On( Driver, "SetTimingOutInterval" );
    QString cmd = QString( "SetTimingOutInterval %1" ).arg( interval );
    s->SendCMD2( Uid, DevCh, cmd );
  }
}

void AUnit::SetTimingOutReady( int ready )
{
  if ( Type == "PM" ) {
    IsBusy2On( Driver, "SetTimingOutReady" );
    QString cmd = QString( "SetTimingOutReady %1" ).arg( ready );
    s->SendCMD2( Uid, DevCh, cmd );
  }
}

void AUnit::AskIsBusy( void )
{
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23
  //     AIO は今のところ、個別の ch の IsBusy に対応していない
  //if ( TypeCHK(  1,  0,  0,  0,  0,  0,  0,   0,  1,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,    0 ) ) {
  if (( Type == "PM" )||( Type == "SC" )) {
    s->SendCMD2( Uid, DevCh, "IsBusy" );
  }

  // SSDPには訊かない
  //            PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  if ( TypeCHK(  0,  0,  1,  0,  0,  1,  0,   0,  0,  1,  0,  0,  1, 1,  1,   0,   0,   0,   0,   0,   0 ) ) {
    s->SendCMD2( Uid, DevCh, "IsBusy" );
  }
}

void AUnit::ReceiveValues( SMsg msg )
{
  QString buf;

  CountsInROI.clear();
  CountsAll.clear();
  TotalEvents.clear();
  ICRs.clear();

  if ( ( msg.From() == Driver ) && ( msg.Msgt() == GETVALUES ) ) { // Check !!!!! DevCh/Drv
    if ( Type == "SSD" ) {   // SSD だけ特殊処理。全チャンネルの合計値を取る
      int sum = 0;
      for ( int i = 0; i < MaxSSDs; i++ ) {
	if ( SSDUsingCh[i] ) {
	  sum += msg.Vals().at( i + 1 ).toInt();
	}
      }
      Value = QString::number( sum );
      for ( int i = 0; i < MaxSSDs; i++ ) {
	CountsInROI << msg.Vals().at( i + 1 ).toInt();
	CountsAll   << msg.Vals().at( i + 1 + MaxSSDs ).toInt();
	TotalEvents << msg.Vals().at( i + 1 + MaxSSDs * 2 ).toInt();
	ICRs        << msg.Vals().at( i + 1 + MaxSSDs * 3 ).toDouble();
      }
    } else {
      Value = msg.Vals().at(0);
    }
    Values = msg.Vals();

    emit newValue( Value );
    IsBusy2Off( Driver );
  }
}

void AUnit::RcvAnsGetValueOfDriver( SMsg msg )  // driver 名だけで呼ばれる場合
{
  if ( ( msg.From() == Driver ) && ( msg.Msgt() == READ ) ) {
    if ( Type == "PAM2" ) {
      Values = msg.Val().split( QChar( ',' ) );
      Value = Values.at( Ch.toInt() ); // 親ドライバ宛の返答から自分用の答えを選り分ける
      emit newValue( Value );
      IsBusy2Off( Driver );
    }
  }
}

void AUnit::SetCurPos( SMsg msg )
{
  QString buf;
  
  if ( ( msg.From() == DevCh )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE )
            || ( msg.Msgt() == READ ) ) ) {
    if ( Type == "SC" ) {
      if ( msg.Msgt() == EvCHANGEDVALUE ) {
        Value = msg.Val();
      } else {
        Value = msg.Vals().at(1);
      }
    } else if ( Type == "CCG" ) {
      Value = msg.Vals().at(0);
    } else {
      Value = msg.Val();
#if 0
      if ( Type == "ENC2" )
	qDebug() << "enc2 value " << Value;
      if ( ID == "THETA" )
	qDebug() << "main th " << Value;
#endif
    }
    emit newValue( Value );
    IsBusy2Off( Driver );
  }
}

void AUnit::SetIsBusyByMsg( SMsg msg )
{
  //            PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG　AIOi AIOo FP23 EPIC
  if ( TypeCHK(  1,  0,  0,  0,  0,  0,  0,   0,  1,  0,  0,  0,  1, 1,  1,   0,   0 ,   1,   1,   0,   1 ) ) {
    if ( ( msg.From() == DevCh )
         && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
      IsBusy = ( msg.Val().toInt() == 1 );
      if ( IsBusy )
        LastFunc = "SetIsBusyByMsg";
      else
        LastFunc = "";
      emit ChangedIsBusy1( Driver );
    }
  }
  //            PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  if ( TypeCHK(  0,  0,  1,  0,  0,  0,  0,   1,  0,  1,  1,  0,  0, 0,  0,   0,   0,   1,   1,   0,   0 ) ) {
    if ( ( msg.From() == Driver )   // Check !!!!! DevCh/Drv
         && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
      IsBusy = ( msg.Val().toInt() == 1 );
      if ( IsBusy )
        LastFunc = "SetIsBusyByMsg";
      else
        LastFunc = "";
      emit ChangedIsBusy1( Driver );
    }
  }
  //              PM  PZ CNT PAM ENC SSD SSDP CNT2 SC OTC OTC2 LSR DV DV2 ENC2 PAM2 CCG AIOi AIOo FP23 EPIC
  //if ( TypeCHK(  0,  0,  0,  0,  0,  1,  0,   0,  0,  0,  0,  0,  0, 0,  0,   0,   0,   0,   0,   0,   0 ) ) {
  if ( Type == "SSD" ) {
    if ( ( msg.From() == Driver )    // Check !!!!! DevCh/Drv
         && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
      IsBusy = ( msg.Val().toInt() == 1 );
      if ( IsBusy )
        LastFunc = "SetIsBusyByMsg";
      else
        LastFunc = "";
      emit ChangedIsBusy1( Driver );
    }
  }
}

void AUnit::ClrBusy( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {
    IsBusy2Off( Driver );
  }
  if ( Has2ndDriver ) {
    if ( ( msg.From() == DevCh2 ) || ( msg.From() == Driver2 ) ) {
      IsBusy2Off( Driver2 );
    }
  }
}

void AUnit::Stop( void )
{
  if (( Type == "PM" )||( Type == "SC" )) {
    s->SendCMD2( Uid, DevCh, "Stop" );
  }
}

double AUnit::SetTime( double dtime )   // in sec  // この関数は、複数ステップ化できない
{
  double time;
  long int ltime;
  int M, N;

  if (( Type == "SSD" )||( Type == "SSDP" )) {
    IsBusy2On( Driver, "SetTime" );
    s->SendCMD2( Uid, Driver, "RunStop" );   // コマンド連続発行可能か? いちおういけてる
    s->SendCMD2( Uid, DevCh, "SetPresetValue", QString::number( dtime ) );
    setTime = dtime;
  }
  if (( Type == "PAM" )||( Type == "PAM2" )) {
    IsBusy2On( Driver, "SetTime" );
    // 1 sec -> 1/60 sec
    time = dtime * 60;
    if ( time < 1 ) time = 1;
    if ( time > 40 ) time = 40;
    if ( Type == "PAM" )
      s->SendCMD2( Uid, DevCh, "SetNPLCycles", QString::number( time ) );
    else 
      s->SendCMD2( Uid, Driver, "SetNPLCycles " + Ch, QString::number( time ) );
    setTime = time / 60;    // これで「秒」単位の普通の時間に戻ってる
  }
  if (( Type == "CNT" )||( Type == "CNT2" )) {
    IsBusy2On( Driver, "SetTime" );
    ltime = dtime * 1e6;
    s->SendCMD2( Uid, Driver, "SetTimerPreset", QString::number( ltime ) );
    setTime = dtime;
  }
  if (( Type == "OTC" )||( Type == "OTC2" )) {
    IsBusy2On( Driver, "SetTime" );
    N = log10( dtime * 10 );
    M = ceil( dtime / pow( 10., N - 1 ) );
    s->SendCMD2( Uid, Driver, "SetCountPreset", QString( "%1,%2" ).arg( M ).arg( N ) );
    setTime = M * pow( 10, N ) * 0.1;  // これで秒単位の普通の時間に戻ってる
  }
  if (( Type == "DV" )||( Type == "DV2" )) {
    if ( dtime < 0.0001 ) dtime = 0.0001;
    if ( dtime > 1.0 ) dtime = 1.0;
    if (( HasMaxIntTime )&&( dtime > MaxIntTime )) { dtime = MaxIntTime; };
    if ( Type == "DV2" ) {   // DV の場合、ここでは内部変数 setTime に値を設定するだけ。
      IsBusy2On( Driver, "SetAperture" );
      s->SendCMD2( Uid, DevCh, "SetAperture", QString( "%1" ).arg( dtime ) );
    }
    setTime = dtime;
  }
  if (( Type == "ENC2" )||( Type == "AIOi" )||( Type == "EPIC" )) {
    setTime = dtime;          // setTime できたと見せかけるだけ。
  }

  return setTime;
}

void AUnit::SetTriggerDelay( double time )  // 使っていない
{
  if (( Type == "DV" )||( Type == "DV2" )) {
    IsBusy2On( Driver, "SetTriggerDelay" );
    s->SendCMD2( Uid, DevCh, "SetTriggerDelay", QString::number( time ) );
  }
}

void AUnit::SetSamplingSource( QString source )   // source := TIM, IMM   // 使っていない
{
  if (( Type == "DV" )||( Type == "DV2" )) {
    IsBusy2On( Driver, "SetSamplingSource" );
    s->SendCMD2( Uid, DevCh, "SetSamplingSource", source );
  }
}

void AUnit::SetTriggerSource( QString source )   // source := IMM, EXT, BUS  // 使っていない
{
  if (( Type == "DV" )||( Type == "DV2" )) {
    IsBusy2On( Driver, "SetTriggerSource" );
    s->SendCMD2( Uid, DevCh, "SetTriggerSource", source );
  }
}

void AUnit::SetTriggerCounts( int counts )   
{
  if (( Type == "DV" )||( Type == "DV2" )) {
    IsBusy2On( Driver, "SetTriggerCounts" );

    QString arg;
    if ( counts > 0 )
      arg = QString::number( counts );
    else
      arg = "INF";

    s->SendCMD2( Uid, DevCh, "SetTriggerCounts", arg );
  }
}

void AUnit::SetTriggerSlope( QString type )   // POS : rising-edge
{
  if (( Type == "DV" )||( Type == "DV2" )) {
    IsBusy2On( Driver, "SetTriggerSlope" );
    s->SendCMD2( Uid, DevCh, "SetTriggerSlope", type );
  }
}

void AUnit::GetDataPoints( void )
{
  if (( Type == "DV" )||( Type == "DV2" )) {
    IsBusy2On( Driver, "GetDataPoints" );
    s->SendCMD2( Uid, DevCh, "GetDataPoints" );
  }
}

void AUnit::ReadDataPoints( int datas )
{
  if (( Type == "DV" )||( Type == "DV2" )) {
    IsBusy2On( Driver, "ReadDataPoints" );
    s->SendCMD2( Uid, DevCh, "GetDataPoints", QString::number( datas ) );
  }
}

void AUnit::Abort( void )
{
  if (( Type == "DV" )||( Type == "DV2" )) {
    IsBusy2On( Driver, "Abort" );
    s->SendCMD2( Uid, DevCh, "Abort" );
  }
}

void AUnit::RcvQGetData( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Driver ) )
       && ( ( msg.Msgt() == GETDATAPOINTS )
	    || ( msg.Msgt() == QGETDATA )
	    || ( msg.Msgt() == GETDATA ) ) ) {
    if ( Type == "DV" ) {
      Values = msg.Vals();
      emit newQData();
      IsBusy2Off( Driver );
    }
    if ( Type == "ENC2" ) {
      Values = msg.Vals();
      emit newQData();
      IsBusy2Off( Driver );
    }
  }
}

void AUnit::RcvStat( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Driver ) )  // Check !!!!! DevCh/Drv
       && ( ( msg.Msgt() == GETSTAT ) ) ) {
    if ( Type == "ENC2" ) {
      //      Values = msg.Vals();
      //      emit newQData();
      IsBusy2Off( Driver );
    }
  }
}

void AUnit::AskHighSpeed( void )
{
  if ( Type == "PM" ) {
    s->SendCMD2( "Init", DevCh, "GetHighSpeed" );
  }
}

void AUnit::AskMiddleSpeed( void )
{
  if ( Type == "PM" ) {
    s->SendCMD2( "Init", DevCh, "GetMiddleSpeed" );
  }
}

void AUnit::AskLowSpeed( void )
{
  if ( Type == "PM" ) {
    s->SendCMD2( "Init", DevCh, "GetLowSpeed" );
  }
}

void AUnit::RcvHighSpeed( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Driver ) )  // Check !!!!! DevCh/Drv
       && ( ( msg.Msgt() == GETHIGHSPEED ) ) ) {
    if ( Type == "PM" ) {
      HighS = msg.Val().toInt();
      if ( ! HaveSetMaxS ) {
	MaxS = HighS;
	HaveSetMaxS = true;
      }
      IsBusy2Off( Driver );
      emit gotHighS( HighS );
    }
  }
}

void AUnit::RcvMiddleSpeed( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Driver ) )  // Check !!!!! DevCh/Drv
       && ( ( msg.Msgt() == GETMIDDLESPEED ) ) ) {
    if ( Type == "PM" ) {
      MiddleS = msg.Val().toInt();
      IsBusy2Off( Driver );
      emit gotMiddleS( MiddleS );
    }
  }
}

void AUnit::RcvLowSpeed( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Driver ) )  // Check !!!!! DevCh/Drv
       && ( ( msg.Msgt() == GETLOWSPEED ) ) ) {
    if ( Type == "PM" ) {
      LowS = msg.Val().toInt();
      IsBusy2Off( Driver );
      emit gotLowS( LowS );
    }
  }
}

void AUnit::InitLocalStage( void )
{
  LocalStage = 0;
}

bool AUnit::InitSensor( void )
{
  bool rv = false;

  if (( Type == "PAM" )||( Type == "PAM2" )) { // Keithley 6845
    QString Dev;
    if ( Type == "PAM" ) { Dev = Driver; } else { Dev = Driver; } // !!
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "InitSensor-c0" );
      s->SendCMD2( "Scan", Dev, "Reset", "" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2On( Driver, "InitSensor-c1" );
      if ( Type == "PAM" ) 
	s->SendCMD2( "Scan", Dev, "SetAutoRangeEnable", "1" );
      if ( Type == "PAM2" ) 
	s->SendCMD2( "Scan", Dev, "SetAutoRangeEnable " + Ch, "1" );
      LocalStage++;
      rv = true;
      break;
    case 2:
      IsBusy2On( Driver, "InitSensor-c2" );
      if ( Type == "PAM" )
	s->SendCMD2( "Scan", Dev, "SetDataFormatElements", "READ" );
      if ( Type == "PAM2" )
	s->SendCMD2( "Scan", Dev, "SetDataFormatElements", "CURR1,CURR2" );

      if ( Type == "PAM" ) {
	LocalStage++;
	rv = true;
      } else {
	rv = false;      // PAM2 は ZeroCheck の設定ないらしい
	LocalStage = 4;
      }
      break;
    case 3:
      IsBusy2On( Driver, "InitSensor-c3" );
      s->SendCMD2( "Scan", Dev, "SetZeroCheckEnable", "0" );
      rv = false;
      LocalStage++;
      break;
    default:
      rv = false;
    }
  }

  if ( Type == "DV" ) {      // 34410 QXAFS mode
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "InitSensor-c0" );
      s->SendCMD2( "Scan", DevCh, "Reset", "" );
      LocalStage++;
      rv = false;
      break;
    default:
      rv = false;
    }
  }

  if ( Type == "DV2" ) {      // 34410 normal mode
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver, "InitSensor-c0" );
      s->SendCMD2( "Scan", DevCh, "Reset", "" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2On( Driver, "InitSensor-c1" );
      s->SendCMD2( "Scan", DevCh, "SetAutoZero", "OFF" );
      LocalStage++;
      rv = false;
      break;
    default:
      rv = false;
    }
  }

  if (( Type == "CNT2" )||( Type == "OTC2" )) {
    // CNT2, OTC2 のとき カウンタの向こうにつながるのは
    // keithley なのでそれ用の処理をしておく
    QString Type2 = the2ndDriver->getType();
    switch( LocalStage ) {
    case 0:
      IsBusy2On( Driver2, "InitSensor-c0" );
      s->SendCMD2( "Scan", DevCh2, "Reset", "" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2On( Driver2, "InitSensor-c1" );
      if ( autoRange ) {
	if ( Type2 == "PAM" )
	  s->SendCMD2( "Scan", DevCh2, "SetAutoRangeEnable", "1" );
	if ( Type2 == "PAM2" )
	  s->SendCMD2( "Scan", Driver2, "SetAutoRangeEnable " + Ch2, "1" );
	LocalStage = 3;
      } else {
	if ( Type2 == "PAM" )
	  s->SendCMD2( "Scan", DevCh2, "SetAutoRangeEnable", "0" );
	if ( Type2 == "PAM2" )
	  s->SendCMD2( "Scan", Driver2, "SetAutoRangeEnable " + Ch2, "0" );
	LocalStage = 2;
      }
      rv = true;
      break;
    case 2:
      IsBusy2On( Driver2, "InitSensor-c2" );
      if ( Type2 == "PAM" ) {
	s->SendCMD2( "Scan", DevCh2, "SetRange", QString( "2E%1" ).arg( SelectedRange ) );
	LocalStage++;
	rv = true;
      }
      if ( Type2 == "PAM2" ) {
	s->SendCMD2( "Scan", Driver2, "SetRange " + Ch2,
		     QString( "2E%1" ).arg( SelectedRange ) );
	LocalStage+=2;     // PAM2 の時は、LocalStage == 3 をとばす
	rv = false;
      }
      break;
    case 3:
      IsBusy2On( Driver2, "InitSensor-c3" );
      s->SendCMD2( "Scan", DevCh2, "SetZeroCheckEnable", "0" );
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
      IsBusy2On( Driver, "InitSensor-c0" );
      s->SendCMD2( "Init", Driver, "RunStop" );
      LocalStage++;
      rv = true;
      break;
    case 1:
      IsBusy2On( Driver, "InitSensor-c1" );
      s->SendCMD2( "Init", Driver, "SetPresetType", SSDPresetType );
      LocalStage++;
      rv = true;
      break;
    case 2:
      IsBusy2On( Driver, "InitSensor-c2" );
      ROIs = ROIStart[0] + " " + ROIEnd[0];
      for ( int i = 1; i < MaxSSDs; i++ ) {
	ROIs += " " + ROIStart[i] + " " + ROIEnd[i];
      }
      s->SendCMD2( "Init", Driver, "SetROIs", ROIs );
      LocalStage++;
      rv = false;
      break;
    }
  }
  if ( Type == "ENC2" ) {
    IsBusy2On( Driver, "InitSensor-c0" );
    s->SendCMD2( "Init", DevCh, "GetValue" );
    LocalStage++;
    rv = false;
  }

  return rv;
}

void AUnit::SetRange( int range )
{
  if (( Type == "CNT2" )||( Type == "OTC2" )||( Type == "PAM" )||( Type == "PAM2" )) {
    IsBusy2On( Driver2, "SetRange" );
    if (( Type == "CNT2" )||( Type == "OTC2" )) {
      // CNT2, OTC2 のとき カウンタの向こうにつながるのは
      // keithley なのでそれ用の処理をしておく
      QString Type2 = the2ndDriver->getType();
      if ( Type2 == "PAM" ) {
	s->SendCMD2( "Scan", DevCh2, "SetRange", QString( "2E%1" ).arg( range ) );
      }
      if ( Type2 == "PAM2" ) {
	s->SendCMD2( "Scan", Driver2, "SetRange " + Ch2,
		     QString( "2E%1" ).arg( range ) );
      }
    }
    if ( Type == "PAM" ) {
      s->SendCMD2( "Scan", DevCh, "SetRange", QString( "2E%1" ).arg( range ) );
    }
    if ( Type == "PAM2" ) {
      s->SendCMD2( "Scan", Driver, "SetRange " + Ch,
		   QString( "2E%1" ).arg( range ) );
    }
    SelectedRange = range;
  }
}

/*** XMAP SSD ***/

bool AUnit::GetStat( void )
{
  bool rv = false;

  if ( Type == "SSD" ) {
    IsBusy2On( Driver, "GetStat" );
    s->SendCMD2( Uid, Driver, "GetStatistics" );
    rv = false;
  }

  return rv;
}

void AUnit::ReactGetStat( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {  // Check !!!!! DevCh/Drv
    IsBusy2Off( Driver );
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
  bool rv = false;

  if ( Type == "SSDP" ) {
    IsBusy2On( Driver, "SetRealTime1" );
    s->SendCMD2( Uid, DevCh, "SetRealTime", QString::number( val ) );
    rv = false;
  }

  return rv;
}

bool AUnit::SetRealTime( int ch, double val )
{
  bool rv = false;

  if ( Type == "SSD" ) {
    IsBusy2On( Driver, "SetRealTime2" );
    s->SendCMD2( Uid, Driver, "SetRealTime",
		 QString::number( ch ) + " " + QString::number( val ) );
    rv = false;
  }

  return rv;
}

bool AUnit::GetRealTime( int ch )
{
  bool rv = false;

  if ( Type == "SSD" ) {
    IsBusy2On( Driver, "GetRealTime" );
    s->SendCMD2( Uid, Driver, "GetRealTime", QString::number( ch ) );
    rv = false;
  }

  return rv;
}

void AUnit::ReactGetRealTime( SMsg msg )
{
  int ch;

  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {  // Check !!!!! DevCh/Drv
    IsBusy2Off( Driver );
    MCARealTime[ ch = msg.Vals().at(0).toInt() ] = msg.Vals().at(1).toDouble();
    emit ReceivedNewMCARealTime( ch );
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
  bool rv = false;

  if ( Type == "SSDP" ) {
    IsBusy2On( Driver, "SetLiveTime1" );
    s->SendCMD2( Uid, DevCh, "SetLiveTime", QString::number( val ) );
    rv = false;
  }

  return rv;
}

bool AUnit::SetLiveTime( int ch, double val )
{
  bool rv = false;

  if ( Type == "SSD" ) {
    IsBusy2On( Driver, "SetLiveTime2" );
    s->SendCMD2( Uid, Driver, "SetLiveTime",
		 QString::number( ch ) + " " + QString::number( val ) );
    rv = false;
  }

  return rv;
}

bool AUnit::GetLiveTime( int ch )
{
  bool rv = false;

  if ( Type == "SSD" ) {
    IsBusy2On( Driver, "GetLiveTime2" );
    s->SendCMD2( Uid, Driver, "GetLiveTime", QString::number( ch ) );
    rv = false;
  }

  return rv;
}

void AUnit::ReactGetLiveTime( SMsg msg )
{
  int ch;

  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {  // Check !!!!! DevCh/Drv
    IsBusy2Off( Driver );
    MCALiveTime[ ch = msg.Vals().at(0).toInt() ] = msg.Vals().at(1).toDouble();
    emit ReceivedNewMCALiveTime( ch );
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

void AUnit::SetLowLimit( int ch, int llpix )
{
  if ( ch < MaxSSDs ) {
    //    MCALowLimit[ ch ] = llpix;
    s->SendCMD2( "SSDSetting", Driver,
		 QString( "SetLLimit %1 %2" ).arg( ch ).arg( llpix ) );
  } else {
    qDebug() << "Setting LowLimit the ch " << ch << "is too big";
  }
}

void AUnit::ReactGetDataLinkCh( SMsg msg )
{
  if ( Type == "SSD" ) {
    if ( msg.From() == Driver ) {
      if ( msg.Vals().count() == 2 ) {
	IsBusy2Off( Driver );
	DataLinkHostName = msg.Vals().at(0);
	DataLinkHostPort = msg.Vals().at(1).toInt();
	ConnectToDataLinkServer( DataLinkHostName, DataLinkHostPort );
	qDebug() << "Connect to SSD server" << DataLinkHostName << DataLinkHostPort;
      }
    }
  }
}

bool AUnit::GetMCAs( void )
{
  if ( Type == "SSD" ) {
    IsBusy2On( Driver2, "GetMCAs" );
    // 変則 : この IsBusy2 は @GetMCAs Ok: を受けても消さない
    //        data-link 経由で完全なデータをもらった時に消す
    s->SendCMD2( Uid, DevCh, QString( "GetMCAs" ) );
  }

  return false;
}

void AUnit::getMCALength( SMsg msg )
{
  if ( msg.From() == Driver ) {  //   // Check !!!!! DevCh/Drv
    MCALength = msg.Val().toInt();
  }
}

void AUnit::getNewValue( QString )
{
  Value = QString::number( theParent->getCountsInROI().at( Ch.toInt() ) );
}

void AUnit::getNewDark( double )
{
  Dark = theParent->getDarkCountsInROI().at( Ch.toInt() );
}

void AUnit::ConnectToDataLinkServer( QString host, qint16 port )
{
  if ( !connectingDLink ) {
    connectingDLink = true;
    qDebug() << "data link server" << host << port;
    if ( dLink != NULL ) delete dLink;
    dLink = new QTcpSocket;
    if ( dLinkStream != NULL ) delete dLinkStream;
    dLinkStream = new QDataStream( dLink );
    if ( MCAs0 != NULL )
      delete [] MCAs0;
    MCAs0 = new char [ MCABUFSIZE ];
    dLinkCount = 0;
    MCAsReady = false;  // MCAs のバッファに有効なデータが無い
    connect( dLink, SIGNAL( readyRead() ), this, SLOT( receiveMCAs() ),
	     Qt::UniqueConnection );
    dLink->connectToHost( host, port );
  }
}

void AUnit::receiveMCAs( void )
{
  uint bytes0, bytes;

  bytes0 = dLink->bytesAvailable();
  // 今届いた分を全部読んでもバッファサイズより小さいなら
  if ( dLinkCount + bytes0 <= MCABUFSIZE )
    bytes = bytes0;                    // 全部読む
  else
    bytes = MCABUFSIZE - dLinkCount;   // 大きいなら、読める分だけ読む

  bytes = dLinkStream->readRawData( MCAs0 + dLinkCount, bytes );
  dLinkCount += bytes;

  if ( dLinkCount >= MCABUFSIZE ) {
    IsBusy2Off( Driver );
    dLinkCount = 0;
    if ( MCAs != NULL ) delete [] MCAs;
    MCAs = MCAs0;              // 読み込みが完成したバッファ(MCAs0)を
                               // 最新のデータが置かれたバッファ(MCAs)に移し
    MCAs0 = new char [ MCABUFSIZE ];
                               // MCAs0 は次のデータを受けるために新しくする
    MCAsReady = true;          // MCAs のバッファに有効なデータがある

    CountsInROI.clear();
    CountsAll.clear();
    TotalEvents.clear();
    ICRs.clear();

    quint64 sum = 0;
    quint64 countsAll, countsInROI;
    for ( int i = 0; i < MaxSSDs; i++ ) {
      quint32 *aMCA = getAMCA( i );
      countsAll = countsInROI = 0;
      for ( int j = 0; j < (int)MCALength; j++ ) {
	if ( ( j >= ROIStart[i].toInt() )&&( j <= ROIEnd[i].toInt() ) )
	  countsInROI += aMCA[j];
	countsAll += aMCA[j];
      }
      CountsAll << countsAll;
      sum += countsInROI;
      CountsInROI << countsInROI;
    }

    Value = QString::number( sum );
    for ( int i = 0; i < MaxSSDs; i++ ) {
      TotalEvents << 0;
      ICRs        << getAMCAHead( i ).icr;
    }
    emit LogMsg( "emitted New MCAs" );
    emit NewMCAsAvailable( MCAs );
    emit newValue( Value );
  }
}

quint32 AUnit::getAMCAdata( int ch, int pixel )
{
  if ( !MCAsReady )
    return 0;
  return *((quint32 *)( MCAs + AMCABUF * ch + MCAHEAD ) + pixel );
}

quint32 *AUnit::getAMCA( int ch )
{
  if ( !MCAsReady )
    return NULL;
  return (quint32 *)( MCAs + AMCABUF * ch + MCAHEAD );
}

MCAHead AUnit::getAMCAHead( int ch )
{
  MCAHead rv;

  if ( !MCAsReady )
    return rv;
  rv.ch       = *(qint64*)( MCAs + AMCABUF * ch +  0 );
  rv.stat     = *(qint64*)( MCAs + AMCABUF * ch +  8 );
  rv.len      = *(qint64*)( MCAs + AMCABUF * ch + 16 );
  rv.realTime = *(double*)( MCAs + AMCABUF * ch + 24 );
  rv.liveTime = *(double*)( MCAs + AMCABUF * ch + 32 );
  rv.icr      = *(double*)( MCAs + AMCABUF * ch + 40 );
  return rv;
}

void AUnit::setGain( int ch, double gain )
{
  if ( Type == "SSD" ) {
    s->SendCMD2( Uid, Driver, QString( "SetPreAMPGain %1 %2" ).arg( ch ).arg( gain ) );
  }
}

void AUnit::IsBusy2On( QString drv, QString name )
{
//  qDebug() << "B2ON  " << drv << name;
  IsBusy2 = true;
  Busy2Count++;
  LastFunc2 = name;
  emit ChangedIsBusy2( drv );
  emit ChangedBusy2Count( drv );
}

void AUnit::IsBusy2Off( QString drv )
{
//  qDebug() << "B2Off " << drv;
  IsBusy2 = false;
  Busy2Count--;
  if ( Busy2Count < 0 ) Busy2Count = 0;
  LastFunc2 = "";
  emit ChangedIsBusy2( drv );
  emit ChangedBusy2Count( drv );
}


bool AUnit::GetRange( void )
{
  bool rv = false;

  if (( Type == "CNT2" )||( Type == "OTC2" )) {
    QString Type2 = the2ndDriver->getType();
    IsBusy2On( Driver2, "GetRange" );
    if ( Type2 == "PAM" )
      s->SendCMD2( Uid, DevCh2, QString( "GetRange" ) );
    if ( Type2 == "PAM2" )
      s->SendCMD2( Uid, Driver2, QString( "GetRange " ) + Ch2 );
    rv = false;
  }

  return rv;
}

void AUnit::ReactGetRange( SMsg msg )
{
  double range = RangeL;
  if (( Type == "CNT2" )||( Type == "OTC2" )) {
    if ( ( msg.From() == DevCh2 ) || ( msg.From() == Driver2 ) ) {
      // Check !!!!! DevCh/Drv
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
}

bool AUnit::isAutoRangeAvailable( void )  // PAM と CNT2, OCT2 は AutoRange を選択可
{
  if (( Type == "PAM" )||( Type == "PAM2" )||( Type == "CNT2" )||( Type == "OTC2" )) {
    return true;
  }
  return false;
}

void AUnit::OnReportCurrent( SMsg msg )
{
  if ( Type == "LSR" ) {
    if ( msg.From() == DevCh ) {
      Values = msg.Val().simplified().split( QRegExp( "\\s" ) );
      lastVal = Value;
      Value = Values[ Values.count() - 1 ];
      emit NewRingCurrent( Value, Values );
    }
  }
}

void AUnit::OnReportInjection( SMsg msg )
{
  if ( Type == "LSR" ) {
    if ( msg.From() == DevCh ) {
      Values = msg.Val().simplified().split( QRegExp( "\\s" ) );
      lastVal = Value;
      Value = Values[ Values.count() - 1 ];
      emit NewInjectionReport( Value, Values );
      emit newValue( Value );
    }
  }
}

void AUnit::GoMaxAbs( double start, double end, int steps )
{
  if ( Type == "AIOo" ) {
    IsBusy2On( Driver, "GoMaxAbs" );
    s->SendCMD2( Uid, Driver,
		 QString( "GoMaxAbs 0 %1 %2 %3" ).arg( start ).arg( end ).arg( steps ) );
  }
}

void AUnit::GoMaxAbsQ( double start, double end, int steps, double time )
{
  if ( Type == "AIOo" ) {
    IsBusy2On( Driver, "GoMaxAbsQ" );
    s->SendCMD2( Uid, Driver,
		 QString( "GoMaxAbs 1 %1 %2 %3 %4" ).arg( start ).arg( end ).arg( steps ).arg( time ) );
  }
}

void AUnit::GoMaxRel( double width, int steps )
{
  if ( Type == "AIOo" ) {
    IsBusy2On( Driver, "GoMaxRel" );
    s->SendCMD2( Uid, Driver,
		 QString( "GoMaxRel 0 %1 %2" ).arg( width ).arg( steps ) );
  }
}

void AUnit::GoMaxRelQ( double width, int steps, double time )
{
  if ( Type == "AIOo" ) {
    IsBusy2On( Driver, "GoMaxRelQ" );
    s->SendCMD2( Uid, Driver,
		 QString( "GoMaxRel 1 %1 %2 %3" ).arg( width ).arg( steps ).arg( time ) );
  }
}

void AUnit::CloseShutter( bool close )
{
  if ( Type == "AIOo" ) {
    if ( close ) {
      s->SendCMD2( Uid, Driver, "ShutterOff 1" );
    } else {
      s->SendCMD2( Uid, Driver, "ShutterOff 0" );
    }
  }
}

void AUnit::OnReportValue( SMsg msg )
{
  if ( Type == "FP23" ) {
    if ( msg.From() == DevCh ) {
      Values = msg.Val().simplified().split( QRegExp( "\\s" ) );
      lastVal = Value;
      Value = Values[ Values.count() - 1 ];
      emit NewFP23Temperature( Value );
    }
  }
}

#if 0
void AUnit::RcvAlarmEvent( SMsg msg )
{
  if ( Type == "AIOi" ){
    emit Alarm( Uid, QString( "%1 %2" ).arg( msg.Msg() ).arg( msg.Val() ) );
  }
}
#endif
