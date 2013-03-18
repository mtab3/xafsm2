#ifndef GSBSTAT_H
#define GSBSTAT_H

#include <QPushButton>
#include <QString>

#include "XafsM.h"

struct aGSBS {
  QString label;
  bool stat;
};

class GSBStats : public QObject
{
  Q_OBJECT

  QVector<aGSBS*> GSBSs;

 public:
  GSBStats( void ) {};

  void init( int i );
  void clear( void );
  void addAStat( QString Label = "", bool Stat = PBFalse );
  void setAStat( int n, QString Label, bool Stat );
  void setAStat( int n, bool Stat );
  QString getALabel( int n );
  bool getAStat( int n );
  int count( void );
  QStringList labels( void );
  QVector<bool> flags( void );
};


#endif
