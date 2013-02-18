#ifndef STARSSV2_H
#define STARSSV2_H

#include "ui_StarsSV2.h"

#define SSVHISTORY  "SSVHistory.dat"

class StarsSV2 : public QFrame, private Ui::StarsSV2
{
  Q_OBJECT

  void ReadHistory( void );

public:
  StarsSV2();
  QString SSVAddress( void ) { return StarsSVA->currentText(); };
  QString SSVPort( void ) { return StarsSVP->currentText(); };
  QLabel *sStat() { return SSStat; };

private slots:
  void RecordSSVHistoryA( const QString &item );
  void RecordSSVHistoryP( const QString &item );

signals:
  void SSVNewAddress( QString newA );
  void SSVNewPort( QString newP );
  void AskReConnect( void );
};

#endif
