//
// Created by hecong on 19-5-6.
//
#include "product.h"
#include <fstream>

#define FILE_BUFFER_LENGTH 30000

bool timeflag;    //超时标志
void  sig_handler(int num)
{
    //printf("time out .\n");
    timeflag=false;
}

