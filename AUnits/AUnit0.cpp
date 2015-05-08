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

  // $B2?$i$+$N%3%^%s%I$KBP$9$k1~Ez$,%(%i!<$@$C$?>l9g$NBP=h!#(B
  // $BC1$K!"(BisBusy2 $B$r%/%j%"$7$F$$$k$@$1!#(B
  // $B$3$s$J$KC1=c$G$$$$$+$I$&$+$OFq$7$$$H$3$m$@$1$l$I!"(B
  // enable $B$r$A$c$s$H4IM}$9$k$h$&$K$7$?$N$G!"JQ$J=hM}$KFMF~$9$k$3$H$O$=$A$i$GHr$1$F(B
  // $BJQ$J=hM}$KFMF~$7$F$7$^$C$?>l9g$O!"6[5^HrFqE*$K$3$NJ}K!$GF($2$k$3$H$K$9$k!#(B
  connect( s, SIGNAL( ReceiveError( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  // $B$3$3$G=q$/$H!"A4%f%K%C%H$K6&DL$N(B connect $B$K$J$k!#(B
  // $BK\Ev$O(B LSR $B$K$OITMW$J=i4|2=$@$,0-$/$b$J$$$N$G30$5$J$$(B
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

  init();   // $B3F%f%K%C%H$K8GM-$N=hM}(B

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
  emit ChangedIsBusy1( Dev );    // $B$3$3$N(B3$B$D$N%(%_%C%H$OG0$N0Y(B
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
  _setEnable( enable );    // AUnit0 $B$r7Q>5$7$?%/%i%9$G$N=hM}MQ(B // AUnitSFluo $B$,8F$s$G$k(B
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
  //  if ( msg.ToCh() == Uid )  // $B%$%Y%s%H$r<u$1$k2DG=@-$,$"$k$N$G$3$N%A%'%C%/$OF~$l$i$l$J$$(B
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
    Value = msg.Vals().at( 0 );   // $BCMJB$S$N>l9g$O@hF,$NCM(B
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
