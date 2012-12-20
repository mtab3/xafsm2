#ifndef CHCOORD_H
#define CHCOORD_H

#include <QtGui>
#include <QObject>

enum DRAWTXTF { FIXSIZE, SCALESIZE };

class ChCoord : public QObject
{
  Q_OBJECT

  int minx, maxx, miny, maxy;
  double wminx, wmaxx, wminy, wmaxy;

 public:
  ChCoord();

  void SetView( int x1, int y1, int x2, int y2 ); // on screen window size and position
  void SetWindow( double x1, double y1, double x2, double y2 );
  void SetWindowX( double x1, double x2 );
  void SetWindowY( double y1, double y2 );
  int w2rx( double x );
  int w2ry( double y );
  int w2rdx( double x );
  int w2rdy( double y );
  double r2wx( int x );
  double r2wy( int y );
  double r2wxLimit( int x );
  double r2wyLimit( int y );
  double r2wdx( int x );
  double r2wdy( int y );

  double Wmaxx( void ) { return wmaxx; };
  double Wminx( void ) { return wminx; };
  double Wmaxy( void ) { return wmaxy; };
  double Wminy( void ) { return wminy; };

  int Maxx( void ) { return maxx; };
  int Minx( void ) { return minx; };
  int Maxy( void ) { return maxy; };
  int Miny( void ) { return miny; };

  void getSEDy( double *sy, double *ey, double *dy );
  void calcScale( double div, double min, double max, double *s, double *d );

  /* ChCoord とは直接関係ないけど、グラフ関係の一般的な関数なのでここに置いとく */

  void DrawText( QPainter *p, 
		 QRectF rec, QFont font, int flags, DRAWTXTF f, QString msg );
};

#endif
