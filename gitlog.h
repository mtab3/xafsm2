#ifndef GITLOG_H
#define GITLOG_H


#define BRANCH_NAME ( "stable" )
#define GITLOGS ""\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri Nov 13 14:58:27 2015 +0900\n"\
                "    Merge remote-tracking branch 'origin/stable' into stable\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri Nov 13 14:58:03 2015 +0900\n"\
                "    LION\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri Nov 13 14:55:12 2015 +0900\n"\
                "    ¥À¡¼¥¯¬ÄêÃæ¤Ï XAFS ¬Äê¤򳫻ϤǤ­¤ʤ¤¤褦¤ˤ¹¤롣\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu Nov 12 22:14:33 2015 +0900\n"\
                "    remove qDebugs from S2DView\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Wed Nov 11 18:03:38 2015 +0900\n"\
                "    merged with RecROI\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Wed Nov 11 18:02:46 2015 +0900\n"\
                "    the change of the ROI records in Log file and displaied in log-tag as well\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Wed Nov 11 17:19:00 2015 +0900\n"\
                "    record the change of the ROI to the log file\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Wed Nov 4 11:24:21 2015 +0900\n"\
                "    new ?\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri Oct 23 16:49:28 2015 +0900\n"\
                "    廣友さんが発見したパタンの 2次元スキャンの不具合解消\n"\
"    1) 2次元スキャンを行うと、スクリーン上に\n"\
"    　　「最後に測定されたデータの値」「マウスポインタがあった場所のデータの値」\n"\
"    　　等が表示されるが、これを表示するために、内部には\n"\
"    　　「最後に測定されたデータ番号」「マウスポインタがあった位置のデータ番号」\n"\
"    　　を記録する変数がある。\n"\
"    　　スキャン範囲を変えて再測定した際、これらの変数がクリアされていなかったので\n"\
"    　　範囲外のデータにアクセスしてしまっていた。\n"\
"    　　例えば、\n"\
"          10x10 の範囲を測定することにして、\n"\
"          (0,0)から測定を開始し、最後には(10,10)まで測定が終わった。\n"\
"          さらに途中でマウスを動かして(8,8)の点のデータを確認した。\n"\
"          この場合「最後の測定位置」は(10,10)、「マウス位置」は(8,8)と内部に記録される。\n"\
"          この状態で 測定範囲を 5x5 に変えて測定を開始すると\n"\
"          画面上に「最後の測定位置のデータ」「マウスがあった位置のデータ」を表示するため\n"\
"          data[10][10] とか data[8][8] とかにアクセスしようとして、\n"\
"          範囲外のデータアクセスで落ちる。(data の範囲は data[0-5][0-5] になってるので)\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri Oct 23 15:25:06 2015 +0900\n"\
                "    additional documentation in XAFSM0.def and XAFSM.def.BL5S1\n"\
		"\n"\
                ""
#endif
