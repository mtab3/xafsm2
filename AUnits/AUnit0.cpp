#include "AUnit0.h"

AUnit0::AUnit0( QObject *parent ) : QObject( parent )
{
  ParentObject = parent;
  s = NULL;
  Enable = false;
  ALine = -1;

  Using = false;
  UserName = "";
  
  GType = "";
  Type = "";
  Type2 = "";
  ID = "";
  Uid = "";
  Name = "";
  Dev = "";
  Ch = "";
  DevCh = "";
  Unit = "";
  UPP = 1;

  HasParent = false;
  TheParent = NULL;
  PUid = "";

  Has2ndDev = false;
  Uid2 = "";
  Dev2 = "";
  Ch2 = "";
  DevCh2 = "";       // Dev + "." + Ch
  The2ndDev = NULL;

  LocalStage = 0;

  IsBusy = IsBusy2 = false;
  Busy2Count = 0;
  busy2Off( "" );

  LastFunc = "";
  LastFunc2 = "";

  Value = "";
  Values.clear();
  LastValue = "";
  ILastSetV = 0;
  DLastSetV = 0;
}

void AUnit0::Initialize( Stars *S )
{
  s = S;

  // 何らかのコマンドに対する応答がエラーだった場合の対処。
  // 単に、isBusy2 をクリアしているだけ。
  // こんなに単純でいいかどうかは難しいところだけれど、
  // enable をちゃんと管理するようにしたので、変な処理に突入することはそちらで避けて
  // 変な処理に突入してしまった場合は、緊急避難的にこの方法で逃げることにする。
  connect( s, SIGNAL( ReceiveError( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  // ここで書くと、全ユニットに共通の connect になる。
  // 本当は LSR には不要な初期化だが悪くもないので外さない
  connect( s, SIGNAL( AnsIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( EvIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetValue( SMsg ) ),this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsRead( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );
  s->SendCMD2( Uid, "System", "flgon", Dev );
  s->SendCMD2( Uid, "System", "flgon", DevCh );

  init();   // 各ユニットに固有の処理

  if ( ID == "THETA" ) {
    AskIsBusy();
    GetValue();
  }
  if ( Type == "SSD" ) {
    connect( s, SIGNAL( AnsGetMCALength( SMsg ) ), this, SLOT( getMCALength( SMsg ) ),
	     Qt::UniqueConnection );
    s->SendCMD2( Uid, Dev, "GetMCALength" );
  }
  if ( ID == "ENCTH" ) {
    GetValue();
  }
  emit ChangedIsBusy1( Dev );    // ここの3つのエミットは念の為
  emit ChangedIsBusy2( Dev );
  emit ChangedBusy2Count( Dev );
}

QString AUnit0::makeDevCh( const QString &dev, const QString &ch )
{
  if ( dev == "" ) {
    return "";
  }
  if ( ch != "" )
    return dev + "." + ch;

  return dev;
}

void AUnit0::busy2On( QString drv, QString name )
{
  IsBusy2 = true;
  Busy2Count++;
  LastFunc2 = name;
  emit ChangedIsBusy2( drv );
  emit ChangedBusy2Count( drv );
}

void AUnit0::busy2Off( QString drv )
{
  IsBusy2 = false;
  Busy2Count--;
  if ( Busy2Count < 0 ) Busy2Count = 0;
  LastFunc2 = "";
  emit ChangedIsBusy2( drv );
  emit ChangedBusy2Count( drv );
}

void AUnit0::SetIsBusyByMsg( SMsg msg )
{
  if ( ( msg.From() == DevCh )    // Dev -> DevCh 2015.02.13
  //  if ( ( msg.ToCh() == Uid )          // 2015.05.07
       && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
    IsBusy = ( msg.Val().toInt() == 1 );
    if ( IsBusy )
      LastFunc = "SetIsBusyByMsg";
    else
      LastFunc = "";
    emit ChangedIsBusy1( DevCh );    // Dev -> DevCh 2015.02.13
  }
}

void AUnit0::setEnable( bool enable )
{
  Enable = enable;
  IsBusy = false;
  LastFunc = "";
  _setEnable( enable );    // AUnit0 を継承したクラスでの処理用 // AUnitSFluo が呼んでる
  emit Enabled( Dev, enable );
  emit ChangedIsBusy1( Dev );
  busy2Off( "" );
}

bool AUnit0::GetValue( void )
{
  busy2On( Dev, "GetValue" );
  s->SendCMD2( Uid, DevCh, "GetValue" );

  return false;
}

void AUnit0::ClrBusy( SMsg msg )
{
  //  if ( msg.ToCh() == Uid )  // イベントを受ける可能性があるのでこのチェックは入れられない
  if ( ( msg.From() == DevCh ) || ( msg.From() == Dev ) ) {
    busy2Off( Dev );
  }
  if ( Has2ndDev ) {
    if ( ( msg.From() == DevCh2 ) || ( msg.From() == Dev2 ) ) {
      busy2Off( Dev2 );
    }
  }
}

void AUnit0::SetCurPos( SMsg msg )
{
  QString buf;

  if ( ( msg.From() == DevCh )
  //  if ( ( msg.ToCh() == Uid )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE )
            || ( msg.Msgt() == READ ) ) ) {
    Value = msg.Vals().at( 0 );   // 値並びの場合は先頭の値
    emit NewValue( Value );
    busy2Off( Dev );
  }
}

#if 0
//void AUnit0::ReceiveValues( SMsg msg )
{
  QString buf;

  if ( ( msg.From() == Dev ) && ( msg.Msgt() == GETVALUES ) ) { // Check !!!!! DevCh/Drv
    Value = msg.Vals().at(0);
    Values = msg.Vals();

    emit NewValue( Value );
    busy2Off( Dev );
  }
}
#endif
