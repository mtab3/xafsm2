#ifndef MCAPEAK_H
#define MCAPEAK_H

struct MCAPeak {
  double start;      // start, end, center ��ñ�̤� MCA pixel �����ɡ��ե��åƥ��󥰤���
  double end;        // �����ʲ��η夬�Ф��ǽ��������Τǡ��¿�
  double center;
  double centerE;
  int peakH0, peakH;
};

#endif
