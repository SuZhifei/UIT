TEMPLATE = app

QT += qml quick network widgets
CONFIG += c++11

uit_dist = ../../../../dist/g6s-win32/uit

SOURCES += main.cpp \
    TreeModel.cpp \
    TreeNode.cpp \
    ModelCluster.cpp \
    CurrentDataInfo.cpp \
    Type.cpp \
    ImageProvider.cpp \
    CurrentEventInfo.cpp \
    CurrentClusterInfo.cpp \
    CurrentEnumDefinitionInfo.cpp \
    CurrentItemTemplateInfo.cpp \
    SocketDebugger.cpp

RESOURCES += qml.qrc

HEADERS += \
    TreeModel.h \
    TreeNode.h \
    ModelCluster.h \
    CurrentDataInfo.h \
    Type.h \
    ImageProvider.h \
    CurrentEventInfo.h \
    CurrentClusterInfo.h \
    CurrentEnumDefinitionInfo.h \
    CurrentItemTemplateInfo.h \
    SocketDebugger.h

INCLUDEPATH += \
    $$uit_dist/include

LIBS += -L$$uit_dist/lib/debug \
-luit_core \
-luit_datafw \
-lPocoFoundationd \
-lPocoJSONd

DISTFILES +=

QMAKE_POST_LINK += copy /y debug\Simulator.exe ..\simulator\simulator\
#win32 {
#system(copy /y e:\default.png d:\1.png)
#}
#system(xcopy e:/default.png d:/)

