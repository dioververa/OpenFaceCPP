
#include "torchwrap.h"

#include <iostream>

#include "lua.hpp"
#include "lua.h"
#include "TH/TH.h"
#include "luaT.h"
#include "lualib.h"
#include "lauxlib.h"
#include "TH/generic/THTensor.h"

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>

#include "opencv2/opencv.hpp"


using namespace std;
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

TorchWrap::TorchWrap(const char * inModelPath, int inImgDim, const char * inLuaFilePath){
    modelPath = inModelPath;
    imgDim  = inImgDim;
    luaFilePath = inLuaFilePath;
}


bool TorchWrap::forwardImage(cv::Mat img,std::vector<double> *features)
{

    int width = imgDim , height = imgDim;
    int size = width * height;

    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_loadfile(L, luaFilePath)) {
      cerr << "Something went wrong loading the chunk (syntax error?)" << endl;
      cerr << lua_tostring(L, -1) << endl;
      lua_pop(L,1);
      return false;
    }

    if (lua_pcall(L, 0, 0, 0)){
        cout << "Torchandroid Error running function: %s" << lua_tostring(L, -1) << endl;
        return false;
    }

    int numElements = width * height * 3;
    float *mydata = new float[numElements];
    THFloatStorage *mystorage =  THFloatStorage_newWithData(mydata, numElements);
    THFloatTensor* mytensor2 = THFloatTensor_newWithStorage1d(mystorage, 0, numElements, 1);

    cv::Mat dst;

    img.convertTo(dst, CV_32FC3, 1/255.0);

    int w=0;
    for(int i = 0; i < dst.rows; i++)
    {
        for(int j = 0; j < dst.cols; j++)
        {
        cv::Vec3f bgrPixel = dst.at<cv::Vec3f>(i, j);
        mydata[w] = bgrPixel[0]; // B
        mydata[size + w] = bgrPixel[1]; // G
        mydata[size * 2 + w] = bgrPixel[2]; // R
        ++w;

        }
    }

    lua_getglobal(L, "getFeatures");

    luaT_pushudata(L,(void *)mytensor2,"torch.FloatTensor");

    lua_pushstring(L,modelPath);
    lua_pushnumber(L, imgDim);
    if(lua_pcall(L,3,1,0) != 0){
        cout << "Error running function: %s" << lua_tostring(L, -1) << endl;
        return false;
     }else{
        //cout << "The return value of the function was" << endl;

        int i = 0;
        lua_pushnil(L);
        while(lua_next(L, -2) != 0){  //while table or integers exist on stack
            if(lua_isnumber(L, -1))
            {
                features->push_back((double)lua_tonumber(L, -1));
                i++;
            }
            else // If table = detection. All detections stored in a table of tables
            {
                cout << "error:" << endl;
                break;
            }
            lua_pop(L,1); //Pop number off stack
        }

        THFloatStorage_free(mystorage);
        lua_close(L);
        return true;
    }
}


