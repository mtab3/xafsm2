#ifndef SELMC_H
#define SELMC_H

#include "ui_SelMC.h"
#include "XafsM.h"
#include "Mccd.h"

class QMessageBox;

class SelMC : public QDialog, private Ui::SelMC
{
  Q_OBJECT

public:
  SelMC( QVector<MCCD*> &Mccd );

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

