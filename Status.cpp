#include "Status.h"

Status::Status( QWidget *p ) : QFrame( p )
{
  setupUi( this );
}

void Status::setupStatArea( QVector<AUnit*> *Ams, QVector<AUnit*> *Ass )
{
  ams = Ams;
  ass = Ass;

  OKcolor = "background-color: #aaffaa";
  NGcolor = "background-color: #ffaaaa";

  for ( int i = 0; i < ams->count(); i++ ) {
    Drivers << ams->at(i)->getDriver();
    connect( ams->at(i), SIGNAL( Enabled( QString, bool ) ),
	     this, SLOT( OnEnabled( QString, bool ) ) );
    connect( ams->at(i), SIGNAL( ChangedIsBusy1( QString ) ),
	     this, SLOT( OnChangedIsBusy1( QString ) ) );
    connect( ams->at(i), SIGNAL( ChangedIsBusy2( QString ) ),
	     this, SLOT( OnChangedIsBusy2( QString ) ) );
  }
  for ( int i = 0; i < ass->count(); i++ ) {
    Drivers << ass->at(i)->getDriver();
    connect( ass->at(i), SIGNAL( Enabled( QString, bool ) ),
	     this, SLOT( OnEnabled( QString, bool ) ) );
    connect( ass->at(i), SIGNAL( ChangedIsBusy1( QString ) ),
	     this, SLOT( OnChangedIsBusy1( QString ) ) );
    connect( ass->at(i), SIGNAL( ChangedIsBusy2( QString ) ),
	     this, SLOT( OnChangedIsBusy2( QString ) ) );
  }
  Drivers.removeDuplicates();

  QRadioButton *RB0 = new QRadioButton;
  RB0->setText( ": Status Watch active" );
  RB0->setCheckable( true );
  RB0->setChecked( true );
  MainGrid->addWidget( RB0, 0, 0, 1, 2 );
  SWactive = true;
  connect( RB0, SIGNAL( clicked() ), this, SLOT( SelStatWatch() ) );

  QLabel *TT;
  QVector<QLabel*> TTs;
  QString TBack = "background-color: #eeffee";
  TT = new QLabel; TT->setText( "Drivers" ); TTs << TT;
  TT = new QLabel; TT->setText( "Devices" ); TTs << TT;
  TT = new QLabel; TT->setText( "Enable" ); TTs << TT;
  TT = new QLabel; TT->setText( "Clear Enable" ); TTs << TT;
  TT = new QLabel; TT->setText( "IsBusy" ); TTs << TT;
  TT = new QLabel; TT->setText( "Busy Units" ); TTs << TT;
  TT = new QLabel; TT->setText( "IsBusy2" ); TTs << TT;
  TT = new QLabel; TT->setText( "Busy2 Units" ); TTs << TT;
  TT = new QLabel; TT->setText( "Clear Busys" ); TTs << TT;
  for ( int i = 0; i < TTs.count(); i++ ) {
    TTs.at(i)->setStyleSheet( TBack );
    TTs.at(i)->setFrameStyle( QFrame::StyledPanel );
    TTs.at(i)->setAlignment( Qt::AlignCenter );
    MainGrid->addWidget( TTs.at(i), 1, i );
  }

  QLabel *L1, *LEnable, *LIsB1, *LIsB2;
  QPushButton *CEB, *CBB;
  QComboBox *CB1, *IBBx1, *IBBx2;
  QString LBack = "background-color: #f8f8e0";
  QString CBack = "background-color: #f0f0f0";
  QString PBBack = "background-color: "
    "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
    "stop:0 rgba(225, 235, 225, 255), stop:1 rgba(255, 255, 255, 255));";
  int col;
  for ( int i = 0; i < Drivers.count(); i++ ) {
    col = 0;
    QVector<AUnit*> *Units = new QVector<AUnit*>;
    for ( int j = 0; j < ams->count(); j++ ) {
      if ( Drivers.at(i) == ams->at(j)->getDriver() )
	*Units << ams->at(j);
    }
    for ( int j = 0; j < ass->count(); j++ ) {
      if ( Drivers.at(i) == ass->at(j)->getDriver() )
	*Units << ass->at(j);
    }
    DrvUnits << Units;

    L1 = new QLabel;
    L1->setText( Drivers.at(i) );
    L1->setStyleSheet( LBack );
    L1->setFrameStyle( QFrame::StyledPanel );
    MainGrid->addWidget( L1, i + 2, col++ );

    CB1 = new QComboBox;
    for ( int j = 0; j < ams->count(); j++ ) {
      if ( ams->at(j)->getDriver() == Drivers.at(i) ) {
	CB1->addItem( ams->at(j)->getName() );
      }
    }
    for ( int j = 0; j < ass->count(); j++ ) {
      if ( ass->at(j)->getDriver() == Drivers.at(i) ) {
	CB1->addItem( ass->at(j)->getName() );
      }
    }
    CB1->setStyleSheet( CBack );
    MainGrid->addWidget( CB1, i+2, col++ );

    LEnable = new QLabel;
    LENBLs << LEnable;
    LEnable->setStyleSheet( NGcolor );
    LEnable->setFrameStyle( QFrame::Box | QFrame::Raised );
    LEnable->setLineWidth( 1 );
    LEnable->setMidLineWidth( 2 );
    LEnable->setMinimumWidth( 20 );
    MainGrid->addWidget( LEnable, i+2, col++, Qt::AlignHCenter );

    CEB = new QPushButton;
    CEBs << CEB;
    CEB->setText( "Clear" );
    CEB->setStyleSheet( PBBack );
    MainGrid->addWidget( CEB, i+2, col++ );
    connect( CEB, SIGNAL( clicked() ), this, SLOT( OnClear1() ) );

    LIsB1 = new QLabel;
    LIB1s << LIsB1;
    LIsB1->setStyleSheet( NGcolor );
    LIsB1->setFrameStyle( QFrame::Box | QFrame::Raised );
    LIsB1->setLineWidth( 1 );
    LIsB1->setMidLineWidth( 2 );
    LIsB1->setMinimumWidth( 20 );
    MainGrid->addWidget( LIsB1, i+2, col++, Qt::AlignHCenter );

    IBBx1 = new QComboBox;
    IBBx1s << IBBx1;
    MainGrid->addWidget( IBBx1, i+2, col++ );

    LIsB2 = new QLabel;
    LIB2s << LIsB2;
    LIsB2->setStyleSheet( NGcolor );
    LIsB2->setFrameStyle( QFrame::Box | QFrame::Raised );
    LIsB2->setLineWidth( 1 );
    LIsB2->setMidLineWidth( 2 );
    LIsB2->setMinimumWidth( 20 );
    MainGrid->addWidget( LIsB2, i+2, col++, Qt::AlignHCenter );

    IBBx2 = new QComboBox;
    IBBx2s << IBBx2;
    MainGrid->addWidget( IBBx2, i+2, col++ );

    CBB = new QPushButton;
    CBBs << CBB;
    CBB->setText( "Clear" );
    CBB->setStyleSheet( PBBack );
    MainGrid->addWidget( CBB, i+2, col++ );
    connect( CBB, SIGNAL( clicked() ), this, SLOT( OnClear2() ) );

    OnChangedIsBusy1( Drivers.at(i) );
    OnChangedIsBusy2( Drivers.at(i) );
  }

  QSizePolicy *HSP, *VSP;
  QLabel *HS, *VS;
  HS = new QLabel;
  VS = new QLabel;
  HSP = new QSizePolicy;
  VSP = new QSizePolicy;
  //  HS->setText( "aa" );
  //  VS->setText( "bb" );
  HSP->setHorizontalPolicy( QSizePolicy::Expanding );
  //  HSP->setHorizontalStretch( 10 );
  VSP->setVerticalPolicy( QSizePolicy::Expanding );
  //  VSP->setVerticalStretch( 10 );
  HS->setSizePolicy( *HSP );
  VS->setSizePolicy( *VSP );
  MainGrid->addWidget( HS, 0, TTs.count() );
  MainGrid->addWidget( VS, Drivers.count() + 2, 0 );
}

