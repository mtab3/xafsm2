#ifndef XYVIEW_H
#define XYVIEW_H

#include <QWidget>
#include "ui_XView.h"

#include "MouseC.h"
#include "ChCoord.h"

#define MAXPOINTS  ( 10000 )
#define MAXLINENO  ( 30000 )  // 0 �` 19999      // I0 �� I, mu ������̂Ŏ��� 10,000
#define GROUPLINES ( 3 )      // ���{�̐�����̃O���[�v�ɂȂ邩 (I0, I, mu)
#define MAXLINES   ( 600 )    // �����ŏ���������̐� (�ق�Ƃ� 594 �ł����͂�)
// �󂯓������ line No. �̍ő�l
// (���Օ\���̐��̐�) * (���ՊԊu) + �A�ԕ\���̐��̐�
// �̂͂������ǁA������ƕ|���̂ŁA(���Օ\���̐��̐�) * (���ՊԊu)�Ƃ������ɂ��Ă���

// l �Ԗڂ̃��C���̓����ł̃��C���ԍ� L ��
//   G1 = ( l / GROUPLINES )
//   G2 = ( l mod GROUPLINES )
//   L = ( ( G1 mod CGROUPS ) + B ) * GROUPLINES + G2
//   B = (int)( G1 / FINTERVAL ) 
// �Ƃ����
//
// FINTERVAL = 10, CGROUPS = 10, GROUPLINES = 3 �̏ꍇ
// ( 0  1  2)( 3  4  5)( 6  7  8)( 9 10 11)(12 13 14)...(27 28 29)
//           (30 31 32)(33 34 35)(36 37 38)...                    (57 58 59)
//                     (60 61 62)(63 64 65)(66 67 68)...                    (97 98 99)
//                             (100 101 102)...
// �Ƃ������ƂŁA
// 1) �� group �� 100 �܂� (line �� 300�܂�)
//    CGROUPS = 100, FINTERVAL = 100 �ɂ��Ă���
//    �v�́A�S���� line �����̂܂ܕ`�� (�ő� L=300)
// 2) �� group ���� 100 �𒴂������_��
//    �擪���� 10 group�� (g=0,1,2,...9)��
//    g = 0, 10, 20, 30, ... 90 �ɂ���
//    g = 0, 10, 20, 30, ... 90 �ƒu��������
//    CGROUPS = 10, FINTERVAL = 10 �ɕύX����
//    �ȍ~�A�Ⴆ�� 100 �Ԗڂ̃O���[�v�ɂ��Ă�
//          G = ( 100 mod 10 ) + (int)( 100 / 10 ) = 10
//    �ɂȂ��āA�ŏ����� CGROUPS = 10, FINTERVAL = 10 �������ꍇ�Ɠ����ʒu�ɒu�����
// 3) �� group ���� 1000 �𒴂������_��
//    0 10 20 ... 990(G=99) 991 992 993 ... 999(G=108,L=324)
//    �擪���� 10 �O���[�v��(G=0,1,...9)��
//    G = 0, 10, 20, 30, ... 90 �ɂ���
//    g = 0, 200, 400, 600, ... 1800 �ƒu��������
//    CGROUPS = 100, FINTERVAL = 100 �ɕύX����
//    �ȍ~�A�Ⴆ�� 1000 �Ԗڂ� G = ( 1000 mod 100 ) + (int)( 1000 / 100 ) = 0 + 10 = 10
//    0 100 ... 9900(G=99) 9901 9902 ... 9999 (G=198,L=594) ���ő�

enum LINEF { NODRAW, LEFT, RIGHT, LINEFS };
enum SCALET { FULLSCALE, I0TYPE, SCALETS }; 
enum LRAX { LEFT_AX, RIGHT_AX };

class XYView : public QFrame, private Ui::XView
{
  Q_OBJECT

private:
  ChCoord cc;
  bool autoScale;
  bool QXafsMode;

  QColor bgColor, BLACK;
  QVector<QColor> LC;
  QColor MCLineC;          // mouse cursor line color
  QColor ASelC;

  LRAX LineLR[ MAXLINES ];
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

  double origMinx, origMaxx;
  double XShift, XShift0, xshift;
  double YShift[ MAXLINES ], YShift0[ MAXLINES ], yshift[ MAXLINES ];

  int CGroups, FInterval;      // Continuous drawn groups, Interval of foot print groups
  int maxGroups;               // �O�����猩������ ���C���� �ł���܂Ŏw�肳�ꂽ�ő�l
  int maxLines;
  int inLines;                 // �����Ŏg���Ă��郉�C���ԍ��̍ő�l
  bool dispf[ MAXLINES ];
  int points[ MAXLINES ];
  double x[ MAXLINES ][ MAXPOINTS ];
  double y[ MAXLINES ][ MAXPOINTS ];
  double miny[ MAXLINES ];
  double maxy[ MAXLINES ];
  double SaveYatNowXp[ MAXLINES ];

  MouseC m;
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
  void wheelEvent( QWheelEvent *e );
  int getL( int l );
  void ReFillFirst10Groups( void );

public:
  XYView( QWidget *parent = NULL );

  void NewPoint( int l, double xx, double yy );
  void Clear( void );
  // XYView �ł́A�����̐��𓯎��ɕ`����B
  // �e���������A�E���ǂ���Ɋ֌W���Ă��邩���w�肷��̂� SetLR(), LintLR[]
  // ��R������̒��ŁA���E�̎��Ɏ��ۂɃX�P�[����\���������I������̂�
  // SetLLine(), SetRLine(), SelLR[]
  void SetLLine( int l ) { SelLR[ LEFT_AX ] = getL( l ); };
  void SetRLine( int l ) { SelLR[ RIGHT_AX ] = getL( l ); };
  void SetLR( int L, LRAX lr ) { LineLR[ getL( L ) ] = lr; };
  // �X�P�[���̃^�C�v : �t���X�P�[���ɂ��邩�AI0 �̗l�ɏ�����ɂ��炷��
  // ���[�v�P�ʂŎw��B
  void SetScaleType( int l, SCALET t ) { scaleType[ getL( l ) ] = t; };
  void SetColor( int l, QColor c ) { LC[ getL( l ) ] = c; };
  QColor GetColor( int l ) { return LC.at( getL( l ) % LC.count() ); };

  // �c���͍��E��{�����v 2�{�����Ȃ��̂ŁA�c�������Ă�O���[�v�͍ő� 2 �����B
  void SetLineName( int l, QString Name ) { LNames[ getL( l ) ] = Name; };
  void SetLeftName( QString Name ) { LeftName = Name; };
  void SetRightName( QString Name ) { RightName = Name; };
  void SetXName( QString Name ) { XName = Name; };
  void SetXUnitName( QString name ) { XUnitName = name; };
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

public slots:
  void ChooseAG( int i, bool f );

private:
  void paintEvent( QPaintEvent *event );
  void Draw( QPainter *p );
  void UpDateYWindow( int g, SCALET s );
  void ShowAScaleButton( QPainter *p );
  void CheckASPush( void );
};

#endif
