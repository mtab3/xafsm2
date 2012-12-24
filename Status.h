#ifndef STATUS_H
#define STATUS_H

#include <QtGui>

#include "ui_FrameWGrid.h"
#include "AUnit.h"

class Status : public QFrame, private Ui::FrameWGrid
{
  Q_OBJECT

  QVector<AUnit*> *ams;
  QVector<AUnit*> *ass;
  QStringList Drivers;
  QVector<QVector<AUnit*>*> DrvUnits;

  bool SWactive;

  QLabel *l1;
  QVector<QLabel*> LENBLs, LIB1s, LIB2s;  // Label-Enable, -IsBusy1, -IsBusy2
  QVector<QPushButton*> CEBs, CBBs;       // Clear-Enable-Buttons, Clear-Busy-Buttons
  QVector<QComboBox*> IBBx1s, IBBx2s;     // Is-Busy1,2-Box
  QString OKcolor, NGcolor;

public:
  Status( QWidget *parent = 0 );

  void setupStatArea( QVector<AUnit*> *ams, QVector<AUnit*> *ass );

public slots:
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
