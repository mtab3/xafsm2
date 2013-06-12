#ifndef CBAR_H
#define CBAR_H

#include <QFrame>
#include "ui_cBar.h"
#include "ChCoord.h"

class cBar : public QFrame, private Ui::cBar
{
  Q_OBJECT

  ChCoord cc;
  QColor cbar[ 256 * 4 ];

 public:
  cBar( QWidget *p );

 private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
};

#endif
