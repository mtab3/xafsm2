#include "FluoDBase.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <math.h>

FluoDBase::FluoDBase( void ) : QObject()
{
  QFile f( ":xray-KLIII.txt" );

  if ( !f.open( QIODevice::ReadOnly ) ) {
    qDebug() << "Cannot open [xray-KLIII.txt]";
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
	  fluo->dispf = true;
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

QVector<Fluo> FluoDBase::inRange( double Es, double Ee, double dE )
{
  QVector<Fluo> rv;

  for ( int i = 0; i < fluos.count(); i++ ) {
    if (( fluos[i].val > Es )&&( fluos[i].val < Ee )) {
      rv << fluos[i];
    }
  }

  rv = removeTooNears( rv, dE );  // rv はソート済み

  return rv;
}

QVector<Fluo> FluoDBase::removeTooNears( QVector<Fluo> list, double dE )
// list はソート済みと仮定する
{
  double val0;
  int no0;

  for ( int i = 0; i < list.count(); i++ ) {
    val0 = list[i].val;
    no0 = list[i].aNumber;
    for ( int j = i+1; j < list.count() && (( list[j].val - val0 ) < dE ); j++ ) {
      if ( list[j].aNumber == no0 )
	list.remove( j );
    }
  }

  return list;
}

QVector<Fluo> FluoDBase::nears( double E, double dE )
// nears( E, range ) の簡易版。10keV で、range = +/- 0.25 になる。
{
  if ( E < 0 ) {
    QVector<Fluo> rv;
    return rv;
  }
  return nears( E, E / 50 + 0.05, dE );
}


QVector<Fluo> FluoDBase::nears( double E, double range, double dE )
// 指定エネルギー E の前後 range の範囲に入る元素リスト作成。
// 最初は E に近いもの順にしてたけど、今はエネルギーの小さいもの順
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

  nFluos = removeTooNears( nFluos, dE );  // ソート済み

  return nFluos;
}

int FluoDBase::nearest( double E, int s, int e )
// 2 分探索をやった後、最後の詰めが自信ないので最終候補全員比較
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

void FluoDBase::ElementSelected( bool f, int eNo )
{
  for ( int i = 0; i < fluos.count(); i++ ) {
    if ( fluos[i].aNumber - 1 == eNo ) {
      fluos[i].dispf = f;
    }
  }
}
