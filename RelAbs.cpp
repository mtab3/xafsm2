
#include <QDebug>
#include "RelAbs.h"

RelAbs::RelAbs( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  Stat = REL;
  Show();
  connect( Rel, SIGNAL( clicked() ), this, SLOT( ToRel() ), Qt::UniqueConnection );
  connect( Abs, SIGNAL( clicked() ), this, SLOT( ToAbs() ), Qt::UniqueConnection );
}

void RelAbs::ToRel( void )
{
  Stat = REL;
  Show();
}

void RelAbs::ToAbs( void )
{
  Stat = ABS;
  Show();
}

void RelAbs::Show( void )
{
  Rel->setStyleSheet( ( Stat == REL )? RadioBOn : RadioBOff );
  Abs->setStyleSheet( ( Stat == ABS )? RadioBOn : RadioBOff );
  emit statChanged( Stat );
}
