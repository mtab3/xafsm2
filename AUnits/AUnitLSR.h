
#ifndef AUNITLSR_H
#define AUNITLSR_H

#include "ASensor.h"

class AUnitLSR : public ASensor
{
  Q_OBJECT

 public:
  AUnitLSR( void );
  void init0( void );

 private slots:
  void OnReportCurrent( SMsg msg );
  void OnReportInjection( SMsg msg );

 signals:
  void NewRingCurrent( QString val, QStringList vals );
  void NewInjectionReport( QString val, QStringList vals );
};

#endif
