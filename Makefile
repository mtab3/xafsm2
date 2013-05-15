#############################################################################
# Makefile for building: XafsM2
# Generated by qmake (2.01a) (Qt 4.8.2) on: ? 5? 15 15:35:33 2013
# Project:  XafsM2.pro
# Template: app
# Command: /usr/local/bin/qmake-qt4 -o Makefile XafsM2.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W $(DEFINES)
INCPATH       = -I/usr/local/share/qt4/mkspecs/freebsd-g++ -I. -I/usr/local/include/qt4/QtCore -I/usr/local/include/qt4/QtNetwork -I/usr/local/include/qt4/QtGui -I/usr/local/include/qt4 -I. -I. -I. -I/usr/local/include/qt4 -I/usr/local/include
LINK          = g++
LFLAGS        = -Wl,-O1 -pthread -Wl,-rpath,/usr/local/lib/qt4
LIBS          = $(SUBLIBS)  -L/usr/local/lib/qt4 -L/usr/local/lib -lQtGui -L/usr/local/lib -L/usr/local/lib/qt4 -lQtNetwork -lQtCore 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/local/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -R
STRIP         = 
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = Atoms.cpp \
		AUnit.cpp \
		ChCoord.cpp \
		Conditions.cpp \
		Dark.cpp \
		DarkTable.cpp \
		Data.cpp \
		Fio.cpp \
		FluoDBase.cpp \
		Goto.cpp \
		GSBStats.cpp \
		Head9809.cpp \
		IonCh.cpp \
		KeV2Pix.cpp \
		MainWindow.cpp \
		MCAView.cpp \
		Measurement.cpp \
		Monitor.cpp \
		MouseC.cpp \
		MsgType.cpp \
		MultiUnits.cpp \
		MwCommon.cpp \
		MwLog.cpp \
		MwMeas.cpp \
		MwSetup.cpp \
		MwSSDSetUp.cpp \
		MwView.cpp \
		PeakFit.cpp \
		PeriodicT.cpp \
		QFio.cpp \
		QXafs.cpp \
		ReadData.cpp \
		ReadDef.cpp \
		Scan.cpp \
		SelMC2.cpp \
		SMsg.cpp \
		Sounds.cpp \
		Stars.cpp \
		StarsSV.cpp \
		StarsSV2.cpp \
		Status.cpp \
		TYView.cpp \
		ViewCtrl.cpp \
		XafsM.cpp \
		XYView.cpp moc_AUnit.cpp \
		moc_ChCoord.cpp \
		moc_Conditions.cpp \
		moc_DarkTable.cpp \
		moc_Data.cpp \
		moc_FluoDBase.cpp \
		moc_GSBStats.cpp \
		moc_Head9809.cpp \
		moc_KeV2Pix.cpp \
		moc_MainWindow.cpp \
		moc_MCAView.cpp \
		moc_MouseC.cpp \
		moc_MultiUnits.cpp \
		moc_PeakFit.cpp \
		moc_PeriodicT.cpp \
		moc_SelMC2.cpp \
		moc_Stars.cpp \
		moc_StarsSV.cpp \
		moc_StarsSV2.cpp \
		moc_Status.cpp \
		moc_TYView.cpp \
		moc_Units.cpp \
		moc_ViewCtrl.cpp \
		moc_XYView.cpp \
		qrc_XafsM.cpp
