#ifndef A_TYPE_H
#define A_TYPE_H

#include <stdio.h>
// anqueti Type API
typedef char 			int8;
typedef short 		int16;
typedef int 			int32;
//typedef double  		double;
//typedef float   		float;
//typedef void 			void;
typedef long			long32;

typedef unsigned char 	uint8;
typedef unsigned short 	uint16;
typedef unsigned int 	uint32;

//typedef size_t size_t;
//typedef struct timespec timespec;

#ifndef FALSE
#define FALSE               0
#endif	// FALSE

#ifndef TRUE
#define TRUE                1
#endif	// TRUE

// TODO: simple LOG system, need implement it later
#ifdef _AQTDUMPLOG
#define AQTLOGE(fmt, ...) \
{ \
	time_t rawtime; \
       time(&rawtime); \
       char   pblgtime[20]; \
    	strftime(pblgtime, 20, "%Y-%m-%d %X", localtime(&rawtime)); \
	printf("AErr: %s,%s[%d],  " fmt "\n", pblgtime,(char *)__FUNCTION__, (int)__LINE__, ##__VA_ARGS__); \
}

#define AQTLOGW(fmt, ...) \
{ \
	time_t rawtime; \
       time(&rawtime); \
       char   pblgtime[20]; \
    	strftime(pblgtime, 20, "%Y-%m-%d %X", localtime(&rawtime)); \
	printf("AWarn: %s,%s[%d],  " fmt "\n", pblgtime, (char *)__FUNCTION__, (int)__LINE__, ##__VA_ARGS__); \
}

#define AQTLOGI(fmt, ...) \
{ \
	time_t rawtime; \
       time(&rawtime); \
       char   pblgtime[20]; \
    	strftime(pblgtime, 20, "%Y-%m-%d %X", localtime(&rawtime)); \
	printf("AInfo: %s,%s[%d],  " fmt "\n", pblgtime, (char *)__FUNCTION__, (int)__LINE__, ##__VA_ARGS__); \
}
#else	// _AQTDUMPLOG
#define AQTLOGW(fmt, ...)
#define AQTLOGI(fmt, ...)
#define AQTLOGE(fmt, ...)
#endif	// _AQTDUMPLOG

#endif //A_TYPE_H

