#include "FluoDBase.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>

FluoDBase::FluoDBase( void ) : QObject()
{
  QFile f( ":xray-KLIII.dat" );

  if ( !f.open( QIODevice::ReadOnly ) ) {
    qDebug() << "Cannot open [xray-KLIII.dat]";
    return;
  }

  QStringList TNames;
  TNames << "Ka2" << "Ka1" << "Kb" << "La2" << "La1" << "Lb1" << "Lb2";
  
  QTextStream in(&f);

  QStringList items;
  while ( !in.atEnd() ) {
    items = in.readLine().simplified().split( QRegExp( "\\s+" ) );
    if ( items[0] != "" ) {
      for ( int i = 3; i < items.count() && i < 9; i++ ) {
	if ( items[i].toDouble() != 0 ) {
	  Fluo *fluo = new Fluo;
	  fluo->aName = items[0];
	  fluo->transName = TNames[ i - 3 ];
	  fluo->aNumber = items[1].toInt();
	  fluo->val = items[i].toDouble() / 1000.;
	  fluo->fullName = fluo->aName + " " + fluo->transName;
	  fluos << fluo;
	}
      }
    }
  }

  for ( int i = 0; i < fluos.count(); i++ ) {
    qDebug() << fluos.at(i)->fullName << fluos.at(i)->val;
  }
  qDebug() << fluos.count();

  f.close();
}
