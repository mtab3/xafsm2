#include "XafsM.h"
#include "MCAView.h"

// �����ˤ� 3�Ĥ�ñ�̤����롣
// MCA pixel, eV (��Energy), ���� pixel
// MCAView ��β����ϴ���Ū�� keV
// MCA pixel -> keV : k2p->p2E( MCA pixel ) = keV
// keV -> MCA pixel : k2p->p2E( keV ) = MCA pixel
//    k2p �� keV-to-Pixel �ΤĤ�ꡣ�͡��ߥ󥰤��ޤ�����...
// ���� pixel �Ȥδ����ϰ��� keV ���̤ä� cc.r2sx, cc.s2rx,...

#define NEAR ( 10 )     // ROI �Υ��å��˶ᤤ��Ƚ�Ǥ����Υ(���� pixel)
#define NEAR2 ( 10 )    // �����֤���������˶ᤤ��Ƚ�Ǥ����Υ

MCAView::MCAView( QWidget *parent ) : QFrame( parent )
{
  setupUi( this );

  Parent = parent;
  setToolTip( "" );

  k2p = NULL;
  MCA = NULL;
  MCALen = 0;
  MCACh = -1;
  realTime = 0;
  liveTime = 0;
  MaxE = 20.;
  MinE = 0.;
  mMode = M_NO;

  valid = false;
  dispLog = false;

  Black = QColor( 0, 0, 0 );
  White = QColor( 255, 255, 255 );

  MCursorC    = QColor( 255,   0,   0 );  // �ޥ����������뿧
  MCursorC2   = QColor(   0, 200, 245 );  // �����֤��������뿧
  MCursorC3   = QColor( 250, 180,   0 );  // �ޥ������ᤤ���ο����֤��������뿧
  ROIRangeC   = QColor( 170, 210,   0 );  // ROI ��Υ��ڥ��ȥ뿧
  ExROIRangeC = QColor(   0, 100, 230 );  // ROI ���Υ��ڥ��ȥ뿧
  ROIEdgeC    = QColor( 250, 180,   0 );  // ROI ��ü��ɽ��
  GridC       = QColor( 100, 100, 190 );  // ����åɤο�
  AListC      = QColor( 200,   0, 100 );  // ����̾�ꥹ�� 

  rROIsx = 0;
  rROIex = 20;

  // data-disp ��ɽ�����������˥ޥ�������
  connect( this, SIGNAL( CurrentValues( int, int ) ),
	   Parent, SLOT( showCurrentValues( int, int ) ) );
  connect( this, SIGNAL( newROI( int, int ) ),
	   Parent, SLOT( setNewROI( int, int ) ) );
}

MCAView::~MCAView( void )
{
  if ( MCA != NULL )
    delete MCA;
  disconnect( this, SIGNAL( CurrentValues( int, int ) ),
	   Parent, SLOT( showCurrentValues( int, int ) ) );
  disconnect( this, SIGNAL( newROI( int, int ) ),
	   Parent, SLOT( setNewROI( int, int ) ) );
}

int *MCAView::setMCAdataPointer( int len )
{
  MCA = new int[ MCALen = len ];
  return MCA;
}

void MCAView::paintEvent( QPaintEvent * )
{
  QStylePainter painter( this );

  Draw( &painter );
}

double LOGS[ 9 ] = {
  0.0,         // log10( 1 )
  0.30103,     // log10( 2 )
  0.47712,     // log10( 3 )
  0.60206,     // log10( 4 )
  0.69897,     // log10( 5 )
  0.77815,     // log10( 6 )
  0.84510,     // log10( 7 )
  0.90309,     // log10( 8 )
  0.95424,     // log10( 9 )
};

