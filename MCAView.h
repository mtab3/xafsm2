#ifndef MCAVIEW_H
#define MCAVIEW_H

#include "ui_MCAView.h"
#include "ChCoord.h"

class MCAView : public QFrame, private Ui::MCAView
{
  Q_OBJECT

private:
  int valid;
  int *MCA;
  int MCALen;

  bool dispLog;

  ChCoord cc;
  bool inPress;
  int nx, ny, sx, sy, ex, ey;         // mouse 

  QColor Blue, Black, Red, Green, White;

  void Draw( QPainter *p );

  void paintEvent( QPaintEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
public:
  MCAView( QWidget *parent = NULL );
  ~MCAView( void );

  int *setMCAdataPointer( int length );

public slots:
  void setLog( bool f ) { dispLog = f; update(); };

signals:
  void CurrentValues( int atCur, int inROI );
  void newROI( int ROIstart, int ROIend );
};

#endif
