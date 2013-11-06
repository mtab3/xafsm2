#ifndef SPECCHANGER_H
#define SPECCHANGER_H

#include <QObject>
#include <QString>
#include <QVector>

#include "Changer.h"

class SpecName : public QObject
{
  Q_OBJECT

  int Position;
  QString Name;
  QStringList Attribs;

 public:
  SpecName() { Position = 0; Name = ""; Attribs.clear(); };

  void setPosition( int p ) { Position = p; };
  void setName( QString name ) { Name = name; };
  void addAttrib( QString attr ) { Attribs << attr; };

  int position( void ) { return Position; };
  QString name( void ) { return Name; };
  QString attrib( int i )
  { if ( i < Attribs.count() ) return Attribs[i]; else return ""; };
};

class SpecChanger : public QObject
{
  Q_OBJECT

  QString ID;
  QString Name;
  QString BaseUnitID;
  Changer *BaseUnit;
  QVector<SpecName*> SpecNames;

 public:
  SpecChanger()
    { ID = ""; Name = ""; BaseUnitID = ""; BaseUnit = NULL; SpecNames.clear(); };
  
  void setID( QString id ) { ID = id; };
  void setName( QString name ) { Name = name; };
  void setBaseChangerID( QString id ) { BaseUnitID = id; };
  void setBaseChangerP( Changer *p ) { BaseUnit = p; };
  void addSpecName( SpecName *p ) { SpecNames << p; };

  QString id( void ) { return ID; };
  QString name( void ) { return Name; };
  QString baseChangerId( void ) { return BaseUnitID; };
  Changer *baseChangerP( void ) { return BaseUnit; };
  int specNames( void ) { return SpecNames.count(); };
  SpecName *specName( int i )
  { if ( i < SpecNames.count() ) return SpecNames[i]; else return NULL; }
};

#endif
