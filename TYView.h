#ifndef TYVIEW_H
#define TYVIEW_H

#include <QWidget>
#include "ui_XView.h"

#include "ChCoord.h"

#if 0
#define MAXPOINTS ( 10000 )
#define MAXLINES  ( 30 )

enum LINEF { NODRAW, LEFT, RIGHT, LINEFS };
enum SCALET { FULLSCALE, I0TYPE, SCALETS }; 
enum GTYPE { XYPLOT, MONITOR, GTYPES };
#endif

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

  int valid;
  int MonScale;
  QVector<QString> LNames;

  int lines;
  double Rwmaxy[ MaxMon ];
  double Rwminy[ MaxMon ];
  double mony[ MaxMon ][ RingMax ]; // Monitor —p‚Ì”z—ñ
  int mont[ RingMax ];
  int points;

  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );

#if 0
  QString XName;

  SCALET ScaleTR, ScaleTL;
#endif

public:
  TYView( QWidget *parent = NULL );

  void ReDraw( void );
  void NewPointR( int tt, double yy0, double yy1, double yy2 );
  void ClearDataR( void );
  void SetLines( int line );
  void SetLName( int i, QString Name ) { LNames.insert( i, Name ); }; // ‘½•ªŠÔˆá‚Á‚Ä‚é
  void makeValid( int v = true ) { valid = v; };
  int getMonScale( void ) { return MonScale; };
#if 0
  void DrawMonitor( QPainter *p );
  void SetXName( QString Name ) { XName = Name; };
  void SetScaleT( SCALET s1, SCALET s2 ) { ScaleTR = s1; ScaleTL = s2; };
  void SetWindow( double x1, double y1, double x2, double y2 )
  { cc.SetRealCoord( x1, y1, x2, y2 ); };
#endif

public slots:
  void SetMonScale( int ms );

private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
  void UpDateYWindowRing( void );
#if 0

  void calcScale( double div, double min, double max, double *s, double *d );
#endif
};
#endif
