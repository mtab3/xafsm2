#ifndef GITLOG_H
#define GITLOG_H


#define BRANCH_NAME ( "stable" )
#define GITLOGS ""\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Mon Jun 29 22:52:31 2015 +0900\n"\
                "    The QXAFS measurement of 20,000 data points was OK with new qct08\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu Jun 18 13:52:19 2015 +0900\n"\
                "    it may ok for QXafs with up to 20,000 points by changing the value of maxQXafsPoints.  Practically, the value should be less than 19990 or so, for safety.\n"\
		"\n"\
                "Author      : M. Tabuchi <m.tabuchi@nusr.nagoya-u.ac.jp>\n"\
                "Date        : Thu May 28 21:22:42 2015 +0900\n"\
                "    only checking why q-xafs can not go well with q34410a when the dtime is longer than 0.05sec, but not solved\n"\
		"\n"\
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
                ""
#endif
