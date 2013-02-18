#############################################################################
# Makefile for building: XafsM2
# Generated by qmake (2.01a) (Qt 4.8.2) on: ? 2? 18 11:43:30 2013
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
		Dark.cpp \
		Data.cpp \
		Fio.cpp \
		Goto.cpp \
		Head9809.cpp \
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
		PeriodicT.cpp \
		ReadData.cpp \
		ReadDef.cpp \
		Scan.cpp \
		SelMC.cpp \
		SMsg.cpp \
		Stars.cpp \
		StarsSV.cpp \
		Status.cpp \
		TYView.cpp \
		ViewCtrl.cpp \
		XafsM.cpp \
		XYView.cpp moc_AUnit.cpp \
		moc_ChCoord.cpp \
		moc_Data.cpp \
		moc_Head9809.cpp \
		moc_MainWindow.cpp \
		moc_MCAView.cpp \
		moc_MouseC.cpp \
		moc_MultiUnits.cpp \
		moc_PeriodicT.cpp \
		moc_SelMC.cpp \
		moc_Stars.cpp \
		moc_StarsSV.cpp \
		moc_Status.cpp \
		moc_TYView.cpp \
		moc_ViewCtrl.cpp \
		moc_XYView.cpp \
		qrc_XafsM.cpp
OBJECTS       = Atoms.o \
		AUnit.o \
		ChCoord.o \
		Dark.o \
		Data.o \
		Fio.o \
		Goto.o \
		Head9809.o \
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
		PeriodicT.o \
		ReadData.o \
		ReadDef.o \
		Scan.o \
		SelMC.o \
		SMsg.o \
		Stars.o \
		StarsSV.o \
		Status.o \
		TYView.o \
		ViewCtrl.o \
		XafsM.o \
		XYView.o \
		moc_AUnit.o \
		moc_ChCoord.o \
		moc_Data.o \
		moc_Head9809.o \
		moc_MainWindow.o \
		moc_MCAView.o \
		moc_MouseC.o \
		moc_MultiUnits.o \
		moc_PeriodicT.o \
		moc_SelMC.o \
		moc_Stars.o \
		moc_StarsSV.o \
		moc_Status.o \
		moc_TYView.o \
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

$(TARGET): ui_Data.h ui_MainWindow.h ui_MCAView.h ui_PeriodicT.h ui_ScrollWGrid.h ui_SelMC.h ui_StarsSV.h ui_XView.h $(OBJECTS)  
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
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/XafsM21.0.0/ && $(COPY_FILE) --parents AtomGroup.h Atoms.h AUnit.h ChCoord.h Data.h Head9809.h MainWindow.h MCAView.h Mccd.h MouseC.h MsgType.h MultiUnits.h PeriodicT.h SelMC.h SMsg.h Stars.h StarsSV.h Status.h TYView.h ViewCtrl.h XafsM.h XYView.h .tmp/XafsM21.0.0/ && $(COPY_FILE) --parents XafsM.qrc .tmp/XafsM21.0.0/ && $(COPY_FILE) --parents Atoms.cpp AUnit.cpp ChCoord.cpp Dark.cpp Data.cpp Fio.cpp Goto.cpp Head9809.cpp MainWindow.cpp MCAView.cpp Measurement.cpp Monitor.cpp MouseC.cpp MsgType.cpp MultiUnits.cpp MwCommon.cpp MwLog.cpp MwMeas.cpp MwSetup.cpp MwSSDSetUp.cpp MwView.cpp PeriodicT.cpp ReadData.cpp ReadDef.cpp Scan.cpp SelMC.cpp SMsg.cpp Stars.cpp StarsSV.cpp Status.cpp TYView.cpp ViewCtrl.cpp XafsM.cpp XYView.cpp .tmp/XafsM21.0.0/ && $(COPY_FILE) --parents Data.ui MainWindow.ui MCAView.ui PeriodicT.ui ScrollWGrid.ui SelMC.ui StarsSV.ui XView.ui .tmp/XafsM21.0.0/ && $(COPY_FILE) --parents XafsMsg_jp.ts .tmp/XafsM21.0.0/ && (cd `dirname .tmp/XafsM21.0.0` && $(TAR) XafsM21.0.0.tar XafsM21.0.0 && $(COMPRESS) XafsM21.0.0.tar) && $(MOVE) `dirname .tmp/XafsM21.0.0`/XafsM21.0.0.tar.gz . && $(DEL_FILE) -r .tmp/XafsM21.0.0


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

