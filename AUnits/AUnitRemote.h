#ifndef AUNITREMOTE_H
#define AUNITREMOTE_H

#include "../SMsg.h"
#include "AUnit0.h"

class AUnitRemote : public AUnit0
{
  Q_OBJECT

public:
  AUnitRemote( void );
  void init( void );

private slots:
  void RcvStart( SMsg msg );
  void RcvStop( SMsg msg );

signals:
  void setMeasBlockF( bool f );  
};

#endif
