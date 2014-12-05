#ifndef CBAR_H
#define CBAR_H

#include <QFrame>
#include <QPainter>

#include "ui_cBar.h"
#include "ChCoord.h"

class cBar : public QFrame, private Ui::cBar
{
  Q_OBJECT

  ChCoord cc;
  QVector<QColor*> cbar;
  QColor White, Black;
  int cmin, cmax;

 public:
  cBar( QWidget *p );

 private:
  void initColor( void );
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
};

#endif
