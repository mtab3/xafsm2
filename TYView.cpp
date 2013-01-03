#include <QtGui>

#include "XafsM.h"
#include "TYView.h"

TYView::TYView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  valid = false;
  cc.SetRealCoord( 0, 0, 1, 1 );
  bgColor = QColor( 255, 255, 255 );
  BLACK = QColor( 0, 0, 0 );
  MCLineC = QColor( 210, 180, 0 );     // mouse cursor line color

  LC << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0,   0, 255 ) << QColor(   0, 255,   0 )
     << QColor( 255, 255,   0 ) << QColor( 255,   0, 255 ) << QColor(   0, 255, 255 )
     << QColor( 127,   0,   0 ) << QColor(   0, 127,   0 ) << QColor(   0,   0, 127 )
     << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0, 255,   0 ) << QColor(   0,   0, 255 )
     << QColor( 255, 255,   0 ) << QColor( 255,   0, 255 ) << QColor(   0, 255, 255 )
     << QColor( 127,   0,   0 ) << QColor(   0, 127,   0 ) << QColor(   0,   0, 127 )
     << QColor(   0,   0,   0 )
     << QColor( 255,   0,   0 ) << QColor(   0, 255,   0 ) << QColor(   0,   0, 255 )
     << QColor( 255, 255,   0 ) << QColor( 255,   0, 255 ) << QColor(   0, 255, 255 )
     << QColor( 127,   0,   0 ) << QColor(   0, 127,   0 ) << QColor(   0,   0, 127 );
  MonScale = 0;
  nx = ny = 0;
  ep = 0;
  datas = 0;
}

void TYView::ClearDataR( void )
{
  for ( int j = 0; j < RingMax; j++ ) {
    for ( int i = 0; i < MaxMon; i++ ) {
      mony[ i ][ j ] = 0;
    }
    mont[ j ] = 0;
  }
  ep = 0;
  datas = 0;
}

void TYView::SetLines( int Lines )
{
  lines = Lines;
}

void TYView::SetMonScale( int ms )
{
  MonScale = ms;
}

// �����O�o�b�t�@�ւ̃f�[�^�ǉ�
void TYView::NewPointR( int tt, double yy0, double yy1, double yy2 )
{
  mont[ ep ] = tt;
  mony[0][ ep ] = yy0;
  mony[1][ ep ] = yy1;
  mony[2][ ep ] = yy2;
  ep++;
  if ( ep >= RingMax )
    ep = 0;
  if ( datas < RingMax )
    datas++;
}

void TYView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

