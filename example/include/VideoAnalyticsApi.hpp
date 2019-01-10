#pragma once
#include<vector>
#include<string.h>

namespace VAC{
    typedef void* VACTaskHandle;
    
    struct VACRectangle
    {
        unsigned int x;
        unsigned int y;
        unsigned int width;
        unsigned int height;
        VACRectangle ()
        {
            memset(this,0,sizeof(*this));
        }
        const VACRectangle &operator=(const VACRectangle &right)
        {
            x = right. x;
            y = right. y;
            width = right. width;
            height = right. height;
            return *this;
        }
    };

    struct VACInputImageFormat{
        unsigned int imageType;
        unsigned int memType;
        unsigned int width;
        unsigned int height;
        VACInputImageFormat ()
        {
            memset(this,0,sizeof(*this));
        }
        const VACInputImageFormat &operator=(const VACInputImageFormat &right)
        {
            imageType = right.imageType;
            width= right.width;
            height = right.height;
            memType = right.memType;
            return *this;
        }
    };
    
    struct VACOutputImageFormat{
        unsigned int imageType;
        unsigned int memType;
        unsigned int imageMode;
        unsigned int imageQuality;
        unsigned int imageOutMode;
        VACOutputImageFormat ()
        {
            memset(this,0,sizeof(*this));
        }
        const VACOutputImageFormat &operator=(const VACOutputImageFormat &right)
        {
            imageType = right.imageType;
            imageMode = right. imageMode;
            imageQuality = right. imageQuality;
            memType = right.memType;
            imageOutMode = right.imageOutMode;
            return *this;
        }
    };
    
    struct VACInputImageInfo{
        unsigned int timestampSec;
        unsigned int timestampUsec;
        void *pImageData;//gpu uint32*
        unsigned int imageDataLen;
        unsigned int imageStep;
        void *pPrivateData;
        unsigned int privateDataLen;
        VACInputImageInfo ()
        {
            timestampSec = 0;
            timestampUsec = 0;
            pImageData = NULL;
            imageDataLen = 0;
            imageStep = 0;
            pPrivateData = NULL;
            privateDataLen = 0;
        }
    };
    struct VACDetectMetaData {
        unsigned int detectAlarmType;
        unsigned int dID;
        VACRectangle rect;
        unsigned char *pImageData;
        unsigned int iImageDataLen;
        unsigned char *pPrivateData;
        unsigned int privateDataLen;
        VACDetectMetaData()
        {
            detectAlarmType = 0;
            dID = 0;
            pImageData = NULL; 
            iImageDataLen = 0;
            pPrivateData = NULL;
            privateDataLen = 0;

        }
    };
    
    struct VACDetectionMeta{
        unsigned int timestampSec;
        unsigned int timestampUsec;
        std::vector<VACDetectMetaData> vDetectMeta;
        VACDetectionMeta()
        {
            timestampSec = 0;
            timestampUsec = 0;
        }
    };
    
    struct VACRecogMetaData{
        unsigned int recogAlarmType;
        unsigned char *pPrivateData;
        unsigned int privateDataLen;
        VACRecogMetaData ()
        {
            recogAlarmType = 0;
            pPrivateData = NULL;
            privateDataLen = 0;
        }
    };
    
    struct VACRecogMeta{
        int recogAlarmType;
        unsigned int timestampSec;
        unsigned int timestampUsec;
        std::vector<VACRecogMetaData> vRecogMeta;
    };
    
    enum VACMessage{
        VACOMMON_OK = 0,
        VACOMMON_ERROR,
        VACOMMON_CREATE_TSK_HANDLE_ERROR = 50,
        VACOMMON_TSK_HANDLE_NULL,
        VACOMMON_CREATE_THREAD_ERROR = 100,
        VACOMMON_THREAD_ERROR,
        VACOMMON_CREATE_MEMORY_ERROR = 150,
        VACOMMON_NO_MEMORY,
        VACOMMON_MEMORY_FULL_ERROR,
        VACOMMON_INPARAM_ERROR=200,
        VACOMMON_MODE_NOT_SUPPORT
    };
    
    enum VACEventType{
        VACOMMON_EVENT_TYPE_DETECTION_ALARM = 1,
        VACOMMON_EVENT_TYPE_RECOGNITION_ALARM,
        VACOMMON_EVENT_TYPE_ERROR
    };
    
