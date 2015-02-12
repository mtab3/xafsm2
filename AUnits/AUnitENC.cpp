
#include "AUnitENC.h"

void AUnitENC::init0( void  )
{
  connect( s, SIGNAL(AnsGetStat( SMsg )), this, SLOT( RcvStat( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsTrigger( SMsg )), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsStandBy( SMsg )), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsGetData( SMsg )), this, SLOT( RcvQGetData( SMsg ) ),
	   Qt::UniqueConnection );

  init00();
}

void AUnitENC::init00( void )
{
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );
  s->SendCMD2( "Init", DevCh, "IsBusy" );
  s->SendCMD2( "Init", DevCh, "GetValue" );
}

void AUnitENC2::init00( void )
{
}

bool AUnitENC::InitSensor( void )
{
  return _InitSensor();
}

bool AUnitENC::_InitSensor( void )
{
  return false;
}

bool AUnitENC2::_InitSensor( void )
{
  bool rv = false;

  if ( Type == "ENC2" ) {
    IsBusy2On( Dev, "InitSensor-c0" );
    s->SendCMD2( "Init", DevCh, "GetValue" );
    LocalStage++;
    rv = false;
  }

  return rv;
}

void AUnitENC::SetValue( double v )
{
  s->SendCMD2( Uid, DevCh, "SetValue", QString::number( DLastSetV = v ) );
}

void AUnitENC2::AskIsBusy( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}

void AUnitENC2::SetIsBusyByMsg( SMsg msg )
{
  if ( ( msg.From() == DevCh )
       && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
    IsBusy = ( msg.Val().toInt() == 1 );
    if ( IsBusy )
      LastFunc = "SetIsBusyByMsg";
    else
      LastFunc = "";
    emit ChangedIsBusy1( Dev );
  }
}

bool AUnitENC2::QStart( void )
{
  IsBusy2On( Dev, "Start" );
  s->SendCMD2( Uid, DevCh, "StandBy" );
  return false;
}

bool AUnitENC2::QRead( void )
{
  IsBusy2On( Dev, "Read" );
  s->SendCMD2( Uid, DevCh, "GetData" );

  return false;
}

double AUnitENC2::SetTime( double dtime ) // in sec // ���δؿ��ϡ�ʣ�����ƥåײ��Ǥ��ʤ�
{
  setTime = dtime;          // setTime �Ǥ����ȸ��������������

  return setTime;
}

void AUnitENC2::RcvQGetData( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Dev ) )
       && ( ( msg.Msgt() == GETDATAPOINTS )
	    || ( msg.Msgt() == QGETDATA )
	    || ( msg.Msgt() == GETDATA ) ) ) {
    
    Values = msg.Vals();
    emit newQData();
    IsBusy2Off( Dev );
  }
}


void AUnitENC2::RcvStat( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Dev ) )  // Check !!!!! DevCh/Drv
       && ( ( msg.Msgt() == GETSTAT ) ) ) {
    //      Values = msg.Vals();
    //      emit newQData();
    IsBusy2Off( Dev );
  }
}
