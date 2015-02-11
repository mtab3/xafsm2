#include "AUnit0.h"

AUnit::AUnit0( QObject *parent ) : QObject( parent )
{
  s = NULL;
  Enable = false;
  aLine = -1;

  LocalStage = 0;

  IsBusy = IsBusy2 = false;
  Busy2Count = 0;
}

void AUnit::Initialize( Stars *S )
{
  s = S;

  // 何らかのコマンドに対する応答がエラーだった場合の対処。
  // 単に、isBusy2 をクリアしているだけ。
  // こんなに単純でいいかどうかは難しいところだけれど、
  // enable をちゃんと管理するようにしたので、変な処理に突入することはそちらで避けて
  // 変な処理に突入してしまった場合は、緊急避難的にこの方法で逃げることにする。
  connect( s, SIGNAL( ReceiveError( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  // LSR には不要な初期化だが悪くもないので外さない
  connect( s, SIGNAL( AnsIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( EvIsBusy( SMsg ) ), this, SLOT( SetIsBusyByMsg( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetValue( SMsg ) ),this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );
  s->SendCMD2( "Init", "System", "flgon", Dev );
  s->SendCMD2( "Init", "System", "flgon", DevCh );
  
  init( s );   // 各ユニットに固有の処理

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

QString AUnit0::makeDevCh( const QString &dev, const QString &ch )
{
  if ( dev == "" ) {
    return "";
  }
  if ( ch != "" )
    return dev + "." + ch;

  return dev;
}

void AUnit::setEnable( bool enable )
{
  Enable = enable;
  IsBusy = false;
  LastFunc = "";
  ConnectedToSSDServer = false;
  emit Enabled( Driver, enable );
  emit ChangedIsBusy1( Driver );
  IsBusy2Off( "" );
}

bool AUnit::GetValue( void )
{
  IsBusy2On( Driver, "GetValue" );
  s->SendCMD2( Uid, DevCh, "GetValue" );

  return false;
}
