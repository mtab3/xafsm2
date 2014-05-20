#include <QtGui>

#include "PeriodicT.h"

const QString ENames[5] = { "K", "LI", "LII", "LIII", "M" };

PeriodicTable::PeriodicTable( VICS *Vic ) : QMainWindow()
{
  setupUi( this );

  SetPBs();    // QPushButton * を集めた配列作成

  for ( int i = 0; i < PBs; i++ ) {
    if ( i < Vics ) {
      QString buf = tr ( "Atom No. : %1" ).arg( Vic[i+1].AN );
      for ( int j = 0; j < 5; j++ ) {
	if ( Vic[i+1].AE[j] > 0 ) {
	  buf += tr( "\n %1-edge : %2 keV" ).arg( ENames[j] ).arg( Vic[i+1].AE[j] );
	}
      }
      PB[i]->setToolTip( buf );
    }
  }
  ReadFluoDB();

  isExclusive = true;
  SelectedAtom = -1;
  WhenSelected = PT_STAY;
  WhenClosed = PT_HIDE;

  //  SetPTMask( 5, 0 );
  //  SetPTMasks( 12, 15, 0 );

  PBMap = new QSignalMapper( this );
  for ( int i = 0; i < PBs; i++ ) {
    PBMap->setMapping( PB[i], i );
    connect( PB[i], SIGNAL( clicked() ), PBMap, SLOT( map() ),
	     Qt::UniqueConnection );
  }
  connect( PBMap, SIGNAL( mapped( int ) ), this, SLOT( doPB( int ) ),
	   Qt::UniqueConnection );
  connect( Close, SIGNAL( clicked() ), this, SLOT( PTClose() ),
	   Qt::UniqueConnection );
  connect( SetAll, SIGNAL( clicked() ), this, SLOT( setAll() ),
	   Qt::UniqueConnection );
  connect( SetNone, SIGNAL( clicked() ), this, SLOT( setNone() ),
	   Qt::UniqueConnection);

  AllNoneBs->setVisible( false );
}


void PeriodicTable::ReadFluoDB( void )
{
  QFile f( ":xray-KLIII.txt" );

  if ( !f.open( QIODevice::ReadOnly ) ) {
    qDebug() << "Cannot open [xray-KLIII.txt]";
    return;
  }

  QStringList TNames;
  TNames << "Ka2" << "Ka1" << "Kb" << "La2" << "La1" << "Lb1" << "Lb2";

  QTextStream in(&f);

  double eng;
  QStringList items;
  while ( !in.atEnd() ) {
    items = in.readLine().simplified().split( QRegExp( "\\s+" ) );
    if ( items[0] != "" ) {
      int AN = items[1].toInt();
      if ( ( AN <= PBs )&&( AN > 0 ) ) {
	QString buf = PB[ AN - 1 ]->toolTip();
	for ( int i = 2; i < items.count() && i < 9; i++ ) {
	  if ( ( eng = items[i].toDouble() ) > 0 ) {
	    buf += tr( "\n   %1 : %2 keV" ).arg( TNames[i-2] ).arg( eng / 1000. );
	  }
	}
	PB[ AN - 1 ]->setToolTip( buf );
      }
    }
  }
  f.close();
}


QStringList PeriodicTable::getSelectedAtoms( void )
{
  QStringList atoms;

  for ( int i = 0; i < PBs; i++ ) {
    if ( PB[i]->isChecked() )
      atoms << PB[i]->text().simplified();
  }
  return atoms;
}

void PeriodicTable::setAll( void )
{
  for ( int i = 0; i < PBs; i++ ) {
    if ( PB[i]->isCheckable() ) {
      PB[i]->setChecked( true );
      emit AtomToggled( true, i );
    }
  }
}

void PeriodicTable::setNone( void )
{
  for ( int i = 0; i < PBs; i++ ) {
    if ( PB[i]->isCheckable() ) {
      PB[i]->setChecked( false );
      emit AtomToggled( false, i );
    }
  }
}

void PeriodicTable::doPB( int i )
{
  SelectedAtom = i+1;

  //  printf( "AN = %d\n", SelectedAtom );

  emit AtomSelected( SelectedAtom );
  emit AtomToggled( PB[i]->isChecked(), i );
  switch( WhenSelected ) {
  case PT_CLOSE:
    close();
    break;
  case PT_HIDE:
    hide();
    break;
  case PT_STAY:
    break;
  }
}

void PeriodicTable::PTClose( void )
{
  emit PTClosing( SelectedAtom );

  switch( WhenClosed ) {
  case PT_CLOSE:
    close();
    break;
  case PT_HIDE:
    hide();
    break;
  default:
    close();
    break;
  }
}

void PeriodicTable::SetPTMask( int i, int mask )
{
  if ( ( i >= 1 )&&( i <= PBs ) ) {
    PTMask[i] = mask;     // mask 番号は 1 始まり
    if ( mask == 0 ) {
      PB[i-1]->setEnabled( false );
      //      printf( "disable %d", i );
    } else {
      PB[i-1]->setEnabled( true );
      //      printf( "enable %d", i );
    }
  }
}

void PeriodicTable::SetPTMasks( int s, int e, int mask )
{
  for ( int i = s; i <= e; i++ ) {
    if ( ( i >= 1 )&&( i <= PBs ) ) {
      PTMask[i] = mask;     // mask 番号は 1 始まり
      if ( mask == 0 ) {
	PB[i-1]->setEnabled( false );
	//	printf( "disable %d", i );
      } else {
	PB[i-1]->setEnabled( true );
	//	printf( "enable %d", i );
      }
    }
  }
}

