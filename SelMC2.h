#ifndef SELMC2_H
#define SELMC2_H

#include "ui_SelMC2.h"
#include "XafsM.h"
#include "Mccd.h"

class QMessageBox;

class SelMC2 : public QFrame, private Ui::SelMC2
{
  Q_OBJECT

public:
  SelMC2( QVector<MCCD*> &Mccd );

  void SetMC( int crystal ) {  SelectedCry = crystal; };
  int MC( void ) { return SelectedCry; };

private:
  int SelectedCry;
  QMessageBox *IsReallyChgMC;
  QVector<MCCD*> mccd;

private slots:
  void SurelyChgMC( void );
  void HaveChgMC( int index );

signals:
  void NewLogMsg( QString msg );
  void NewLatticeConstant( double d );
};

#endif

