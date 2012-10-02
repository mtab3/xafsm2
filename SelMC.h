#ifndef SELMC_H
#define SELMC_H

#include "ui_SelMC.h"
#include "XafsM.h"

class QMessageBox;

class SelMC : public QDialog, private Ui::SelMC
{
  Q_OBJECT

public:
  SelMC();

  void SetMC( MCC crystal ) {  SelectedCry = crystal; };
  MCC MC( void ) { return SelectedCry; };

private:
  MCC SelectedCry;
  QMessageBox *IsReallyChgMC;

private slots:
  void SurelyChgMC( void );
  void HaveChgMC( int index );

signals:
  void NewLogMsg( QString msg );
  void NewLatticeConstant( double d );
};

#endif