compiler_moc_header_make_all: moc_AUnit.cpp moc_ChCoord.cpp moc_Data.cpp moc_Head9809.cpp moc_MainWindow.cpp moc_MCAView.cpp moc_MouseC.cpp moc_MultiUnits.cpp moc_PeriodicT.cpp moc_SelMC.cpp moc_Stars.cpp moc_StarsSV.cpp moc_Status.cpp moc_TYView.cpp moc_ViewCtrl.cpp moc_XYView.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_AUnit.cpp moc_ChCoord.cpp moc_Data.cpp moc_Head9809.cpp moc_MainWindow.cpp moc_MCAView.cpp moc_MouseC.cpp moc_MultiUnits.cpp moc_PeriodicT.cpp moc_SelMC.cpp moc_Stars.cpp moc_StarsSV.cpp moc_Status.cpp moc_TYView.cpp moc_ViewCtrl.cpp moc_XYView.cpp
moc_AUnit.cpp: XafsM.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		AUnit.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) AUnit.h -o moc_AUnit.cpp

moc_ChCoord.cpp: ChCoord.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) ChCoord.h -o moc_ChCoord.cpp

moc_Data.cpp: ui_Data.h \
		Data.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) Data.h -o moc_Data.cpp

moc_Head9809.cpp: Head9809.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) Head9809.h -o moc_Head9809.cpp

moc_MainWindow.cpp: Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h \
		MainWindow.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) MainWindow.h -o moc_MainWindow.cpp

moc_MCAView.cpp: ui_MCAView.h \
		MouseC.h \
		ChCoord.h \
		MCAView.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) MCAView.h -o moc_MCAView.cpp

moc_MouseC.cpp: MouseC.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) MouseC.h -o moc_MouseC.cpp

moc_MultiUnits.cpp: AUnit.h \
		XafsM.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		MultiUnits.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) MultiUnits.h -o moc_MultiUnits.cpp

moc_PeriodicT.cpp: ui_PeriodicT.h \
		AtomGroup.h \
		PeriodicT.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) PeriodicT.h -o moc_PeriodicT.cpp

moc_SelMC.cpp: ui_SelMC.h \
		XafsM.h \
		Mccd.h \
		SelMC.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) SelMC.h -o moc_SelMC.cpp

moc_Stars.cpp: SMsg.h \
		MsgType.h \
		Stars.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) Stars.h -o moc_Stars.cpp

moc_StarsSV.cpp: ui_StarsSV.h \
		StarsSV.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) StarsSV.h -o moc_StarsSV.cpp

moc_Status.cpp: ui_ScrollWGrid.h \
		AUnit.h \
		XafsM.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		Status.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) Status.h -o moc_Status.cpp

moc_TYView.cpp: ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h
	/usr/local/bin/moc-qt4 $(DEFINES) $(INCPATH) TYView.h -o moc_TYView.cpp

moc_ViewCtrl.cpp: Data.h \
		ui_Data.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
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
		Victoreen1.txt \
		XafsM2.key \
		XAFSM0.def \
		XafsMsg_ja_JP.qm
	/usr/local/bin/rcc -name XafsM XafsM.qrc -o qrc_XafsM.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_Data.h ui_MainWindow.h ui_MCAView.h ui_PeriodicT.h ui_ScrollWGrid.h ui_SelMC.h ui_StarsSV.h ui_XView.h
compiler_uic_clean:
	-$(DEL_FILE) ui_Data.h ui_MainWindow.h ui_MCAView.h ui_PeriodicT.h ui_ScrollWGrid.h ui_SelMC.h ui_StarsSV.h ui_XView.h
ui_Data.h: Data.ui
	/usr/local/bin/uic-qt4 Data.ui -o ui_Data.h

ui_MainWindow.h: MainWindow.ui \
		Data.h \
		ui_Data.h
	/usr/local/bin/uic-qt4 MainWindow.ui -o ui_MainWindow.h

ui_MCAView.h: MCAView.ui
	/usr/local/bin/uic-qt4 MCAView.ui -o ui_MCAView.h

ui_PeriodicT.h: PeriodicT.ui
	/usr/local/bin/uic-qt4 PeriodicT.ui -o ui_PeriodicT.h

ui_ScrollWGrid.h: ScrollWGrid.ui
	/usr/local/bin/uic-qt4 ScrollWGrid.ui -o ui_ScrollWGrid.h

ui_SelMC.h: SelMC.ui
	/usr/local/bin/uic-qt4 SelMC.ui -o ui_SelMC.h

ui_StarsSV.h: StarsSV.ui
	/usr/local/bin/uic-qt4 StarsSV.ui -o ui_StarsSV.h

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
		MsgType.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AUnit.o AUnit.cpp

