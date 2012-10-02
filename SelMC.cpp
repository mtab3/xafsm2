#include <QtGui>

#include "SelMC.h"


SelMC::SelMC() : QDialog()
{
  setupUi( this );

  SelectedCry = Si111;
  for ( int i = 0; i < MCCS; i++ ) {
    SelectMonoCry->addItem( mccd[i].MCName );
  }
  SelectMonoCry->setCurrentIndex( SelectedCry );
  SelectMonoCry->setEnabled( false );
  ShowMCLC->setText( QString( tr( "%1 A" ) ).arg( mccd[ SelectedCry ].d ) );

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
  SelectedCry = (MCC)index;
  SelectMonoCry->setEnabled( false );
  ShowMCLC->setText( QString( tr( "%1 A" ) ).arg( mccd[ SelectedCry ].d ) );
  emit NewLatticeConstant( mccd[ SelectedCry ].d );
  emit NewLogMsg( QString( tr( "Mono: Cnaged to %1 (%2 A)\n" ) )
		  .arg( mccd[ SelectedCry ].MCName ).arg( mccd[ SelectedCry ].d ) );
}
