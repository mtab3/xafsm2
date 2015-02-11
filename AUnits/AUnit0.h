#ifndef AUNIT_H
#define AUNIT_H

#include <QObject>

#include "../Stars.h"

class AUnit0 : public QObject
{
  Q_OBJECT

  Stars *s;
  bool Enable;          // if the unit is enable on Stars server or not
  int aLine;            // the line number where the definition appears in .def file

  int LocalStage;

  QString GType;        // Motor, Sensor
  QString Type;         // PM, PZ, ENC, ...
  QString Uid;          // Uniq ID
  QString ID;           // MainTh, StageX, General, ...
  QString Name;         // Displayed name
  QString Dev;
  QString Ch;
  QString DevCh;        // Dev + "." + Ch
  QString Unit;         // metric unit "mm", "mA", ...
  double UPP;

  QString LastFunc;     // last function which enabled isBusy
  QString LastFunc2;    // last function which enabled isBusy2

  bool IsBusy;
  bool IsBusy2;
  int Busy2Count;
  
  QString Value;
  QStringList Values;
  QString LastValue;
  int ilastSetV;
  double dlastSetV;

  bool HasParent;
  AUnit0 *theParent;
  QString PUid;

  bool Has2ndDev;
  QString Uid2;
  QString Dev2;
  QString Ch2;
  QString DevCh2;       // Dev + "." + Ch
  AUnit0 *the2ndDev;
  
public:
  AUnit0( QObject *parent = 0 );

  void Initialize( Stars *S );
  virtual void init( Stars *S );

  void setEnable( bool enable );
  void setALine( int aline ) { aLine = aline; };
  bool isEnable( void ) { return Enable; };

  Stars *getStars( void ) { return s; };
  QString getGType( void ) { return GType; };        // Motor, Sensor
  QString getType( void ) { return Type; };          // PM, PZ, ENC, ...
  QString getUid( void ) { return Uid; };            // Uniq Uid
  QString get2ndUid( void ) { return Uid2; };         /* return Uid2; */  // 2nd Uid
  QString getID( void ) { return ID; };              // MainTh, StageX, General, ...
  QString getName( void ) { return Name; };          // Displayed name
  QString getDev( void ) { return Dev; };
  QString getCh( void ) { return Ch; };
  QString getDevCh( void ) { return DevCh; };        // Driver + "." + Ch
  QString getUnit( void ) { return Unit; };          // metric unit "mm", "mA", ...
  double getUPP( void ) { return UPP; };

  void setStars( Stars *S ) { s = S; };
  void setGType( QString gtype ) { GType = gtype; }; // Motor, Sensor
  void setType( QString type ) { Type = type; };     // PM, PZ, ENC, ...
  void setUid( QString uid ) { Uid = uid; };         // Uniq Uid
  void set2ndUid( QString uid ) { Uid2 = uid; };
  void setID( QString id ) { ID = id; };
  void setName( QString name ) { Name = name; };
  void setDev( QString dev )
  { Dev = dev; if ( Ch != "" ) DevCh = makeDevCh( Dev, Ch ); };
  void setCh( QString ch )
  { Ch = ch; if ( Dev != "" ) DevCh = makeDevCh( Dev, Ch ); };
  void setUnit( QString unit ) { Unit = unit; };
  void setUPP( QString upp ) { UPP = upp.toDouble(); };

  QString makeDevCh( const QString &dev, const QString &ch );

  void setIsBusy( bool busy ) { IsBusy = busy; emit ChangedIsBusy1( Dev ); };
  void setIsBusy2( bool busy ) { IsBusy2 = busy; emit ChangedIsBusy2( Dev ); };
  bool isBusy0( void ) { return IsBusy || IsBusy2; };
  bool isBusy( void ) { return IsBusy; };
  bool isBusy2( void ) { return IsBusy2; };

  virtual bool GetValue0( void );
  virtual bool GetValue02( void );
  virtual bool GetValue( void );
  QString value( void ) { return Value; };
  QString lastValue( void ) { return LastValue; };
  void setLastValue( QString v ) { LastValue = v; };
  QStringList values( void ) { return Values; };

  void InitLocalStage( void ) { LocalStage = 0; };

  // parent : ������������� Uid ����ĥǥХ����ֿƥǥХ�����
  // �㤨�� nct08 ��ʣ���Υ����ͥ�ϥХ�Х�˥�����ȥ������ȥ��ȥåפǤ��ʤ���
  // ch1 �ʳ��Υ����ͥ�Υ������ȥ��ȥåפ����� ch1 �ˤޤȤ��Ǥ���뤿���
  // ch1 �� parent �Ȥ���¾�� ch ������(ch1������) ch1 ��Ƥ˻���
  void setHasParent( bool hasParent ) { HasParent = hasParent; };
  bool hasParent( void ) { return HasParent; };
  void setPUid( QString puid ) { PUid = puid; };
  QString getPUid( void ) { return PUid; };
  //  void setParent( QString pUid ) { PUid = pUid; };
  void setTheParent( AUnit0 *p ) { theParent = p; };
  AUnit0 *getTheParent( void ) { return theParent; };
  
  // Dev2, Ch2, DevCh2 : ��˥åȤ������2�ĤΥɥ饤�Ф�ɬ�פʻ�������2���ܤΥɥ饤��
  // �㤨��Keithley ����ή/�Ű�����פȤ��ƻ��Ѥ��ơ����ν��Ϥ��㤨�Х����󥿤ǿޤ���
  // Keithley �����򥳥�ȥ��뤹��ȥ�����꤬�Ǥ��륫���󥿤Τ褦�˰�����
  // ���λ��ᥤ��ΥǥХ����ϥ����󥿤�����Keithley ��2nd�ɥ饤�ФȤ��ƻ��ꤹ��
  void setHas2ndDev( bool has2ndDev ) { Has2ndDev = has2ndDev; };
  bool has2ndDev( void ) { return Has2ndDev; };
  AUnit0 *getThe2ndDev( void ) { return the2ndDev; };
  void setThe2ndDev( AUnit0 *dev2 ) { the2ndDev = dev2; };
  QString get2ndDev( void ) { return Dev2; };
  QString get2ndCh( void ) { return Ch2; };
  QString get2ndDevCh( void ) { return DevCh2; };
  void set2ndDev( QString dev )
  { Dev2 = dev; if ( Ch2 != "" ) DevCh2 = makeDevCh( Dev2, Ch2 ); };
  void set2ndCh( QString ch )
  { Ch2 = ch; if ( Dev2 != "" ) DevCh2 = makeDevCh( Dev2, Ch2 ); };
  
 signals:
  void ChangedIsBusy1( QString Drv );
  void ChangedIsBusy2( QString Drv );
};

#endif
