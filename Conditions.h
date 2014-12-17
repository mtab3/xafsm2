#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "ui_Conditions.h"

class Conditions : public QFrame, private Ui::Conditions
{
  Q_OBJECT

public:
  Conditions();

  void setVersionInfo( QString ver, QString date, QString time );
  bool isEncAsTh( void );
  bool isAddInfos( void );
  bool isRecordAllSSDChs( void );
  bool isRecordEachMCASpectrum( void );
  void RecordEachMCASpectrumEnabled( bool f );
  bool isMakeInfo( void );
  bool isMeasInDeg( void );
  bool isUseDTh1TTable( void );
  int Diff1Type( void );
  int Diff2Type( void );

  bool tuneESAbs( void );
  double tuneESStart( void );
  double tuneESEnd( void );
  int tuneESSteps( void );
  bool tuneESQuick( void );
  double tuneESQuickTime( void );

  bool I0ShouldBeChecked( void );
  double I0Threshold( void );

 signals:
  void SetDXMPMC( void );
  void AskToSaveDTh1TTable( void );
  void AskToShowDTh1TTable( void );
  void NewDiff1( int i );
  void NewDiff2( int i );
};

#endif

