#ifndef CHANGER_H
#define CHANGER_H

#include <QObject>
#include <QString>

class Changer : public QObject
{
  Q_OBJECT

  QString ID;
  QString Name;
  QString UnitID1, UnitID2;
  int Holders1, Holders2;
  int CenterHolder;
  double Spacing1, Spacing2;

 public:
  Changer();
  
  void setID( QString id ) { ID = id; };
  void setName( QString name ) { Name = name; };
  void setUnitId1( QString uid1 ) { UnitID1 = uid1; };
  void setUnitId2( QString uid2 ) { UnitID2 = uid2; };
  void setHolders1( int h1 ) { Holders1 = h1; };
  void setHolders2( int h2 ) { Holders2 = h2; };
  void setCenterHolder( int c ) { CenterHolder = c; };
  void setSpacing1( double s1 ) { Spacing1 = s1; };
  void setSpacing2( double s2 ) { Spacing2 = s2; };

  QString id( void ) { return ID; };
  QString name( void ) { return Name; };
  QString unitID1( void ) { return UnitID1; };
  QString unitID2( void ) { return UnitID2; };
  int holders1( void ) { return Holders1; };
  int holders2( void ) { return Holders2; };
  int cneterHolder( void ) { return CenterHolder; };
  double spacing1( void ) { return Spacing1; };
  double spacing2( void ) { return Spacing2; };
};

#endif
