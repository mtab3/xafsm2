#ifndef STARSSV_H
#define STARSSV_H

#include "ui_StarsSV.h"

#define SSVHISTORY  "SSVHistory.dat"

class StarsSV : public QDialog, private Ui::StarsSV
{
  Q_OBJECT

  void ReadHistory( void );

public:
  StarsSV();
  QString SSVAddress( void ) { return StarsSVA->currentText(); };
  QString SSVPort( void ) { return StarsSVP->currentText(); };

private slots:
  void RecordSSVHistoryA( const QString &item );
  void RecordSSVHistoryP( const QString &item );

signals:
  void SSVNewAddress( QString newA );
  void SSVNewPort( QString newP );
  void AskReConnect( void );
};

#endif
