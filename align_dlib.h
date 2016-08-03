#ifndef ALIGNDLIB_H
#define ALIGNDLIB_H

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
using namespace cv;

class AlignDlib
{
public:
    AlignDlib();

    dlib::array2d<dlib::bgr_pixel> img;
    dlib::frontal_face_detector detector;
    dlib::shape_predictor shapePredictor;

    vector<float> templateMeanX,templateMeanY;

    void init(const char *faceModelFileName,
                std::string shapePredictorFileName, int imgDim);

    void loadShapePredictor(std::string shapePredictorFileName);
    int loadMeanPointsFromCSV(const char *faceModelName);
    void checkOpenedFile(std::ifstream &inFile);

    bool alignDlib(char *imgName, Mat *matAlight);
    cv::Mat dlibImgtoCV(dlib::array2d<dlib::bgr_pixel> &img);

    int imgDim;

    vector<int> OUTER_EYES_AND_NOSE = {36, 45, 33};
};

#endif // ALIGNDLIB_H
