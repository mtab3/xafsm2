#include "AUnit0.h"

AUnit::AUnit0( QObject *parent ) : QObject( parent )
{
  s = NULL;
  Enable = false;
  aLine = -1;

  LocalStage = 0;

  IsBusy = IsBusy2 = false;
  Busy2Count = 0;
}

void AUnit0::makeDevCh( void )
{
  if ( Dev == "" ) {
    DevCh = "";
    return;
  }
  if ( Ch != "" )
    DevCh = Dev + "." + Ch;
  else
    DevCh = Dev;
}
