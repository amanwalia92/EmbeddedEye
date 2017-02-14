#-------------------------------------------------
#
# Project created by QtCreator 2017-02-13T17:35:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QEmbeddedEye
TEMPLATE = app


SOURCES += main.cpp\
        qembeddedeye.cpp \
    qcamerasettingsdialog.cpp \
    qframedisplay.cpp \
    qframecapturethread.cpp

HEADERS  += qembeddedeye.h \
    qcamerasettingsdialog.h \
    qframedisplay.h \
    qmaindefinitions.h \
    qopencvincludables.h \
    qframecapturethread.h

FORMS    += qembeddedeye.ui \
    qcamerasettingsdialog.ui


INCLUDEPATH += C:\opencv\opencv\build\include
INCLUDEPATH += C:\opencv\opencv\build\include\opencv
INCLUDEPATH += C:\opencv\opencv\build\include\opencv2

LIBS += -LC:\opencv\opencv\build\x64\vc12\lib \
            -lopencv_calib3d2411 \
            -lopencv_contrib2411 \
            -lopencv_core2411 \
            -lopencv_features2d2411 \
            -lopencv_flann2411 \
            -lopencv_gpu2411 \
            -lopencv_highgui2411 \
            -lopencv_imgproc2411 \
            -lopencv_legacy2411 \
            -lopencv_ml2411 \
            -lopencv_nonfree2411 \
            -lopencv_objdetect2411 \
            -lopencv_ocl2411 \
            -lopencv_photo2411 \
            -lopencv_stitching2411 \
            -lopencv_superres2411 \
            -lopencv_ts2411 \
            -lopencv_video2411 \
            -lopencv_videostab2411