OBJECTS       = Atoms.o \
		AUnit.o \
		ChCoord.o \
		Conditions.o \
		Dark.o \
		DarkTable.o \
		Data.o \
		Fio.o \
		FluoDBase.o \
		Goto.o \
		GSBStats.o \
		Head9809.o \
		IonCh.o \
		KeV2Pix.o \
		MainWindow.o \
		MCAView.o \
		Measurement.o \
		Monitor.o \
		MouseC.o \
		MsgType.o \
		MultiUnits.o \
		MwCommon.o \
		MwLog.o \
		MwMeas.o \
		MwSetup.o \
		MwSSDSetUp.o \
		MwView.o \
		PeakFit.o \
		PeriodicT.o \
		QFio.o \
		QXafs.o \
		ReadData.o \
		ReadDef.o \
		Scan.o \
		SelMC2.o \
		SMsg.o \
		Sounds.o \
		Stars.o \
		StarsSV.o \
		StarsSV2.o \
		Status.o \
		TYView.o \
		ViewCtrl.o \
		XafsM.o \
		XYView.o \
		moc_AUnit.o \
		moc_ChCoord.o \
		moc_Conditions.o \
		moc_DarkTable.o \
		moc_Data.o \
		moc_FluoDBase.o \
		moc_GSBStats.o \
		moc_Head9809.o \
		moc_KeV2Pix.o \
		moc_MainWindow.o \
		moc_MCAView.o \
		moc_MouseC.o \
		moc_MultiUnits.o \
		moc_PeakFit.o \
		moc_PeriodicT.o \
		moc_SelMC2.o \
		moc_Stars.o \
		moc_StarsSV.o \
		moc_StarsSV2.o \
		moc_Status.o \
		moc_TYView.o \
		moc_Units.o \
		moc_ViewCtrl.o \
		moc_XYView.o \
		qrc_XafsM.o
DIST          = /usr/local/share/qt4/mkspecs/common/unix.conf \
		/usr/local/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/local/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/local/share/qt4/mkspecs/common/g++-base.conf \
		/usr/local/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/local/share/qt4/mkspecs/qconfig.pri \
		/usr/local/share/qt4/mkspecs/modules/qt_phonon.pri \
		/usr/local/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/local/share/qt4/mkspecs/features/qt_config.prf \
		/usr/local/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/local/share/qt4/mkspecs/features/default_pre.prf \
		/usr/local/share/qt4/mkspecs/features/release.prf \
		/usr/local/share/qt4/mkspecs/features/default_post.prf \
		/usr/local/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/local/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/local/share/qt4/mkspecs/features/warn_on.prf \
		/usr/local/share/qt4/mkspecs/features/qt.prf \
		/usr/local/share/qt4/mkspecs/features/moc.prf \
		/usr/local/share/qt4/mkspecs/features/resources.prf \
		/usr/local/share/qt4/mkspecs/features/uic.prf \
		/usr/local/share/qt4/mkspecs/features/yacc.prf \
		/usr/local/share/qt4/mkspecs/features/lex.prf \
		/usr/local/share/qt4/mkspecs/features/include_source_dir.prf \
		XafsM2.pro
QMAKE_TARGET  = XafsM2
DESTDIR       = 
TARGET        = XafsM2

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_Conditions.h ui_DarkTable.h ui_Data.h ui_MainWindow.h ui_MCAView.h ui_PeriodicT.h ui_ScrollWGrid.h ui_SelMC2.h ui_StarsSV.h ui_StarsSV2.h ui_XView.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: XafsM2.pro  /usr/local/share/qt4/mkspecs/freebsd-g++/qmake.conf /usr/local/share/qt4/mkspecs/common/unix.conf \
		/usr/local/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/local/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/local/share/qt4/mkspecs/common/g++-base.conf \
		/usr/local/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/local/share/qt4/mkspecs/qconfig.pri \
		/usr/local/share/qt4/mkspecs/modules/qt_phonon.pri \
		/usr/local/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/local/share/qt4/mkspecs/features/qt_config.prf \
		/usr/local/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/local/share/qt4/mkspecs/features/default_pre.prf \
		/usr/local/share/qt4/mkspecs/features/release.prf \
		/usr/local/share/qt4/mkspecs/features/default_post.prf \
		/usr/local/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/local/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/local/share/qt4/mkspecs/features/warn_on.prf \
		/usr/local/share/qt4/mkspecs/features/qt.prf \
		/usr/local/share/qt4/mkspecs/features/moc.prf \
		/usr/local/share/qt4/mkspecs/features/resources.prf \
		/usr/local/share/qt4/mkspecs/features/uic.prf \
		/usr/local/share/qt4/mkspecs/features/yacc.prf \
		/usr/local/share/qt4/mkspecs/features/lex.prf \
		/usr/local/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/local/lib/qt4/libQtGui.prl \
		/usr/local/lib/qt4/libQtCore.prl \
		/usr/local/lib/qt4/libQtNetwork.prl
	$(QMAKE) -o Makefile XafsM2.pro
