#ifndef MCAPEAK_H
#define MCAPEAK_H

struct MCAPeak {
  //  int sp, ep, cp;
  //  double sE, eE;
  //  int peakH0;
  double A;   // peakH �� A
  double BinE;    // ���ͥ륮��ñ�̤Υԡ�������
  double BinP;    // pixel ñ�̤Υԡ�������
  double CinE;    // C �� A exp C * (x-B)*(x-B) �� C
  double CinP;    // pixel ñ�̤�ľ���� C
  double WinE;    // ���ͥ륮��ñ�̤�Ⱦ������
  double WinP;    // pixel ñ�̤�Ⱦ������
};

#endif