void PeriodicTable::SetAGColor( AtomGroup *ag )
{
  bool ingroup = false;
  for ( int i = 0; i < PBs; i++ ) {
    if ( PB[i]->text().simplified() == ag->startAtom.simplified() )
      ingroup = true;
    if ( PB[i]->text().simplified() == ag->endAtom.simplified() )
      break;
    if ( ingroup )
      PB[i]->setStyleSheet( "background-color: " + ag->groupColor );
  }
}

void PeriodicTable::SetCheckable( bool f )
{
  for ( int i = 0; i < PBs; i++ ) {
    PB[i]->setCheckable( f );
  }
}

void PeriodicTable::SetPBs( void )
{
  PBs = 0;
  PB[ PBs++ ] = PB001;
  PB[ PBs++ ] = PB002;
  PB[ PBs++ ] = PB003;
  PB[ PBs++ ] = PB004;
  PB[ PBs++ ] = PB005;
  PB[ PBs++ ] = PB006;
  PB[ PBs++ ] = PB007;
  PB[ PBs++ ] = PB008;
  PB[ PBs++ ] = PB009;
  PB[ PBs++ ] = PB010;

  PB[ PBs++ ] = PB011;
  PB[ PBs++ ] = PB012;
  PB[ PBs++ ] = PB013;
  PB[ PBs++ ] = PB014;
  PB[ PBs++ ] = PB015;
  PB[ PBs++ ] = PB016;
  PB[ PBs++ ] = PB017;
  PB[ PBs++ ] = PB018;
  PB[ PBs++ ] = PB019;
  PB[ PBs++ ] = PB020;

  PB[ PBs++ ] = PB021;
  PB[ PBs++ ] = PB022;
  PB[ PBs++ ] = PB023;
  PB[ PBs++ ] = PB024;
  PB[ PBs++ ] = PB025;
  PB[ PBs++ ] = PB026;
  PB[ PBs++ ] = PB027;
  PB[ PBs++ ] = PB028;
  PB[ PBs++ ] = PB029;
  PB[ PBs++ ] = PB030;

  PB[ PBs++ ] = PB031;
  PB[ PBs++ ] = PB032;
  PB[ PBs++ ] = PB033;
  PB[ PBs++ ] = PB034;
  PB[ PBs++ ] = PB035;
  PB[ PBs++ ] = PB036;
  PB[ PBs++ ] = PB037;
  PB[ PBs++ ] = PB038;
  PB[ PBs++ ] = PB039;
  PB[ PBs++ ] = PB040;

  PB[ PBs++ ] = PB041;
  PB[ PBs++ ] = PB042;
  PB[ PBs++ ] = PB043;
  PB[ PBs++ ] = PB044;
  PB[ PBs++ ] = PB045;
  PB[ PBs++ ] = PB046;
  PB[ PBs++ ] = PB047;
  PB[ PBs++ ] = PB048;
  PB[ PBs++ ] = PB049;
  PB[ PBs++ ] = PB050;

  PB[ PBs++ ] = PB051;
  PB[ PBs++ ] = PB052;
  PB[ PBs++ ] = PB053;
  PB[ PBs++ ] = PB054;
  PB[ PBs++ ] = PB055;
  PB[ PBs++ ] = PB056;
  PB[ PBs++ ] = PB057;
  PB[ PBs++ ] = PB058;
  PB[ PBs++ ] = PB059;
  PB[ PBs++ ] = PB060;

  PB[ PBs++ ] = PB061;
  PB[ PBs++ ] = PB062;
  PB[ PBs++ ] = PB063;
  PB[ PBs++ ] = PB064;
  PB[ PBs++ ] = PB065;
  PB[ PBs++ ] = PB066;
  PB[ PBs++ ] = PB067;
  PB[ PBs++ ] = PB068;
  PB[ PBs++ ] = PB069;
  PB[ PBs++ ] = PB070;

  PB[ PBs++ ] = PB071;
  PB[ PBs++ ] = PB072;
  PB[ PBs++ ] = PB073;
  PB[ PBs++ ] = PB074;
  PB[ PBs++ ] = PB075;
  PB[ PBs++ ] = PB076;
  PB[ PBs++ ] = PB077;
  PB[ PBs++ ] = PB078;
  PB[ PBs++ ] = PB079;
  PB[ PBs++ ] = PB080;

  PB[ PBs++ ] = PB081;
  PB[ PBs++ ] = PB082;
  PB[ PBs++ ] = PB083;
  PB[ PBs++ ] = PB084;
  PB[ PBs++ ] = PB085;
  PB[ PBs++ ] = PB086;
  PB[ PBs++ ] = PB087;
  PB[ PBs++ ] = PB088;
  PB[ PBs++ ] = PB089;
  PB[ PBs++ ] = PB090;

  PB[ PBs++ ] = PB091;
  PB[ PBs++ ] = PB092;
  PB[ PBs++ ] = PB093;
  PB[ PBs++ ] = PB094;
  PB[ PBs++ ] = PB095;
  PB[ PBs++ ] = PB096;
  PB[ PBs++ ] = PB097;
  PB[ PBs++ ] = PB098;
  PB[ PBs++ ] = PB099;
  PB[ PBs++ ] = PB100;

  PB[ PBs++ ] = PB101;
  PB[ PBs++ ] = PB102;
  PB[ PBs++ ] = PB103;
  PB[ PBs++ ] = PB104;
  PB[ PBs++ ] = PB105;
  PB[ PBs++ ] = PB106;
  PB[ PBs++ ] = PB107;
  PB[ PBs++ ] = PB108;
  PB[ PBs++ ] = PB109;
  PB[ PBs++ ] = PB110;

  PB[ PBs++ ] = PB111;
  PB[ PBs++ ] = PB112;
}