/usr/local/share/qt4/mkspecs/common/unix.conf:
/usr/local/share/qt4/mkspecs/common/gcc-base.conf:
/usr/local/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/local/share/qt4/mkspecs/common/g++-base.conf:
/usr/local/share/qt4/mkspecs/common/g++-unix.conf:
/usr/local/share/qt4/mkspecs/qconfig.pri:
/usr/local/share/qt4/mkspecs/modules/qt_phonon.pri:
/usr/local/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/local/share/qt4/mkspecs/features/qt_functions.prf:
/usr/local/share/qt4/mkspecs/features/qt_config.prf:
/usr/local/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/local/share/qt4/mkspecs/features/default_pre.prf:
/usr/local/share/qt4/mkspecs/features/release.prf:
/usr/local/share/qt4/mkspecs/features/default_post.prf:
/usr/local/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/local/share/qt4/mkspecs/features/unix/thread.prf:
/usr/local/share/qt4/mkspecs/features/warn_on.prf:
/usr/local/share/qt4/mkspecs/features/qt.prf:
/usr/local/share/qt4/mkspecs/features/moc.prf:
/usr/local/share/qt4/mkspecs/features/resources.prf:
/usr/local/share/qt4/mkspecs/features/uic.prf:
/usr/local/share/qt4/mkspecs/features/yacc.prf:
/usr/local/share/qt4/mkspecs/features/lex.prf:
/usr/local/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/local/lib/qt4/libQtGui.prl:
/usr/local/lib/qt4/libQtCore.prl:
/usr/local/lib/qt4/libQtNetwork.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile XafsM2.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/XafsM21.0.0 || $(MKDIR) .tmp/XafsM21.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/XafsM21.0.0/ && $(COPY_FILE) --parents AtomGroup.h Atoms.h AUnit.h ChCoord.h Conditions.h DarkTable.h Data.h FluoDBase.h GSBStats.h Head9809.h KeV2Pix.h MainWindow.h MCAHead.h MCAView.h Mccd.h MouseC.h MsgType.h MultiUnits.h PeakFit.h PeriodicT.h SelMC2.h SMsg.h Stars.h StarsSV.h StarsSV2.h Status.h TYView.h Units.h ViewCtrl.h XafsM.h XYView.h .tmp/XafsM21.0.0/ && $(COPY_FILE) --parents XafsM.qrc .tmp/XafsM21.0.0/ && $(COPY_FILE) --parents Atoms.cpp AUnit.cpp ChCoord.cpp Conditions.cpp Dark.cpp DarkTable.cpp Data.cpp Fio.cpp FluoDBase.cpp Goto.cpp GSBStats.cpp Head9809.cpp IonCh.cpp KeV2Pix.cpp MainWindow.cpp MCAView.cpp Measurement.cpp Monitor.cpp MouseC.cpp MsgType.cpp MultiUnits.cpp MwCommon.cpp MwLog.cpp MwMeas.cpp MwSetup.cpp MwSSDSetUp.cpp MwView.cpp PeakFit.cpp PeriodicT.cpp QFio.cpp QXafs.cpp ReadData.cpp ReadDef.cpp Scan.cpp SelMC2.cpp SMsg.cpp Sounds.cpp Stars.cpp StarsSV.cpp StarsSV2.cpp Status.cpp TYView.cpp ViewCtrl.cpp XafsM.cpp XYView.cpp .tmp/XafsM21.0.0/ && $(COPY_FILE) --parents Conditions.ui DarkTable.ui Data.ui MainWindow.ui MCAView.ui PeriodicT.ui ScrollWGrid.ui SelMC2.ui StarsSV.ui StarsSV2.ui XView.ui .tmp/XafsM21.0.0/ && $(COPY_FILE) --parents XafsMsg_jp.ts .tmp/XafsM21.0.0/ && (cd `dirname .tmp/XafsM21.0.0` && $(TAR) XafsM21.0.0.tar XafsM21.0.0 && $(COMPRESS) XafsM21.0.0.tar) && $(MOVE) `dirname .tmp/XafsM21.0.0`/XafsM21.0.0.tar.gz . && $(DEL_FILE) -r .tmp/XafsM21.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_AUnit.cpp moc_ChCoord.cpp moc_Conditions.cpp moc_DarkTable.cpp moc_Data.cpp moc_FluoDBase.cpp moc_GSBStats.cpp moc_Head9809.cpp moc_KeV2Pix.cpp moc_MainWindow.cpp moc_MCAView.cpp moc_MouseC.cpp moc_MultiUnits.cpp moc_PeakFit.cpp moc_PeriodicT.cpp moc_SelMC2.cpp moc_Stars.cpp moc_StarsSV.cpp moc_StarsSV2.cpp moc_Status.cpp moc_TYView.cpp moc_Units.cpp moc_ViewCtrl.cpp moc_XYView.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_AUnit.cpp moc_ChCoord.cpp moc_Conditions.cpp moc_DarkTable.cpp moc_Data.cpp moc_FluoDBase.cpp moc_GSBStats.cpp moc_Head9809.cpp moc_KeV2Pix.cpp moc_MainWindow.cpp moc_MCAView.cpp moc_MouseC.cpp moc_MultiUnits.cpp moc_PeakFit.cpp moc_PeriodicT.cpp moc_SelMC2.cpp moc_Stars.cpp moc_StarsSV.cpp moc_StarsSV2.cpp moc_Status.cpp moc_TYView.cpp moc_Units.cpp moc_ViewCtrl.cpp moc_XYView.cpp
moc_AUnit.cpp: XafsM.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		MCAHead.h \
		AUnit.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) AUnit.h -o moc_AUnit.cpp

