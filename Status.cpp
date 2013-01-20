#include "Status.h"

Status::Status( QWidget *p ) : QFrame( p )
{
  setupUi( this );
}

void Status::setupStatArea( QVector<AUnit*> *Ams, QVector<AUnit*> *Ass )
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
  QString TBack = "background-color: #eeffee";
  QString LBack = "background-color: #f8f8e0";
  QString CBack = "background-color: #f0f0f0";
  QString PBBack = "background-color: "
    "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
    "stop:0 rgba(225, 235, 225, 255), stop:1 rgba(255, 255, 255, 255));";

  QFrame *SSFrame = new QFrame;
  MainGrid->addWidget( SSFrame, 0, 0, 1, 5 );
  QGridLayout *SSS = new QGridLayout;
  SSS->setContentsMargins( 3, 3, 3, 3 );
  SSS->setVerticalSpacing( 1 );
  SSFrame->setLayout( SSS );
  SSFrame->setStyleSheet( TBack );
  SSFrame->setFrameStyle( QFrame::StyledPanel );
  QLabel *TS;
  TS = new QLabel; TS->setText(tr("Stars server :")); SSS->addWidget( TS, 0, 0 );
  SSAddr = new QLabel; SSAddr->setText(tr("SS Address")); SSS->addWidget( SSAddr, 0, 1 );
  SSPort = new QLabel; SSPort->setText(tr("SS Port")); SSS->addWidget( SSPort, 0, 2 );
  SSStat = new QLabel; SSStat->setText(tr("not connected")); SSS->addWidget( SSStat, 0, 3 );

  SSAddr->setStyleSheet( LBack );
  SSAddr->setFrameStyle( QFrame::StyledPanel );
  SSPort->setStyleSheet( LBack );
  SSPort->setFrameStyle( QFrame::StyledPanel );
  SSStat->setStyleSheet( NGcolor );
  SSStat->setFrameStyle( QFrame::StyledPanel );

  QRadioButton *RB0 = new QRadioButton;
  RB0->setText( tr( ": Drivers status watch active" ) );
  RB0->setCheckable( true );
  RB0->setChecked( true );
  MainGrid->addWidget( RB0, 1, 0, 1, 2 );
  SWactive = true;
  connect( RB0, SIGNAL( clicked() ), this, SLOT( SelStatWatch() ) );

  QLabel *TT;
  QVector<QLabel*> TTs;
  TT = new QLabel; TT->setText( tr( "Drivers" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Devices" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Enable" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Clear Enable" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "IsBusy" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Busy Units" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "IsBusy2" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Busy2 Units" ) ); TTs << TT;
  TT = new QLabel; TT->setText( tr( "Clear Busys" ) ); TTs << TT;
  for ( int i = 0; i < TTs.count(); i++ ) {
    TTs.at(i)->setStyleSheet( TBack );
    TTs.at(i)->setFrameStyle( QFrame::StyledPanel );
    TTs.at(i)->setAlignment( Qt::AlignCenter );
    MainGrid->addWidget( TTs.at(i), 2, i );
  }

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
    MainGrid->addWidget( L1, i + 3, col++ );

    CB1 = new QComboBox;
    for ( int j = 0; j < drivers.count(); j++ ) {
      if ( drivers.at(j)->getDriver() == Drivers.at(i) )
	CB1->addItem( drivers.at(j)->getName() );
      if ( drivers.at(j)->has2ndDriver() )
	if ( drivers.at(j)->get2ndDriver() == Drivers.at(i) )
	  CB1->addItem( drivers.at(j)->getName() );
    }
    CB1->setStyleSheet( CBack );
    MainGrid->addWidget( CB1, i+3, col++ );

    LEnable = new QLabel;
    LENBLs << LEnable;
    LEnable->setStyleSheet( NGcolor );
    LEnable->setFrameStyle( QFrame::Box | QFrame::Raised );
    LEnable->setLineWidth( 1 );
    LEnable->setMidLineWidth( 2 );
    LEnable->setMinimumWidth( 20 );
    MainGrid->addWidget( LEnable, i+3, col++, Qt::AlignHCenter );

    CEB = new QPushButton;
    CEBs << CEB;
    CEB->setText( tr( "Clear" ) );
    CEB->setStyleSheet( PBBack );
    MainGrid->addWidget( CEB, i+3, col++ );
    connect( CEB, SIGNAL( clicked() ), this, SLOT( OnClear1() ) );

    LIsB1 = new QLabel;
    LIB1s << LIsB1;
    LIsB1->setStyleSheet( NGcolor );
    LIsB1->setFrameStyle( QFrame::Box | QFrame::Raised );
    LIsB1->setLineWidth( 1 );
    LIsB1->setMidLineWidth( 2 );
    LIsB1->setMinimumWidth( 20 );
    MainGrid->addWidget( LIsB1, i+3, col++, Qt::AlignHCenter );

    IBBx1 = new QComboBox;
    IBBx1s << IBBx1;
    MainGrid->addWidget( IBBx1, i+3, col++ );

    LIsB2 = new QLabel;
    LIB2s << LIsB2;
    LIsB2->setStyleSheet( NGcolor );
    LIsB2->setFrameStyle( QFrame::Box | QFrame::Raised );
    LIsB2->setLineWidth( 1 );
    LIsB2->setMidLineWidth( 2 );
    LIsB2->setMinimumWidth( 20 );
    MainGrid->addWidget( LIsB2, i+3, col++, Qt::AlignHCenter );

    IBBx2 = new QComboBox;
    IBBx2s << IBBx2;
    MainGrid->addWidget( IBBx2, i+3, col++ );

    CBB = new QPushButton;
    CBBs << CBB;
    CBB->setText( tr( "Clear" ) );
    CBB->setStyleSheet( PBBack );
    MainGrid->addWidget( CBB, i+3, col++ );
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
  MainGrid->addWidget( VS, Drivers.count() + 3, 0 );
}

void Status::SetSSVA( QString Server )
{
  SSAddr->setText( Server );
}

void Status::SetSSVP( qint16 Port )
{
  SSPort->setText( QString::number( Port ) );
}

void Status::SetSSVStat( bool f )
{
  if ( f ) {
    SSStat->setText( tr( "Connected" ) );
    SSStat->setStyleSheet( OKcolor );
  } else {
    SSStat->setText( tr( "not Connected" ) );
    SSStat->setStyleSheet( NGcolor );
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
