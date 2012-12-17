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
  QString Uid;          // Uniq ID
  QString ID;           // MainTh, StageX, General, ...
  QString Name;         // Disiplayed name
  QString Driver;
  QString Ch;
  QString DevCh;        // Driver + "." + Ch
  QString Unit;         // metric unit "mm", "mA", ...
  double UPP;           // Unit per Puls : only for PM
  bool IsInt;           // is the controlling unit is integer or not.
  bool HasParent;       // does the unit have parent (group leader).
  QString PUid;         // the Uid of the parenet
  AUnit *theParent;        // the parent

  double Center;        // Center position in puls : only for PM

  double MaxV;          // only for PZ
  double MinV;          // only for PZ

  bool IsBusy;
  bool IsBusy2;
  QString Value;
  QString lastVal;
  QStringList Values;
  QStringList MCAValues;

  int LocalStage;
  int lastSetV;

 private:
  bool TypeCHK( int pm, int pz, int cnt, int pam, int enc, int ssd, int ssdp );

public:
  AUnit( QObject *parent = 0 );

  void Initialize( Stars *S );
  void setGType( QString gtype ) { GType = gtype; };
  void setStars( Stars *S ) { s = S; };
  void setType( QString type ) { Type = type; };
  void setID( QString id ) { ID = id; };
  void setUid( QString uid ) { Uid = uid; };
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
  void setIsBusy( bool busy ) { IsBusy = busy; };
  void setIsInt( bool isInt ) { IsInt = isInt; };
  void setHasParent( bool hasParent ) { HasParent = hasParent; };
  void setParent( QString pUid ) { PUid = pUid; };
  void setTheParent( AUnit *p ) { theParent = p; };

  bool checkNewVal( void )
  {
    bool rv = ( Value != lastVal );
    lastVal = Value;
    return rv;
  };

  double u2p( double u ) { return u / UPP + Center; };
  double p2u( double p ) { return ( p - Center ) * UPP; };
  double any2p( double a, int selU, RELABS ra ) {
    return a / ( ( selU == 0 ) ? 1 : UPP )
      + ( ( ra == REL ) ? Value.toDouble() : ( ( selU == 0 ) ? 0 : Center ) );
  }

  // only for PM
  void setUPP( QString upp ) { UPP = upp.toDouble(); };
  void setCenter( QString center ) { Center = center.toDouble(); };

  // only for PZ
  void setMaxV( QString maxv ) { MaxV = maxv.toDouble(); };
  void setMinV( QString minv ) { MinV = minv.toDouble(); };

  QString getGType( void ) { return GType; };
  QString getType( void ) { return Type; };
  QString getID( void ) { return ID; };
  QString getUid( void ) { return Uid; };
  QString getName( void ) { return Name; };
  QString getDriver( void ) { return Driver; };
  QString getCh( void ) { return Ch; };
  QString getDevCh( void ) { return DevCh; };
  QString getUnit( void ) { return Unit; };
  bool isBusy( void ) { return IsBusy; };
  bool isBusy2( void ) { return IsBusy2; };
  QString value( void ) { return Value; };
  QStringList values( void ) { return Values; };
  QStringList MCAvalues( void ) { return MCAValues; };
  double getUPP( void ) { return UPP; };
  bool isInt( void ) { return IsInt; };
  bool hasParent( void ) { return HasParent; };
  QString getPUid( void ) { return PUid; };
  AUnit *getTheParent( void ) { return theParent; };

  int getLastSetV( void ) { return lastSetV; };

  // only for PM
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
  bool GetMCA( int ch );

public slots:
  void ClrBusy( SMsg msg );
  void SetIsBusyByMsg( SMsg msg );
  void SetCurPos( SMsg msg );
  void ReceiveValues( SMsg msg );

  void ReactGetMCA( SMsg msg );

signals:
  //  void CountFinished( void );
  void newValue( QString value );
};

#endif


