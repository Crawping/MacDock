QT += core gui

FORMS +=    DockWidget.ui \
            MacDock.ui
		
SOURCES += 	DockWidget.cpp \
			IconItem.cpp \
			MacDock.cpp \
			main.cpp
			
HEADERS += 	DockWidget.h \
			IconItem.h \
			MacDock.h

RESOURCES += \
    MacDock.qrc
