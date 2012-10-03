#include <QtGui>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "MsgType.h"
#include "SMsg.h"

SMsg::SMsg( void )
{
}

MSGOREVENT SMsg::ParseMsg( QString Message )
{
  int i1, i2, i3;

  from.clear();
  fromDev.clear();
  fromCh.clear();
  to.clear();
  toDev.clear();
  toCh.clear();
  msg.clear();
  val.clear();

  Message = Message.simplified();
  i1 = Message.indexOf( '>' );      // i1 : '>' �̈ʒu
  if ( i1 < 0 ) {
    MorE = RES_OTHER;         // '>' ���Ȃ��ƁAStars server ����̃��b�Z�[�W�̍\��
    val = Message;            // �Ƃ��Ă͂����������A�Ƃ肠�������̓��e������
    return RES_OTHER;         // val �Ɏc���� return
  }
  from = Message.mid( 0, i1 );
  i3 = from.indexOf( '.' );
  if ( i3 < 0 ) {       // from �� ch �����������
    fromDev = from;     // fromDev �����Z�b�g fromCh �͋�̂܂�
  } else {
    fromDev = from.mid( 0, i3 );
    fromCh = from.mid( i3 + 1 );
  }

  i2 = Message.indexOf( ' ', i1 );  // i2 : �ŏ��� ' ' �̈ʒu
  if ( i2 < 0 ) {
    MorE = RES_OTHER;
    val = Message.mid( i1 + 1 );
    return RES_OTHER;
  }
  to = Message.mid( i1 + 1, i2 - i1 - 1 );
  i3 = to.indexOf( '.' );
  if ( i3 < 0 ) {       // to �� ch �����������
    toDev = from;     // toDev �����Z�b�g toCh �͋�̂܂�
  } else {
    toDev = to.mid( 0, i3 );
    toCh = to.mid( i3 + 1 );
  }

  if ( Message.at( i2 + 1 ) == '_' )
    MorE = RES_EVENT;
  else
    MorE = RES_MSG;

  i3 = Message.indexOf( ' ', i2 + 1 );
  if ( i3 < 0 ) {
    msg = Message.mid( i2 + 1 );
    setMsgType();
    return MorE;
  }
  msg = Message.mid( i2 + 1, i3 - i2 - 1 );
  setMsgType();

  val = Message.mid( i3 + 1 );
  vals = val.split( QRegExp( "\\s+" ) );

  return MorE;
}

void SMsg::setMsgType( void )
{
  int i = 0;

  msgt = NOTMSG;
  while ( MsgTypeV[i] != NOTMSG ) {
    if ( msg == MsgType[i] ) {
      msgt = MsgTypeV[i];
      break;
    }
    i++;
  }
}

#if 0
bool SMsg::isMsg( int MsgT )
{
  if ( msgt == MsgT )
    return true;

  return false;
}
#endif

bool SMsg::isOk( void )
{
  if ( val.mid( 0, 3 ) == "Ok:" )
    return true;
  if ( val.mid( 0, 2 ) == "OK" )
    return true;
  return false;
}
