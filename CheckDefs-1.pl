#!/usr/local/bin/perl

# MainWindow.h �˰�¸���Ƥ���ե������õ�����ɤ�������ɤΥե�����ȴ�Ϣ���Ƥ��뤫��
# Ĵ�٤뤿��Υġ��롣
# 1) MainWindow class ����ǥ��󥹥ȥ饯�������������Ĥ���¾�����������ȥ����Ȥ���
# 2) ���Υե������¹�
#    ./CheckDefs-1.pl | tcsh
# 3) ³���� -2.pl ��¹�
#    ./CheckDefs-2.pl > MainWindow.res

$GCC = "g++ -c -pipe -O2 -Wall -W -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/local/share/qt4/mkspecs/freebsd-g++ -I. -I/usr/local/include/qt4/QtCore -I/usr/local/include/qt4/QtNetwork -I/usr/local/include/qt4/QtGui -I/usr/local/include/qt4 -I. -I. -I. -I/usr/local/include/qt4 -I/usr/local/include -o a.o ";

$PROC = "|& grep error | sed -e \"s/.*error: \\'//g\" | sed -e \"s/\\' was not.*//g\" | sed -e \"s/.*MainWindow:://g\" | sed -e \"s/\\' member function.*//g\" | sort | uniq";

@Files = ( "AUnit", "Atoms", "ChCoord", "Fio", "Goto", "MCAView", "MainWindow", "Measurement", "Monitor", "MsgType", "MultiUnits", "MwCommon", "MwLog", "MwMeas", "MwSSDSetUp", "MwSetup", "MwView", "PeriodicT", "ReadDef", "SMsg", "Scan", "SelMC", "Stars", "StarsSV", "ViewCtrl", "XView", "XafsM" );

foreach( @Files ) {
    print "$GCC $_.cpp $PROC > $_.key\n";
}

