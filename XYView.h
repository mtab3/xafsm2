#ifndef XYVIEW_H
#define XYVIEW_H

#include <QWidget>
#include <QPrinter>

#include "ui_XView.h"

#include "Diff.h"
#include "MouseC.h"
#include "ChCoord.h"

#define MAXPOINTS  ( 10000 )
#define MAXLINENO  ( 50000 ) // 0 - 19999      // I0 と I, mu があるので実質 10,000
#define MAXGROUPLINES ( 5 )  // 最大何本の線が一つのグループになるか (I0, I, mu, A1, mu2)
#define MAXLINES   ( 1000 )  // 内部で準備する線の数 (ほんとは 990 でいいはず)
// 受け入れられる line No. の最大値
// (足跡表示の線の数) * (足跡間隔) + 連番表示の線の数
// のはずだけど、ちょっと怖いので、(足跡表示の線の数) * (足跡間隔)という事にしておく

// 以下 GROUPLINES == 3 の時の話。
// 確認の為 GROUPLINES == 5 の時のこともこの下に書いてみた

// l 番目のラインの内部でのライン番号 L を
//   G1 = ( l / GROUPLINES )
//   G2 = ( l mod GROUPLINES )
//   L = ( ( G1 mod CGROUPS ) + B ) * GROUPLINES + G2
//   B = (int)( G1 / FINTERVAL ) 
// とすると
//
// FINTERVAL = 10, CGROUPS = 10, GROUPLINES = 3 の場合
// ( 0  1  2)( 3  4  5)( 6  7  8)( 9 10 11)(12 13 14)...(27 28 29)
//           (30 31 32)(33 34 35)(36 37 38)...                    (57 58 59)
//                     (60 61 62)(63 64 65)(66 67 68)...                    (97 98 99)
//                             (100 101 102)...
// ということで、
// 1) 総 group 数 100 まで (line 数 300まで) は
//    CGROUPS = 100, FINTERVAL = 100 にしておく
//    要は、全部の line をそのまま描く (最大 L=300)
// 2) 総 group 数が 100 を超えた時点で
//    先頭から 10 group分 (g=0,1,2,...9)を
//    g = 0, 10, 20, 30, ... 90 にある group と置き換えて
//    CGROUPS = 10, FINTERVAL = 10 に変更する
//    以降、例えば 100 番目のグループについては
//          G = ( 100 mod 10 ) + (int)( 100 / 10 ) = 10
//    になって、最初から CGROUPS = 10, FINTERVAL = 10 だった場合と同じ位置に置かれる
// 3) 総 group 数が 1000 を超えた時点で
//    0 10 20 ... 990(G=99) 991 992 993 ... 999(G=108,L=324)
//    先頭から 10 グループ分(G=0,1,...9)を
//    G = 0, 10, 20, 30, ... 90 にある
//    g = 0, 200, 400, 600, ... 1800 と置き換えて
//    CGROUPS = 100, FINTERVAL = 100 に変更する
//    以降、例えば 1000 番目は G = ( 1000 mod 100 ) + (int)( 1000 / 100 ) = 0 + 10 = 10
//    0 100 ... 9900(G=99) 9901 9902 ... 9999 (G=198,L=594) が最大

// 確認の為 GROUPLINES == 5 の時

// l 番目のラインの内部でのライン番号 L を
//   G1 = ( l / GROUPLINES )
//   G2 = ( l mod GROUPLINES )
//   L = ( ( G1 mod CGROUPS ) + B ) * GROUPLINES + G2
//   B = (int)( G1 / FINTERVAL ) 
// とすると
//
// FINTERVAL = 10, CGROUPS = 10, GROUPLINES = 5 の場合
// (0...)(5...)(10...)...              (45...)
//       (50..)(55...)(60...)                 (95...)
//             (100..)(105..)                            (145...)
//                    (150..)...
//
// ということで、
// 1) 総 group 数 100 まで (line 数 500まで) は
//    CGROUPS = 100, FINTERVAL = 100 にしておく
//    要は、全部の line をそのまま描く (最大 L=500)
// 2) 総 group 数が 100 を超えた時点で
//    先頭から 10 group分 (g=0,1,2,...9)を
//    g = 0, 10, 20, 30, ... 90 にある group と置き換えて
//    CGROUPS = 10, FINTERVAL = 10 に変更する
//    以降、例えば 100 番目のグループについては
//          G = ( 100 mod 10 ) + (int)( 100 / 10 ) = 10
//    になって、最初から CGROUPS = 10, FINTERVAL = 10 だった場合と同じ位置に置かれる
// 3) 総 group 数が 1000 を超えた時点で
//    0 10 20 ... 990(G=99) 991 992 993 ... 999(G=108,L=540)
//    先頭から 10 グループ分(G=0,1,...9)を
//    G = 0, 10, 20, 30, ... 90 にある
//    g = 0, 200, 400, 600, ... 1800 と置き換えて
//    CGROUPS = 100, FINTERVAL = 100 に変更する
//    以降、例えば 1000 番目は G = ( 1000 mod 100 ) + (int)( 1000 / 100 ) = 0 + 10 = 10
//    0 100 ... 9900(G=99) 9901 9902 ... 9999 (G=198,L=990) が最大



enum LINEF { NODRAW, LEFT, RIGHT, LINEFS };
enum SCALET { FULLSCALE, I0TYPE, SCALETS }; 
enum LRAX { LEFT_AX, RIGHT_AX };

class XYView : public QFrame, private Ui::XView
{
  Q_OBJECT

private:
  ChCoord cc;

  QWidget *parent;
  bool autoScale;
  bool singleScale;
  bool QXafsMode;

