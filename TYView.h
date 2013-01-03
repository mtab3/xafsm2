#ifndef TYVIEW_H
#define TYVIEW_H

#include <QWidget>
#include "ui_XView.h"

#include "ChCoord.h"

const int RingMax = 5 * 60 * 60 * 6;
const int MaxMon = 3;

class TYView : public QFrame, private Ui::XView
{
  Q_OBJECT

private:
  ChCoord cc;
  QColor bgColor, BLACK;
  QVector<QColor> LC;
  QColor MCLineC;          // mouse cursor line color
  int nx, ny;              // current mouse position

  bool valid;
  int MonScale;
  QVector<QString> LNames;

  int lines;
  double Rwmaxy[ MaxMon ];
  double Rwminy[ MaxMon ];
  double mony[ MaxMon ][ RingMax ]; // Monitor 用の配列
  int mont[ RingMax ];
  int ep;     // Ring Buffer の end point 
  int datas;  // Ring Buffer 内の有効なデータ点数

  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );

public:
  TYView( QWidget *parent = NULL );

  void ReDraw( void );
  void NewPointR( int tt, double yy0, double yy1, double yy2 );
  void ClearDataR( void );
  void SetLines( int line );
  void SetLName( int i, QString Name ) { LNames.insert( i, Name ); }; // 多分間違ってる
  void makeValid( bool v = true ) { valid = v; };
  int getMonScale( void ) { return MonScale; };

public slots:
  void SetMonScale( int ms );

private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
  void UpDateYWindowRing( void );
};

#endif
