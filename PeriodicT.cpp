#include <QtGui>

#include "PeriodicT.h"

PeriodicTable::PeriodicTable() : QMainWindow()
{
  setupUi( this );

  SetPBs();

  SelectedAtom = -1;
  WhenSelected = PT_STAY;
  WhenClosed = PT_HIDE;

  //  SetPTMask( 5, 0 );
  //  SetPTMasks( 12, 15, 0 );

  PBMap = new QSignalMapper( this );
  for ( int i = 0; i < PBs; i++ ) {
    PBMap->setMapping( PB[i], i );
    connect( PB[i], SIGNAL( clicked() ), PBMap, SLOT( map() ) );
  }
  connect( PBMap, SIGNAL( mapped( int ) ), this, SLOT( doPB( int ) ) );
  connect( Close, SIGNAL( clicked() ), this, SLOT( PTClose() ) );
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

void PeriodicTable::doPB( int i )
{
  SelectedAtom = i+1;

  //  printf( "AN = %d\n", SelectedAtom );

  emit AtomSelected( SelectedAtom );
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
      PB[i-1]->setEnabled( FALSE );
      //      printf( "disable %d", i );
    } else {
      PB[i-1]->setEnabled( TRUE );
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
	PB[i-1]->setEnabled( FALSE );
	//	printf( "disable %d", i );
      } else {
	PB[i-1]->setEnabled( TRUE );
	//	printf( "enable %d", i );
      }
    }
  }
}
