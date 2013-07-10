#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "ui_Conditions.h"

class Conditions : public QFrame, private Ui::Conditions
{
  Q_OBJECT

public:
  Conditions();

#if 0
  void setEncAsTh( bool encAsTh );
  void setAddInfos( bool addInfos );
#endif

  void setVersionInfo( QString ver, QString date, QString time );
  bool isEncAsTh( void );
  bool isAddInfos( void );
  bool isRecordAllSSDChs( void );
  bool isMakeInfo( void );
  bool isMeasInDeg( void );

 signals:
  void SetDXMPMC( void );
};

#endif

