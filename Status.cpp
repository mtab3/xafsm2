#include "Status.h"
#include "StarsSV2.h"

Status::Status( QWidget *p ) : QScrollArea( p )
{
  setupUi( this );
}

void Status::setupStatArea( QVector<AUnit*> *Ams, QVector<AUnit*> *Ass,
			    StarsSV2 *StarsSV, SelMC2 *SelMC, Conditions *Conds )
{
  for ( int i = 0; i < Ams->count(); i++ ) {
    drivers << Ams->at(i);
  }
  for ( int i = 0; i < Ass->count(); i++ ) {
    drivers << Ass->at(i);
  }

  for ( int i = 0; i < drivers.count(); i++ ) {   // XAFSM.def にある全ドライバ名を集める
    Drivers << drivers.at(i)->getDriver();
    connect( drivers.at(i), SIGNAL( Enabled( QString, bool ) ),
	     this, SLOT( OnEnabled( QString, bool ) ) );
    connect( drivers.at(i), SIGNAL( ChangedIsBusy1( QString ) ),
	     this, SLOT( OnChangedIsBusy1( QString ) ) );
    connect( drivers.at(i), SIGNAL( ChangedIsBusy2( QString ) ),
	     this, SLOT( OnChangedIsBusy2( QString ) ) );
  }
  Drivers.removeDuplicates();

  OKcolor = "background-color: #aaffaa";
  NGcolor = "background-color: #ffaaaa";
  QString WBack = "background-color: #ffffff";
  QString TBack = "background-color: #eeffee";
  QString LBack = "background-color: #f8f8e0";
  QString CBack = "background-color: #f0f0f0";
  QString PBBack = "background-color: "
    "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
    "stop:0 rgba(225, 235, 225, 255), stop:1 rgba(255, 255, 255, 255));";

  int VItems = 0;
  starsSV = StarsSV;
  selMC = SelMC;
  conds = Conds;

  MainGrid->addWidget( starsSV, VItems++, 0, 1, 7 );

  DrvsFrame = new QFrame;
  DrvsGrid = new QGridLayout;
  MainGrid->addWidget( DrvsFrame, VItems++, 0, 1, 8 );
  DrvsFrame->setLayout( DrvsGrid );
  DrvsFrame->setFrameShape( StyledPanel );
  DrvsGrid->setContentsMargins( 3, 3, 3, 3 );
  DrvsGrid->setHorizontalSpacing( 3 );
  DrvsGrid->setVerticalSpacing( 1 );
  int DrvVItems = 0;

  QRadioButton *RB0 = new QRadioButton;
  RB0->setText( tr( ": Drivers status watch active" ) );
  RB0->setCheckable( true );
  RB0->setChecked( true );
  DrvsGrid->addWidget( RB0, DrvVItems++, 0, 1, 2 );
  SWactive = true;
  connect( RB0, SIGNAL( clicked() ), this, SLOT( SelStatWatch() ) );

  QLabel *TT;
  QVector<QLabel*> TTs;
  TT = new QLabel; TT->setText( tr( "Drivers" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Devices" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Enable" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Clr. Enable" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "IsBusy" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Busy Units" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "IsBusy2" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Busy2 Units" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Clr. Busys" ) ); TTs << TT;
  for ( int i = 0; i < TTs.count(); i++ ) {
    TTs.at(i)->setStyleSheet( TBack );
    TTs.at(i)->setFrameStyle( QFrame::StyledPanel );
    TTs.at(i)->setAlignment( Qt::AlignCenter );
    DrvsGrid->addWidget( TTs.at(i), DrvVItems, i );
  }
  DrvVItems++;

  QLabel *L1, *LEnable, *LIsB1, *LIsB2;
  QPushButton *CEB, *CBB;
  QComboBox *CB1, *IBBx1, *IBBx2;
  int col;
  for ( int i = 0; i < Drivers.count(); i++ ) {   // 集めた全ドライバ名の一つ一つに対して
    col = 0;
    QVector<AUnit*> *Units = new QVector<AUnit*>;
    // それぞれの名前のドライバを使っているユニットのポインタを集める
    for ( int j = 0; j < drivers.count(); j++ ) {
      if ( Drivers.at(i) == drivers.at(j)->getDriver() )
	*Units << drivers.at(j);
      if ( drivers.at(j)->has2ndDriver() )
	if ( Drivers.at(i) == drivers.at(j)->get2ndDriver() )
	  *Units << drivers.at(j);
    }
    DrvUnits << Units;

    L1 = new QLabel;
    L1->setText( Drivers.at(i) );
    L1->setStyleSheet( LBack );
    L1->setFrameStyle( QFrame::StyledPanel );
    DrvsGrid->addWidget( L1, i + DrvVItems, col++ );

    CB1 = new QComboBox;
    for ( int j = 0; j < drivers.count(); j++ ) {
      if ( drivers.at(j)->getDriver() == Drivers.at(i) )
	CB1->addItem( drivers.at(j)->getName() );
      if ( drivers.at(j)->has2ndDriver() )
	if ( drivers.at(j)->get2ndDriver() == Drivers.at(i) )
	  CB1->addItem( drivers.at(j)->getName() );
    }
    CB1->setStyleSheet( CBack );
    DrvsGrid->addWidget( CB1, i + DrvVItems, col++ );

    LEnable = new QLabel;
    LENBLs << LEnable;
    LEnable->setStyleSheet( NGcolor );
    LEnable->setFrameStyle( QFrame::Box | QFrame::Raised );
    LEnable->setLineWidth( 1 );
    LEnable->setMidLineWidth( 2 );
    LEnable->setMinimumWidth( 20 );
    DrvsGrid->addWidget( LEnable, i + DrvVItems, col++, Qt::AlignHCenter );

    CEB = new QPushButton;
    CEBs << CEB;
    CEB->setText( tr( "Clear" ) );
    CEB->setStyleSheet( PBBack );
    DrvsGrid->addWidget( CEB, i + DrvVItems, col++ );
    connect( CEB, SIGNAL( clicked() ), this, SLOT( OnClear1() ) );

    LIsB1 = new QLabel;
    LIB1s << LIsB1;
    LIsB1->setStyleSheet( NGcolor );
    LIsB1->setFrameStyle( QFrame::Box | QFrame::Raised );
    LIsB1->setLineWidth( 1 );
    LIsB1->setMidLineWidth( 2 );
    LIsB1->setMinimumWidth( 20 );
    DrvsGrid->addWidget( LIsB1, i + DrvVItems, col++, Qt::AlignHCenter );

    IBBx1 = new QComboBox;
    IBBx1s << IBBx1;
    DrvsGrid->addWidget( IBBx1, i + DrvVItems, col++ );

    LIsB2 = new QLabel;
    LIB2s << LIsB2;
    LIsB2->setStyleSheet( NGcolor );
    LIsB2->setFrameStyle( QFrame::Box | QFrame::Raised );
    LIsB2->setLineWidth( 1 );
    LIsB2->setMidLineWidth( 2 );
    LIsB2->setMinimumWidth( 20 );
    DrvsGrid->addWidget( LIsB2, i + DrvVItems, col++, Qt::AlignHCenter );

    IBBx2 = new QComboBox;
    IBBx2s << IBBx2;
    DrvsGrid->addWidget( IBBx2, i + DrvVItems, col++ );

    CBB = new QPushButton;
    CBBs << CBB;
    CBB->setText( tr( "Clear" ) );
    CBB->setStyleSheet( PBBack );
    DrvsGrid->addWidget( CBB, i + DrvVItems, col++ );
    connect( CBB, SIGNAL( clicked() ), this, SLOT( OnClear2() ) );

    OnChangedIsBusy1( Drivers.at(i) );
    OnChangedIsBusy2( Drivers.at(i) );
  }

  MainGrid->addWidget( selMC, VItems, 0, 1, 4 );

  QFrame *EncFrame = new QFrame;
  QGridLayout *EncGrid = new QGridLayout;
  MainGrid->addWidget( EncFrame, VItems++, 4, 1, 3 );
  EncFrame->setLayout( EncGrid );
  EncFrame->setFrameShape( StyledPanel );
  EncFrame->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
  EncGrid->setContentsMargins( 3, 3, 3, 3 );
  EncGrid->setHorizontalSpacing( 3 );
  EncGrid->setVerticalSpacing( 1 );
  QLabel *EncL1 = new QLabel;
  EncL1->setText( tr( "Encorder" ) );
  EncL1->setStyleSheet( TBack );
  EncL1->setFrameStyle( QFrame::StyledPanel );
  EncL1->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  EncGrid->addWidget( EncL1, 0, 0 );
  EncV = new QLineEdit;
  EncV->setText( tr( "   " ) );
  EncV->setStyleSheet( WBack );
  EncGrid->addWidget( EncV, 0, 1 );
  EncB = new QPushButton;
  EncB->setText( tr( "Set" ) );
  EncB->setStyleSheet( PBBack );
  EncB->setMaximumWidth( 40 );
  EncGrid->addWidget( EncB, 0, 2 );
  connect( EncB, SIGNAL( clicked() ), this, SLOT( setEnc() ) );

  MainGrid->addWidget( conds, VItems++, 0, 1, 8 );

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
  MainGrid->addWidget( VS, VItems++, 0 );
}

