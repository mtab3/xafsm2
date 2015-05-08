#ifndef GITLOG_H
#define GITLOG_H


#define BRANCH_NAME ( "newMwSSD2" )
#define GITLOGS ""\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri May 8 12:59:47 2015 +0900\n"\
                "    SendCMD is called only in AUnits. The From-ch is set to Uid.\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri May 8 12:28:32 2015 +0900\n"\
                "    Now, all of the messages to the drivers connecting Stars are sent via the AUnit.   When a message is sent to Stars via an AUnit, the from-ch is always set as Uid\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu May 7 17:11:56 2015 +0900\n"\
                "    before changing 'From() == DevCh' to 'ToCh() == Uid'\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri May 1 20:02:14 2015 +0900\n"\
                "    merged with master\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri May 1 20:01:08 2015 +0900\n"\
                "    a problem, when qct08 is used for QXAFS, since the actual measurement time is different from the calculated one, the back-groud subtraction cause some err, was solved\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri May 1 19:44:34 2015 +0900\n"\
                "    For DV : messages from stars are identified by the condition that 'msg.ToCh() == Uid' in SetCurPos.   The way might be used for all of the messages for all devices\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Tue Apr 28 20:37:51 2015 +0900\n"\
                "    lion\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Tue Apr 28 20:24:56 2015 +0900\n"\
                "    merged with master\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Tue Apr 28 20:10:23 2015 +0900\n"\
                "    merged\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Tue Apr 28 20:07:22 2015 +0900\n"\
                "    LION\n"\
		"\n"\
                ""
#endif