  QColor bgColor, BLACK;
  QVector<QColor> LC;
  QColor MCLineC;          // mouse cursor line color
  QColor ASelC;
  QColor ProgressBBC;      // Progress Bar box color
  QColor ProgressBC;       // Progress Bar color
  
  int DispGroup[ MAXLINES ];     // 同じスケールで表示される線のグループ分け
  LRAX LineLR[ MAXLINES ];       // 線のスケールを左右どちらに表示するか
  SCALET scaleType[ MAXLINES ];
  QString LeftName, RightName;
  QString LNames[ MAXLINES ];
  QString XName;
  QString XUnitName;
  bool valid;
  double upp;
  double center;
  bool AreaSelecting;
  int SelLR[2];
  int unitType;
  double offset;

  double origMinx, origMaxx;
  double XShift, XShift0, xshift;
  double YShift[ MAXLINES ], YShift0[ MAXLINES ], yshift[ MAXLINES ];

  int CGroups, FInterval;      // Continuous drawn groups, Interval of foot print groups
  int maxGroups;               // 外部から見た時の ライン数 でこれまで指定された最大値
  int maxLines;
  int inLines;                 // 内部で使われているライン番号の最大値
  bool dispf[ MAXLINES ];
  int points[ MAXLINES ];
  double x[ MAXLINES ][ MAXPOINTS ];
  double y[ MAXLINES ][ MAXPOINTS ];
  double miny[ MAXLINES ];
  double maxy[ MAXLINES ];
  double minGy[ MAXLINES ];
  double maxGy[ MAXLINES ];
  double SaveYatNowXp[ MAXLINES ];
  int grouplines;

  MouseC m;
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
  void wheelEvent( QWheelEvent *e );
  int getL( int l );
  void ReFillFirst10Groups( void );

  bool ShowProgressB;
  double ProgressScale;
  double Progress;

  bool showDiff1;
  bool showDiff2;
  WTYPE dwtype1, dwtype2;

public:
  XYView( QWidget *parent = NULL );

  void enabledSingleScale( bool f ) { singleScale = f; };
  void NewPoint( int l, double xx, double yy );
  void ReNewPoint( int l, int ix, double yy );
  void Clear( void );
  // XYView では、複数の線を同時に描ける。
  // 各線が左軸、右軸どちらに関係しているかを指定するのが SetLR(), LintLR[]
  // 沢山ある線の中で、左右の軸に実際にスケールを表示する線を選択するのが
  // SetLLine(), SetRLine(), SelLR[]
  void SetLLine( int l ) { SelLR[ LEFT_AX ] = getL( l ); };
  void SetRLine( int l ) { SelLR[ RIGHT_AX ] = getL( l ); };
  void SetLR( int L, LRAX lr ) { LineLR[ getL( L ) ] = lr; };
  // 共通のスケーリングを行う線のグループ登録
  void SetDG( int L, int grp ) { DispGroup[ getL( L ) ] = grp; };
  // スケールのタイプ : フルスケールにするか、I0 の様に少し上にずらすか
  // ループ単位で指定。
  void SetScaleType( int l, SCALET t ) { scaleType[ getL( l ) ] = t; };
  void SetColor( int l, QColor c ) { LC[ getL( l ) ] = c; };
  QColor GetColor( int l ) { return LC.at( getL( l ) % LC.count() ); };
  void ScaleChange( int l );

  // 縦軸は左右一本ずつ合計 2本しかないので、縦軸を持てるグループは最大 2 つだけ。
  void SetLineName( int l, QString Name ) { LNames[ getL( l ) ] = Name; };
  void SetLeftName( QString Name ) { LeftName = Name; };
  void SetRightName( QString Name ) { RightName = Name; };
  void SetXName( QString Name ) { XName = Name; };
  void SetXUnitName( QString name ) { XUnitName = name; };
  void SetUnitType( int ut ) { unitType = ut; };
  void SetOffset( double off ) { offset = off; };
  void makeValid( bool v = true ) { valid = v; };
  int GetPoints( int l );
  void SetPoints( int l, int p ) { points[ getL( l ) ] = p; };
  int GetLines( void ) { return maxLines; };
  double GetX( int l, int p );
  double GetY( int l, int p );
  double *GetXp( int l ) { return x[ getL( l ) ]; };
  double *GetYp( int l ) { return y[ getL( l ) ]; };
  void SetWindow( double x1, double y1, double x2, double y2 )
  { cc.SetRealCoord( x1, y1, x2, y2 ); };
  void SetWindow0( double x1, double y1, double x2, double y2 )
    { origMinx = x1; origMaxx = x2; cc.SetRealCoord0( x1, y1, x2, y2 ); };
  void SetUpp( double upp0 ) { upp = upp0; };
  void SetCenter( double center0 ) { center = center0; };
  void SetAutoScale( bool ascale ) { autoScale = ascale; };
  void SetDispF( int l, bool f ) { dispf[ getL( l ) ] = f; };
  void SetQXafsMode( bool f ) { QXafsMode = f; };
  void SetGroupLines( int ls ) { grouplines = ls; };
  void ShowProgressBar( bool f ) { ShowProgressB = f; };
  void SetProgressScale( double s ) { ProgressScale = s; };
  void SetProgress( double p ) { Progress = p; };
  void print( QPrinter *p );

  void setParent( QWidget *p );

public slots:
  void ChooseAG( int i, bool f );
  void setDiffType1( int i ) { dwtype1 = (WTYPE)i; };
  void setDiffType2( int i ) { dwtype2 = (WTYPE)i; };

 signals:
  void SelectAPoint( double x, double y );
  void MovedToNewX( int ix, double x );

private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
  void UpDateYWindow( int g, SCALET s );
  void ShowAScaleButton( QPainter *p );
  void CheckASPush( QMouseEvent *e );
};

#endif
