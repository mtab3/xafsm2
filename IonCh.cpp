#include "MainWindow.h"

double MainWindow::calcMuT( int ch, int gas, double keV )
// ch  : selected Ion Chamber
// gas : selected gas
// keV : calculation point
{
  QVector<Element> list;
  double mut0, mut;
  double compAll = 1.0;

#if 0        // �����������פǵ��ʲ����ʤ�
             //  --> ��פ� 1 �ˤʤ�ʤ����ϲð� or ������ɽ��
  for ( int i = 0; i < Gases[gas]->GasComps.count(); i++ ) {
    compAll += Gases[gas]->GasComps[i]->comp;
  }
#endif

  mut = 0;
  for ( int i = 0; i < Gases[gas]->GasComps.count(); i++ ) {
    list = ParseCompString( Gases[gas]->GasComps[i]->GasForm );
    mut0 = 0;
    for ( int j = 0; j < list.count(); j++ ) {
      mut0 += calcAMuT( list[j].aNum, keV ) * list[j].Num;
    }
    mut += mut0 * Gases[gas]->GasComps[i]->comp / compAll;
  }
  // �����ޤǤ� mut �ϻ���κ��祬����ɸ����� 1mol/cm2 ����Ȥ��� mut �ˤʤäƤ�
  // ���ʤ����t = 22.4 * 1000 [cm] �λ��� mut
  // ---> 1mol/cm2 �ǤϤʤ� [comp �ι�� mol]/cm2 ���Ѥ�ä���

  return mut * ICLengths[ ch ]->length / ( 22.4 * 1000 );
}

double MainWindow::calcAMuT( int an, double E )
{
  double L, L3, L4;
  double mut = 0;

  L = u->keV2a( E );
  L3 = L * L * L;
  L4 = L3 * L;

  int edge;
  for ( edge = 0; edge < 4; edge++ ) {
    if ( Vic[ an ].AE[ edge ] < E )
      break;
  }
  if ( edge < 4 )
    mut = ( Vic[an].C[edge]*L3 - Vic[an].D[edge]*L4 ) * A[an].M;
  
  return mut;
}

QVector<Element> MainWindow::ParseCompString( const QString& cmp )
{
  QVector<Element> list0;
  QVector<Element> list1;
  QVector<Element> list2;
  int bc = 0;      // bress count for '(' and ')', these brackets define sub-chemical-form

  // 1 ���ܤΥѡ���
  // '(', ')', ����̾, ���� ��4�Ĥ����ǤΤɤ줫�ǤǤ����ꥹ�Ȥ��ڤ�ʬ����
  for ( int i = 0; i < cmp.length(); i++ ) {
    Element a;
    a.Num = 0;
    if ( cmp[i].isSpace() ) continue;
    if ( cmp[i] == '(' ) { a.special = BOPEN; list0 << a; bc++; }
    if ( cmp[i] == ')' ) { a.special = BCLOSE; list0 << a; bc--; }
    if ( cmp[i].isUpper() ) {  // ����ʸ����1ʸ���ޤ��� +�־�ʸ����1ʸ���ϸ���̾
      a.special = ENAME;
      a.Name = cmp.mid( i, 1 );
      if ( i + 1 < cmp.length() ) {
        if ( cmp[i+1].isLower() ) {
          a.Name += cmp.mid( i+1, 1 );
          i++;
        }
      }
      for ( int j = 0; j < ATOMS; j++ ) {
        if ( a.Name == A[j].AName ) {
          a.Weight = A[j].M;
	  a.aNum = j;
          break;
        }
      }
      list0 << a;
    }
    if (( cmp[i].isNumber() )||( cmp[i] == '.' )) {
      a.special = ENUM;
      int j = i;
      while( ( ( cmp[j].isNumber() )||( cmp[j] == '.' ))&&( j < cmp.length() ) ) j++;
      a.Num = cmp.mid( i, j-i ).toDouble();
      i = j-1;
      list0 << a;
    }
  }
  if ( bc != 0 )
    return list1;   // ���å��ο������ʤ�   list1 is null list
  if ( list0.count() < 1 )
    return list1;   // no elements are there

  // 2���ܤΥѡ��� ����̾���Ĥ���̤�ɬ�������ȥڥ��ˤ��롢Ʊ���˿���ñ�Ȥ����ǤϾʤ�
  for ( int i = 0; i < list0.count(); i++ ) {
    if ( list0[i].special == BOPEN ) { list1 << list0[i]; }
    if (( list0[i].special == BCLOSE )||( list0[i].special == ENAME )) {
      list0[i].Num = 1;
      if ( i + 1 < list0.count() ) {
        if ( list0[i+1].special == ENUM ) {
          list0[i].Num = list0[i+1].Num;
        }
      }
      list1 << list0[i];
    }
  }

  // 3���ܤΥѡ��� '(' ')' ��ۤɤ������ ')' ��³�������� 1�ˤʤ�褦�˵��ʲ�
  for ( int i = list1.count(); i >= 0; i-- ) {
    if ( list1[i].special == BCLOSE ) {
      qDebug() << "in 3rd pass " << i;
      int bc = 1;
      double num = list1[i].Num;
      list1[i].Num = 1.0;
      for ( int j = i - 1; j >= 0; j-- ) {
        if ( list1[j].special == ENAME ) list1[j].Num *= num;
        if ( list1[j].special == BOPEN ) bc--;
        if ( list1[j].special == BCLOSE ) bc++;
        if ( bc == 0 )
          break;
      }
    }
  }

  // 4���ܤΥѡ���
  // ���λ����� '(' ')' ��ñ���̵�뤷���ɤ��ʤäƤ���Τǡ��ꥹ�Ȥ���ΤƤ롣
  // Ʊ���˶��̸��ǤϤҤȤĤˤޤȤ��
  for ( int i = 0; i < list1.count(); i++ ) {
    if ( list1[i].special == ENAME ) {
      if ( list1[i].Num > 0 ) {
        double num = list1[i].Num;
        for ( int j = i+1; j < list1.count(); j++ ) {
          if ( list1[j].special == ENAME ) {
            if ( list1[j].Name == list1[i].Name ) {
              num += list1[j].Num;
              list1[j].Num = 0;
            }
          }
        }
        list1[i].Num = num;
        list2 << list1[i];
      }
    }
  }

  return list2;
}
