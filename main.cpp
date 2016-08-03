#include "mainwindow.h"
#include <QApplication>

#include "opencv2/opencv.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "align_dlib.h"
#include "torchwrap.h"

#include "dirent.h"

#include <cstdio>
#include <memory>
#include <stdlib.h>
#include <cstdlib>
#include <facerecognition.h>

using namespace cv;
using namespace std;

AlignDlib align;

#define TRAINING          1
#define CLASSIFICATION    2

int main(int argc, char *argv[])
{

    int TrainingOrClassification = CLASSIFICATION;

    const char * modelPath = "models/openface/nn4.small2.v1.t7";
    const char * luaFilePath = "models/lua/openface_server1.lua";
    std::string shapePredictorFileName = "models/dlib/shape_predictor_68_face_landmarks.dat";
    const char * faceModelFileName = "models/dlib/mean.csv";

    int imgDim  = 96;

    //Init Align
    align.init(faceModelFileName,shapePredictorFileName ,imgDim);

    //Init Torch
    TorchWrap tw(modelPath, imgDim, luaFilePath );

    FaceRecognition objetFaceRecognition;


    if(TrainingOrClassification == TRAINING){

        char * trainingPath = "images/trainingPath/";
        objetFaceRecognition.Training(&align,&tw,trainingPath);


    }else if(TrainingOrClassification == CLASSIFICATION){

        char * pathNameImage = "imageProf.jpg";
        objetFaceRecognition.Classification(&align,&tw,pathNameImage);

    }
    return 0;

}
