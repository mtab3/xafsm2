#ifndef TYVIEW_H
#define TYVIEW_H

#include <QWidget>
#include <QPrinter>

#include "ui_XView.h"

#include "CColors.h"
#include "MouseC.h"
#include "ChCoord.h"
#include "MonInfo.h"

const int RingMax = 5 * 60 * 60 * 6;
const int MaxMon = 20;

class TYView : public QFrame, private Ui::XView
{
  Q_OBJECT

private:
  QWidget *parent;

  MonInfo monInfo;
  ChCoord cc;
  QColor bgColor, BLACK, ASelC;
  //  QVector<QColor> LC;
  QColor MCLineC;          // mouse cursor line color
  CColors *LCs;

  bool valid;
  int MonScale;
  QVector<QString> LNames;
  bool autoScale;
  bool logScale;
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
  int movingAvr;

  MouseC m;
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
  void wheelEvent( QWheelEvent *e );

public:
  TYView( QWidget *parent = NULL );

  static int maxMon( void ) { return MaxMon; };
  void ReDraw( void );
  void NewPointR( int tt, double *yy, int n );
  void ClearDataR( void );
  void SetLines( int Lines );
  int GetLines( void ) { return lines; };
  void SetLName( int i, QString Name );
  void makeValid( bool v = true ) { valid = v; };
  int getMonScale( void ) { return MonScale; };
  QColor getColor( int i ) { return LCs->at(i); };
  int getDatas( void ) { return datas; };
  int getT( int i ) { return mont[ ep - datas + i ]; };
  double getY( int l, int i ) { return mony[l][ ep - datas + i ]; };
  void print( QPrinter *p );

  void setParent( QWidget *p );
  void setMovingAvr( int ma ) { movingAvr = ma; };
  void setMonInfo( MonInfo mi ) { monInfo = mi; };
  MonInfo getMonInfo() { return monInfo; };

public slots:
  void SetMonScale( int ms ) { MonScale = ms; };

private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
  void UpDateYWindowRing( void );
  void CheckASPush( QMouseEvent *e );
  void CheckLSPush( QMouseEvent *e );
  void drawYLabel( QPainter *p, int i, double LM, double VDiv, bool manyLs );

signals:
  void UpScale( void );
  void DownScale( void );
};

#endif
