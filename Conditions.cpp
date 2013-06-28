
#include "Conditions.h"

Conditions::Conditions( void ) : QFrame()
{
  setupUi( this );

  connect( CalibDXMPMC, SIGNAL( clicked() ), this, SIGNAL( SetDXMPMC() ) );
}

bool Conditions::isEncAsTh( void )
{
  return SelEncAsTh->isChecked();
}

bool Conditions::isAddInfos( void )
{
  return SelAddInfos->isChecked();
}

bool Conditions::isRecordAllSSDChs( void ) 
{
  return RecordAllSSDChs->isChecked();
}

bool Conditions::isMakeInfo( void ) 
{
  return MakeInfoFile->isChecked();
}

bool Conditions::isMeasInDeg( void )
{
  return SelStepInDegree->isChecked();
}
