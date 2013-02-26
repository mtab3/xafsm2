
#include "Conditions.h"

Conditions::Conditions( void ) : QFrame()
{
  setupUi( this );
}

void Conditions::setEncAsTh( bool encAsTh )
{
  SelEncAsTh->setChecked( encAsTh );
}

void Conditions::setAddInfos( bool addInfos )
{
  SelAddInfos->setChecked( addInfos );
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
