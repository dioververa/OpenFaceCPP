# OpenFaceCPP

Face recognition with deep neural networks c++

Getting Started

Prerequisites
. OpenCV (2.4.11 or less)
. Torch (http://torch.ch/docs/getting-started.html#_)
. Dlib Go into the lib/dlib-18.18 folder and type:
$ mkdir build;
$cd build; cmake .. ;
$cmake --build
. built libraries will be generated within the lib/dlib-18.18/build/dlib folder

Build & development

I have not done the makefile, soon I will.

$ g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o OpenFace main.cpp torchwrap.cpp svm.cpp align_dlib.cpp facerecognition.cpp torchwrap.h svm.h align_dlib.h facerecognition.h -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching -I/path to the folder OpenFaceCPP-1/lib/dlib-18.18/dlib -L/path to the folder OpenFaceCPP-1/lib/dlib-18.18/build/dlib -ldlib -I/path to the folder Torch/distro/install/include -L/path to the folder Torch/distro/install/lib -lluajit -lluaT -lTH -lm -ldl -g -o OpenFace main.cpp torchwrap.cpp svm.cpp align_dlib.cpp facerecognition.cpp lib/dlib-18.18/dlib/threads/threads_kernel_shared.cpp lib/dlib-18.18/dlib/entropy_decoder/entropy_decoder_kernel_2.cpp lib/dlib-18.18/dlib/base64/base64_kernel_1.cpp lib/dlib-18.18/dlib/threads/threads_kernel_1.cpp lib/dlib-18.18/dlib/threads/threads_kernel_2.cpp -std=c++11 -I/path to the folder OpenFaceCPP-1 -llapack -lblas
