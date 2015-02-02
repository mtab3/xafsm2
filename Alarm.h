#ifndef ALARM_H
#define ALARM_H

class Alarm
{
public:
  
  QString Uid;
  QString AlarmOn, AlarmOff;
  QString AlarmMsg;
  bool alarm;

  Alarm( void ) { Uid.clear(); AlarmOn.clear(); AlarmOff.clear(); AlarmMsg.clear(); alarm = false; };
};

#endif