moc_ChCoord.cpp: ChCoord.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) ChCoord.h -o moc_ChCoord.cpp

moc_Conditions.cpp: ui_Conditions.h \
		Conditions.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) Conditions.h -o moc_Conditions.cpp

moc_DarkTable.cpp: ui_DarkTable.h \
		DarkTable.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) DarkTable.h -o moc_DarkTable.cpp

moc_Data.cpp: ui_Data.h \
		Units.h \
		Data.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) Data.h -o moc_Data.cpp

moc_FluoDBase.cpp: FluoDBase.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) FluoDBase.h -o moc_FluoDBase.cpp

moc_GSBStats.cpp: XafsM.h \
		GSBStats.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) GSBStats.h -o moc_GSBStats.cpp

moc_Head9809.cpp: Head9809.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) Head9809.h -o moc_Head9809.cpp

moc_KeV2Pix.cpp: XafsM.h \
		KeV2Pix.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) KeV2Pix.h -o moc_KeV2Pix.cpp

moc_MainWindow.cpp: ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h \
		MainWindow.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) MainWindow.h -o moc_MainWindow.cpp

moc_MCAView.cpp: ui_MCAView.h \
		MouseC.h \
		ChCoord.h \
		FluoDBase.h \
		KeV2Pix.h \
		XafsM.h \
		MCAView.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) MCAView.h -o moc_MCAView.cpp

moc_MouseC.cpp: MouseC.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) MouseC.h -o moc_MouseC.cpp

moc_MultiUnits.cpp: AUnit.h \
		XafsM.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		MCAHead.h \
		MultiUnits.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) MultiUnits.h -o moc_MultiUnits.cpp

moc_PeakFit.cpp: PeakFit.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) PeakFit.h -o moc_PeakFit.cpp

moc_PeriodicT.cpp: ui_PeriodicT.h \
		AtomGroup.h \
		PeriodicT.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) PeriodicT.h -o moc_PeriodicT.cpp

