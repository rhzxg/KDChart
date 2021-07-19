system('echo ==================================================================================')
system('echo The QMake Buildsystem is deprecated and will be removed in the next major release.')
system('echo Please consider migrating to the CMake Buildsystem at your earliest opportunity.')
system('echo Instructions can be found in INSTALL-cmake.txt')
system('echo ==================================================================================')
system('echo')

include( variables.pri )

CONFIG += ordered

TEMPLATE=subdirs
SUBDIRS = src include

SUBDIRS += examples

unittests: SUBDIRS +=qtests
SUBDIRS +=tests

VERSION  = 2.8.0

MAJOR_VERSION = 2

QT_VERSION=$$[QT_VERSION]
isEmpty(QT_VERSION) {
    error("ERROR: qmake from Qt3 detected. Please use qmake from Qt4 or Qt5!")
}

unix:DEFAULT_INSTALL_PREFIX = /usr/local/KDAB/KDChart-$$VERSION
win32:DEFAULT_INSTALL_PREFIX = "C:\KDAB\KDChart"-$$VERSION

# for backw. compat. we still allow manual invocation of qmake using PREFIX:
isEmpty( KDCHART_INSTALL_PREFIX ): KDCHART_INSTALL_PREFIX=$$PREFIX

# if still empty we use the default:
isEmpty( KDCHART_INSTALL_PREFIX ): KDCHART_INSTALL_PREFIX=$$DEFAULT_INSTALL_PREFIX

# if the default was either set by configure or set by the line above:
equals( KDCHART_INSTALL_PREFIX, $$DEFAULT_INSTALL_PREFIX ) {
    INSTALL_PREFIX=$$DEFAULT_INSTALL_PREFIX
    unix:message( "No install prefix given, using default of" $$DEFAULT_INSTALL_PREFIX (use configure.sh -prefix DIR to specify))
    !unix:message( "No install prefix given, using default of" $$DEFAULT_INSTALL_PREFIX (use configure -prefix DIR to specify))
} else {
    INSTALL_PREFIX=$$KDCHART_INSTALL_PREFIX
}

DEBUG_SUFFIX=""
VERSION_SUFFIX=""
CONFIG(debug, debug|release) {
  !unix: DEBUG_SUFFIX = d
}
!unix:!mac:!static:VERSION_SUFFIX=$$MAJOR_VERSION

KDCHARTLIB = kdchart$$DEBUG_SUFFIX$$VERSION_SUFFIX
TESTTOOLSLIB = testtools$$VERSION_SUFFIX

message(Install prefix is $$INSTALL_PREFIX)
message(This is KD Chart version $$VERSION)

# These files are in the build directory (Because "somecommand >> somefile" puts them there)
CONFQMAKE_CACHE = "$${OUT_PWD}/.confqmake.cache"
QMAKE_CACHE = "$${OUT_PWD}/.qmake.cache"

# Create a .qmake.cache file
unix:MESSAGE = '\\'$$LITERAL_HASH\\' KDAB qmake cache file: autogenerated during qmake run'
!unix:MESSAGE = $$LITERAL_HASH' KDAB qmake cache file: autogenerated during qmake run'

system('echo $${MESSAGE} > $${QMAKE_CACHE}')
TMP_SOURCE_DIR = $${IN_PWD}
TMP_BUILD_DIR = $${OUT_PWD}
system('echo TOP_SOURCE_DIR=$${TMP_SOURCE_DIR} >> $${QMAKE_CACHE}')
system('echo TOP_BUILD_DIR=$${TMP_BUILD_DIR} >> $${QMAKE_CACHE}')

# store PREFIX:
windows:INSTALL_PREFIX=$$replace(INSTALL_PREFIX, \\\\, /)
system('echo INSTALL_PREFIX=$$INSTALL_PREFIX >> $${QMAKE_CACHE}')
system('echo VERSION=$$VERSION >> $${QMAKE_CACHE}')
system('echo KDCHARTLIB=$$KDCHARTLIB >> $${QMAKE_CACHE}')
system('echo TESTTOOLSLIB=$$TESTTOOLSLIB >> $${QMAKE_CACHE}')
system('echo KDCHARTUITOOLSLIB=$$KDCHARTUITOOLSLIB >> $${QMAKE_CACHE}')
system('echo KDCHARTPLUGINLIBLIB=$$KDCHARTPLUGINLIBLIB >> $${QMAKE_CACHE}')

INSTALL_DOC_DIR = $$INSTALL_PREFIX/share/doc/KDChart

# install license(s):
exists( LICENSE.txt ):     licenses.files  = LICENSE.txt
exists( LICENSE.US.txt ):  licenses.files += LICENSE.US.txt
exists( LICENSE.GPL.txt ): licenses.files += LICENSE.GPL.txt
licenses.path = $$INSTALL_DOC_DIR
INSTALLS += licenses

# install readme(s):
exists ( README.txt ):            readme.files  = README.txt
exists ( README-commercial.txt ): readme.files += README-commercial.txt
readme.path = $$INSTALL_DOC_DIR
INSTALLS += readme

# forward make test calls to qtests:
test.target=test
test.commands=(cd qtests && $(MAKE) test)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test