void Status::SetSSVStat( bool f )
{
  if ( f ) {
    starsSV->sStat()->setText( tr( "Connected" ) );
    starsSV->sStat()->setStyleSheet( OKcolor );
  } else {
    starsSV->sStat()->setText( tr( "not Connected" ) );
    starsSV->sStat()->setStyleSheet( NGcolor );
  }
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
      IBBx1s.at(drv)->addItem( DrvUnits.at(drv)->at(i)->getName()
			       + ":" + DrvUnits.at(drv)->at(i)->lastFunc());
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
      IBBx2s.at(drv)->addItem( DrvUnits.at(drv)->at(i)->getName()
			       + ":" + DrvUnits.at(drv)->at(i)->lastFunc2());
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
	    IBBx1s.at(i)->addItem( DrvUnits.at(i)->at(j)->getName()
				   + ":" + DrvUnits.at(i)->at(j)->lastFunc() );
	  }
	}
	LIB1s.at(i)->setStyleSheet( ( IBBx1s.at(i)->count() > 0 ) ? NGcolor : OKcolor );

	IBBx2s.at(i)->clear();
	cnt = DrvUnits.at(i)->count();
	for ( int j = 0; j < cnt; j++ ) {
	  if ( DrvUnits.at(i)->at(j)->isBusy2() ) {
	    IBBx2s.at(i)->addItem( DrvUnits.at(i)->at(j)->getName()
				   + ":" + DrvUnits.at(i)->at(j)->lastFunc2() );
	  }
	}
	LIB2s.at(i)->setStyleSheet( ( IBBx2s.at(i)->count() > 0 ) ? NGcolor : OKcolor );
      }
    }
  }
}

void Status::newEncTh( QString th )
{
  OrigV = th;
  EncV->setText( th );
}

void Status::setEnc( void )
{
  emit setEncNewTh( OrigV, EncV->text() );
}
