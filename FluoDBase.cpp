#include "FluoDBase.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <math.h>

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
      for ( int i = 2; i < items.count() && i < 9; i++ ) {
	if ( items[i].toDouble() != 0 ) {
	  Fluo *fluo = new Fluo;
	  fluo->aName = items[0];
	  fluo->transName = TNames[ i - 2 ];
	  fluo->aNumber = items[1].toInt();
	  fluo->val = items[i].toDouble() / 1000.;
	  fluo->fullName = fluo->aName + " " + fluo->transName;
	  fluos << *fluo;
	}
      }
    }
  }
  f.close();

  qSort( fluos.begin(), fluos.end() );
}

QVector<Fluo> FluoDBase::nears( double E )
{
  if ( E < 0 ) {
    QVector<Fluo> rv;
    return rv;
  }
  return nears( E, sqrt( E ) / 50 + 0.1 );
}


QVector<Fluo> FluoDBase::nears( double E, double range )
{
  if ( E < 0 ) {
    QVector<Fluo> rv;
    return rv;
  }
  QVector<Fluo> nFluos;

  int p = nearest( E, 0, fluos.count() - 1 );
  int u = p;
  int d = p - 1;

  while ( ( u < fluos.count() )&&( fabs( fluos[u].val - E ) < range ) ) {
    nFluos << fluos[u];
    u++;
  }
  while ( ( d >= 0 )&&( fabs( fluos[d].val - E ) < range ) ) {
    nFluos << fluos[d];
    d--;
  }
  qSort( nFluos.begin(), nFluos.end() );
  return nFluos;
}

int FluoDBase::nearest( double E, int s, int e )
{
  if ( ( e - s ) < 2 ) {
    int ss = s - 1;
    int ee = e + 1;
    if ( ss < 0 ) ss = 0;
    if ( ee >= fluos.count() )
      ee = fluos.count() - 1;
    int minp = ss;
    for ( int i = ss; i <= ee; i++ ) {
      if ( fabs( fluos[i].val - E ) < fabs( fluos[minp].val - E ) ) {
	minp = i;
      }
    }
    return minp;
  }

  int np = ( s + e ) / 2.;
  if ( fluos[np].val < E ) {
    return nearest( E, np, e );
  } else {
    return nearest( E, s, np );
  }
}
