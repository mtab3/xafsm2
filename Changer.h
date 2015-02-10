#ifndef CHANGER_H
#define CHANGER_H

#include <QObject>
#include <QString>

#include "AUnits/AMotor.h"

class Changer : public QObject
{
  Q_OBJECT

  QString ID;
  QString Name;
  QString UnitID1, UnitID2;
  AMotor *Unit1, *Unit2;
  int Holders1, Holders2;
  double Center1, Center2;
  double Spacing1, Spacing2;
  int Dir1, Dir2;
  int Width1, Width2;

 public:
  Changer();
  
  void setID( QString id ) { ID = id; };
  void setName( QString name ) { Name = name; };
  void setUnitId1( QString uid1 ) { UnitID1 = uid1; };
  void setUnitId2( QString uid2 ) { UnitID2 = uid2; };
  void setUnit1( AMotor *u1 ) { Unit1 = u1; };
  void setUnit2( AMotor *u2 ) { Unit2 = u2; };
  void setHolders1( int h1 ) { Holders1 = h1; };
  void setHolders2( int h2 ) { Holders2 = h2; };
  void setCenter1( double c1 ) { Center1 = c1; };
  void setCenter2( double c2 ) { Center2 = c2; };
  void setSpacing1( double s1 ) { Spacing1 = s1; };
  void setSpacing2( double s2 ) { Spacing2 = s2; };
  void setDir1( int d1 ) { Dir1 = d1; };
  void setDir2( int d2 ) { Dir2 = d2; };
  void setWidth1( int w1 ) { Width1 = w1; };
  void setWidth2( int w2 ) { Width2 = w2; };

  QString id( void ) { return ID; };
  QString name( void ) { return Name; };
  QString unitID1( void ) { return UnitID1; };
  QString unitID2( void ) { return UnitID2; };
  int holders1( void ) { return Holders1; };
  int holders2( void ) { return Holders2; };
  int holders( void ) { return Holders1 * Holders2; };
  double center1( void ) { return Center1; };
  double center2( void ) { return Center2; };
  double spacing1( void ) { return Spacing1; };
  double spacing2( void ) { return Spacing2; };
  AMotor *unit1( void ) { return Unit1; };
  AMotor *unit2( void ) { return Unit2; };
  int dir1( void ) { return Dir1; };
  int dir2( void ) { return Dir2; };
  int width1( void ) { return Width1; };
  int width2( void ) { return Width2; };
};

#endif
