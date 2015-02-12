
#include "AUnitXMAP2.h"

void AUnitXMAP2::init0( void )
{
  connect( s, SIGNAL( AnsSetPresetValue( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetROIs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  s->SendCMD2( "Init", "System", "flgon", DevCh );
}

double AUnitXMAP2::stat( STATELM i )
{
  return ((AUnitXMAP*)TheParent)->stat( Ch.toInt(), i );
}

void AUnitXMAP2::getNewValue( QString )
{
  Value = QString::number( ((AUnitXMAP*)TheParent)->getCountsInROI().at( Ch.toInt() ) );
}

void AUnitXMAP2::getNewDark( double )
{
  Dark = ((AUnitXMAP*)TheParent)->getDarkCountsInROI().at( Ch.toInt() );
}

double AUnitXMAP2::SetTime( double dtime ) // in sec, ���δؿ��ϡ�ʣ�����ƥåײ��Ǥ��ʤ�
{
  IsBusy2On( Dev, "SetTime" );
  s->SendCMD2( Uid, Dev, "RunStop" );   // ���ޥ��Ϣ³ȯ�Բ�ǽ��? �������������Ƥ�
  s->SendCMD2( Uid, DevCh, "SetPresetValue", QString::number( dtime ) );
  setTime = dtime;

  return setTime;
}

bool AUnitXMAP2::SetRealTime( double val )
{
  bool rv = false;

  IsBusy2On( Dev, "SetRealTime1" );
  s->SendCMD2( Uid, DevCh, "SetRealTime", QString::number( val ) );
  rv = false;

  return rv;
}
