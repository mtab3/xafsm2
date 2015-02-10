#ifndef STATUS_H
#define STATUS_H


#include "ui_ScrollWGrid.h"
#include "AUnits/AUnits.h"
#include "StarsSV2.h"
#include "SelMC2.h"
#include "Conditions.h"
#include "PMConditions.h"

class Status : public QScrollArea, private Ui::ScrollWGrid
{
  Q_OBJECT

  QFrame *DrvsFrame;
  QGridLayout *DrvsGrid;
  StarsSV2 *starsSV;
  SelMC2 *selMC;
  Conditions *conds;

  QVector<AUnit0*> drivers;
  QStringList Drivers;
  QVector<QVector<AUnit0*>*> DrvUnits;

  bool SWactive;

  QLabel *l1;
  QVector<QLabel*> LENBLs, LIB1s, LIB2s;  // Label-Enable, -IsBusy1, -IsBusy2
  QVector<QPushButton*> CEBs, CBBs;       // Clear-Enable-Buttons, Clear-Busy-Buttons
  QVector<QComboBox*> IBBx1s, IBBx2s;     // Is-Busy1,2-Box
  QString OKcolor, NGcolor;
  QLabel *SSAddr, *SSPort, *SSStat;

  QLineEdit *EncV;
  QPushButton *EncB;
  QString OrigV;

public:
  Status( QWidget *parent = 0 );

  void setupStatArea( QVector<AMotor*> *ams, QVector<ASensor*> *ass,
		      StarsSV2 *starsSV, SelMC2 *selMC,
		      Conditions *conds, PMConditions *pmConds );

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

  void newEncTh( QString th );
  void setEnc( void );

  void showOptionBox( void );

signals:
  void NeedListNodes( void );
  void setEncNewTh( QString Orig, QString newv );
};

#endif