void TYView::Draw( QPainter *p )
{
  if ( valid != true ) return;

  QString buf, buf2;
  double RM, LM, TM, BM, HDiv, VDiv;
  QPen pen0, pen1;
  QFont F1;
  QRectF rec;
  int ms = MScales[ MonScale ].div * 1000;   // �P�O���b�h�̎��� �b x 1000 = �~���b

  double HDNum = 6;    // ���������̃O���b�h��
  double VDNum = 10;   // ���������̃O���b�h��

  cc.SetRealCoord( -ms * HDNum, 0, 0, 1 );   // �����W�̎w��(�Ƃ肠�������������� 1)

  p->fillRect( 0, 0, width(), height(), bgColor ); // �w�i�S�̂̓h��Ԃ�
  pen0.setWidth( 1 );
  pen0.setColor( QColor( 0, 0, 0 ) );
  p->setPen( pen0 );
  p->setFont( F1 );

  RM = width() * 0.03;    // �`��̈�̒��ł̃O���t�̉E�}�[�W��
  LM = width() * 0.15;    // �`��̈�̒��ł̃O���t�̍��}�[�W��
  TM = height() * 0.05;   // �`��̈�̒��ł̃O���t�̏�(top)�}�[�W��
  BM = height() * 0.10;   // �`��̈�̒��ł̃O���t�̉�(bottom)�}�[�W��
  HDiv = ( width() - RM - LM ) / HDNum;     // �O���b�h��
  VDiv = ( height() - TM - BM ) / VDNum;    // �O���b�h����

  cc.SetScreenCoord( LM, TM, width()-RM, height()-BM );
  // screen ���W�̎w��B����A�E���̍��W�w��
  // (�����ŏ㉺�������������W���w�肷�邱�Ƃ� real ���W�ŕ��ʂɍ����� 0,0 �̍��W�ɂȂ�)

  p->drawRect( LM, TM, width()-RM-LM, height()-BM-TM );  // �O���t�̘g��

  for ( double xx = cc.Rminx(); xx <= cc.Rmaxx(); xx += ms ) {   // ����������
    p->drawLine( cc.r2sx( xx ), TM, cc.r2sx( xx ), height()-BM );  // �c�r��
                                                                   // �����̃���������
    rec = QRect( cc.r2sx( xx )-HDiv/2, height()-BM*0.95, HDiv, BM*0.4 );
    cc.DrawText( p, rec, F1, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE, 
		 QString( tr( "%1" ) )
		 .arg( (int)((cc.Rmaxx()-xx)/ms) * MScales[ MonScale ].dispDiv ) );
  }
  rec = QRect( width()-VDiv, height()-BM*0.5, RM*0.9, BM*0.4 );  // �����̃��x��
  cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE,
	       MScales[ MonScale ].unit );

  pen1.setWidth( 1 );
  pen1.setColor( BLACK );
  p->setPen( pen1 );
  for ( double yy = 0; yy < 1; yy += 0.1 ) {
    p->drawLine( LM, cc.r2sy( yy ), width()-RM, cc.r2sy( yy ) );   // ���̌r��
  }

  double sy, ey, dy;
  UpDateYWindowRing();   // �ő�l�ŏ��l(5%�}�[�W��)��T�� (Rminy, Rmaxy �ɕԂ�)

  for ( int j = 0; j < lines; j++ ) {
    //      sy = Rwminy[j];
    //      dy = ( Rwmaxy[j] - Rwminy[j] ) / 10.;
    cc.SetRealY( Rwminy[j], Rwmaxy[j] );
    cc.getSEDy( &sy, &ey, &dy, 5 );
    pen1.setColor( LC[ j ] );
    p->setPen( pen1 );

    /*  �����܂Ŋm�F����  */

    for ( double yy = sy; yy <= cc.Rmaxy(); yy += dy ) {
      rec = QRectF( LM - LM * 0.32 * ( j + 1 ), cc.r2sy( yy )-VDiv*0.45,
		    LM * 0.3, VDiv * 0.9 ); // ����������
      buf.sprintf( "%6.4g", yy );
      cc.DrawText( p, rec, F1, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, buf );
    }
    rec = QRectF( LM + HDiv * 0.1 + HDiv * 2 * j, TM * 0.05, 
		  HDiv * 2, TM * 0.9 );  // ���̃��x��
    
    int pp1, pp2;
    int t0 = mont[ ( ep == 0 ) ? RingMax - 1 : ep - 1 ];
    pen1.setWidth( 2 );
    pen1.setColor( LC[ j ] );
    p->setPen( pen1 );
    int nowt = cc.s2rx( nx ) + t0;  // �}�E�X�|�C���^�̈ʒu�ɑ������鎞��
    int nowtp = 0;
    
    for ( int i = 0; i < datas - 1; i++ ) { // �f�[�^�v���b�g
      pp1 = ep - 1 - i;
      pp2 = ep - 1 - ( i + 1 );
      if ( pp1 < 0 ) pp1 += RingMax;
      if ( pp2 < 0 ) pp2 += RingMax;
      if ( ( mont[ pp2 ] - t0 ) < ( - ms * 6 ) ) {
	break;
      }
      p->drawLine( cc.r2sx( mont[pp1] - t0 ), cc.r2sy( mony[j][pp1] ),
		   cc.r2sx( mont[pp2] - t0 ), cc.r2sy( mony[j][pp2] ) );

      // �}�E�X�|�C���^�̎w�������ɏo���邾���߂������_
      if (( mont[pp1] >= nowt )&&( mont[pp2] < nowt ))
	nowtp = pp1;

    }
    cc.DrawText( p, rec, F1, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 LNames[j] + " : " + QString::number(mony[j][nowtp]) );
  }

  // �}�E�X�|�C���^�̈ʒu�ɏc��������
  if ( ( nx > LM ) && ( nx < width()-RM ) ) {
    p->setPen( MCLineC );
    p->drawLine( nx, TM, nx, height()-BM );
  }
}

void TYView::UpDateYWindowRing( void )
{
  int dx = MScales[ MonScale ].div;
  int p, t0;

  t0 = mont[ ( ep == 0 ) ? RingMax -1 : ep - 1 ];

  for ( int j = 0; j < lines; j++ ) {
    double nmaxy = -1e300;
    double nminy = 1e300;
    for ( int i = 0; i < datas; i++ ) {
      p = ep - 1 - i;
      if ( p < 0 ) p += RingMax;
      if ( (  mont[ p ] - t0 ) > ( - dx * 6 * 1000 ) ) {
	if ( mony[j][ p ] < nminy )
	  nminy = mony[j][ p ];
	if ( mony[j][ p ] > nmaxy ) 
	  nmaxy = mony[j][ p ];
      }
    }
    double dy = nmaxy - nminy;
    if ( dy == 0 )
      dy = 1.;
    Rwminy[j] = nminy - dy * 0.05;
    Rwmaxy[j] = nmaxy + dy * 0.05;
  }
}

/*******************************************************************/

void TYView::mouseMoveEvent( QMouseEvent *e )
{
  nx = e->x();
  ny = e->y();

  update();
}

void TYView::mousePressEvent( QMouseEvent * )
{
}

void TYView::mouseReleaseEvent( QMouseEvent * )
{
}

void TYView::mouseDoubleClickEvent( QMouseEvent * )
{
}
