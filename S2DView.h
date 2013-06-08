#ifndef S2DVIEW_H
#define S2DVIEW_H

#include <QFrame>
#include "ui_S2DView.h"
#include "ChCoord.h"

enum RATIO_TYPE { REAL_RATIO, AS_SCREEN };

class S2DView : public QFrame, private Ui::S2DView
{
  Q_OBJECT

  ChCoord cc;
  double rType;
  double minx, maxx, miny, maxy;
  int xSteps, ySteps;

 public:
  S2DView( QWidget *p );

  void setRatioType( RATIO_TYPE r ) { rType = r; };
  void setRange( double x1, double x2, double y1, double y2 ) {
    if ( x1 > x2 ) { maxx = x1; minx = x2; } else { maxx = x2; minx = x1; }
    if ( y1 > y2 ) { maxy = y1; miny = y2; } else { maxy = y2; miny = y1; }
    cc.SetRealCoord( minx, maxx, miny, maxy );
  };
  void setSteps( int x, int y ) { xSteps = x; ySteps = y; };

 private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );

};

#endif
