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
  RATIO_TYPE rType;
  double minx, maxx, miny, maxy;
  double sx, sy, dx, dy;
  int maxix, maxiy;
  double **data;

  bool AutoScale;
  QColor cbar[ 256 * 4 ];
  double minz, maxz;   // �ǡ����κ���Ǿ�
  double vmin, vmax;   // ɽ����󥸤κ���Ǿ�
  int cmin, cmax;      // ���ֹ�κ���Ǿ�

 public:
  S2DView( QWidget *p );

  void setRatioType( RATIO_TYPE r ) { rType = r; };
  void setRange( double sx, double sy, double dx, double dy, int ix, int iy );
  void setData( int ix, int iy, double v );

 private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
  int cNum( double v );

};

#endif
