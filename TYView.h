#ifndef TYVIEW_H
#define TYVIEW_H

#include <QWidget>
#include "ui_XView.h"

#include "MouseC.h"
#include "ChCoord.h"

const int RingMax = 5 * 60 * 60 * 6;
const int MaxMon = 3;

class TYView : public QFrame, private Ui::XView
{
  Q_OBJECT

private:
  ChCoord cc;
  QColor bgColor, BLACK, ASelC;
  QVector<QColor> LC;
  QColor MCLineC;          // mouse cursor line color

  bool valid;
  int MonScale;
  QVector<QString> LNames;
  bool autoScale;
  bool AreaSelecting;

  int lines;
  double Rwmaxy[ MaxMon ];
  double Rwminy[ MaxMon ];
  double mony[ MaxMon ][ RingMax ]; // Monitor 用の配列
  int mont[ RingMax ];
  int ep;     // Ring Buffer の end point 
  int datas;  // Ring Buffer 内の有効なデータ点数
  int timeShift, timeShift0, tts;   // 表示の時間を現在の時間からどれだけずらすか
  double YShift[ MaxMon ], YShift0[ MaxMon ], yshift[ MaxMon ];

  MouseC m;
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
  void wheelEvent( QWheelEvent *e );

public:
  TYView( QWidget *parent = NULL );

  void ReDraw( void );
  void NewPointR( int tt, double yy0, double yy1, double yy2 );
  void ClearDataR( void );
  void SetLines( int Lines ) { lines = Lines; };
  int GetLines( void ) { return lines; };
  void SetLName( int i, QString Name ) { LNames.value( i ) = Name; };
  void makeValid( bool v = true ) { valid = v; };
  int getMonScale( void ) { return MonScale; };
  QColor getColor( int i ) { return LC[i]; };
  int getDatas( void ) { return datas; };
  int getT( int i ) { return mont[ ep - datas + i ]; };
  int getY( int l, int i ) { return mony[l][ ep - datas + i ]; };

public slots:
  void SetMonScale( int ms ) { MonScale = ms; };;

private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
  void UpDateYWindowRing( void );
  void CheckASPush( void );
};

#endif