void MCAView::Draw( QPainter *p )
{
  if ( valid != true ) 
    return;

  QFont f;
  QRectF rec;

  k2p->setMCALen( MCALen );

  int w = width();
  int h = height();

  int LM = w * 0.25;
  if ( LM > 180 ) LM = 180;
  int RM = w * 0.05;
  if ( RM > 40 ) RM = 40;
  int HW = w - LM - RM;
  int TM = h * 0.05;
  if ( TM > 40 ) TM = 40;
  int BM = h * 0.1;
  if ( BM > 80 ) BM = 80;
  int VW = h - TM - BM;
  double dLM = LM / 10;
  double dVW = VW / 20;     // 1�Ԥι⤵(ʸ���ι⤵)
  double dVW2 = dVW * 1.2;  // �Դ�

  cc.SetScreenCoord( LM, TM, LM+HW, TM+VW );
  p->fillRect( 0, 0, w, h, White );

  double max;
  double max0 = 0;    // y ��������ɽ�������������Τ���ɽ���ϰϤǤκ����ͤ�õ��
  for ( int i = 0; i < MCALen; i++ ) {
    double E = k2p->p2E( MCACh, i );
    if (( E > MinE )&&( E < MaxE )) {
      if ( MCA[i] > max0 ) {
	max0 = MCA[i];
      }
    }
  }
  if ( dispLog ) {   // log ����������ä�������ͤ� log ��Ȥäƽļ��Υ�������ˤ���
    if ( max0 > 0 )
      max = log10( max0 );
    else 
      max = 1;
  } else {
    max = max0;
  }
  //  cc.SetRealCoord( 0, 0, MCALen-1, max );
  cc.SetRealCoord( MinE, 0, MaxE, max );          // ���䲣���� MCA pixel �ǤϤʤ���
                                               // ���ͥ륮-[keV]
  // ������Ĵ������Ƥ���� MCP pixel = ���ͥ륮��[eV]/10 �ˤʤäƤ���Ϥ���

  double rmx = cc.s2rx( m.x() );
  double wrROIsx = rROIsx;  // wrROI.. working-real-ROI.., rROI.. real-ROI..
  double wrROIex;
  if ( m.inPress() ) {      // �ޥ����ܥ���򲡤��Ƥ���ϡ��ֽ����פϥޥ����θ��߰���
    wrROIex = rmx;
  } else {
    wrROIex = rROIex;       // �����Ǥʤ���кǸ�˥ܥ����Υ��������
  }
  double tmp;
  if ( wrROIsx > wrROIex ) { // �������������羮�ط����դˤʤäƤ��������ؤ��Ƥ���
    tmp = wrROIsx;
    wrROIsx = wrROIex;
    wrROIex = tmp;
  }
  if ( m.inPress() && ( mMode == M_ROI ) ) {
    emit newROI( k2p->E2p( MCACh, wrROIsx ), k2p->E2p( MCACh, wrROIex ) );
  }

  int sum = 0;
  for ( int i = 0; i < MCALen; i++ ) {       // ROI ���ϰϤ��ѻ��� MCA ���ڥ��ȥ������
    double E = k2p->p2E( MCACh, i );         // MCA pixel ���� ���ͥ륮���ؤδ���
    if (( E >= wrROIsx )&&( E <= wrROIex )) {
      p->setPen( ROIRangeC );
      sum += MCA[i];
    } else {
      p->setPen( ExROIRangeC );
    }
    if ( dispLog ) {
      if ( MCA[i] > 0 )
	p->drawLine( cc.r2sx( E ), cc.r2sy( log10( MCA[i] ) ),
		     cc.r2sx( E ), cc.r2sy( 0 ) );
    } else {
      p->drawLine( cc.r2sx( E ), cc.r2sy( MCA[i] ), cc.r2sx( E ), cc.r2sy( 0 ) );
    }
  }
  p->setPen( Black );                      // ����ճ��Ȥλͳ�����
  p->drawRect( LM, TM, HW, VW );
  p->setPen( GridC );                      // ����դη�������
  for ( double E = 2; E < MaxE; E += 2.0 ) { // 2keV ��ߤǼ��������������
    p->drawLine( cc.r2sx( E ), TM, cc.r2sx( E ), TM + VW );
    rec.setRect( cc.r2sx( E ) - dLM * 2, TM + VW + dVW * 0.2,
		 dLM * 4, dVW );
    cc.DrawText( p, rec, f, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE, 
		 QString::number( E ) );
  }
  rec.setRect( cc.r2sx( MaxE ) - dLM * 2, TM + VW + dVW * 0.2,
	       dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignHCenter | Qt::AlignVCenter, SCALESIZE, "[keV]" );
  for ( double E = 1; E < 20; E += 2.0 ) { // ���η�֤�����
    p->drawLine( cc.r2sx( E ), TM, cc. r2sx( E ), TM + dVW / 2 );
    p->drawLine( cc.r2sx( E ), TM + VW, cc. r2sx( E ), TM + VW - dVW / 2 );
  }

  // �ļ��ԡ�����
  rec.setRect( LM - dLM*8, TM - dVW / 2, dLM * 7.5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( max0 ) );
  if ( !dispLog ) {
    for ( double y = max / 10.0; y < max; y += max / 10. ) {
      p->drawLine( LM, cc.r2sy( y ), LM + HW, cc.r2sy( y ) );
    }
  } else {
    double ten = 0;
    int sub = 0;
    double y;
    while ( 1 == 1 ) {
      if ( ( y = ten + LOGS[ sub ] ) > max )
	break;
      p->drawLine( LM, cc.r2sy( y ), LM + HW, cc.r2sy( y ) );
      sub++;
      if ( sub > 8 ) {
	sub = 0;
	ten += 1;
      }
    }
  }

  if ( ( m.x() > LM )&&( m.x() < LM+HW ) ) {   // �ޥ�������������֤˽�������
    p->setPen( MCursorC );
    p->drawLine( m.x(), TM, m.x(), TM+VW );
  }

  // ���ͥ륮�����������ޥ�������������֤��б����� MCA pixel
  int curp = k2p->E2p( MCACh, cc.s2rxLimit( m.x() ) );
  emit CurrentValues( MCA[ curp ], sum );

  if ( nearf ) {              // �ޥ����������뤬 ROI ��ξü�˶ᤤ��ǧ�����Ƥ���Ȥ���
                              // �ᤤ�ȻפäƤ������ζ���Ĵ? ɽ��
    p->setPen( ROIEdgeC );
    p->drawLine( nearX, TM, nearX, TM+VW );
  }

  for ( int i = 0; i < cPoints.count(); i++ ) {  // �����֤�����������֤˽���
    int cpx = cc.r2sx( cPoints[i] );
    if ( abs( cpx - m.x() ) < NEAR2 )
      p->setPen( MCursorC3 );
    else
      p->setPen( MCursorC2 );
    p->drawLine( cpx, TM, cpx, TM+VW );
  }

  if ( showElements ) {
    if ( showElementsAlways ) {
      if ( ! ( m.getMod() & Qt::ShiftModifier ) ) {
	QVector<Fluo> inRange = fdbase->inRange( MinE, MaxE );
	p->setPen( AListC );
	for ( int i = 0; i < inRange.count(); i++ ) {
	  double v = inRange[i].val;
	  p->drawLine( cc.r2sx( v ), TM+VW, cc.r2sx( v ), TM+VW-dVW*3 );
	  QString show = QString( "%1 %2" )
	    .arg( inRange[i].fullName ).arg( inRange[i].val );
	  rec.setRect( cc.r2sx( v ) - dLM * 10, TM+VW-dVW*(4+0.5), dLM * 9.5, dVW );
	  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, show );
	}
      }
    } else {
      if ( ! ( m.getMod() & Qt::ShiftModifier ) ) {
	// �ޥ������������ն�θ��ǥꥹ��
	QVector<Fluo> nears = fdbase->nears( rmx );
	p->setPen( AListC );
	bool isUpper = ( rmx > ( MaxE + MinE ) / 2 );
	for ( int i = 0; i < nears.count(); i++ ) {
	  int pp;
	  double v = nears[i].val;
	  pp = ( isUpper ) ? i : ( nears.count() - i - 1 );
	  p->drawLine( cc.r2sx( v ), TM+VW, cc.r2sx( v ), TM+VW-dVW*(3+pp) );
	  p->drawLine( cc.r2sx( v ), TM+VW-dVW*(3+pp), 
		       cc.r2sx( v ) + ( ( isUpper ) ? -dLM * 0.5 : dLM * 0.5 ),
		       TM+VW-dVW*(4+pp) );
	  QString show = QString( "%1 %2" )
	    .arg( nears[i].fullName ).arg( nears[i].val );
	  if ( isUpper ) {
	    rec.setRect( cc.r2sx( v ) - dLM * 10, TM+VW-dVW*(4+pp+0.5), dLM * 9.5, dVW );
	    cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, show );
	  } else {
	    rec.setRect( cc.r2sx( v ) + dLM * 0.5, TM+VW-dVW*(4+pp+0.5), dLM * 9.5, dVW );
	    cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, show );
	  }
	}
      }
    }
  }

  p->setPen( Black );

  int LINE = 1;
  if ( MCACh >= 0 ) {   // MCA ch �ֹ�
    rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
    cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
		 tr( "MCA Ch. : " ) );
    rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
    cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
		 QString::number( MCACh ) );
    LINE++;
  }

  // ��������(�����ȥ�)
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 6, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Cursor" ) );
  LINE++;

  // �����������( �¥��ͥ륮������ )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " Pos. [keV] : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( cc.s2rxLimit( m.x() ), 'f', 3 ) );
  LINE++;

  // �����������( MCA pixel )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " Pos. [ch] : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( k2p->E2p( MCACh, cc.s2rxLimit( m.x() ) ) ) );
  LINE++;

  // ����������֤� MCA ��
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " Val. : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( MCA[ curp ] ) );
  LINE++;

  // ROI �����ȥ�
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "ROI " ) );
  LINE++;

  // ROI �Υ������Ȱ��� ( MCA pixel )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " Start [keV] : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( wrROIsx, 'g', 3 ) );
  LINE++;

  // ROI �ν�λ���� ( MCA pixel )
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " End [keV] : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( wrROIex , 'g', 3 ) );
  LINE++;

  // ROI �����ʬ��
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( " Count : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( sum ) );
  LINE++;

  // ROI �����ʬ�ͤ� LiveTime �ǳ�ä� cps
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "   CPS : " ) );
  rec.setRect( dLM*5, TM + dVW2 * LINE, dLM * 4, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( sum / (( realTime == 0 )? 1 : realTime ), 'f', 2 ) );
  LINE++;

  // Real Time
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Real Time : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( realTime, 'f', 2 ) );
  LINE++;

  // Live Time
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Live Time : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( liveTime, 'f', 2 ) );
  LINE++;

  // �ǥåɥ�����
  double dt = ( realTime != 0 ) ? ( realTime - liveTime )/realTime * 100: 0;
  //���� ��   ->  ��   ->  ��   -> ����� -> �֡����ˤ���Ȥ����顩
  //    0 0 1 -> 0 1 0 -> 1 1 0 ->    ->      1 0 0
  //     0 %      10%      20%        ->       100%
  int r, g, b;
  r = g = b = 0;
  if ( dt < 10 ) { r = 0; g = (int)( dt / 10 * 255 ); b = 255 - g; };
  if (( 10 <= dt )&&( dt < 20 )) { r = (int)( ( dt - 10 ) / 10 * 255 ); g = 255; b = 0; };
  if ( dt >= 20 ) { r = 255; g = 255 - (int)( ( dt - 20 ) / 80 * 255 ); b = 0; };
  if ( r < 0 ) r = 0;
  if ( g < 0 ) g = 0;
  if ( b < 0 ) b = 0;
  QColor DTC = QColor( r, g, b );
  rec.setRect( dLM, TM + dVW2 * LINE, dLM * 5, dVW );
  cc.DrawText( p, rec, f, Qt::AlignLeft | Qt::AlignVCenter, SCALESIZE, 
	       tr( "Dead Time : " ) );
  rec.setRect( dLM*6, TM + dVW2 * LINE, dLM * 3, dVW );
  p->setPen( DTC );
  cc.DrawText( p, rec, f, Qt::AlignRight | Qt::AlignVCenter, SCALESIZE, 
	       QString::number( dt, 'f', 2 ) );
  LINE++;
}

