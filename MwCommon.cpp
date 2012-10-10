#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupCommonArea( void )   /* ã§í ÉGÉäÉA */
{
  QString buf;

  GoUnit << GoUnit1 << GoUnit2 << GoUnit3 << GoUnit4;
  GoPosEdit << GoPos1 << GoPos2 << GoPos3 << GoPos4;

  MonoCryD = mccd[ selMC->MC() ].d;     // Si (111) 

  SelectedA = Cu;
  SelectedE = Kedge;
  ShowTAE();

  for ( int i = 0; i < ATOMS; i++ ) {
    buf = QString( "%1 %2" ).arg( i, 2 ).arg( A[i].AName );
    SelectTA->addItem( buf, QVariant( i ) );
  }
  SelectTA->setCurrentIndex( SelectedA );

  for ( int i = 0; i < Edges; i++ ) {
    SelectTE->addItem( QString( AbENames[i] ), QVariant( i ) );
  }
  SelectTE->setCurrentIndex( SelectedE );

  PT = new PeriodicTable;
  PT->SetActionOnSelected( PT_STAY );
  PT->SetActionOnClosed( PT_CLOSE );

  connect( SelectTA, SIGNAL( activated( int ) ), this, SLOT( NewSelA( int ) ) );
  connect( SelectTE, SIGNAL( activated( int ) ), this, SLOT( NewSelE( int ) ) );
  connect( ManTEdeg, SIGNAL( editingFinished() ), SLOT( ManSelTEdeg() ) );
  connect( ManTEkeV, SIGNAL( editingFinished() ), SLOT( ManSelTEkeV() ) );
  connect( ShowPT, SIGNAL( clicked() ), PT, SLOT( show() ) );
  connect( PT, SIGNAL( AtomSelected( int ) ), this, SLOT( AtomSelectedByPT( int ) ) );
}

void MainWindow::NewSelA( int i )
{
  if ( keV2deg( Vic[ i ].AE[ SelectedE ] ) > 0 ) {
    SelectedA = (AtomNo)i;
    ShowTAE();
    SetNewGos();
  } else {
    statusbar->showMessage( tr( "Absorption edge of the atom is out of range." ), 5000 );
    SelectTA->setCurrentIndex( SelectedA );
  }
}

void MainWindow::NewSelE( int i )
{
  if ( keV2deg( Vic[ SelectedA ].AE[ i ] ) > 0 ) {
    SelectedE = (AbEN)i;
    ShowTAE();
    SetNewGos();
  } else {
    statusbar->showMessage( tr( "Absorption edge of the atom is out of range." ), 5000 );
    SelectTE->setCurrentIndex( SelectedE );
  }
}

void MainWindow::ShowTAE( void )
{
  QString buf;
  
  buf.sprintf( UnitName[DEG].form, keV2deg( Vic[SelectedA].AE[SelectedE] ) );
  ManTEdeg->setText( buf );
  buf.sprintf( UnitName[KEV].form, Vic[SelectedA].AE[SelectedE] );
  ManTEkeV->setText( buf );
}

void MainWindow::ManSelTEdeg( void )
{
  QString buf;

  buf.sprintf( UnitName[KEV].form, deg2keV( ManTEdeg->text().toDouble() ) );
  ManTEkeV->setText( buf );
  SetNewGos();
}

void MainWindow::ManSelTEkeV( void )
{
  QString buf;

  buf.sprintf( UnitName[DEG].form, keV2deg( ManTEkeV->text().toDouble() ) );
  ManTEdeg->setText( buf );
  SetNewGos();
}

void MainWindow::OpenPT( void )
{
  PT->show();
}

void MainWindow::AtomSelectedByPT( int i )
{
  NewSelA( i );
  SelectTA->setCurrentIndex( i );
}
