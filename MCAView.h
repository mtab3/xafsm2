#ifndef MCAVIEW_H
#define MCAVIEW_H

#include "ui_MCAView.h"

class MCAView : public QFrame, private Ui::MCAView
{
  Q_OBJECT

private:
  int valid;
  int *MCA;
  int MCALen;

  bool dispLog;

  int minx, maxx, miny, maxy;
  double wminx, wmaxx, wminy, wmaxy;

  bool inPress;
  int nx, ny, sx, sy, ex, ey;         // mouse 

  QColor Blue, Black, Red, Green, White;

  void Draw( QPainter *p );

  void paintEvent( QPaintEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );

  void SetView( int x1, int y1, int x2, int y2 ); // on screen window size and position
  void SetWindow( double x1, double y1, double x2, double y2 );
  int w2rx( double x );
  int w2ry( double y );
  int w2rdx( double x );
  int w2rdy( double y );
  double r2wx( int x );
  double r2wy( int y );
  double r2wdx( int x );
  double r2wdy( int y );

public:
  MCAView( QWidget *parent = NULL );
  void setMCAdataPointer( int *mca, int length );

public slots:
  void setLog( bool f ) { dispLog = f; update(); };

signals:
  void CurrentValues( int atCur, int inROI );
  void newROI( int ROIstart, int ROIend );
};

#endif
