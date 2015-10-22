#ifndef GITLOG_H
#define GITLOG_H


#define BRANCH_NAME ( "stable" )
#define GITLOGS ""\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Wed Oct 21 19:48:30 2015 +0900\n"\
                "    GetPM16CParamsForQXAFS was checked and modified as CheckQXafsParams has alerady been.  As the result, the displayed parameters is getting much robust.   It means the displayed parameters are actually used for the QXAFS measurements without changing.\n"\
"    This version has been tested with simmotor but not in real environment.\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Wed Oct 21 17:40:26 2015 +0900\n"\
                "    GetPM16CParamsForQXAFS is updated as CheckQXafsParams in QXafs.cpp\n"\
"    !!! But not tested at all !!!\n"\
		"\n"\
                "Author      : M. Tabuchi <tabuchi@dimos3.nusr.nagoya-u.ac.jp>\n"\
                "Date        : Mon Oct 19 17:26:31 2015 +0900\n"\
                "    nothing\n"\
		"\n"\
                "Author      : M. Tabuchi <tabuchi@dimos3.nusr.nagoya-u.ac.jp>\n"\
                "Date        : Mon Oct 19 17:26:18 2015 +0900\n"\
                "    nothing\n"\
		"\n"\
                "Author      : M. Tabuchi <tabuchi@dimos3.nusr.nagoya-u.ac.jp>\n"\
                "Date        : Mon Oct 19 17:25:59 2015 +0900\n"\
                "    Merge remote-tracking branch 'origin/stable-more-points-for-quick' into stable-more-points-for-quick\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu Oct 15 19:25:06 2015 +0900\n"\
                "    QXAFS の際の「測定点数」が実際の測定時に変化してしまうのが気になり、\n"\
"    変化しないように(あるいは最悪でも予定していた点数よりも少ない方向にズレるように)したいと思い作業を行った。\n"\
"    \n"\
"    現時点では、入力時の入力内容チェックを行う\n"\
"    void MainWindow::CheckQXafsParams( void ) in QXafs.cpp\n"\
"    の変更は済んだ。\n"\
"    この時点(入力時点)では、当初の目標を達成できている。\n"\
"    \n"\
"    しかし、QXafs 直前に再度パラメータの計算をし直している部分が有り\n"\
"    (void MainWindow::GetPM16CParamsForQXAFS( void ) in QXafs.cpp)\n"\
"    これもチェックしないと完全ではない。\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu Oct 15 19:25:06 2015 +0900\n"\
                "    QXAFS の際の「測定点数」が実際の測定時に変化してしまうのが気になり、\n"\
"    変化しないように(あるいは最悪でも予定していた点数よりも少ない方向にズレるように)したいと思い作業を行った。\n"\
"    \n"\
"    現時点では、入力時の入力内容チェックを行う\n"\
"    void MainWindow::CheckQXafsParams( void ) in QXafs.cpp\n"\
"    の変更は済んだ。\n"\
"    この時点(入力時点)では、当初の目標を達成できている。\n"\
"    \n"\
"    しかし、QXafs 直前に再度パラメータの計算をし直している部分が有り\n"\
"    (void MainWindow::CheckQXafsParams( void ) in QXafs.cpp)\n"\
"    これもチェックしないと完全ではない。\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Wed Oct 14 19:00:56 2015 +0900\n"\
                "    working\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Tue Oct 13 17:17:18 2015 +0900\n"\
                "    merged with current stable\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri Oct 9 21:23:21 2015 +0900\n"\
                "    modified XAFSM.def.BL5S1\n"\
		"\n"\
                ""
#endif
