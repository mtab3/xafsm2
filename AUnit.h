#ifndef AMOTOR_H
#define AMOTOR_H

#include <QObject>

#include "XafsM.h"
#include "Stars.h"

class AUnit : public QObject
{
  Q_OBJECT

  Stars *s;

  QString GType;        // Motor, Sensor
  QString Type;         // PM, PZ, ENC, ...
  QString ID;           // MainTh, StageX, General, ...
  QString UID;          // Uniq ID ( ID + number )
  QString Name;         // Disiplayed name
  QString Driver;
  QString Ch;
  QString DevCh;        // Driver + "." + Ch
  QString Unit;         // metric unit "mm", "mA", ...

  double UPP;           // Unit per Puls : only for PM
  double Center;        // Center position in puls : only for PM

  double MaxV;          // only for PZ
  double MinV;          // only for PZ

  bool isBusy;
  bool isBusy2;
  QString Value;

  int LocalStage;

public:
  AUnit( QObject *parent = 0 );

  void Initialize( Stars *S );
  void setGType( QString gtype ) { GType = gtype; };
  void setStars( Stars *S ) { s = S; };
  void setType( QString type ) { Type = type; };
  void setID( QString id ) { ID = id; };
  void setUniqID( QString num ) { UID = "N" + num + "N" + ID; };
  void setName( QString name ) { Name = name; };
  void setDriver( QString driver ) { Driver = driver; };
  void setCh( QString ch ) { Ch = ch; };
  void setDevCh( void ) {
    if ( Ch.isEmpty() )
      DevCh = Driver;
    else 
      DevCh = Driver + "." + Ch;
  }
  void setUnit( QString unit ) { Unit = unit; };
  void setIsBusy( bool busy ) { isBusy = busy; };

  // only for PM
  void setUPP( QString upp ) { UPP = upp.toDouble(); };
  void setCenter( QString center ) { Center = center.toDouble(); };

  // only for PZ
  void setMaxV( QString maxv ) { MaxV = maxv.toDouble(); };
  void setMinV( QString minv ) { MinV = minv.toDouble(); };

  QString getGType( void ) { return GType; };
  QString getType( void ) { return Type; };
  QString getID( void ) { return ID; };
  QString getUID( void ) { return UID; };
  QString getName( void ) { return Name; };
  QString getDriver( void ) { return Driver; };
  QString getCh( void ) { return Ch; };
  QString getDevCh( void ) { return DevCh; };
  QString getUnit( void ) { return Unit; };
  bool getIsBusy( void ) { return isBusy; };
  bool getIsBusy2( void ) { return isBusy2; };
  QString value( void ) { return Value; };

  // only for PM
  double getUPP( void ) { return UPP; };
  double getCenter( void ) { return Center; };
  // only for PZ
  double getMaxV( void ) { return MaxV; };
  double getMinV( void ) { return MinV; };


  void show( void );   // mainly for debugging

  // wrapper functions of stars communication
  void InitLocalStage( void );
  bool InitSensor( void );
  bool GetValue0( void );
  bool GetValue( void );
  void SetValue( double v );
  void AskIsBusy( void );
  void SetSpeed( MSPEED speed );
  void SetTime( double dtime );   // in sec
  void Stop( void );

public slots:
  void ClrBusy( SMsg msg );
  void SetIsBusyByMsg( SMsg msg );
  void SetCurPos( SMsg msg );

signals:
  void CountFinished( void );
};

#endif


