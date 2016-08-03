#ifndef TORCHWRAP_H
#define TORCHWRAP_H


#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <string>
#include <exception>
#include <fstream>
#include <vector>
#include "opencv2/opencv.hpp"


class TorchWrap{
    const char * modelPath;
    int imgDim;
    const char * luaFilePath;
public:
    TorchWrap();
    TorchWrap(const char *inModelPath, int inImgDim, const char *inLuaFilePath);
    bool forwardImage(cv::Mat img, std::vector<double> *features);
};

#endif
