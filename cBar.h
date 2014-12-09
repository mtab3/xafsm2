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
  QColor White, Black, LimitC;
  int colors;
  int cmax, cmin;
  int cmaxShowP, cminShowP;
  double zmax, zmin;
  double rmax, rmin;
  bool autoScale;

public:
  cBar( QWidget *p );
  QColor *c( double z );

private:
  void initColor( void );
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );

  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );

  void showZZ( void );

public slots:
  void setAutoScale( bool f ) { autoScale = f; newScale(); };
  void newAutoZmax( double max );
  void newAutoZmin( double min );
  void newInputZmax( double max );
  void newInputZmin( double min );

signals:
  void newZZ( QString max, QString min );
  void newScale( void );
};

#endif
