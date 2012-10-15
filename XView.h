#ifndef XVIEW_H
#define XVIEW_H

#include <QWidget>

#define MAXPOINTS ( 10000 )
#define MAXLINES  ( 30 )

const int RingMax = 5 * 60 * 60 * 6;
enum LINEF { NODRAW, LEFT, RIGHT, LINEFS };
enum SCALET { FULLSCALE, I0TYPE, SCALETS }; 
enum GTYPE { XYPLOT, MONITOR, GTYPES };

class XView : public QWidget
{
  Q_OBJECT

private:
  GTYPE GType;
  int valid;
  QColor bgColor;
  QVector<QColor> LC;
  QVector<QString> LNames;
  QString XName;
  double wminx, wmaxx;
  double wminy, wmaxy;
  int minx, miny, maxx, maxy;
  double x[ MAXLINES ][ MAXPOINTS ];
  double y[ MAXLINES ][ MAXPOINTS ];
  int points[ MAXLINES ];
  int lines;
  int LineF[ MAXLINES ];
  int SLineR, SLineL;
  SCALET ScaleTR, ScaleTL;
  double mony[2][ RingMax ]; // Monitor 用の配列は2本だけ。x もなし
  int MonScale;

public:
  XView( QWidget *parent = NULL );
  void NewPoint( int l, double xx, double yy );
  void NewPointR( double yy0, double yy );
  void Clear( void );
  void SetWindow( double x1, double y1, double x2, double y2 );
  void DrawXYPlot( QPainter *p );
  void DrawMonitor( QPainter *p );
  void ReDraw( void );
  void SetGType( GTYPE i ) { GType = i; };
  void SetLName( int i, QString Name ) { LNames.insert( i, Name ); }; // 多分間違ってる
  void SetXName( QString Name ) { XName = Name; };
  void makeValid( int v = true ) { valid = v; };
  void SetSLines( int l1, int l2 ) { SLineR = l1; SLineL = l2; };
  void SetLineF( LINEF f00 = RIGHT, LINEF f01 = LEFT,
		 LINEF f02 = LEFT, LINEF f03 = LEFT, LINEF f04 = LEFT, LINEF f05 = LEFT,
		 LINEF f06 = LEFT, LINEF f07 = LEFT, LINEF f08 = LEFT, LINEF f09 = LEFT,
		 LINEF f10 = LEFT, LINEF f11 = LEFT, LINEF f12 = LEFT, LINEF f13 = LEFT,
		 LINEF f14 = LEFT, LINEF f15 = LEFT, LINEF f16 = LEFT, LINEF f17 = LEFT,
		 LINEF f18 = LEFT, LINEF f19 = LEFT, LINEF f20 = LEFT, LINEF f21 = LEFT,
		 LINEF f22 = LEFT, LINEF f23 = LEFT, LINEF f24 = LEFT, LINEF f25 = LEFT,
		 LINEF f26 = LEFT, LINEF f27 = LEFT, LINEF f28 = LEFT, LINEF f29 = LEFT )
  {
    LineF[ 0] = f00; LineF[ 1] = f01; LineF[ 2] = f02; LineF[ 3] = f03; LineF[ 4] = f04;
    LineF[ 5] = f05; LineF[ 6] = f06; LineF[ 7] = f07; LineF[ 8] = f08; LineF[ 9] = f09;
    LineF[10] = f10; LineF[11] = f11; LineF[12] = f12; LineF[13] = f13; LineF[14] = f14;
    LineF[15] = f15; LineF[16] = f16; LineF[17] = f17; LineF[18] = f18; LineF[19] = f19;
    LineF[20] = f20; LineF[21] = f21; LineF[22] = f22; LineF[23] = f23; LineF[24] = f24;
    LineF[25] = f25; LineF[26] = f26; LineF[27] = f27; LineF[28] = f28; LineF[29] = f29;
  };
  void SetScaleT( SCALET s1, SCALET s2 ) { ScaleTR = s1; ScaleTL = s2; };
  int PeakSearch( int l );
  int getMonScale( void ) { return MonScale; };

public slots:
  void SetMonScale( int ms ) { MonScale = ms; };

private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );

  void setX( int l, int p, double xx );
  void setY( int l, int p, double yy );

  void calcScale( double div, double min, double max, double *s, double *d );
  void SetView( int x1, int y1, int x2, int y2 );
  void UpDateYWindow( int l, SCALET s );
  void UpDateYWindowRing( int LR );
  int w2rx( double x );
  int w2ry( double y );
  int w2rdx( double x );
  int w2rdy( double y );
  double r2wx( int x );
  double r2wy( int y );
  double r2wdx( int x );
  double r2wdy( int y );
};


#endif
