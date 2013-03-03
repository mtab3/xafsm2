#include <QtGui>

#include "SelMC.h"
#include "Mccd.h"

SelMC::SelMC( QVector<MCCD*> &Mccd ) : QDialog()
{
  setupUi( this );

  mccd = Mccd;

  SelectedCry = 0;
  for ( int i = 0; i < mccd.count(); i++ ) {
    SelectMonoCry->addItem( mccd[i]->getMCName() );
  }
  SelectMonoCry->setCurrentIndex( SelectedCry );
  SelectMonoCry->setEnabled( false );
  ShowMCLC->setText( QString( tr( "%1 A" ) ).arg( mccd[ SelectedCry ]->getD() ) );
  
  IsReallyChgMC = new QMessageBox;
  QPushButton *tmpB
    = IsReallyChgMC->addButton( tr( "Cancel" ), QMessageBox::RejectRole );
  IsReallyChgMC->addButton( tr( "OK" ), QMessageBox::AcceptRole );
  IsReallyChgMC->setText( tr( "<h1><center>Do you really change it ?</center></h1>" ) );
  IsReallyChgMC->setWindowTitle( tr( "Really Change?" ) );
  IsReallyChgMC->setDefaultButton( tmpB );

  connect( ChgMonoCry, SIGNAL( clicked() ), IsReallyChgMC, SLOT( show() ) );
  connect( IsReallyChgMC, SIGNAL( accepted() ), this, SLOT( SurelyChgMC() ) );
  connect( SelectMonoCry, SIGNAL( activated( int ) ), this, SLOT( HaveChgMC( int ) ) );
}

void SelMC::SurelyChgMC( void )
{
  SelectMonoCry->setEnabled( true );
}

void SelMC::HaveChgMC( int index )
{
  SelectedCry = index;
  SelectMonoCry->setEnabled( false );
  ShowMCLC->setText( QString( tr( "%1 A" ) ).arg( mccd[ SelectedCry ]->getD() ) );
  emit NewLatticeConstant( mccd[ SelectedCry ]->getD() );
  emit NewLogMsg( QString( tr( "Mono: Cnaged to %1 (%2 A)" ) )
		  .arg( mccd[ SelectedCry ]->getMCName() )
		  .arg( mccd[ SelectedCry ]->getD() ) );
}
