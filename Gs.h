#ifndef GS_H
#define GS_H

#include <QObject>
#include <QString>

#include "math.h"
#include "LinearA.h"

// f = A*A exp -C*C ( x-B )^2
// ピーク高さ (A*A) と、ピーク幅(C*C) が必ず正であることを自動的に保証した形
class aG
{
  double A, B, C;

 public:
  aG( void ) { A = B = C = 0; };
  void setA( double a ) { A = a; };
  void setB( double b ) { B = b; };
  void setC( double c ) { C = c; };
  void setW( double w ) { C = sqrt( 4.*log(2.)/(w*w) ); }
  void setHw( double hw ) { C = sqrt( log(2.)/(hw*hw) ); }
  double a( void ) { return A; };
  double b( void ) { return B; };
  double c( void ) { return C; };
  double w( void ) { return 2. * sqrt( log(2.) / ( C * C ) ); }
  double hw( void ) { return sqrt( log(2.) / ( C * C ) ); }
  double x2( double x ) { return ( x - B ) * ( x - B ); }
  double core( double x ) { return exp( - C * C * x2( x ) ); }

  double f( double x ) { return A * A * core( x ); }
  double da( double x ) { return 2 * A * core( x ); }
  double db( double x ) { return 2 * C * C * ( x - B ) * A * A * core( x ); }
  double dc( double x ) { return -2 * C * A * A * x2( x ) * core( x ); }

  double di( int i, double x ) {
    double rv = 0;
    switch( i ) {
    case 0: rv = da( x ); break;
    case 1: rv = db( x ); break;
    case 2: rv = dc( x ); break;
    }
    return rv;
  }
};

class Gs : public QObject
{
  Q_OBJECT
  
  int n;   // number of gaussians
  aG *gs;

  QString stat;
  LA la;
  
 public:
  Gs( int N ) { n = N; gs = new aG[ n ]; }
  ~Gs( void ) {};

  bool fit( int points, double *x, double *e,
	    double *p, int Loop, double damp, double prec1, double prec2 );
  // Gs はガウスピークの数, パラメータも一緒に渡す
  // prec1 : 残差 0.1 切ればまあいい
  // prec2 : 残差の変化率の2階微分 1e-6 程度か?

  int peaks( void ) { return n; };
  void setABC( double *p )   // パラメータ設定 : A*exp( -C*(x-B)^2 )
  {
    for ( int i = 0; i < n; i++ ) {
      gs[i].setA( p[ i * 3 ] );
      gs[i].setB( p[ i * 3 + 1 ] );
      gs[i].setC( p[ i * 3 + 2 ] );
    }
  }
  void setABW( double *p )   // パラメータ設定 : A, B, 半値全幅 w
  {
    for ( int i = 0; i < n; i++ ) {
      gs[i].setA( p[ i * 3 ] );
      gs[i].setB( p[ i * 3 + 1 ] );
      gs[i].setW( p[ i * 3 + 2 ] );
    }
  }
  void setABHw( double *p )   // パラメータ設定 : A, B, 半値半幅 hw
  {
    for ( int i = 0; i < n; i++ ) {
      gs[i].setA( p[ i * 3 ] );
      gs[i].setB( p[ i * 3 + 1 ] );
      gs[i].setHw( p[ i * 3 + 2 ] );
    }
  }
  double f( double x )
  {
    double rv = 0;
    for ( int i = 0; i < n; i++ ) {
      rv += gs[i].f( x );
    }
    return rv;
  }
  double f( int i, double x )
  {
    if ( i < n ) {
      return gs[i].f( x );
    }
    return 0;
  }
  double dp( int p, double x )
  {
    int n = (int)( p / 3 );
    int i = p % 3;
    return gs[n].di( i, x );
  }
  aG *ag( int i )
  {
    if ( i < n )
      return &(gs[i]);
    return NULL;
  }

 signals:
  void nowStat( QString &msg );
};

#endif
