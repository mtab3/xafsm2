#include "KeV2Pix.h"

#include <QString>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QStringList>

KeV2Pix::KeV2Pix( void ) : QObject()
{
  //  MakeUpAB( 19, 2 );    // 2次の最小自乗にした
}

void KeV2Pix::setMCALen(int mcaLen )
{
  MCALen = mcaLen;
}

void KeV2Pix::MakeUpAB( int MCAChs, int Dim, QString fname )
{
  if ( Dim > 0 )
    dim = Dim;
  else 
    dim = 1;

  ab.clear();
  ba.clear();
  for ( int i = 0; i < MCAChs; i++ ) {
    QVector<double> ab0, ba0;
    for ( int j = 0; j < dim + 1; j++ ) {
      ab0 << 0;
      ba0 << 0;
    }
    ab << ab0;
    ba << ba0;
  }
  for ( int i = 0; i < MCAChs; i++ ) {
    ab[i][1] = 0.01;
    ba[i][1] = 100;
  }

  //  QFileInfo finfo( "KeV2MCApix.txt" );
  QFileInfo finfo( fname );
  if ( ! finfo.exists() )
    fname = ":" + fname;
  QFile f( fname );

  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    qDebug() << "Can not open " << fname;
  } else {
    QTextStream in( &f );
    
    QVector<double> KeVs;
    QVector<double> Chs[ SSDChs ];
    QStringList items;
    
    while( !in.atEnd() ) {
      items = in.readLine().simplified().split( QRegExp( "\\s+" ) );
      if ( items.count() > 0 ) {
	if ( items.at( 0 ).mid( 0, 1 ) == "#" ) continue;
	if ( items.count() > SSDChs ) {       // == MaxSSDs + 1 (のはず)
	  KeVs << items.at( 0 ).toDouble();
	  for ( int i = 0; i < SSDChs; i++ ) {
	    Chs[ i ] << items.at( i + 1 ).toDouble();
	  }
	}
      }
    }
  }

  for ( int i = 0; i < SSDChs; i++ ) {
    double ab0[ dim + 2 ];
    if ( !calcAB( Chs[ i ], KeVs, ab0 ) ) {       // MCApix -> keV に直す係数を求める
      qDebug() << "ab can not be calculated for " << i;
    } else {
      for ( int j = 0; j < dim + 1; j++ ) {
        ab[i][j] = ab0[j];
      }
    }

    double ba0[ dim + 2 ];
    if ( !calcAB( KeVs, Chs[ i ], ba0 ) ) {       // keV -> MCApix に直す係数を求める
      qDebug() << "ba can not be calculated for " << i;
    } else {
      for ( int j = 0; j < dim + 1; j++ ) {
        ba[i][j] = ba0[j];
      }
    }
  }
}

double KeV2Pix::p2E( int i, int p )
{
  if (( i < 0 )||( i >= MaxSSDs )) {
    return p * 0.01;
  }
  double rv = 0;
  double P = 1.0;
  for ( int j = 0; j < dim + 1; j++ ) {
    rv += ab[i][j] * P;
    P *= (double)p;
  }
  return rv;
}

int KeV2Pix::E2p( int i, double E )
{
  if (( i < 0 )||( i >= MaxSSDs ))
    return E * 100;

  double rv = 0;
  double e = 1.0;
  for ( int j = 0; j < dim + 1; j++ ) {
    rv += ba[i][j] * e;
    e *= E;
  } 

  int irv = (int)( rv + 0.5 );
  if ( irv < 0 ) irv = 0;
  if ( irv > MCALen ) irv = MCALen - 1;

  return irv;
}

const QVector<double>& KeV2Pix::getAB( int i )
{
  return ab[i];
}

bool KeV2Pix::calcAB( QVector<double>&x, QVector<double>&y, double *ab )
{
  double xn[ dim + 2 ];   // <x^2> <x> <1> と x^n  は n=2～0 まで 3つ (dim+2)いる　
  double xny[ dim + 2 ];  // <xy>  <y>     と x^ny は n=1～0 まで 2つ (dim+1)いる
//  double ab[ dim + 2 ]; // 多項式の係数の数は次数 + 1 で 2つ (dim+1)いる。
                          // が、処理を簡単にするために、全部 dim+2 にしてしまう。

  for ( int i = 0; i < dim + 2; i++ ) {
    ab[i] = xny[i] = xn[i] = 0;
  }

  for ( int i = 0; i < x.count(); i++ ) {   // <X^2>, <X>, <1>, <XY>, <Y>, 等を準備
    double xx = 1;
    double yy = y[i];
    for ( int j = 0; j < dim + 2; j++ ) {
      xn[j] += xx;
      xny[j] += xx * yy;
      xx *= x[i];
    }
  }

  double m[ dim + 1 ][ dim + 1 ];   // <X^n> を並べた行列と単位行列を準備
  double s[ dim + 1 ][ dim + 1 ];   // m を横 2倍の行列にして s をなくした方が
                                    // プログラムはシンプルになるが分かりやすさをとる。
  for ( int i = 0; i < dim + 1; i++ ) {
    for ( int j = 0; j < dim + 1; j++ ) {
      m[i][j] = xn[i+j];
      s[i][j] = ( ( i == j ) ? 1 : 0 );
    }
  }

  for ( int i = 0; i < dim + 1; i++ ) {   // 掃き出し(?) で逆行列作成
    double a = m[i][i];
    if ( a == 0 ) return false;           // 途中で 0 が出たらダメなデータ
    for ( int j = 0; j < dim + 1; j++ ) { // i 番目の行を [i][i] が 1 になるように規格化
      m[i][j] /= a;
      s[i][j] /= a;
    }
    for ( int i2 = 0; i2 < dim + 1; i2++ ) { // i2 番目の行から i 番目の行を引いて
                                             // [i2][i] が 0 になるようにする。
      if ( i2 == i ) continue;
      double b = m[i2][i];
      for ( int j = 0; j < dim + 1; j++ ) {
	m[i2][j] -= m[i][j] * b;
	s[i2][j] -= s[i][j] * b;
      }
    }
  }

  // できた逆行列 ( s ) を xny ベクトルにかけると ab が求まる。
  for ( int i = 0; i < dim + 1; i++ ) {
    for ( int j = 0; j < dim + 1; j++ ) {
      ab[i] += s[i][j] * xny[j];
    }
  }

  return true;
}
