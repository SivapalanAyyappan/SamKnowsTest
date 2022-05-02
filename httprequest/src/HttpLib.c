/******************************************************************************
* File Name		: HttpLib.c
* Description  	: Implements routines to Get curl requests
                  and find the medians of various time
* Author  		: Siva
* Created on	: 02 May 2022
* Related files	:
*------------------------------------------------------------------------------
*			Revision History		
* Version		Date				Modified by			Change
*------------------------------------------------------------------------------
* 0.1			02 May 2022		    Siva				created
******************************************************************************/

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "statscommon.h"

/* MACRO Definitions */
#define TEST_URL "http://www.google.com/"

/* Global and static variables */
static struct curl_slist *psHandle = NULL;

/* Functions Definitions */

/*-----------------------------------------------------------------------------
Purpose : This function is to sort the Array in ascending order
Inputs  : nNumCount  - number of elements in the array
Outputs : dTimeArray - Sorted array
Returns : None
Notes   : 
-----------------------------------------------------------------------------*/
void SortTime(double *dTimeArray , int nNumCount)
{
    int nRowIndex    = 0;
    int nColIndex    = 0;
    double nTemp     = 0.0;

    for( nRowIndex = 0; nRowIndex < nNumCount; nRowIndex++)
    {
        for( nColIndex = 0 ; nColIndex < nNumCount-1 ; nColIndex++)
        {
            if( dTimeArray[nColIndex] > dTimeArray[nColIndex+1])
            {
                nTemp                    = dTimeArray[nColIndex];
                dTimeArray[nColIndex]    = dTimeArray[nColIndex+1];
                dTimeArray[nColIndex+1]  = nTemp;
            }
        }
    }
}

/*-----------------------------------------------------------------------------
Purpose : This function is used to find the median
Inputs  : dTimeArray  - The Array with sorted elements
          nNumCount   - number of elements in the array
Outputs : None
Returns : sTimeMedian - The median of time
Notes   : 
-----------------------------------------------------------------------------*/
double FindMedian(double dTimeArray[] , int nNumCount)
{
    double sTimeMedian = 0.0;
    
    /* for even number of elements */
    if( nNumCount%2 == 0 )
    {
        sTimeMedian = (dTimeArray[(nNumCount-1)/2] + dTimeArray[nNumCount/2])/2.0;
    }
    /* for odd number of elements */
    else
    {
        sTimeMedian = dTimeArray[nNumCount/2];
    }
    
    return sTimeMedian;
}

/*-----------------------------------------------------------------------------
Purpose : This function is used to set the header
Inputs  : pcHeader  - header to set
Outputs : None
Returns : None
Notes   : 
-----------------------------------------------------------------------------*/
void SetHeader(char* pcHeader)
{
    psHandle = curl_slist_append(psHandle, pcHeader);
}

