#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H

#include <vector>
#include "svm.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "align_dlib.h"
#include "torchwrap.h"

using namespace std;
using namespace cv;

class FaceRecognition
{
public:
    FaceRecognition();
    bool Classification(AlignDlib *align, TorchWrap *tw,char * pathNameImage);
    int Training(AlignDlib *align, TorchWrap *tw, char *trainingImagesPath);
};

#endif // FACERECOGNITION_H