ChCoord.o: ChCoord.cpp ChCoord.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ChCoord.o ChCoord.cpp

Dark.o: Dark.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Dark.o Dark.cpp

Data.o: Data.cpp Head9809.h \
		Data.h \
		ui_Data.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Data.o Data.cpp

Fio.o: Fio.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Fio.o Fio.cpp

Goto.o: Goto.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Goto.o Goto.cpp

Head9809.o: Head9809.cpp Head9809.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Head9809.o Head9809.cpp

MainWindow.o: MainWindow.cpp XafsM.h \
		MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MainWindow.o MainWindow.cpp

MCAView.o: MCAView.cpp XafsM.h \
		MCAView.h \
		ui_MCAView.h \
		MouseC.h \
		ChCoord.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MCAView.o MCAView.cpp

Measurement.o: Measurement.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Measurement.o Measurement.cpp

Monitor.o: Monitor.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
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
		MsgType.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MultiUnits.o MultiUnits.cpp

MwCommon.o: MwCommon.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwCommon.o MwCommon.cpp

MwLog.o: MwLog.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwLog.o MwLog.cpp

MwMeas.o: MwMeas.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwMeas.o MwMeas.cpp

MwSetup.o: MwSetup.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwSetup.o MwSetup.cpp

MwSSDSetUp.o: MwSSDSetUp.cpp SMsg.h \
		MsgType.h \
		MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwSSDSetUp.o MwSSDSetUp.cpp

MwView.o: MwView.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MwView.o MwView.cpp

PeriodicT.o: PeriodicT.cpp PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PeriodicT.o PeriodicT.cpp

ReadData.o: ReadData.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ReadData.o ReadData.cpp

ReadDef.o: ReadDef.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ReadDef.o ReadDef.cpp

Scan.o: Scan.cpp MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Scan.o Scan.cpp

SelMC.o: SelMC.cpp SelMC.h \
		ui_SelMC.h \
		XafsM.h \
		Mccd.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SelMC.o SelMC.cpp

SMsg.o: SMsg.cpp MsgType.h \
		SMsg.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SMsg.o SMsg.cpp

Stars.o: Stars.cpp Stars.h \
		SMsg.h \
		MsgType.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Stars.o Stars.cpp

StarsSV.o: StarsSV.cpp StarsSV.h \
		ui_StarsSV.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o StarsSV.o StarsSV.cpp

Status.o: Status.cpp Status.h \
		ui_ScrollWGrid.h \
		AUnit.h \
		XafsM.h \
		Stars.h \
		SMsg.h \
		MsgType.h
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
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ViewCtrl.o ViewCtrl.cpp

XafsM.o: XafsM.cpp XafsM.h \
		MainWindow.h \
		Mccd.h \
		ui_MainWindow.h \
		Data.h \
		ui_Data.h \
		SelMC.h \
		ui_SelMC.h \
		StarsSV.h \
		ui_StarsSV.h \
		ViewCtrl.h \
		XYView.h \
		ui_XView.h \
		MouseC.h \
		ChCoord.h \
		TYView.h \
		MCAView.h \
		ui_MCAView.h \
		Atoms.h \
		Stars.h \
		SMsg.h \
		MsgType.h \
		PeriodicT.h \
		ui_PeriodicT.h \
		AtomGroup.h \
		AUnit.h \
		MultiUnits.h \
		Status.h \
		ui_ScrollWGrid.h
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

moc_Data.o: moc_Data.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Data.o moc_Data.cpp

moc_Head9809.o: moc_Head9809.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Head9809.o moc_Head9809.cpp

moc_MainWindow.o: moc_MainWindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MainWindow.o moc_MainWindow.cpp

moc_MCAView.o: moc_MCAView.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MCAView.o moc_MCAView.cpp

moc_MouseC.o: moc_MouseC.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MouseC.o moc_MouseC.cpp

moc_MultiUnits.o: moc_MultiUnits.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MultiUnits.o moc_MultiUnits.cpp

moc_PeriodicT.o: moc_PeriodicT.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_PeriodicT.o moc_PeriodicT.cpp

moc_SelMC.o: moc_SelMC.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_SelMC.o moc_SelMC.cpp

moc_Stars.o: moc_Stars.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Stars.o moc_Stars.cpp

moc_StarsSV.o: moc_StarsSV.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_StarsSV.o moc_StarsSV.cpp

moc_Status.o: moc_Status.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Status.o moc_Status.cpp

moc_TYView.o: moc_TYView.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_TYView.o moc_TYView.cpp

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

