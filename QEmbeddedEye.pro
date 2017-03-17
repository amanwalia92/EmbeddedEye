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
    qframecapturethread.cpp \
    qimagesaver.cpp \
    qtrackingsettingsdialog.cpp \
    qstreamingsettings.cpp \
    qstreamer.cpp \
    Motor_Sources/ControllerAPI.cpp \
    Motor_Sources/ControlTables.cpp \
    Motor_Sources/Dynamixel.cpp \
    Motor_Sources/DynamixelController.cpp \
    Motor_Sources/DynamixelSimpleAPI.cpp \
    Motor_Sources/DynamixelTools.cpp \
    Motor_Sources/minitraces.cpp \
    Motor_Sources/SerialPort.cpp \
    Motor_Sources/SerialPortLinux.cpp \
    Motor_Sources/Servo.cpp \
    Motor_Sources/ServoDynamixel.cpp \
    Motor_Sources/ServoMX.cpp \
    Motor_Sources/Utils.cpp \
    MotorMatrix.cpp

HEADERS  += qembeddedeye.h \
    qcamerasettingsdialog.h \
    qframedisplay.h \
    qmaindefinitions.h \
    qopencvincludables.h \
    qframecapturethread.h \
    qimagesaver.h \
    qtrackingsettingsdialog.h \
    qstreamingsettings.h \
    qstreamer.h \
    Motor_Headers/ControllerAPI.h \
    Motor_Headers/ControlTables.h \
    Motor_Headers/ControlTablesDynamixel.h \
    Motor_Headers/Dynamixel.h \
    Motor_Headers/DynamixelController.h \
    Motor_Headers/DynamixelSimpleAPI.h \
    Motor_Headers/DynamixelTools.h \
    Motor_Headers/minitraces.h \
    Motor_Headers/minitraces_conf.h \
    Motor_Headers/SerialPort.h \
    Motor_Headers/SerialPortLinux.h \
    Motor_Headers/Servo.h \
    Motor_Headers/ServoDynamixel.h \
    Motor_Headers/ServoMX.h \
    Motor_Headers/Utils.h \
    MotorMatrix.h

FORMS    += qembeddedeye.ui \
    qcamerasettingsdialog.ui \
    qtrackingsettingsdialog.ui \
    qstreamingsettings.ui


INCLUDEPATH += /usr/local/include/
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/local/lib -L/usr/local/cuda-8.0/lib64 \
            -lopencv_contrib \
            -lopencv_core \
            -lopencv_features2d \
            -lopencv_flann \
            -lopencv_gpu \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_legacy \
            -lopencv_ml \
            -lopencv_nonfree \
            -lopencv_objdetect \
            -lopencv_ocl \
            -lopencv_photo \
            -lopencv_stitching \
            -lopencv_superres \
            -lopencv_ts \
            -lopencv_video \
            -lopencv_videostab \
            -lcufft \
            -lcublas \
            -lnpps \
            -lnppi \
            -lnppc \
            -lcudart \
            -latomic \
            -ltbb \
            -lrt \
            -lpthread \
            -lm \
            -ldl \

# Tell qmake to use pkg-config to find QtGStreamer.
CONFIG += link_pkgconfig

PKGCONFIG +=  gstreamer-1.0
LIBS += -llockdev