moc_SelMC2.cpp: ui_SelMC2.h \
		XafsM.h \
		Mccd.h \
		SelMC2.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) SelMC2.h -o moc_SelMC2.cpp

moc_Stars.cpp: SMsg.h \
		MsgType.h \
		Stars.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) Stars.h -o moc_Stars.cpp

moc_StarsSV.cpp: ui_StarsSV.h \
		StarsSV.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) StarsSV.h -o moc_StarsSV.cpp

moc_StarsSV2.cpp: ui_StarsSV2.h \
		StarsSV2.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) StarsSV2.h -o moc_StarsSV2.cpp

moc_Status.cpp: ui_ScrollWGrid.h \
		AUnit.h \
		XafsM.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		MCAHead.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		SelMC2.h \
		ui_SelMC2.h \
		Mccd.h \
		Conditions.h \
		ui_Conditions.h \
		Status.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) Status.h -o moc_Status.cpp

moc_TYView.cpp: ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) TYView.h -o moc_TYView.cpp

moc_Units.cpp: Units.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) Units.h -o moc_Units.cpp

moc_ViewCtrl.cpp: Data.h \
		ui_Data.h \
		Units.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		XafsM.h \
		GSBStats.h \
		ViewCtrl.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) ViewCtrl.h -o moc_ViewCtrl.cpp

moc_XYView.cpp: ui_XView.h \
		MouseC.h \
		ChCoord.h \
		XYView.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) XYView.h -o moc_XYView.cpp

compiler_rcc_make_all: qrc_XafsM.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_XafsM.cpp
qrc_XafsM.cpp: XafsM.qrc \
		finished.wav \
		Victoreen1.txt \
		Changer.png \
		KeV2MCApix.txt \
		xray-KLIII.txt \
		XafsM2.key \
		XAFSM0.def \
		XafsMsg_ja_JP.qm
	/usr/local/bin/rcc -name XafsM XafsM.qrc -o qrc_XafsM.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_Conditions.h ui_DarkTable.h ui_Data.h ui_MainWindow.h ui_MCAView.h ui_PeriodicT.h ui_ScrollWGrid.h ui_SelMC2.h ui_StarsSV.h ui_StarsSV2.h ui_XView.h
compiler_uic_clean:
	-$(DEL_FILE) ui_Conditions.h ui_DarkTable.h ui_Data.h ui_MainWindow.h ui_MCAView.h ui_PeriodicT.h ui_ScrollWGrid.h ui_SelMC2.h ui_StarsSV.h ui_StarsSV2.h ui_XView.h
ui_Conditions.h: Conditions.ui
	/usr/local/bin/uic-qt4 Conditions.ui -o ui_Conditions.h

ui_DarkTable.h: DarkTable.ui
	/usr/local/bin/uic-qt4 DarkTable.ui -o ui_DarkTable.h

ui_Data.h: Data.ui
	/usr/local/bin/uic-qt4 Data.ui -o ui_Data.h

ui_MainWindow.h: MainWindow.ui \
		Data.h \
		ui_Data.h \
		Units.h
	/usr/local/bin/uic-qt4 MainWindow.ui -o ui_MainWindow.h

ui_MCAView.h: MCAView.ui
	/usr/local/bin/uic-qt4 MCAView.ui -o ui_MCAView.h

ui_PeriodicT.h: PeriodicT.ui
	/usr/local/bin/uic-qt4 PeriodicT.ui -o ui_PeriodicT.h

ui_ScrollWGrid.h: ScrollWGrid.ui
	/usr/local/bin/uic-qt4 ScrollWGrid.ui -o ui_ScrollWGrid.h

ui_SelMC2.h: SelMC2.ui
	/usr/local/bin/uic-qt4 SelMC2.ui -o ui_SelMC2.h

ui_StarsSV.h: StarsSV.ui
	/usr/local/bin/uic-qt4 StarsSV.ui -o ui_StarsSV.h

ui_StarsSV2.h: StarsSV2.ui
	/usr/local/bin/uic-qt4 StarsSV2.ui -o ui_StarsSV2.h

