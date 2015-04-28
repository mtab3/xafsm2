#include <QDebug>

#include "AUnitRemote.h"

AUnitRemote::AUnitRemote( void )
{
}

void AUnitRemote::init( void )
{
  qDebug() << "Initializing Remote Unit" << s;
  connect( s, SIGNAL( AskStart( SMsg ) ), this, SLOT( RcvStart( SMsg ) ), Qt::UniqueConnection );
  connect( s, SIGNAL( AskStop( SMsg ) ), this, SLOT( RcvStop( SMsg ) ), Qt::UniqueConnection );
}

void AUnitRemote::RcvStart( SMsg msg )
{
  if ( msg.ToCh().toUpper() == "REMOTE" ) {
    emit setMeasBlockF( false );
  }
}

void AUnitRemote::RcvStop( SMsg msg )
{
  if ( msg.ToCh().toUpper() == "REMOTE" ) {
    emit setMeasBlockF( true );
  }
}
