#include"VAGetStream.h"

#include <fstream>
using namespace std;
namespace VAC
{
inline std::string get_tegra_pipeline_0 ( int width, int height, int fps )
{
    return "nvcamerasrc sensor-id = 0  ! video/x-raw(memory:NVMM), width=(int)" + std::to_string ( width ) + ", height=(int)" +
           std::to_string ( height ) + ", format=(string)I420, framerate=(fraction)" + std::to_string ( fps ) +
           "/1 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}
inline std::string get_tegra_pipeline_1 ( int width, int height, int fps )
{
    return "nvcamerasrc sensor-id = 1 ! video/x-raw(memory:NVMM), width=(int)" + std::to_string ( width ) + ", height=(int)" +
           std::to_string ( height ) + ", format=(string)I420, framerate=(fraction)" + std::to_string ( fps ) +
           "/1 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

VAGetStream::VAGetStream()
{
    pCommon_base = new VACommonBase(NULL);
    std::string filename_ = "./source/parameter.txt";
    loadFile(filename_,parameter_);

    inputImgFormat_.imageType = parameter_.inputImageFormat_imageType;
    inputImgFormat_.memType = parameter_.inputImageFormat_memType;
    inputImgFormat_.width = parameter_.inputImageFormat_width;
    inputImgFormat_.height = parameter_.inputImageFormat_height;
    mode_ = (VACMode)parameter_.mode;
    rec_.x = parameter_.setRectangle_x;
    rec_.y = parameter_.setRectangle_y;
    rec_.width = parameter_.setRectangle_width;
    rec_.height = parameter_.setRectangle_height;
    value_ = parameter_.value;
    time_ =  parameter_.time;
    outputImgFormat_.imageType = parameter_.outputImageFormat_imageTpye;
    outputImgFormat_.memType = parameter_.outputImageFormat_memTpye;
    outputImgFormat_.imageMode = parameter_.outputImageFormat_imageMode;
    outputImgFormat_.imageQuality = parameter_.outputImageFormat_imageQuality;
   std::cout<<" inputImgFormat_.imageType = "<< inputImgFormat_.imageType
	    <<" inputImgFormat_.memType = "<<inputImgFormat_.memType
            <<" inputImgFormat_.width =  "<<inputImgFormat_.width
            <<" inputImgFormat_.height = "<<inputImgFormat_.height
            <<" mode_ ="<<mode_<<std::endl;
   
    pipeline_0 = get_tegra_pipeline_0 (  inputImgFormat_.width, inputImgFormat_.height, FPS );
    pipeline_1 = get_tegra_pipeline_1 (  inputImgFormat_.width, inputImgFormat_.height, FPS );
 
    // Create OpenCV capture object, ensure it works.
    cap_0.open(pipeline_0);
    cap_1.open(pipeline_1);

    pCommon_base->createTask(inputImgFormat_,mode_,tskHandle0_);
    pCommon_base->createTask(inputImgFormat_,mode_,tskHandle1_);
    pCommon_base->setRectangle(tskHandle0_,mode_,rec_);
    pCommon_base->setRectangle(tskHandle1_,mode_,rec_);
    pCommon_base->setSensitivity(tskHandle0_,mode_,value_);
    pCommon_base->setSensitivity(tskHandle1_,mode_,value_);
    pCommon_base->setAnalysisMinTime(tskHandle0_,mode_,time_);
    pCommon_base->setAnalysisMinTime(tskHandle1_,mode_,time_);
    pCommon_base->setOutputImageFormat(tskHandle0_,mode_,outputImgFormat_);
    pCommon_base->setOutputImageFormat(tskHandle1_,mode_,outputImgFormat_);

    getStreamThread0_ = std::thread ( std::bind ( &VAGetStream::produceImage0, this ) );
    getStreamThread1_ = std::thread ( std::bind ( &VAGetStream::produceImage1, this ) );

    analyticsStreamThread0_ = std::thread( std::bind(&VAGetStream::startDetectionThread,this));
    analyticsStreamThread1_ = std::thread( std::bind(&VAGetStream::startDetectionThread1, this));

    getStreamThread0_.join();
    getStreamThread1_.join();
    analyticsStreamThread0_.join();
    analyticsStreamThread1_.join();
}

VAGetStream::~VAGetStream()
{

}

void VAGetStream::loadFile(std::string &filename, VACParameterInit &parameter)
{
    std::fstream fp;
    fp.open(filename.c_str(),std::ios::in);
    if(!fp.is_open())
    {
        std::string error_message ="No valid input file was given,please check the given filename.";
        CV_Error(CV_StsBadArg,error_message);
    }

    while(!fp.eof())
    {
        fp >> parameter.inputImageFormat_imageType 
           >> parameter.inputImageFormat_memType 
           >> parameter.inputImageFormat_width 
           >> parameter.inputImageFormat_height 
           >> parameter.mode
           >> parameter.setRectangle_x 
           >> parameter.setRectangle_y 
           >> parameter.setRectangle_width 
           >> parameter.setRectangle_height
           >> parameter.value 
           >> parameter.time
           >> parameter.outputImageFormat_imageTpye 
           >> parameter.outputImageFormat_memTpye 
           >> parameter.outputImageFormat_imageMode 
           >> parameter.outputImageFormat_imageQuality;
    }
    fp.close();
}

void VAGetStream::produceImage0()
{
   if(!cap_0.isOpened())
    {
        std::cout << "open camera 0 fail";
        return;
    }
    VACFrame frame0;
    while ( true )
    {
       cap_0 >> frame0.frame;
       getCurrentTime();
       frame0.sec = sec_;
       frame0.usec = usec_;
       {
        g_global_mutex0_.lock();
        if(g_frameList0_.size())
         {
            g_frameList0_.clear();
         }
        g_frameList0_.push_back(frame0);
        g_global_mutex0_.unlock();
       }
       frame0.frame.release();
       usleep ( 10000 );
    }
}

void VAGetStream::produceImage1()
{
   if(!cap_1.isOpened())
    {
        std::cout << "open camera 1 fail";
        return;
    }
    VACFrame frame1;
    while ( true )
    {
      cap_1 >> frame1.frame;
       getCurrentTime();
       frame1.sec = sec_;
       frame1.usec = usec_;
       {
        g_global_mutex1_.lock();
        if(g_frameList1_.size())
         {
            g_frameList1_.clear();
         }
        g_frameList1_.push_back(frame1);
        g_global_mutex1_.unlock();
       }
       frame1.frame.release();
       usleep ( 10000 );
    }
}



void VAGetStream::getCurrentTime()
{
    gettimeofday(&tv,NULL);
    sec_  = tv.tv_sec;
    usec_ = tv.tv_usec;
}

void VAGetStream::startDetectionThread()
{
   VACFrame frame0;
   int num = 0;
    while(true)
    {
       {
         g_global_mutex0_.lock();
         if(g_frameList0_.size())
         {
             std::list<VACFrame>::iterator it;
             it = g_frameList0_.begin();
             frame0 = (*(it));
         }
         g_frameList0_.clear();
         g_global_mutex0_.unlock();
        if(!frame0.frame.empty())
        {
            num++;
            imgInfo_0.timestampSec = frame0.sec;
            imgInfo_0.timestampUsec = frame0.usec;
            imgInfo_0.pImageData = frame0.frame.data;
            imgInfo_0.imageDataLen = frame0.frame.rows*frame0.frame.cols*frame0.frame.channels();
        
            pCommon_base->inputFrameForDetection(tskHandle0_,imgInfo_0,dMeta_0);
           
            if(dMeta_0.vDetectMeta.size()>0)
            {
              for(int i =0;i<dMeta_0.vDetectMeta.size();i++)
              {  
               cv::rectangle(frame0.frame,cv::Rect(dMeta_0.vDetectMeta[i].rect.x,dMeta_0.vDetectMeta[i].rect.y,dMeta_0.vDetectMeta[i].rect.width,dMeta_0.vDetectMeta[i].rect.height),cv::Scalar(0,255,0),2,8);
                  
               }
cv::imwrite("/home/nvidia/pic-0/"+std::to_string(num)+"-result-rect.jpg",frame0.frame);
               dMeta_0.vDetectMeta.clear();
            }
            pCommon_base->detectionMetaRelease(tskHandle0_,dMeta_0);
         }
         usleep(10000);
     }
    }
  }
void VAGetStream::startDetectionThread1()
{
  VACFrame frame1;
  int num = 0;
  while(true)
   {
      {
        g_global_mutex1_.lock();
        if(g_frameList1_.size())
        {
            std::list<VACFrame>::iterator it;
            it = g_frameList1_.begin();
            frame1 = (*(it));
        }
       g_frameList1_.clear();
       g_global_mutex1_.unlock();


       if(!frame1.frame.empty())
       {
          num++;
           imgInfo_1.timestampSec = frame1.sec;
           imgInfo_1.timestampUsec = frame1.usec;
           imgInfo_1.pImageData = frame1.frame.data;
           imgInfo_1.imageDataLen = frame1.frame.rows*frame1.frame.cols*frame1.frame.channels();

           pCommon_base->inputFrameForDetection(tskHandle1_,imgInfo_1,dMeta_1);
          
           if(dMeta_1.vDetectMeta.size()>0)
            {
              for(int i =0;i<dMeta_1.vDetectMeta.size();i++)
                {
                  cv::rectangle(frame1.frame,cv::Rect(dMeta_1.vDetectMeta[i].rect.x,dMeta_1.vDetectMeta[i].rect.y,dMeta_1.vDetectMeta[i].rect.width,dMeta_1.vDetectMeta[i].rect.height),cv::Scalar(0,255,0),2,8);
                }
              cv::imwrite("/home/nvidia/pic-1/"+std::to_string(num)+"-result-rect.jpg",frame1.frame);
              dMeta_1.vDetectMeta.clear();
            }

           pCommon_base->detectionMetaRelease(tskHandle1_,dMeta_1);

       }
       usleep(10000);
   }
  }
 }

}