ui_XView.h: XView.ui
	/usr/local/bin/uic-qt4 XView.ui -o ui_XView.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

Atoms.o: Atoms.cpp Atoms.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Atoms.o Atoms.cpp

AUnit.o: AUnit.cpp AUnit.h \
		XafsM.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		MCAHead.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AUnit.o AUnit.cpp

ChCoord.o: ChCoord.cpp ChCoord.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ChCoord.o ChCoord.cpp

Conditions.o: Conditions.cpp Conditions.h \
		ui_Conditions.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Conditions.o Conditions.cpp

Dark.o: Dark.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Dark.o Dark.cpp

DarkTable.o: DarkTable.cpp DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o DarkTable.o DarkTable.cpp

Data.o: Data.cpp Head9809.h \
		Data.h \
		ui_Data.h \
		Units.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		XafsM.h \
		GSBStats.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Data.o Data.cpp

Fio.o: Fio.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Fio.o Fio.cpp

FluoDBase.o: FluoDBase.cpp FluoDBase.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FluoDBase.o FluoDBase.cpp

Goto.o: Goto.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Goto.o Goto.cpp

GSBStats.o: GSBStats.cpp GSBStats.h \
		XafsM.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o GSBStats.o GSBStats.cpp

Head9809.o: Head9809.cpp Head9809.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Head9809.o Head9809.cpp

IonCh.o: IonCh.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o IonCh.o IonCh.cpp

KeV2Pix.o: KeV2Pix.cpp KeV2Pix.h \
		XafsM.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o KeV2Pix.o KeV2Pix.cpp

MainWindow.o: MainWindow.cpp XafsM.h \
		MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MainWindow.o MainWindow.cpp

MCAView.o: MCAView.cpp XafsM.h \
		MCAView.h \
		ui_MCAView.h \
		MouseC.h \
		ChCoord.h \
		FluoDBase.h \
		KeV2Pix.h \
		PeakFit.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MCAView.o MCAView.cpp

Measurement.o: Measurement.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Measurement.o Measurement.cpp

Monitor.o: Monitor.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Monitor.o Monitor.cpp

MouseC.o: MouseC.cpp MouseC.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MouseC.o MouseC.cpp

MsgType.o: MsgType.cpp MsgType.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MsgType.o MsgType.cpp

MultiUnits.o: MultiUnits.cpp MultiUnits.h \
		AUnit.h \
		XafsM.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		MCAHead.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MultiUnits.o MultiUnits.cpp

MwCommon.o: MwCommon.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwCommon.o MwCommon.cpp

MwLog.o: MwLog.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwLog.o MwLog.cpp

MwMeas.o: MwMeas.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwMeas.o MwMeas.cpp

MwSetup.o: MwSetup.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwSetup.o MwSetup.cpp

MwSSDSetUp.o: MwSSDSetUp.cpp SMsg.h \
		MsgType.h \
		MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwSSDSetUp.o MwSSDSetUp.cpp

MwView.o: MwView.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwView.o MwView.cpp

PeakFit.o: PeakFit.cpp PeakFit.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PeakFit.o PeakFit.cpp

PeriodicT.o: PeriodicT.cpp PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PeriodicT.o PeriodicT.cpp

QFio.o: QFio.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o QFio.o QFio.cpp

QXafs.o: QXafs.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o QXafs.o QXafs.cpp

ReadData.o: ReadData.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ReadData.o ReadData.cpp

ReadDef.o: ReadDef.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ReadDef.o ReadDef.cpp

Scan.o: Scan.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Scan.o Scan.cpp

SelMC2.o: SelMC2.cpp SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		Mccd.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SelMC2.o SelMC2.cpp

SMsg.o: SMsg.cpp MsgType.h \
		SMsg.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SMsg.o SMsg.cpp

Sounds.o: Sounds.cpp MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		XafsM.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Sounds.o Sounds.cpp

Stars.o: Stars.cpp Stars.h \
		SMsg.h \
		MsgType.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Stars.o Stars.cpp