void Status::OnEnabled( QString Drv, bool flg )
{
  if ( !SWactive )
    return;

  for ( int i = 0; i < Drivers.count(); i++ ) {
    if ( Drivers.at(i) == Drv ) {
      LENBLs.at(i)->setStyleSheet( flg ? OKcolor : NGcolor );
      break;
    }
  }
}

void Status::OnChangedIsBusy1( QString Drv )
{
  if ( !SWactive )
    return;

  int drv, i;
  for ( drv = 0; drv < Drivers.count(); drv++ ) {
    if ( Drivers.at(drv) == Drv )
      break;
  }
  if ( drv >= Drivers.count() )
    return;

  IBBx1s.at(drv)->clear();
  int cnt = DrvUnits.at(drv)->count();
  for ( i = 0; i < cnt; i++ ) {
    if ( DrvUnits.at(drv)->at(i)->isBusy() ) {
      IBBx1s.at(drv)->addItem( DrvUnits.at(drv)->at(i)->getName() );
    }
  }
  if ( IBBx1s.at(drv)->count() > 0 ) {
    LIB1s.at(drv)->setStyleSheet( NGcolor );
  } else {
    LIB1s.at(drv)->setStyleSheet( OKcolor );
  }
}

void Status::OnChangedIsBusy2( QString Drv )
{
  if ( !SWactive )
    return;

  int drv, i;
  for ( drv = 0; drv < Drivers.count(); drv++ ) {
    if ( Drivers.at(drv) == Drv )
      break;
  }
  if ( drv >= Drivers.count() )
    return;

  IBBx2s.at(drv)->clear();
  int cnt = DrvUnits.at(drv)->count();
  for ( i = 0; i < cnt; i++ ) {
    if ( DrvUnits.at(drv)->at(i)->isBusy2() ) {
      IBBx2s.at(drv)->addItem( DrvUnits.at(drv)->at(i)->getName() );
    }
  }
  if ( IBBx2s.at(drv)->count() > 0 ) {
    LIB2s.at(drv)->setStyleSheet( NGcolor );
  } else {
    LIB2s.at(drv)->setStyleSheet( OKcolor );
  }
}

