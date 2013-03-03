#!/usr/local/bin/perl

# MainWindow.h に依存しているファイルを探し、どの宣言がどのファイルと関連しているかを
# 調べるためのツール。
# 1) MainWindow class 宣言でコンストラクタの宣言だけを残して他を全部コメントアウトする
# 2) このファイルを実行
#    ./CheckDefs-1.pl | tcsh
# 3) 続いて -2.pl を実行
#    ./CheckDefs-2.pl > MainWindow.res

$GCC = "g++ -c -pipe -O2 -Wall -W -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/local/share/qt4/mkspecs/freebsd-g++ -I. -I/usr/local/include/qt4/QtCore -I/usr/local/include/qt4/QtNetwork -I/usr/local/include/qt4/QtGui -I/usr/local/include/qt4 -I. -I. -I. -I/usr/local/include/qt4 -I/usr/local/include -o a.o ";

$PROC = "|& grep error | sed -e \"s/.*error: \\'//g\" | sed -e \"s/\\' was not.*//g\" | sed -e \"s/.*MainWindow:://g\" | sed -e \"s/\\' member function.*//g\" | sort | uniq";

@Files = ( "AUnit", "Atoms", "ChCoord", "Fio", "Goto", "MCAView", "MainWindow", "Measurement", "Monitor", "MsgType", "MultiUnits", "MwCommon", "MwLog", "MwMeas", "MwSSDSetUp", "MwSetup", "MwView", "PeriodicT", "ReadDef", "SMsg", "Scan", "SelMC", "Stars", "StarsSV", "ViewCtrl", "XView", "XafsM" );

foreach( @Files ) {
    print "$GCC $_.cpp $PROC > $_.key\n";
}

