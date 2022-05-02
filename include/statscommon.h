/******************************************************************************
* File Name		: statscommon.h
* Description  	: Implements routines to invoke httpStatsRequest
* Author  		: Siva
* Created on	: 02 May 2022
* Related files	:
*------------------------------------------------------------------------------
*			Revision History		
* Version		Date				Modified by			Change
*------------------------------------------------------------------------------
* 0.1			02 May 2022		    Siva				created
******************************************************************************/

#ifndef STATS_COMMON_H
#define STATS_COMMON_H

/* Macro Definitions */
#define STATS_SUCCESS 1
#define STATS_FAILURE 0

#define IP_ADDR_LENGTH 16

/* data structures */
typedef struct HttpStats
{
    long lnResponseCode;
    char cIpAddress[IP_ADDR_LENGTH];
    double dNamelookupTimeMedian;
    double dConnectTimeMedian;
    double dStartTimeMedian;
    double dTotalTimeMedian;
}sHttpStats;

/* Function Prototypes */
/* Public */
int GetHttpStats (int nNumReqs, sHttpStats* sStats);
void SetHeader (char* cHeader);

/* Private */
void SortTime(double *dTimeArray , int nNumCount);
double Find_median(double dTimeArray[] , int nNumCount);

#endif