StarsSV.o: StarsSV.cpp StarsSV.h \
		ui_StarsSV.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o StarsSV.o StarsSV.cpp

StarsSV2.o: StarsSV2.cpp StarsSV2.h \
		ui_StarsSV2.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o StarsSV2.o StarsSV2.cpp

Status.o: Status.cpp Status.h \
		ui_ScrollWGrid.h \
		AUnit.h \
		XafsM.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		MCAHead.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		SelMC2.h \
		ui_SelMC2.h \
		Mccd.h \
		Conditions.h \
		ui_Conditions.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Status.o Status.cpp

TYView.o: TYView.cpp XafsM.h \
		TYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o TYView.o TYView.cpp

ViewCtrl.o: ViewCtrl.cpp ViewCtrl.h \
		Data.h \
		ui_Data.h \
		Units.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		XafsM.h \
		GSBStats.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ViewCtrl.o ViewCtrl.cpp

XafsM.o: XafsM.cpp XafsM.h \
		MainWindow.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		Units.h \
		Mccd.h \
		SelMC2.h \
		ui_SelMC2.h \
		StarsSV2.h \
		ui_StarsSV2.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		FluoDBase.h \
		KeV2Pix.h \
		GSBStats.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MCAHead.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		Conditions.h \
		ui_Conditions.h \
		DarkTable.h \
		ui_DarkTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o XafsM.o XafsM.cpp

XYView.o: XYView.cpp XafsM.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o XYView.o XYView.cpp

moc_AUnit.o: moc_AUnit.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_AUnit.o moc_AUnit.cpp

moc_ChCoord.o: moc_ChCoord.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ChCoord.o moc_ChCoord.cpp

moc_Conditions.o: moc_Conditions.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Conditions.o moc_Conditions.cpp

moc_DarkTable.o: moc_DarkTable.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_DarkTable.o moc_DarkTable.cpp

moc_Data.o: moc_Data.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Data.o moc_Data.cpp

moc_FluoDBase.o: moc_FluoDBase.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_FluoDBase.o moc_FluoDBase.cpp

moc_GSBStats.o: moc_GSBStats.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_GSBStats.o moc_GSBStats.cpp

moc_Head9809.o: moc_Head9809.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Head9809.o moc_Head9809.cpp

moc_KeV2Pix.o: moc_KeV2Pix.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_KeV2Pix.o moc_KeV2Pix.cpp

moc_MainWindow.o: moc_MainWindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MainWindow.o moc_MainWindow.cpp

moc_MCAView.o: moc_MCAView.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MCAView.o moc_MCAView.cpp

moc_MouseC.o: moc_MouseC.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MouseC.o moc_MouseC.cpp

moc_MultiUnits.o: moc_MultiUnits.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MultiUnits.o moc_MultiUnits.cpp

moc_PeakFit.o: moc_PeakFit.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_PeakFit.o moc_PeakFit.cpp

moc_PeriodicT.o: moc_PeriodicT.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_PeriodicT.o moc_PeriodicT.cpp

moc_SelMC2.o: moc_SelMC2.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_SelMC2.o moc_SelMC2.cpp

moc_Stars.o: moc_Stars.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Stars.o moc_Stars.cpp

moc_StarsSV.o: moc_StarsSV.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_StarsSV.o moc_StarsSV.cpp

moc_StarsSV2.o: moc_StarsSV2.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_StarsSV2.o moc_StarsSV2.cpp

moc_Status.o: moc_Status.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Status.o moc_Status.cpp

moc_TYView.o: moc_TYView.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_TYView.o moc_TYView.cpp

moc_Units.o: moc_Units.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Units.o moc_Units.cpp

moc_ViewCtrl.o: moc_ViewCtrl.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ViewCtrl.o moc_ViewCtrl.cpp

moc_XYView.o: moc_XYView.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_XYView.o moc_XYView.cpp

qrc_XafsM.o: qrc_XafsM.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_XafsM.o qrc_XafsM.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

