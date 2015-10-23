#ifndef GITLOG_H
#define GITLOG_H


#define BRANCH_NAME ( "stable" )
#define GITLOGS ""\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri Oct 23 15:25:06 2015 +0900\n"\
                "    additional documentation in XAFSM0.def and XAFSM.def.BL5S1\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri Oct 23 15:22:03 2015 +0900\n"\
                "    merged with stable-more-points-for-quick branch\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri Oct 23 15:13:56 2015 +0900\n"\
                "    QXAFS の測定点数を 9990 より増やせるようにする\n"\
"    とりあえず完成、光がない状態でテスト済み(このテストは光がなくても大丈夫)\n"\
"    1) XAFSM.def ファイルの QXAFS_MAX_POINTS で指定した点数が最大点数\n"\
"    2) 但し内部的にはハードコードされたリミットがあり 20,000 点が現在のマックス\n"\
"    3) QXAFS の測定パラメータ(ブロックパラメータ)を設定する際、\n"\
"    　従来は表示されている点数と実際に計測される点数が違っていた。\n"\
"    　このため、例えば上限の 20,000点を超えないように 19,000点として測定しても\n"\
"    　実際には 22,000点を測定しようとして、エラーが起こる可能性があった。\n"\
"    　この点を修正し、かならず表示された点数の測定が行われるようにした。\n"\
"    4) これに伴い、測定前に、測定時間や測定点数、測定の刻み幅等をチェックし\n"\
"    　適宜修正していたルーチンを整理しなおした。\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri Oct 23 12:24:25 2015 +0900\n"\
                "    changed UI default\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu Oct 22 17:59:18 2015 +0900\n"\
                "    LION\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu Oct 22 16:25:53 2015 +0900\n"\
                "    adding some documentation\n"\
		"\n"\
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
                ""
#endif
