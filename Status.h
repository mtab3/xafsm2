#ifndef STATUS_H
#define STATUS_H

#include <QtGui>

#include "ui_ScrollWGrid.h"
#include "AUnit.h"
#include "StarsSV2.h"
#include "SelMC2.h"

class Status : public QScrollArea, private Ui::ScrollWGrid
{
  Q_OBJECT

  QFrame *DrvsFrame;
  QGridLayout *DrvsGrid;
  StarsSV2 *starsSV;
  SelMC2 *selMC;

  QVector<AUnit*> drivers;
  QStringList Drivers;
  QVector<QVector<AUnit*>*> DrvUnits;

  bool SWactive;

  QLabel *l1;
  QVector<QLabel*> LENBLs, LIB1s, LIB2s;  // Label-Enable, -IsBusy1, -IsBusy2
  QVector<QPushButton*> CEBs, CBBs;       // Clear-Enable-Buttons, Clear-Busy-Buttons
  QVector<QComboBox*> IBBx1s, IBBx2s;     // Is-Busy1,2-Box
  QString OKcolor, NGcolor;
  QLabel *SSAddr, *SSPort, *SSStat;

public:
  Status( QWidget *parent = 0 );

  void setupStatArea( QVector<AUnit*> *ams, QVector<AUnit*> *ass,
		      StarsSV2 *starsSV, SelMC2 *selMC );

public slots:
  //  void SetSSVA( QString Server );
  //  void SetSSVP( qint16 Port );
  void SetSSVStat( bool f );

  void SelStatWatch( void );
  void OnEnabled( QString Driver, bool flag );
  void OnChangedIsBusy1( QString Drv );
  void OnChangedIsBusy2( QString Drv );
  void OnClear1( void );
  void OnClear2( void );

signals:
  void NeedListNodes( void );
};

#endif
