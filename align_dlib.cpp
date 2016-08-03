#include "align_dlib.h"

AlignDlib::AlignDlib()
{

}

cv::Mat AlignDlib::dlibImgtoCV(dlib::array2d<dlib::bgr_pixel> &img)
{
    return  dlib::toMat(img);
}

void AlignDlib::init(const char * faceModelFileName, std::string shapePredictorFileName , int imgDim)
{
    try{
        loadMeanPointsFromCSV(faceModelFileName);
        loadShapePredictor(shapePredictorFileName);
        detector = dlib::get_frontal_face_detector();
        // And we also need a shape_predictor.  This is the tool that will predict face
        // landmark positions given an image and face bounding box.  Here we are just
        // loading the model from the shape_predictor_68_face_landmarks.dat.
        this->imgDim = imgDim;

    }
    catch (const std::invalid_argument& ia)
    {
        std::cerr << "Invalid argument: " << ia.what() << '\n';
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "Exception opening/reading/closing file\n";
    }
}

void AlignDlib::loadShapePredictor(std::string shapePredictorFileName)
{
    dlib::deserialize(shapePredictorFileName) >> shapePredictor;
}


int AlignDlib::loadMeanPointsFromCSV(const char * faceModelFileName)
{
    std::ifstream inputFaceModelFile(faceModelFileName);
    checkOpenedFile(inputFaceModelFile);

    CvMLData dataFile;

    // Load matrix data in csv format
    if (dataFile.read_csv(faceModelFileName) != 0)
    {
        //fprintf(stderr, "Can't read csv file %s\n", CSV_FILE);
        return -1;
    }

    Mat dataMat(dataFile.get_values()); // Default data type is float

    float *dataPtr = dataMat.ptr<float>(); // Get data from OpenCV Mat

    templateMeanX.resize(dataMat.rows);
    templateMeanY.resize(dataMat.rows);
    for(int r=0;r<dataMat.rows;r++){
        templateMeanX[r] = dataPtr[r*dataMat.cols + 0];//TEMPLATE X
        templateMeanY[r] = dataPtr[r*dataMat.cols + 1];//TEMPLATE Y
    }

}

void AlignDlib::checkOpenedFile(std::ifstream &inFile)
{
    if(!inFile.good())
        throw std::invalid_argument("Unable to process file.");;
}


bool AlignDlib::alignDlib(char *imgName, Mat *matAlight){
    //http://dlib.net/face_landmark_detection_ex.cpp.html

    Mat imgMat = cv::imread(imgName);
    //cv::resize(imgMat,imgMat,cv::Size(200,200), CV_INTER_LINEAR);
    //imshow("imgMat",imgMat);
    //waitKey(0);

    dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(imgMat));
    //load_image(imgLoad, imgName);
    // Now tell the face detector to give us a list of bounding boxes
    // around all the faces in the image.
    std::vector<dlib::rectangle> dets = detector(img);

    if(dets.size()!=1)
        return false;
    // Now we will go ask the shape_predictor to tell us the pose of
    // each face we detected.
    std::vector<dlib::full_object_detection> shapes;
    for (unsigned long j = 0; j < dets.size(); ++j)
    {
        dlib::full_object_detection shape = shapePredictor(img, dets[j]);
        //cout << "number of parts: "<< shape.num_parts() << endl;
        //cout << "pixel position of first part:  " << shape.part(0) << endl;
        //cout << "pixel position of second part: " << shape.part(1) << endl;
        // You get the idea, you can get all the face part locations if
        // you want them.  Here we just store them in shapes so we can
        // put them on the screen.
        shapes.push_back(shape);
    }


    //return the largest face bounding box in an image if more than one face detected,or None .
    dlib::full_object_detection largestFace;
    int largestArea = 0;
    for(dlib::full_object_detection shape:shapes){
        int currentArea = shape.get_rect().width()*shape.get_rect().height();
        if(currentArea>largestArea)
        {
            largestArea = currentArea;
            largestFace = shape;
        }
    }


    auto minmaxX = std::minmax_element(templateMeanX.begin(),templateMeanX.end());// TPL_MIN X and TPL_MAX X
    auto minmaxY = std::minmax_element(templateMeanY.begin(),templateMeanY.end());// TPL_MIN Y and TPL_MAX Y


    //templateMaxmin = imgDim * MINMAX_TEMPLATE[OUTER_EYES_AND_NOSE]
    //MINMAX_TEMPLATE = (TEMPLATE - TPL_MIN) / (TPL_MAX - TPL_MIN)
    vector<cv::Point2f> templateMaxmin;
    templateMaxmin.push_back(cv::Point2f(imgDim*((templateMeanX[OUTER_EYES_AND_NOSE[0]]-*minmaxX.first)/(*minmaxX.second-*minmaxX.first)),
                                         imgDim*((templateMeanY[OUTER_EYES_AND_NOSE[0]]-*minmaxY.first)/(*minmaxY.second-*minmaxY.first))));
    templateMaxmin.push_back(cv::Point2f(imgDim*((templateMeanX[OUTER_EYES_AND_NOSE[1]]-*minmaxX.first)/(*minmaxX.second-*minmaxX.first)),
                                         imgDim*((templateMeanY[OUTER_EYES_AND_NOSE[1]]-*minmaxY.first)/(*minmaxY.second-*minmaxY.first))));
    templateMaxmin.push_back(cv::Point2f(imgDim*((templateMeanX[OUTER_EYES_AND_NOSE[2]]-*minmaxX.first)/(*minmaxX.second-*minmaxX.first)),
                                         imgDim*((templateMeanY[OUTER_EYES_AND_NOSE[2]]-*minmaxY.first)/(*minmaxY.second-*minmaxY.first))));

    //npLandmarks[npLandmarkIndices]
    vector<cv::Point2f> npLandmarks;
    npLandmarks.push_back(Point2f(largestFace.part(OUTER_EYES_AND_NOSE[0]).x(),largestFace.part(OUTER_EYES_AND_NOSE[0]).y()));
    npLandmarks.push_back(Point2f(largestFace.part(OUTER_EYES_AND_NOSE[1]).x(),largestFace.part(OUTER_EYES_AND_NOSE[1]).y()));
    npLandmarks.push_back(Point2f(largestFace.part(OUTER_EYES_AND_NOSE[2]).x(),largestFace.part(OUTER_EYES_AND_NOSE[2]).y()));


    Mat rgbImg = dlib::toMat(img);

    cv::Mat affineTransform = cv::getAffineTransform(npLandmarks, templateMaxmin);
    cv::warpAffine(rgbImg,*matAlight, affineTransform , Size(imgDim, imgDim));

    return true;
}
