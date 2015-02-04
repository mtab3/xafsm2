#ifndef ALARM_H
#define ALARM_H

#include <QObject>
#include <QVector>
#include <QMessageBox>

class Alarm
{
  QString Uid;
  QString AlarmOn, AlarmOff;
  QString Msg;
  bool alarming;

public:

  Alarm( void );
  void setAll( QString uid, QString aon, QString aoff, QString msg, bool f )
  {
    Uid = uid;
    AlarmOn = aon;
    AlarmOff = aoff;
    Msg = msg;
    alarming = f;
  }
  bool isNewAlarmOn( QString uid, QString aMsg );
  bool isNewAlarmOff( QString uid, QString aMsg );
  bool isAlarming( void ) { return alarming; };
  QString msg( void ) { return Msg; };
};

class Alarms : public QObject
{
  Q_OBJECT

  QVector<Alarm> alarms;
  QMessageBox *mbox;

 public:
  Alarms( void );
  void addAlarm( QString uid, QString aon, QString aoff, QString msg ) {
    Alarm a;
    a.setAll( uid, aon, aoff, msg, false );
    alarms << a;
  }

public slots:
  void chkAlarm( QString uid, QString aMsg );

 signals:
  void alarmOn( void );
  void alarmOff( void );
};


#endif
