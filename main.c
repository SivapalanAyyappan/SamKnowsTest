/******************************************************************************
* File Name		: main.c
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

/* Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "statscommon.h"

/* Macro Definition */
#define MAX_HEADER_LEN 32

/* Functions Definitions */

/*-----------------------------------------------------------------------------
Purpose : This is the function to print the usage for the users.
Inputs  : None
Outputs : None
Returns : None
Notes   : 
-----------------------------------------------------------------------------*/
void PrintUsage()
{
    printf("Usage Menu:\n");
    printf("--help : To print help menu.\n");
    printf("-H \"Header-name: Header-value\" : Can be used multiple times, each time specifying an extra HTTP header to add to your request.\n");
    printf("-n <integer> : Number of HTTP requests to make (i.e. the number of samples you will have to take the median of);\n");
}

/*-----------------------------------------------------------------------------
Purpose : This is the main control function.
Inputs  : argc  - number of arguments
          argv  - the list of arguments
Outputs : None
Returns : None
Notes   : Minimum 3 valid arguments required
-----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
    int nArgLen          = 0;
    int nNumReqs         = 0;
    int nRetValue        = 0;
    sHttpStats *psStats  = NULL;
    
    /* Minimum 3 arguments required */
    if (argc < 3) 
    {
        printf("Minimum 3 arguments required\n");
        PrintUsage();
        exit(EXIT_FAILURE);
    }

    /* Read command line arguments */
    for ( nArgLen = 1; nArgLen < argc; nArgLen++)
    {
        nRetValue = strcmp("-n", argv[nArgLen]);
        if(nRetValue == 0)
        {
            /* Check whether a value is passed with -n option
            ** Also ensure that the value is not 0 or negative*/
            if (argv[nArgLen+1] && atoi(argv[nArgLen+1]) > 0)
            {
                nNumReqs = atoi(argv[nArgLen+1]);
            }
            else
            {
                printf("Invalid Arguments, Please use 1 or above with -n option!\n");
                PrintUsage();
                return -1;
            }
        }

        nRetValue = strcmp("-H",argv[nArgLen]);
        if(nRetValue == 0)
        {
            /* check whether a valid Header is passed with -H
            **Also the maximum Header length is limited to 32 characters*/
            if ( (argv[nArgLen+1]) && (strlen (argv[nArgLen+1]) < MAX_HEADER_LEN))
            {
                SetHeader(argv[nArgLen+1]);
            }
            else
            {
                printf("Input a valid header, minimum : 1 maximum : 32 characters!\n");
                PrintUsage();
                return -1;
            }
        }
        /* Print help menu */
        nRetValue = strcmp("--help", argv[nArgLen]);
        if(nRetValue == 0)
        {
            PrintUsage();
            return 0;
        }
    }
    if (nNumReqs == 0)
    {
        printf("-n option is mandatory\n");
        PrintUsage();
        return -1;
    }

    psStats = (sHttpStats*)malloc(sizeof(sHttpStats));
    if(psStats == NULL)
    {
        printf("ERROR! Failed to allocate memory\n");
        return -1;
    }

    /* Get http statistics */
    nRetValue = GetHttpStats(nNumReqs, psStats);
    if(nRetValue != STATS_SUCCESS)
    {
        printf("ERROR! Failed to get http statistics\n");
        return -1;
    }
    else
    {
        printf("\nSKTEST;%s;%ld;%lf;%lf;%lf;%lf\n", psStats->cIpAddress, psStats->lnResponseCode, 
        psStats->dNamelookupTimeMedian, psStats->dConnectTimeMedian, psStats->dStartTimeMedian, psStats->dTotalTimeMedian);
    }

    /* Cleanup */
    free(psStats);
    psStats = NULL;

    return 0;
}
