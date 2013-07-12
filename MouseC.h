#ifndef MOUSEC_H
#define MOUSEC_H

#include <QtGui>

class MouseC : public QObject
{
  Q_OBJECT

  int nx, ny;
  int Sx, Sy;
  int Ex, Ey;
  int DCx, DCy;

  bool inpress;
  Qt::MouseButton Button;
  Qt::KeyboardModifiers Mod;

 public:
  MouseC( void );

  void Moved( QMouseEvent *e );
  void Pressed( QMouseEvent *e );
  void Released( QMouseEvent *e );
  void DClicked( QMouseEvent *e );
  void setBut( Qt::MouseButton b ) { Button = b; };
  void setMod( Qt::KeyboardModifiers m ) { Mod = m; };
  void setInP( bool p ) { inpress = p; };
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
  int dcx( void ) { return DCx; };
  int dcy( void ) { return DCy; };
  Qt::KeyboardModifiers getMod( void ) { return Mod; };

  Qt::MouseButton button( void ) { return Button; };
  //it will return, 'OR' operated following values : 
  //      Qt::LeftButton, Qt::RightButton, Qt::MidButton, Qt::NoButton

  Qt::KeyboardModifiers modifier( void ) { return Mod; };
  //it will return, 'OR' operated following values : 
  //      Qt::NoModifier,
  //      Qt::ShiftModifier, Qt::ControlModifier, Qt::AltModifier, Qt::MetaModifier,
  //      Qt::KeypadModifier, Qt::GroupSwitchModifier

  bool inPress( void ) { return inpress; };
};


#endif