    enum VACDetectAlarmType{
        VACOMMON_ALARM_HD = 1,
        VACOMMON_ALARM_FD,
        VACOMMON_ALARM_CD,
        VACOMMON_ALARM_IPD,
        VACOMMON_ALARM_FSD,
        VACOMMON_ALARM_LLD,
        VACOMMON_ALARM_NGLD,
        VACOMMON_ALARM_WCD
    };
    
    enum VACRecogAlarmType{
        VACOMMON_ALARM_FR = 1,
        VACOMMON_ALARM_IPR
    };
    
    enum VACMode{
        VACOMMON_MODE_HD = 0x01,
        VACOMMON_MODE_CD = 0x02,
        VACOMMON_MODE_FSD= 0x04,
        VACOMMON_MODE_LLD = 0x08,
        VACOMMON_MODE_NGLD = 0x10,
        VACOMMON_MODE_WCD = 0x20,
        VACOMMON_MODE_FD = 0x40,
        VACOMMON_MODE_IPD = 0x80,
        VACOMMON_MODE_FR = 0x100,
        VACOMMON_MODE_IPR = 0x200
    };
    enum VACSens{
        VACOMMON_SENS_LOW = 1,
        VACOMMON_SENS_NORMAL,
        VACOMMON_SENS_HIGH
    };
    enum VACImageQuality{
        VACOMMON_IMAGE_QUALITY_LOW = 1,
        VACOMMON_IMAGE_QUALITY_NORMAL,
        VACOMMON_IMAGE_QUALITY_HIGH
    };
    
    enum VACImageMode{
        VACOMMON_IMAGE_MODE_ORIGN = 1,
        VACOMMON_IMAGE_MODE_TARGET,
        VACOMMON_IMAGE_MODE_NONE
    };
    
    enum VACImageType{
        VACOMMON_IMAGE_TYPE_YUV420 = 1,
        VACOMMON_IMAGE_TYPE_RGB888,
        VACOMMON_IMAGE_TYPE_JPEG,
        VACOMMON_IMAGE_TYPE_BGR888,
        VACOMMON_IMAGE_TYPE_RGBA8888,
        VACOMMON_IMAGE_TYPE_NONE=500
    };

    enum VACMemType{
        VACOMMON_MEM_TYPE_CPU = 1,
        VACOMMON_MEM_TYPE_GPU
    };

    enum VACImageOutMode{
        VACOMMON_IMAGE_OUT_MODE_ONCE = 1,
        VACOMMON_IMAGE_OUT_MODE_CONTINUOUS
    };
    
    class VACommonBase{
    public:
        VACommonBase(char *strLicense);
        ~VACommonBase();
        int getVersion(char *strVersion);
        int createTask(VACInputImageFormat imgFormat, VACMode mode, VACTaskHandle &tskHandle);
        int destroyTask(VACTaskHandle tskHandle);
        int setRectangle(VACTaskHandle tskHandle, VACMode mode, VACRectangle rec);
        int getRectangle(VACTaskHandle tskHandle, VACMode mode, VACRectangle &rec);
        int setSensitivity(VACTaskHandle tskHandle, VACMode mode, int value);
        int getSensitivity(VACTaskHandle tskHandle, VACMode mode, int &value);
        int setAnalysisMinTime(VACTaskHandle tskHandle, VACMode mode, int time);
        int getAnalytisisMinTime(VACTaskHandle tskHandle, VACMode mode, int &time);
        int setOutputImageFormat(VACTaskHandle tskHandle, VACMode mode, VACOutputImageFormat imgFormat);
        int getOutputImageFormat(VACTaskHandle tskHandle, VACMode mode, VACOutputImageFormat &imgFormat);
        int inputFrameForDetection(VACTaskHandle tskHandle, VACInputImageInfo imgInfo,VACDetectionMeta &dMeta);
        int inputFrameForRecognition(VACTaskHandle tskHandle, VACInputImageInfo imgInfo,VACRecogMeta &rMeta);
        int detectionMetaRelease(VACTaskHandle tskHandle, VACDetectionMeta& dMeta);
        int recognitionMetaRelease(VACTaskHandle tskHandle, VACRecogMeta& rMeta);
    };
}
