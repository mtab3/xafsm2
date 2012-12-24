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

  QLabel *l1;
  QVector<QLabel*> LENBLs, LIB1s, LIB2s;
  QVector<QPushButton*> CBBs;
  QVector<QComboBox*> IBBx1s, IBBx2s;
  QString OKcolor, NGcolor;

public:
  Status( QWidget *parent = 0 );

  void setupStatArea( QVector<AUnit*> *ams, QVector<AUnit*> *ass );

public slots:
  void OnEnabled( QString Driver, bool flag );
  void OnChangedIsBusy1( QString Drv );
  void OnChangedIsBusy2( QString Drv );
  void OnClear( void );
};

#endif