/*-----------------------------------------------------------------------------
Purpose : This function is used to set the header
Inputs  : nNumReqs  - requested counter
Outputs : psStats   - Structure with medians of all requested Time
Returns : None
Notes   : 
-----------------------------------------------------------------------------*/
int GetHttpStats(int nNumReqs, sHttpStats* psStats)
{
    CURL *pCurl                = NULL;
    CURLcode nRetValue         = 0 ;
    int nLenReq                = 0;
    long lnResponseCode        = 0;
    int nReqCount              = 0;
    char* pIpAddress           = NULL;
    double* dNameLookupTime    = NULL;
    double* dConnectTime       = NULL;
    double* dStartTransferTime = NULL;
    double* dTotalTime         = NULL;
    
    /* Size required by each array */
    nLenReq = nNumReqs * sizeof(double);

    /* Allocate required memory*/
    dNameLookupTime    = (double*) malloc(nLenReq);
    dConnectTime       = (double*) malloc(nLenReq);
    dStartTransferTime = (double*) malloc(nLenReq);
    dTotalTime         = (double*) malloc(nLenReq);

    /* Initialize to 0 */
    memset(dNameLookupTime, 0.0, nLenReq);
    memset(dConnectTime, 0.0, nLenReq);
    memset(dStartTransferTime, 0.0, nLenReq);
    memset(dTotalTime, 0.0, nLenReq);

    /* Init curl */
    pCurl = curl_easy_init();
    if(!pCurl)
    {
        printf("ERROR! Curl init Failed!\n");
        return STATS_FAILURE;      
    }
    else
    {
        /* Set curl options */
        curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, psHandle);
        curl_easy_setopt(pCurl, CURLOPT_URL, TEST_URL);
        curl_easy_setopt(pCurl, CURLOPT_NOBODY, 1);
        /* Execute curl perform request for nNumReqs times */
        for (nReqCount = 0; nReqCount < nNumReqs; nReqCount++)
        {
            /* Perform the Curl request */
            nRetValue = curl_easy_perform(pCurl);
            if(nRetValue != CURLE_OK)
            {
                printf("ERROR! Failed to perfom Curl request : %s\n", curl_easy_strerror(nRetValue));
                return STATS_FAILURE;
            }
            else
            {
                /* get Response Code */
                nRetValue = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &lnResponseCode);
                if(nRetValue != CURLE_OK)
                {
                    printf("ERROR! Failed to get Response code!\n");
                    return STATS_FAILURE;
                }
                /* Get Ip Address */
                nRetValue = curl_easy_getinfo(pCurl, CURLINFO_PRIMARY_IP, &pIpAddress);
                if(nRetValue != CURLE_OK)
                {
                    printf("ERROR! Failed to get IP Address!\n");
                    return STATS_FAILURE;
                }
                /* Get NameLookup Time */
                nRetValue = curl_easy_getinfo(pCurl, CURLINFO_NAMELOOKUP_TIME, &(dNameLookupTime[nReqCount]));
                if(nRetValue != CURLE_OK)
                {
                    printf("ERROR! Failed to get Namelookup time!\n");
                    return STATS_FAILURE;
                }
                /* Get Connect Time */
                nRetValue = curl_easy_getinfo(pCurl, CURLINFO_CONNECT_TIME, &(dConnectTime[nReqCount]));
                if(nRetValue != CURLE_OK)
                {
                    printf("ERROR! Failed to get Connect time!\n");
                    return STATS_FAILURE;
                }
                /* Get Start Time*/
                nRetValue = curl_easy_getinfo(pCurl, CURLINFO_STARTTRANSFER_TIME, &(dStartTransferTime[nReqCount]));
                if(nRetValue != CURLE_OK)
                {
                    printf("ERROR! Failed to get Start Transfer time!\n");
                    return STATS_FAILURE;
                }
                /* Get Total Time */
                nRetValue = curl_easy_getinfo(pCurl, CURLINFO_TOTAL_TIME, &(dTotalTime[nReqCount]));
                if(nRetValue != CURLE_OK)
                {
                    printf("ERROR! Failed to get Total time!\n");
                    return STATS_FAILURE;
                }
            }
        }

        /* Cleanup curl handles */
        curl_easy_cleanup(pCurl);
        curl_slist_free_all(psHandle);
    }

    /* Fill the structure with values */
    memcpy(psStats->cIpAddress, pIpAddress, IP_ADDR_LENGTH);
    psStats->lnResponseCode        = lnResponseCode;
    
    SortTime(dNameLookupTime, nNumReqs);
    psStats->dNamelookupTimeMedian = FindMedian(dNameLookupTime, nNumReqs);
    
    SortTime(dConnectTime, nNumReqs);
    psStats->dConnectTimeMedian    = FindMedian(dConnectTime, nNumReqs);
    
    SortTime(dStartTransferTime, nNumReqs);
    psStats->dStartTimeMedian      = FindMedian(dStartTransferTime, nNumReqs);
    
    SortTime(dTotalTime, nNumReqs);
    psStats->dTotalTimeMedian      = FindMedian(dTotalTime, nNumReqs);

    /* Free memory */
    free(dNameLookupTime);
    free(dConnectTime);
    free(dStartTransferTime); 
    free(dTotalTime);
    dNameLookupTime = NULL;
    dConnectTime    = NULL;
    dStartTransferTime      = NULL;
    dTotalTime      = NULL;

    return STATS_SUCCESS;
}
