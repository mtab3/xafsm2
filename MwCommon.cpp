#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupCommonArea( void )   /* 共通エリア */
{
  QString buf;

  u->setD( mccd[ selmc->MC() ]->getD() );     // default D is Si (111) 

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
  //  PT->SetCheckable( true );
  //  PT->ShowAllNoneBs( true );
  for ( int i = 0; i < AtomGroups.count(); i++ ) {
    PT->SetAGColor( AtomGroups.at(i) );
  }

  connect( SelectTA, SIGNAL( activated( int ) ), this, SLOT( NewSelA( int ) ) );
  connect( SelectTE, SIGNAL( activated( int ) ), this, SLOT( NewSelE( int ) ) );
  connect( ManTEdeg, SIGNAL( editingFinished() ), SLOT( ManSelTEdeg() ) );
  connect( ManTEkeV, SIGNAL( editingFinished() ), SLOT( ManSelTEkeV() ) );
  connect( ShowPT, SIGNAL( clicked() ), PT, SLOT( show() ) );
  connect( PT, SIGNAL( AtomSelected( int ) ), this, SLOT( NewSelA( int ) ) );

  connect( HideCTRLPanel, SIGNAL( clicked( bool ) ), this, SLOT( HideT( bool ) ) );
  connect( HideGraphView, SIGNAL( clicked( bool ) ), this, SLOT( HideB( bool ) ) );

  GSBs << GSB01 << GSB02 << GSB03 << GSB04 << GSB05
       << GSB06 << GSB07 << GSB08 << GSB09 << GSB10
       << GSB11 << GSB12 << GSB13 << GSB14 << GSB15
       << GSB16 << GSB17 << GSB18 << GSB19 << GSB20
       << GSB21 << GSB22 << GSB23 << GSB24;

  for ( int i = 0; i < GSBs.count(); i++ ) {
    connect( GSBs[i], SIGNAL( toggled( bool ) ), this, SLOT( SelectAGB( bool ) ) );
  }
}

void MainWindow::saveGSBs( int ch )
{
  for ( int i = 0; i < GSBs.count(); i++ ) {
    ViewCtrls[ch]->setGSBStat( i, GSBs[i]->isChecked() );
  }
}

void MainWindow::loadGSBs( int ch )
{
  for ( int i = 0; i < GSBs.count(); i++ ) {
    GSBs[i]->setText( ViewCtrls[ch]->getAGSBSLabel( i ) );
    GSBs[i]->setChecked( ViewCtrls[ch]->getAGSBSStat( i ) );
  }
}

void MainWindow::clearGSBs( void )
{
  for ( int i = 0; i < GSBs.count(); i++ ) {
    GSBs[i]->setText( "" );
    GSBs[i]->setChecked( PBFalse );
  }
}

void MainWindow::SetGSBFlags( QVector<bool> flgs )
{
  for ( int i = 0; i < flgs.count(); i++ ) {
    GSBs[i]->setChecked( flgs[i] );
  }
}

void MainWindow::SetGSBLabels( QStringList lbls )
{
  for ( int i = 0; i < lbls.count(); i++ ) {
    GSBs[i]->setText( lbls[i] );
  }
}

void MainWindow::SelectAGB( bool f )
{
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getDType() != MEASDATA )
    return;
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getView() == NULL )
    return;
  
  for ( int i = 0; i < GSBs.count(); i++ ) {
    if ( sender() == GSBs[i] ) {
      ((XYView*)(ViewCtrls[ ViewTab->currentIndex() ]->getView()))
	->ChooseAG( i, f == PBTrue );
    }
  }
}

void MainWindow::HideT( bool f ) 
{
  if ( f ) {
    MainTab->hide();
  } else {
    MainTab->show();
  }
}

void MainWindow::HideB( bool f ) 
{
  if ( f ) {
    ViewTab->hide();
    GraphSwitchBox->hide();
  } else {
    ViewTab->show();
    GraphSwitchBox->show();
  }
}


void MainWindow::NewSelA( int i )
{
  if ( u->keV2deg( Vic[ i ].AE[ SelectedE ] ) > 0 ) {
    SelectedA = (AtomNo)i;
    SelectTA->setCurrentIndex( i );
    ShowTAE();
    SetNewGos();
  } else {
    // エネルギーが範囲外だった場合、
    if ( SelectedE != Kedge ) {   // もし L エッジを選択していたら、K エッジを試してみる。
      if ( u->keV2deg( Vic[ i ].AE[ Kedge ] ) > 0 ) {
	SelectedA = (AtomNo)i;
	SelectedE = Kedge;
	SelectTA->setCurrentIndex( i );
	SelectTE->setCurrentIndex( Kedge );
	ShowTAE();
	SetNewGos();
	return;
      }
    } else {  // もし K エッジを選択していたら LIII を試してみる
      if ( u->keV2deg( Vic[ i ].AE[ LIIIedge ] ) > 0 ) {
	SelectedA = (AtomNo)i;
	SelectedE = LIIIedge;
	SelectTA->setCurrentIndex( i );
	SelectTE->setCurrentIndex( LIIIedge );
	ShowTAE();
	SetNewGos();
	return;
      }
    } // それでもダメならエラー表示して何もしない
    statusbar->showMessage( tr( "Absorption edge of the atom is out of range." ), 5000 );
    SelectTA->setCurrentIndex( SelectedA );
  }
}

void MainWindow::NewSelE( int i )
{
  if ( u->keV2deg( Vic[ SelectedA ].AE[ i ] ) > 0 ) {
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
  
  buf.sprintf( UnitName[DEG].form, u->keV2deg( Vic[SelectedA].AE[SelectedE] ) );
  ManTEdeg->setText( buf );
  buf.sprintf( UnitName[KEV].form, Vic[SelectedA].AE[SelectedE] );
  ManTEkeV->setText( buf );
}

void MainWindow::ManSelTEdeg( void )
{
  QString buf;

  buf.sprintf( UnitName[KEV].form, u->deg2keV( ManTEdeg->text().toDouble() ) );
  ManTEkeV->setText( buf );
  SetNewGos();
}

void MainWindow::ManSelTEkeV( void )
{
  QString buf;

  buf.sprintf( UnitName[DEG].form, u->keV2deg( ManTEkeV->text().toDouble() ) );
  ManTEdeg->setText( buf );
  SetNewGos();
}

void MainWindow::OpenPT( void )
{
  PT->show();
}

void MainWindow::MoveCurThPosKeV( double keV ) // 分光器の移動指令(keV単位で位置指定)
{
  MMainTh->setIsBusy( true );

  if (( u->keV2any( EV, keV ) < MinEnergyInEV )
      ||( u->keV2any( EV, keV ) > MaxEnergyInEV )) {
    statusbar
      ->showMessage( tr( "The position to go [%1]eV is out of range. [%2]-[%3]eV" )
		     .arg( u->keV2any( EV, keV ) )
		     .arg( MinEnergyInEV )
		     .arg( MaxEnergyInEV ), 2000 );
    return;
  }
    
  if ( SelThEncorder->isChecked() ) {
    MMainTh->SetValue( ( u->keV2deg( keV ) - EncMainTh->value().toDouble() )
		       / MMainTh->getUPP() + MMainTh->value().toInt() );
  } else {
    MMainTh->SetValue( u->keV2deg( keV ) / MMainTh->getUPP() + MMainTh->getCenter() );
  }
}
