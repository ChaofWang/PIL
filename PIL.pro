#-------------------------------------------------
#
# Project created by QtCreator 2016-01-27T13:17:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PIL
TEMPLATE = app


SOURCES += main.cpp\
        file.cpp \
    hist.cpp \
    geo_trans.cpp \
    filter.cpp \
    homomorphicfilter.cpp \
    image_segmentation.cpp

HEADERS  += file.h \
    hist.h \
    geo_trans.h \
    filter.h \
    homomorphicfilter.h \
    image_segmentation.h

FORMS    += file.ui \
    hist.ui \
    geo_trans.ui \
    filter.ui \
    homomorphicfilter.ui \
    image_segmentation.ui
INCLUDEPATH+=D:\OpenCV\opencv2.4.9\opencv\build\include
             D:\OpenCV\opencv2.4.9\opencv\build\include\opencv
             D:\OpenCV\opencv2.4.9\opencv\build\include\opencv2
RC_FILE = \
    icon.rc

LIBS+=D:\OpenCV\QtOpencv\lib\libopencv_calib3d249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_contrib249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_core249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_features2d249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_flann249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_gpu249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_highgui249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_imgproc249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_legacy249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_ml249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_objdetect249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_video249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_nonfree249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_ocl249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_photo249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_stitching249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_superres249.dll.a\
  D:\OpenCV\QtOpencv\lib\libopencv_ts249.a\
  D:\OpenCV\QtOpencv\lib\libopencv_videostab249.dll.a\

RESOURCES += \
    menu_pic.qrc

DISTFILES += \
    icon.rc
