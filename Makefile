#############################################################################
# Makefile for building: WVS
# Generated by qmake (2.01a) (Qt 4.8.6) on: Thu May 14 16:16:46 2015
# Project:  WVS.pro
# Template: app
# Command: /usr/lib/x86_64-linux-gnu/qt4/bin/qmake -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile WVS.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4 -I/usr/X11R6/include -I. -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib/x86_64-linux-gnu -L/usr/X11R6/lib -lQtOpenGL -lQtGui -lQtCore -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/lib/x86_64-linux-gnu/qt4/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		wvs_main_window.cpp \
		GLWidget.cpp \
		GLContainer.cpp \
		stdafx.cpp moc_wvs_main_window.cpp \
		moc_GLContainer.cpp \
		moc_GLWidget.cpp
OBJECTS       = main.o \
		wvs_main_window.o \
		GLWidget.o \
		GLContainer.o \
		stdafx.o \
		moc_wvs_main_window.o \
		moc_GLContainer.o \
		moc_GLWidget.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		WVS.pro
QMAKE_TARGET  = WVS
DESTDIR       = 
TARGET        = WVS

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

$(TARGET): ui_wvs_main_window.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
	{ test -n "$(DESTDIR)" && DESTDIR="$(DESTDIR)" || DESTDIR=.; } && test $$(gdb --version | sed -e 's,[^0-9][^0-9]*\([0-9]\)\.\([0-9]\).*,\1\2,;q') -gt 72 && gdb --nx --batch --quiet -ex 'set confirm off' -ex "save gdb-index $$DESTDIR" -ex quit '$(TARGET)' && test -f $(TARGET).gdb-index && objcopy --add-section '.gdb_index=$(TARGET).gdb-index' --set-section-flags '.gdb_index=readonly' '$(TARGET)' '$(TARGET)' && rm -f $(TARGET).gdb-index || true

Makefile: WVS.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/x86_64-linux-gnu/libQtOpenGL.prl \
		/usr/lib/x86_64-linux-gnu/libQtGui.prl \
		/usr/lib/x86_64-linux-gnu/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile WVS.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/shared.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/opengl.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/x86_64-linux-gnu/libQtOpenGL.prl:
/usr/lib/x86_64-linux-gnu/libQtGui.prl:
/usr/lib/x86_64-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile WVS.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/WVS1.0.0 || $(MKDIR) .tmp/WVS1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/WVS1.0.0/ && $(COPY_FILE) --parents wvs_main_window.h GLContainer.h MyPoint.h stdafx.h GLWidget.h .tmp/WVS1.0.0/ && $(COPY_FILE) --parents main.cpp wvs_main_window.cpp GLWidget.cpp GLContainer.cpp stdafx.cpp .tmp/WVS1.0.0/ && $(COPY_FILE) --parents wvs_main_window.ui .tmp/WVS1.0.0/ && (cd `dirname .tmp/WVS1.0.0` && $(TAR) WVS1.0.0.tar WVS1.0.0 && $(COMPRESS) WVS1.0.0.tar) && $(MOVE) `dirname .tmp/WVS1.0.0`/WVS1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/WVS1.0.0


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

compiler_moc_header_make_all: moc_wvs_main_window.cpp moc_GLContainer.cpp moc_GLWidget.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_wvs_main_window.cpp moc_GLContainer.cpp moc_GLWidget.cpp
moc_wvs_main_window.cpp: wvs_main_window.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) wvs_main_window.h -o moc_wvs_main_window.cpp

moc_GLContainer.cpp: stdafx.h \
		GLWidget.h \
		MyPoint.h \
		GLContainer.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) GLContainer.h -o moc_GLContainer.cpp

moc_GLWidget.cpp: stdafx.h \
		MyPoint.h \
		GLWidget.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) GLWidget.h -o moc_GLWidget.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_wvs_main_window.h
compiler_uic_clean:
	-$(DEL_FILE) ui_wvs_main_window.h
ui_wvs_main_window.h: wvs_main_window.ui \
		GLContainer.h \
		stdafx.h \
		GLWidget.h \
		MyPoint.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/uic wvs_main_window.ui -o ui_wvs_main_window.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

main.o: main.cpp wvs_main_window.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

wvs_main_window.o: wvs_main_window.cpp wvs_main_window.h \
		ui_wvs_main_window.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o wvs_main_window.o wvs_main_window.cpp

GLWidget.o: GLWidget.cpp stdafx.h \
		GLWidget.h \
		MyPoint.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o GLWidget.o GLWidget.cpp

GLContainer.o: GLContainer.cpp stdafx.h \
		GLContainer.h \
		GLWidget.h \
		MyPoint.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o GLContainer.o GLContainer.cpp

stdafx.o: stdafx.cpp stdafx.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o stdafx.o stdafx.cpp

moc_wvs_main_window.o: moc_wvs_main_window.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_wvs_main_window.o moc_wvs_main_window.cpp

moc_GLContainer.o: moc_GLContainer.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_GLContainer.o moc_GLContainer.cpp

moc_GLWidget.o: moc_GLWidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_GLWidget.o moc_GLWidget.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
