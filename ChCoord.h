#ifndef CHCOORD_H
#define CHCOORD_H

#include <QtGui>
#include <QObject>

enum DRAWTXTF { FIXSIZE, SCALESIZE };

class ChCoord : public QObject
{
  Q_OBJECT

  QColor ASBBorderC, ASBOnC, ASBOffC;
  QColor BLACK;

  double sminx, smaxx, sminy, smaxy;
  double rminx, rmaxx, rminy, rmaxy;
  double rminx0, rmaxx0, rminy0, rmaxy0;

 public:
  ChCoord();

  void SetScreenCoord( double x1, double y1, double x2, double y2 );
                                                 // on screen window size and position
  void SetRealCoord( double x1, double y1, double x2, double y2 );
  void SetRealCoord0( double x1, double y1, double x2, double y2 );
  void SetRealX( double x1, double x2 );
  void SetRealX0( double x1, double x2 );
  void SetRealX0( void ) { rminx0 = rminx; rmaxx0 = rmaxx; };
  void SetRealY( double y1, double y2 );
  void SetRealY0( double y1, double y2 );
  void RecallRealCoord( void );
  void RecallRealX( void );
  void RecallRealY( void );
  double r2sx( double x );     // from realsize to screen size
  double r2sy( double y );     
  double r2sdx( double x );
  double r2sdy( double y );
  double s2rx( double x );     // from screen to realsize
  double s2ry( double y );
  double s2rx0( double x );     // from screen to realsize
  double s2ry0( double y );
  double s2rxLimit( double x );
  double s2ryLimit( double y );
  double s2rdx( double x );
  double s2rdy( double y );

  double fixy( double y, double min, double max );

  double Rmaxx( void ) { return rmaxx; };
  double Rminx( void ) { return rminx; };
  double Rmaxy( void ) { return rmaxy; };
  double Rminy( void ) { return rminy; };

  double Smaxx( void ) { return smaxx; };
  double Sminx( void ) { return sminx; };
  double Smaxy( void ) { return smaxy; };
  double Sminy( void ) { return sminy; };

  void getSEDy0( double *sy, double *ey, double *dy, double div );
  void getSEDy( double *sy, double *ey, double *dy, double div );
  void calcScale( double div, double min, double max, double *s, double *d );

  /* ChCoord とは直接関係ないけど、グラフ関係の一般的な関数なのでここに置いとく */

  void DrawText( QPainter *p, 
		 QRectF rec, QFont font, int flags, DRAWTXTF f, QString msg );
  void ShowAButton( QPainter *p, bool BState, QString BName,
		    int x0, int w, int height );
#if 0
  void ShowAScaleButton( QPainter *p, bool autoScale, int height );
  void ShowSScaleButton( QPainter *p, bool singleScale, int height );
#endif
};

#endif
