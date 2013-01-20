#ifndef MOUSEC_H
#define MOUSEC_H

#include <QtGui>

class MouseC : public QObject
{
  Q_OBJECT

  int nx, ny;
  int Sx, Sy;
  int Ex, Ey;

  bool inpress;

 public:
  MouseC( void );

  void Moved( QMouseEvent *e ) { nx = e->x(); ny = e->y(); };
  void Pressed( QMouseEvent *e ) { Sx = e->x(); Sy = e->y(); inpress = true; };
  void Released( QMouseEvent *e ) { Ex = e->x(); Ey = e->y(); inpress = false; };
  void setSx( int x ) { Sx = x; };
  void setSy( int y ) { Sy = y; };
  void setEx( int x ) { Ex = x; };
  void setEy( int y ) { Ey = y; };
  int x( void ) { return nx; };
  int y( void ) { return ny; };
  int sx( void ) { return Sx; };
  int sy( void ) { return Sy; };
  int ex( void ) { return Ex; };
  int ey( void ) { return Ey; };
  bool inPress( void ) { return inpress; };
};


#endif
