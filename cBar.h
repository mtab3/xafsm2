#ifndef CBAR_H
#define CBAR_H

#include <QDebug>
#include <QFrame>
#include <QPainter>

#include "ui_cBar.h"
#include "ChCoord.h"
#include "MouseC.h"

class cBar : public QFrame, private Ui::cBar
{
  Q_OBJECT

  MouseC m;
  ChCoord cc;
  QVector<QColor*> cbar;
  QColor White, Black;
  int colors;
  int cmin, cmax;
  double zmax, zmin;
  bool autoScale;

public:
  cBar( QWidget *p );

private:
  void initColor( void );
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );

  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );

  void showZZ( void ) {
    emit newZZ( QString::number( zmax ), QString::number( zmin ) );
  }

public slots:
  void setAutoScale( bool f ) { autoScale = f; };
  void newAutoZmax( double max ) { if ( autoScale ) zmax = max; showZZ(); };
  void newAutoZmin( double min ) { if ( autoScale ) zmin = min; showZZ(); };
  void newInputZmax( double max ) { if ( ! autoScale ) zmax = max; showZZ(); };
  void newInputZmin( double min ) { if ( ! autoScale ) zmin = min; showZZ(); };

signals:
  void newZZ( QString max, QString min );
};

#endif
