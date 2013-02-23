#include "KeV2Pix.h"

#include <QString>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>

KeV2Pix::KeV2Pix( void ) : QObject()
{
  MakeUpAB( 1 );    // �ǥե���Ȥ� 1���κǾ�����
}

void KeV2Pix::MakeUpAB( int Dim )
{
  if ( Dim > 0 )
    dim = Dim;
  else 
    dim = 1;

  for ( int i = 0; i < MaxSSDs; i++ ) {
    for ( int j = 0; j < dim + 1; j++ ) {
      ab[i] << 0;
    }
  }
  for ( int i = 0; i < MaxSSDs; i++ ) {
    ab[i][1] = 0.01;
  }

  QString fname = "KeV2MCApix.txt";
  QFile f( fname );

  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    qDebug() << "Can not open " << fname;
    return;
  }
  QTextStream in( &f );

  QVector<double> KeVs;
  QVector<double> Chs[ MaxSSDs ];
  QStringList items;

  while( !in.atEnd() ) {
    items = in.readLine().simplified().split( QRegExp( "\\s+" ) );
    if ( items.count() > 0 ) {
      if ( items.at( 0 ).mid( 0, 1 ) == "#" ) continue;
      if ( items.count() > MaxSSDs ) {       // == MaxSSDs + 1 (�ΤϤ�)
	KeVs << items.at( 0 ).toDouble();
	for ( int i = 0; i < MaxSSDs; i++ ) {
	  Chs[ i ] << items.at( i + 1 ).toDouble();
	}
      }
    }
  }

  for ( int i = 0; i < MaxSSDs; i++ ) {
    double ab0[ dim + 2 ];
    if ( !calcAB( Chs[ i ], KeVs, ab0 ) ) {       // MCApix -> keV ��ľ�����������
      qDebug() << "ab can not be calculated for " << i;
    } else {
      for ( int j = 0; j < dim + 1; j++ ) {
	ab[i][j] = ab0[j];
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
  double rv;

  if (( i < 0 )||( i >= MaxSSDs )||( dim < 1 ) ) {
    rv = E * 100;
  } else {
    rv = ( E - ab[i][0] ) / ab[i][1];
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
  double xn[ dim + 2 ];   // <x^2> <x> <1> �� x^n  �� n=2��0 �ޤ� 3�� (dim+2)���롡
  double xny[ dim + 2 ];  // <xy>  <y>     �� x^ny �� n=1��0 �ޤ� 2�� (dim+1)����
//  double ab[ dim + 2 ]; // ¿�༰�η����ο��ϼ��� + 1 �� 2�� (dim+1)���롣
                          // �����������ñ�ˤ��뤿��ˡ����� dim+2 �ˤ��Ƥ��ޤ���

  for ( int i = 0; i < dim + 2; i++ ) {
    ab[i] = xny[i] = xn[i] = 0;
  }

  for ( int i = 0; i < x.count(); i++ ) {   // <X^2>, <X>, <1>, <XY>, <Y>, �������
    double xx = 1;
    double yy = y[i];
    for ( int j = 0; j < dim + 2; j++ ) {
      xn[j] += xx;
      xny[j] += xx * yy;
      xx *= x[i];
    }
  }

  double m[ dim + 1 ][ dim + 1 ];   // <X^n> ���¤٤������ñ�̹�������
  double s[ dim + 1 ][ dim + 1 ];   // m �� 2�ܤι���ˤ��� s ��ʤ���������
                                    // �ץ����ϥ���ץ�ˤʤ뤬ʬ����䤹����Ȥ롣
  for ( int i = 0; i < dim + 1; i++ ) {
    for ( int j = 0; j < dim + 1; j++ ) {
      m[i][j] = xn[i+j];
      s[i][j] = ( ( i == j ) ? 1 : 0 );
    }
  }

  for ( int i = 0; i < dim + 1; i++ ) {   // �ݤ��Ф�(?) �ǵչ������
    double a = m[i][i];
    if ( a == 0 ) return false;           // ����� 0 ���Ф������ʥǡ���
    for ( int j = 0; j < dim + 1; j++ ) { // i ���ܤιԤ� [i][i] �� 1 �ˤʤ�褦�˵��ʲ�
      m[i][j] /= a;
      s[i][j] /= a;
    }
    for ( int i2 = 0; i2 < dim + 1; i2++ ) { // i2 ���ܤιԤ��� i ���ܤιԤ������
                                             // [i2][i] �� 0 �ˤʤ�褦�ˤ��롣
      if ( i2 == i ) continue;
      double b = m[i2][i];
      for ( int j = 0; j < dim + 1; j++ ) {
	m[i2][j] -= m[i][j] * b;
	s[i2][j] -= s[i][j] * b;
      }
    }
  }

  // �Ǥ����չ��� ( s ) �� xny �٥��ȥ�ˤ������ ab ����ޤ롣
  for ( int i = 0; i < dim + 1; i++ ) {
    for ( int j = 0; j < dim + 1; j++ ) {
      ab[i] += s[i][j] * xny[j];
    }
  }

  return true;
}
