
#ifndef SMSG_H
#define SMSG_H

#include <QString>
#include <QStringList>

#include "MsgType.h"

enum MSGOREVENT { RES_MSG, RES_EVENT, RES_OTHER };

class SMsg {
  QString from;
  QString fromDev;
  QString fromCh;
  QString to;
  QString toDev;
  QString toCh;
  QString msg;
  QString val;
  QStringList vals;
  MSGOREVENT MorE;
  MSGTYPE msgt;
  QString fullMsg;
  
 public:
  SMsg();
  QString From( void ) { return from; };
  QString FromDev( void ) { return fromDev; };
  QString FromCh( void ) { return fromCh; };
  QString To( void ) { return to; };
  QString ToDev( void ) { return toDev; };
  QString ToCh( void ) { return toCh; };
  QString Msg( void ) { return msg; };
  QString Val( void ) { return val; };
  QStringList Vals( void ) { return vals; };
  MSGTYPE Msgt( void ) { return msgt; };
  QString FullMsg( void ) { return fullMsg; };

  //  bool isDevCh( QString devch );
  //  bool isMsg0( QString Msg );
  void setMsgType( void );
  //  bool isMsg( int msgt );
  bool isOk( void );

  MSGOREVENT ParseMsg( QString comming );
};

#endif