void MCAView::mouseMoveEvent( QMouseEvent *e )
{
  m.Moved( e );
  nearf = false;
  if ( !m.inPress() ) {
    int dsx = fabs( e->x() - m.sx() );
    int dex = fabs( e->x() - m.ex() );

    if ( ( dsx < NEAR ) || ( dex < NEAR ) ) {
      if ( dsx < dex ) {
	nearX = m.sx();
      } else {
	nearX = m.ex();
      }
      nearf = true;
    }
  }
  update();
}

void MCAView::mousePressEvent( QMouseEvent *e )
{
  if ( e->button() == Qt::LeftButton ) {   // ���ܥ���� ROI ����⡼��
    mMode = M_ROI;
    int dsx = fabs( e->x() - m.sx() );
    int dex = fabs( e->x() - m.ex() );
    int setX;
    if ( ( dsx < NEAR ) || ( dex < NEAR ) ) {   // �᤯�˴��ߤ� ROI ��ü�������뤫
      if ( dsx < dex ) {                        // ����� ROI �ѹ�
	setX = m.ex();
      } else {
	setX = m.sx();
      }
    } else {                                    // �ʤ���� ROI ����
      setX = e->x();
    }
    m.Pressed( e );
    m.setSx( setX );
    rROIsx = cc.s2rx( m.sx() );
    update();
  }
  if ( e->button() == Qt::RightButton ) {  // ���ܥ���� ������������/���
    double checkp = cc.s2rx( e->x() );
    bool del = false;
    int nearp = -1;
    double nearv = 1e300;
    for ( int i = 0; i < cPoints.count(); i++ ) { // ���ߤΥ������뤬�᤯�ˤ��뤫
      if ( fabs( cPoints[i] - checkp ) < nearv ) {  // ���ֶᤤ���ߥ��������õ��
	nearp = i;
	nearv = fabs( cPoints[i] - checkp );
      }
    }
    if ( nearp >= 0 ) {
      if ( fabs( cc.r2sx( cPoints[ nearp ] ) - e->x() ) < NEAR2 ) {
	                                          // �Ƕ��ܤ�NEAR�ʲ��ξ����ä���
	cPoints.remove( nearp );                  // ����������
	del = true;
      }
    }
    if ( ! del ) {    // ��������������뤬�ʤ���Х�����������
      cPoints << checkp;
    }
  }
}

void MCAView::mouseReleaseEvent( QMouseEvent *e )
{
  if ( mMode == M_ROI ) {
    mMode = M_NO;
    m.Released( e );
    rROIex = cc.s2rx( m.ex() );
    update();
  }
}

void MCAView::mouseDoubleClickEvent( QMouseEvent * ) {}

void MCAView::setROI( int s, int e )   // MCA pixel
{
  rROIsx = k2p->p2E( MCACh, s );
  rROIex = k2p->p2E( MCACh, e );
}

void MCAView::wheelEvent( QWheelEvent *e )
{
  double step = ( e->delta() / 8. ) / 15.;     // deg := e->delta / 8.
  double rx = cc.s2rx( e->x() );
  double drx = MaxE - MinE;
  if ( step > 0 ) {
    drx *= 0.9;
  } else {
    drx /= 0.9;
  }
  MinE = rx - ( e->x() - cc.Sminx() ) / ( cc.Smaxx() - cc.Sminx() ) * drx;
  MaxE = MinE + drx;

  if ( MinE < 0 ) MinE = 0;
  if ( MaxE > 20 ) MaxE = 20;

  update();
}
