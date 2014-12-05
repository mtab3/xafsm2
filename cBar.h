#ifndef CBAR_H
#define CBAR_H

#include <QFrame>
#include <QPainter>

#include "ui_cBar.h"
#include "ChCoord.h"

class cBar : public QFrame, private Ui::cBar
{
  Q_OBJECT

  MouseC m;
  ChCoord cc;
  QVector<QColor*> cbar;
  QColor White, Black;
  int colors;
  int cmin, cmax;

 public:
  cBar( QWidget *p );

 private:
  void initColor( void );
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );

  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
};

#endif
