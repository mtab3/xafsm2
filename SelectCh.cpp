

#include "SelectCh.h"

SelectCh::SelectCh( QWidget *p ) : QWidget( p )
{
  setupUi( this );

  excMode = true;

  Bs0 << B00 << B01 << B02 << B03 << B04 << B05 << B06 << B07 << B08 << B09
     << B10 << B11 << B12 << B13 << B14 << B15 << B16 << B17 << B18;
  
  for ( int i = 0; i < Bs0.count(); i++ ) {
    Bs0[i]->setStyleSheet( ActiveStyle );
    Bs0[i]->setToolTip( tr( "Active" ) );
    Bs0[i]->setChecked( false );                // false は選択された状態
    connect( Bs0[i], SIGNAL( clicked() ), this, SLOT( clicked() ),
	     Qt::UniqueConnection );
  }
  setType( SSD19CH );
}

void SelectCh::setType( TYPES Type )
{
  type = Type;
  for ( int i = 0; i < Bs0.count(); i++ ) {
    Bs0[i]->setHidden( true );
  }
  switch( type ) {
  case SSD19CH:
    Bs.clear();
    Bs << B00 << B01 << B02
       << B03 << B04 << B05 << B06
       << B07 << B08 << B09 << B10 << B11
       << B12 << B13 << B14 << B15
       << B16 << B17 << B18;
    break;
  case SDD7CH:
    Bs.clear();
    Bs << B00 << B01
       << B03 << B04 << B05
       << B08 << B09;
    break;
  }
  for ( int i = 0; i < Bs.count(); i++ ) {
    Bs[i]->setHidden( false );
    Bs[i]->setText( QString::number( i ) );
  }
}

bool SelectCh::isSelected( int i )
{
  if ( ( i < 0 )||( i >= Bs.count() ) )
    return false;
  return ! Bs[i]->isChecked();     // Button On の時「選択されていない」
}

void SelectCh::clicked( void )
{
  for ( int i = 0; i < Bs.count(); i++ ) {
    if ( sender() == Bs[i] )
      selACh( i );
  }
}

void SelectCh::selACh( int ch )
{
  for ( int i = 0; i < Bs.count(); i++ ) {
    if ( i == ch ) {
      if ( excMode ) {
	Bs[i]->setChecked( false );        // false は選択状態
	Bs[i]->setStyleSheet( ActiveStyle );
	Bs[i]->setToolTip( tr( "Active" ) );
	emit selectedSingleCh( i );
      } else {
	if ( ! Bs[i]->isChecked() ) {        // false は選択された状態
	  Bs[i]->setStyleSheet( ActiveStyle );
	  Bs[i]->setToolTip( tr( "Active" ) );
	  emit selectedACh( i, true );
	} else {
	  Bs[i]->setStyleSheet( inActiveStyle );
	  Bs[i]->setToolTip( tr( "Inactive" ) );
	  emit selectedACh( i, false );
	}
      }
    } else {
      if ( excMode ) {
	Bs[i]->setChecked( true );        // ture は非選択状態
	Bs[i]->setStyleSheet( inActiveStyle );
	Bs[i]->setToolTip( tr( "Inactive" ) );
      }
    }
  }
}
