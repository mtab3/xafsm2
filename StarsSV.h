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

private slots:
  void SSVAnewIndex( const QString &item );
  void SSVPnewIndex( const QString &item );
  void RecordSSVHistoryA( const QString &item );
  void RecordSSVHistoryP( const QString &item );

signals:
  void SSVNewAddress( QString newA );
  void SSVNewPort( QString newP );
};

#endif
