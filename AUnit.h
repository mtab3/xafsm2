#ifndef AMOTOR_H
#define AMOTOR_H

#include <QObject>

class AUnit : public QObject
{
  Q_OBJECT

  QString Type;
  QString ID;
  QString Name;
  QString Driver;
  QString Ch;
  QString DevCh;
  QString Unit;

  double PPU;           // only for PM
  double Center;        // only for PM

  double MaxV;          // only for PZ
  double MinV;          // only for PZ

public:
  AUnit( QObject *parent = 0 );

  void setType( QString type ) { Type = type; };
  void setID( QString id ) { ID = id; };
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

  // only for PM
  void setPPU( QString ppu ) { PPU = ppu.toDouble(); };
  void setCenter( QString center ) { Center = center.toDouble(); };

  // only for PZ
  void setMaxV( QString maxv ) { MaxV = maxv.toDouble(); };
  void setMinV( QString minv ) { MinV = minv.toDouble(); };

  QString getID( void ) { return ID; };

  void show( void );
};

#endif


