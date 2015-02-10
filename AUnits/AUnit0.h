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
  QString Uid2;         // 2nd Uid
  QString ID;           // MainTh, StageX, General, ...
  QString Name;         // Displayed name
  QString Dev;
  QString Ch;
  QString DevCh;        // Dev + "." + Ch
  QString Unit;         // metric unit "mm", "mA", ...

  QString LastFunc;     // last function which enabled isBusy
  QString LastFunc2;    // last function which enabled isBusy2

  bool IsBusy;
  bool IsBusy2;
  int Busy2Count;
  
  QString Value;
  QStringList Values;
  QString lastVal;
  int ilastSetV;
  double dlastSetV;

public:
  AUnit0( QObject *parent = 0 );

  void setEnable( bool enable );
  void setALine( int aline ) { aLine = aline; };
  bool isEnable( void ) { return Enable; };

  QString getGType( void ) { return GType; };        // Motor, Sensor
  QString getType( void ) { return Type; };          // PM, PZ, ENC, ...
  QString getUid( void ) { return Uid; };            // Uniq Uid
  virtual QString get2ndUid( void ); /* return Uid2; */  // 2nd Uid
  QString getID( void ) { return ID; };              // MainTh, StageX, General, ...
  QString getName( void ) { return Name; };          // Displayed name
  QString getDev( void ) { return Dev; };
  QString getCh( void ) { return Ch; };
  QString getDevCh( void ) { return DevCh; };        // Driver + "." + Ch
  QString getUnit( void ) { return Unit; };          // metric unit "mm", "mA", ...

  void setGType( QString gtype ) { GType = gtype; }; // Motor, Sensor
  void setType( QString type ) { Type = type; };     // PM, PZ, ENC, ...
  void setUid( QString uid ) { Uid = uid; };         // Uniq Uid
  virtual void set2ndUid( QString sUid ); /* Uid2 = uid; */
  void setID( QString id ) { ID = id; };
  void setName( QString name ) { Name = name; };
  void setDev( QString dev ) { Dev = dev; if ( Ch != "" ) makeDevCh(); };
  void setCh( QString ch ) { Ch = ch; if ( Dev != "" ) makeDevCh(); };
  void setUnit( QString unit ) { Unit = unit; };
  void makeDevCh( void );

  void setIsBusy( bool busy ) { IsBusy = busy; emit ChangedIsBusy1( Dev ); };
  void setIsBusy2( bool busy ) { IsBusy2 = busy; emit ChangedIsBusy2( Dev ); };
  bool isBusy0( void ) { return IsBusy || IsBusy2; };
  bool isBusy( void ) { return IsBusy; };
  bool isBusy2( void ) { return IsBusy2; };

  virtual bool GetValue0( void );
  virtual bool GetValue02( void );
  virtual bool GetValue( void );
  QString value( void ) { return Value; };

  void InitLocalStage( void ) { LocalStage = 0; };
  
  virtual bool hasParent( void );       // 継承した AUnit で必ず定義
  //  QString PUid;         // the Uid of the parenet
  //  AUnit *theParent;     // the parent
  virtual QString getPUid( void );
  virtual void setPUid( QString puid );
  virtual AUnit0 *theParent( void );
  virtual void setTheParent( AUnit0 *p );
  virtual AUnit0 *getTheParent( void );

  virtual bool has2ndDriver( void );    // 継承した AUnit で必ず定義
  //  the2ndDriver = NULL;              // true を返すなら必ず theParent も持つ
  //  QString Ch2;          // 2nd Ch;
  //  QString DevCh2;       // 2nd DevCh
  virtual AUnit0 *getThe2ndDriver( void );
  virtual void setThe2ndDriver( AUnit0 *dev2 );
  virtual QString get2ndDriver( void );
  virtual void set2ndDriver( QString dev2 );
  virtual QString get2ndCh( void );
  virtual void set2ndCh( QString ch2 ); 
  virtual QString get2ndDevCh( void );
  virtual void set2ndDevCh( QString devCh2 ); 

 signals:
  void ChangedIsBusy1( QString Drv );
  void ChangedIsBusy2( QString Drv );
};

#endif
