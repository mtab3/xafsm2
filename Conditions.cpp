
#include "XafsM.h"
#include "Conditions.h"

Conditions::Conditions( void ) : QFrame()
{
  setupUi( this );

  VersionInfo->setText( QString( "Ver. %1, Compiled Date : %2, Time : %3" )
			.arg( VERSION )
			.arg( __DATE__ )
			.arg( __TIME__ ) );

  connect( CalibDXMPMC, SIGNAL( clicked() ), this, SIGNAL( SetDXMPMC() ) );
}

void Conditions::setVersionInfo( QString ver, QString date, QString time )
{
  VersionInfo->setText( QString( "Ver. %1, Compiled Date : %2, Time : %3" )
			.arg( ver )
			.arg( date )
			.arg( time ) );
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
