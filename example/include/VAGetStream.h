#ifndef VAGetStream_H
#define VAGetStream_H
#include"VideoAnalyticsApi.hpp"
#include <iostream>
#include<thread>
#include <mutex>
#include <unistd.h>
#include <sys/time.h>
#include<opencv2/opencv.hpp>
#include "aType.h"
namespace VAC{
  #define FPS 30

  struct VACFrame{
      cv::Mat frame;
      unsigned int sec;
      unsigned int usec;
      VACFrame()
      {
        sec = 0;
        usec = 0;
      }
      ~VACFrame()
      {
          sec = 0;
          usec = 0;
          frame.release();
      }

      VACFrame(const VACFrame& right)
      {
          frame =right.frame.clone();
          sec = right.sec;
          usec = right.usec;
      }
      const VACFrame& operator=(const VACFrame& right)
      {

          frame = right.frame.clone();
          sec = right.sec;
          usec = right.usec;
          return *this;
      }
  };

  struct VACParameterInit
  {
      int inputImageFormat_imageType;
      int inputImageFormat_memType;
      int inputImageFormat_width;
      int inputImageFormat_height;
      int mode;
      int setRectangle_x;
      int setRectangle_y;
      int setRectangle_width;
      int setRectangle_height;
      int value;
      int time;
      int outputImageFormat_imageTpye;
      int outputImageFormat_memTpye;
      int outputImageFormat_imageMode;
      int outputImageFormat_imageQuality;

  };

  class VAGetStream  {
    public:
      VAGetStream();
      ~VAGetStream();
        //atl::AnalyticsDataServer *analytics_data_server_;
       // atl::AD adata0;
       // atl::AD adata1;
        VACommonBase *pCommon_base ;
        VACParameterInit parameter_;
        std::string filename_;
        VACInputImageFormat inputImgFormat_;
        VACMode mode_;
        void* tskHandle0_;
        void* tskHandle1_;
        VACRectangle rec_;
        int value_;
        int time_;
        VACOutputImageFormat outputImgFormat_;


        std::string pipeline_0;
        std::string pipeline_1;
        cv::VideoCapture cap_0;
        cv::VideoCapture cap_1;


        VACInputImageInfo imgInfo_0;
        VACInputImageInfo imgInfo_1;
        VACDetectionMeta dMeta_0;
        VACDetectionMeta dMeta_1;

        struct timeval tv;
        int sec_  = 0;
        int usec_ = 0;

        std::mutex       g_global_mutex0_;
        std::mutex       g_global_mutex1_;
        std::list<VACFrame> g_frameList0_;
        std::list<VACFrame> g_frameList1_;

        std::thread getStreamThread0_;
        std::thread getStreamThread1_;
        std::thread analyticsStreamThread0_;
        std::thread analyticsStreamThread1_;


        void loadFile(std::string &filename,VACParameterInit &parameter);
        void initParameter(VACParameterInit parameter,VACInputImageFormat inputImgFormat,VACMode mode, VACRectangle rec,int value,int time,VACOutputImageFormat outputImgFormat);

        void produceImage0();
        void produceImage1();

        void getCurrentTime();


        void startDetectionThread();

       void startDetectionThread1();


    private:

  };
}

#endif
