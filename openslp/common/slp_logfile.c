/***************************************************************************/
/*                                                                         */
/* Project:     OpenSLP - OpenSource implementation of Service Location    */
/*              Protocol Version 2                                         */
/*                                                                         */
/* File:        slp_logfile.h                                              */
/*                                                                         */
/* Abstract:    Header file that defines structures and constants that are */
/*              specific to the SLP log file.                              */
/*                                                                         */
/*-------------------------------------------------------------------------*/
/*                                                                         */
/*     Please submit patches to http://www.openslp.org                     */
/*                                                                         */
/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Copyright (C) 2000 Caldera Systems, Inc                                 */
/* All rights reserved.                                                    */
/*                                                                         */
/* Redistribution and use in source and binary forms, with or without      */
/* modification, are permitted provided that the following conditions are  */
/* met:                                                                    */ 
/*                                                                         */
/*      Redistributions of source code must retain the above copyright     */
/*      notice, this list of conditions and the following disclaimer.      */
/*                                                                         */
/*      Redistributions in binary form must reproduce the above copyright  */
/*      notice, this list of conditions and the following disclaimer in    */
/*      the documentation and/or other materials provided with the         */
/*      distribution.                                                      */
/*                                                                         */
/*      Neither the name of Caldera Systems nor the names of its           */
/*      contributors may be used to endorse or promote products derived    */
/*      from this software without specific prior written permission.      */
/*                                                                         */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     */
/* `AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT      */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR   */
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CALDERA      */
/* SYSTEMS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,  */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON       */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    */
/*                                                                         */
/***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#include <slp_logfile.h>

/*=========================================================================*/
static FILE*   G_LogFile    = 0;
/*=========================================================================*/


/********************************************/
/* TODO: Make these functions thread safe!! */
/********************************************/


/*=========================================================================*/
int SLPLogFileOpen(const char* path, int append)                           
/* Prepares the file at the specified path as the log file.                */
/*                                                                         */
/* path     - (IN) the path to the log file. If path is the empty string   */
/*            (""), then we log to stdout.                                 */
/*                                                                         */
/* append   - (IN) if zero log file will be truncated.                     */
/*                                                                         */
/* Returns  - zero on success. errno on failure.                           */
/*=========================================================================*/
{
    if(G_LogFile)
    {
        /* logfile was already open close it */
        fclose(G_LogFile);
    }

    if(*path == 0)
    {
        /* Log to console. */
        G_LogFile = stdout;
    }
    else
    {
        /* Log to file. */
        if(append)
        {
            G_LogFile = fopen(path,"a");
        }
        else
        {
            G_LogFile = fopen(path,"w");
        }

        if(G_LogFile == 0)
        {
            /* could not open the log file */
            return -1;
        }
    }

    return 0;
}


/*=========================================================================*/
int SLPLogFileClose()
/* Releases resources associated with the log file                         */
/*=========================================================================*/
{
    fclose(G_LogFile);

    return 0;
}


/*=========================================================================*/
void SLPLog(const char* msg, ...)
/* Logs a message                                                          */
/*=========================================================================*/
{
    va_list ap;

    if(G_LogFile)
    {
        va_start(ap,msg);
        vfprintf(G_LogFile,msg,ap); 
        va_end(ap);
        fflush(G_LogFile);
    }
}


/*=========================================================================*/
void SLPFatal(const char* msg, ...)
/* Logs a message and halts the process                                    */
/*=========================================================================*/
{
    va_list ap;

    if(G_LogFile)
    {
        fprintf(G_LogFile,"A FATAL Error has occured:\n");
        va_start(ap,msg);
        vfprintf(G_LogFile,msg,ap); 
        va_end(ap);
        fflush(G_LogFile);
    }
    else
    {
        printf("A FATAL Error has occured:\n");
        va_start(ap,msg);
        vprintf(msg,ap);
        va_end(ap);
    }

    exit(1);
}

/*=========================================================================*/
void SLPLogBuffer(const char* buf, int bufsize)
/* Writes a buffer to the logfile                                          */
/*=========================================================================*/
{
    if(G_LogFile)
    {
        fwrite(buf,bufsize,1,G_LogFile);
        fflush(G_LogFile);
    }
}

