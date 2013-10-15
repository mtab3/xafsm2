#ifndef S2DVIEW_H
#define S2DVIEW_H

#include <QFrame>

#include "ui_S2DView.h"
#include "MouseC.h"
#include "ChCoord.h"

enum RATIO_TYPE { REAL_RATIO, AS_SCREEN };

class S2DView : public QFrame, private Ui::S2DView
{
  Q_OBJECT

  QWidget *Parent;
  ChCoord cc;
  RATIO_TYPE rType;
  double minx, maxx, miny, maxy;
  double sx, sy, dx, dy;
  int maxix, maxiy;
  int lastIx, lastIy;
  int showIx, showIy;
  int showIx0, showIy0;
  double **data;
  bool **valid;

  bool AutoScale;
  QColor Grey;
  QColor cbar[ 256 * 4 ];
  double minz, maxz;   // データの最大最小
  double vmin, vmax;   // 表示レンジの最大最小
  int cmin, cmax;      // 色番号の最大最小

  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
  int cNum( double v );

  MouseC m;
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
  void wheelEvent( QWheelEvent *e );

 public:
  S2DView( QWidget *p );

  void setRatioType( RATIO_TYPE r ) { rType = r; };
  void setRange( double sx, double sy, double dx, double dy, int ix, int iy );
  void setData( int ix, int iy, double v );
  void setParent( QWidget *p );

 signals:
  void AskMoveToPointedPosition( int x, int y );
  void PointerMovedToNewPosition( int x, int y );
  void AskToChangeMCACh( int dCh );
};

#endif
