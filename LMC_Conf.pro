QT  += core gui opengl

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    serial/win_qextserialport.cpp \
    serial/qextserialport.cpp \
    serial/qextserialbase.cpp \
    Graph/qcustomplot.cpp \
    Graph/attitudewidget.cpp \
    GlobalVariables.cpp

HEADERS += \
    mainwindow.h \
    serial/win_qextserialport.h \
    serial/qextserialport.h \
    serial/qextserialbase.h \
    GlobalVariables.h \
    Graph/qcustomplot.h \
    Graph/attitudewidget.h
