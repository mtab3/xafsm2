
#include "Conditions.h"

Conditions::Conditions( void ) : QFrame()
{
  setupUi( this );
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

bool Conditions::isUse1303Format( void ) 
{
  return Use1303Format->isChecked();
}

