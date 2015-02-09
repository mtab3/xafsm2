
#include <QDebug>

#include "Alarm.h"

Alarm::Alarm( void )
{
  Uid.clear();
  AlarmOn.clear();
  AlarmOff.clear();
  Msg.clear();
  alarming = false;
}

bool Alarm::isNewAlarmOn( QString uid, QString aMsg )
{
  if ( ( uid == Uid )
       &&( aMsg.left( AlarmOn.length() ) == AlarmOn )
       &&( ! alarming ) ) {
    alarming = true;
    return true;
  }
  return false;
}

bool Alarm::isNewAlarmOff( QString uid, QString aMsg )
{
  if ( ( uid == Uid )
       &&( aMsg.left( AlarmOff.length() ) == AlarmOff )
       &&( alarming ) ) {
    alarming = false;
    return true;
  }
  return false;
}


Alarms::Alarms( void )
{
  mbox = new QMessageBox();
  mbox->setWindowTitle( tr( "Alarm!" ) );
  mbox->setModal( false );
}

void Alarms::chkAlarm( QString uid, QString aMsg )
{
  bool changed = false;
  for ( int i = 0; i < alarms.count(); i++ ) {
    if ( alarms[i].isNewAlarmOn( uid, aMsg ) ) {
      emit alarmOn();
      changed = true;
    }
    if ( alarms[i].isNewAlarmOff( uid, aMsg ) )
      changed = true;
  }
  if ( changed ) {
    QString msg;
    msg.clear();
    for ( int i = 0; i < alarms.count(); i++ ) {
      if ( alarms[i].isAlarming() ) {
	msg += alarms[i].msg() + "\n";
      }
    }
    if ( msg.isEmpty() ) {
      mbox->hide();
      emit alarmOff();
    } else {
      mbox->setText( msg );
      mbox->show();
    }
  }
}

