#include <QGridLayout>
#include <QLabel>

#include "PMConditions.h"
#include "PMLine.h"

PMConditions::PMConditions( void )
{
  mainFrame = new QFrame;
  QGridLayout *L = new QGridLayout;
  PMLine *pm1 = new PMLine;

  mainFrame->setLayout( L );
  L->addWidget( pm1, 0, 0 );
}
