#include "MainWindow.h"

double MainWindow::calcMuT( int ch, int gas, double keV )
// ch  : selected Ion Chamber
// gas : selected gas
// keV : calculation point
{
  QVector<Element> list;
  double mut0, mut;
  double compAll = 1.0;

  mut = 0;
  for ( int i = 0; i < Gases[gas]->GasComps.count(); i++ ) {
    list = ParseCompString( Gases[gas]->GasComps[i]->GasForm );
    mut0 = 0;
    for ( int j = 0; j < list.count(); j++ ) {
      mut0 += calcAMuT( list[j].aNum, keV ) * list[j].Num;
    }
    mut += mut0 * Gases[gas]->GasComps[i]->comp / compAll;
  }
  // ここまでで mut は指定の混合ガスが標準状態 1mol/cm2 あるときの mut になってる
  // すなわち、t = 22.4 * 1000 [cm] の時の mut
  // ---> 1mol/cm2 ではなく [comp の合計 mol]/cm2 に変わった。

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

  // 1 回目のパース
  // '(', ')', 元素名, 数字 の4つの要素のどれかでできたリストに切り分ける
  for ( int i = 0; i < cmp.length(); i++ ) {
    Element a;
    a.Num = 0;
    if ( cmp[i].isSpace() ) continue;
    if ( cmp[i] == '(' ) { a.special = BOPEN; list0 << a; bc++; }
    if ( cmp[i] == ')' ) { a.special = BCLOSE; list0 << a; bc--; }
    if ( cmp[i].isUpper() ) {  // 「大文字」1文字または +「小文字」1文字は元素名
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
    return list1;   // カッコの数が合わない   list1 is null list
  if ( list0.count() < 1 )
    return list1;   // no elements are there

  // 2回目のパース 元素名と閉じ括弧は必ず数字とペアにする、同時に数字単独の要素は省く
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

  // 3回目のパース '(' ')' をほどくために ')' に続く数字が 1になるように規格化
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

  // 4回目のパース
  // この時点で '(' ')' は単純に無視して良くなっているので、リストから捨てる。
  // 同時に共通元素はひとつにまとめる
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
