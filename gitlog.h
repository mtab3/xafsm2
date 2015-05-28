#ifndef GITLOG_H
#define GITLOG_H


#define BRANCH_NAME ( "stable" )
#define GITLOGS ""\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu May 21 20:47:06 2015 +0900\n"\
                "    The problem on the StopMode selection for qct08 was solved\n"\
		"\n"\
                "Author      : H. Asakura <h.asakura@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu May 21 22:20:55 2015 +0900\n"\
                "    Change InitialkeV from Pulse motor value to Encoder value\n"\
		"\n"\
                "Author      : H. Asakura <h.asakura@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu May 21 22:14:58 2015 +0900\n"\
                "    Fix filename in single measurement mode after using auto sampler\n"\
		"\n"\
                "Author      : H. Asakura <h.asakura@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu May 21 19:59:00 2015 +0900\n"\
                "    Update Standard parameters\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Tue May 19 15:14:19 2015 +0900\n"\
                "    QXafs measurment can do with 4 sensors(with Aux2)\n"\
		"\n"\
                "Author      : Masao TABUCHI <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Tue May 19 14:05:26 2015 +0900\n"\
                "    fix the Std-XANES-Block-parameter\n"\
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
                "Date        : Fri May 1 20:01:08 2015 +0900\n"\
                "    a problem, when qct08 is used for QXAFS, since the actual measurement time is different from the calculated one, the back-groud subtraction cause some err, was solved\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Fri May 1 19:44:34 2015 +0900\n"\
                "    For DV : messages from stars are identified by the condition that 'msg.ToCh() == Uid' in SetCurPos.   The way might be used for all of the messages for all devices\n"\
		"\n"\
                ""
#endif
