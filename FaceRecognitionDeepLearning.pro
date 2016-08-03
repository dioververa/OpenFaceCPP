#-------------------------------------------------
#
# Project created by QtCreator 2016-07-21T11:57:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceRecognitionDeepLearning
TEMPLATE = app


QMAKE_CXXFLAGS += -std=c++11 -DDLIB_PNG_SUPPORT -DDLIB_JPEG_SUPPORT
QMAKE_CXXFLAGS += -DDLIB_USE_BLAS
QMAKE_CXXFLAGS += -DDLIB_USE_LAPACK
#QMAKE_CXXFLAGS += -DDLIB_DISABLE_ASSERTS
#DEFINES += -DDLIB_JPEG_SUPPORT
#define DLIB_JPEG_SUPPORT
CONFIG += link_pkgconfig
PKGCONFIG += x11

SOURCES += main.cpp\
        mainwindow.cpp \
    svm.cpp \
    align_dlib.cpp \
    torchwrap.cpp \
    facerecognition.cpp

HEADERS  += mainwindow.h \
    svm.h \
    align_dlib.h \
    torchwrap.h \
    facerecognition.h

FORMS    += mainwindow.ui


# Opencv 2.4.9  PC-DIOVER
INCLUDEPATH += /home/diover/programs/OpenCv/opencv-2.4.11/include
LIBS += -L/home/diover/programs/OpenCv/opencv-2.4.11/lib

LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann
LIBS += -lopencv_nonfree

INCLUDEPATH += /home/diover/proyectos/opencv/.codigos/dio/FaceRecognition/FaceRecognitionDeepLearning/lib/dlib-18.18

LIBS += -L/home/diover/proyectos/qemes/openface-c++/OpenFaceCpp-master/Lib/dlib-18.18/build/dlib -ldlib

LIBS += -llapack
LIBS += -lblas
LIBS += -lpthread
LIBS += -lpng -ljpeg

LIBS += /home/diover/proyectos/opencv/.codigos/dio/FaceRecognition/FaceRecognitionDeepLearning/lib/libsvmlib.a

INCLUDEPATH += /home/diover/programs/sources/Torch/torch2/distro/install/include
LIBS += -L/home/diover/programs/sources/Torch/torch2/distro/install/lib
LIBS += -lluajit
LIBS += -lluaT
LIBS += -lTH
LIBS += -lTHC
#LIBS += -llua
LIBS += -lm
LIBS += -ldl