void Status::OnClear1( void )
{
  int drv;
  QPushButton *b = (QPushButton*)sender();

  for ( drv = 0; drv < CEBs.count(); drv++ ) {
    if ( CEBs.at(drv) == b ) {
      break;
    }
  }
  if ( drv >= CEBs.count() )
    return;

  int cnt = DrvUnits.at(drv)->count();
  for ( int i = 0; i < cnt; i++ ) {
    DrvUnits.at(drv)->at(i)->setEnable( true );
  }
}

void Status::OnClear2( void )
{
  int drv;
  QPushButton *b = (QPushButton*)sender();

  for ( drv = 0; drv < CBBs.count(); drv++ ) {
    if ( CBBs.at(drv) == b ) {
      break;
    }
  }
  if ( drv >= CBBs.count() )
    return;
  
  int cnt = DrvUnits.at(drv)->count();
  for ( int i = 0; i < cnt; i++ ) {
    DrvUnits.at(drv)->at(i)->setIsBusy( false );
    DrvUnits.at(drv)->at(i)->setIsBusy2( false );
  }
}

void Status::SelStatWatch( void )
{
  int cnt;
  SWactive = ((QRadioButton*)sender())->isChecked();
  if ( SWactive ) {
    for ( int i = 0; i < Drivers.count(); i++ ) {
      if ( DrvUnits.at(i)->count() > 0 ) {
	LENBLs.at(i)
	  ->setStyleSheet( DrvUnits.at(i)->at(0)->isEnable() ? OKcolor : NGcolor );
	
	IBBx1s.at(i)->clear();
	cnt = DrvUnits.at(i)->count();
	for ( int j = 0; j < cnt; j++ ) {
	  if ( DrvUnits.at(i)->at(j)->isBusy() ) {
	    IBBx1s.at(i)->addItem( DrvUnits.at(i)->at(j)->getName() );
	  }
	}
	LIB1s.at(i)->setStyleSheet( ( IBBx1s.at(i)->count() > 0 ) ? NGcolor : OKcolor );

	IBBx2s.at(i)->clear();
	cnt = DrvUnits.at(i)->count();
	for ( int j = 0; j < cnt; j++ ) {
	  if ( DrvUnits.at(i)->at(j)->isBusy2() ) {
	    IBBx2s.at(i)->addItem( DrvUnits.at(i)->at(j)->getName() );
	  }
	}
	LIB2s.at(i)->setStyleSheet( ( IBBx2s.at(i)->count() > 0 ) ? NGcolor : OKcolor );

      }
    }
  }
}
