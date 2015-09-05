#ifndef __MAIN_H__
#define __MAIN_H__

#include <cellstatus.h>
#include <sys/prx.h>

#include <sys/ppu_thread.h>


#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <sys/memory.h>
#include <sys/timer.h>
#include <vector>
#include <ctype.h>

//void* malloc(uint32_t a_uiSize);
//void free(void* a_Addr);
//
//static void* malloc(uint32_t a_uiSize) { return std::malloc(a_uiSize); }
//static void free(void* a_Addr) { std::free(a_Addr); }

struct Vector3 { float x, y, z; };
int float_int(float f);
unsigned int CreateHash(char* Native);

struct NativeArg
{
	unsigned int*    p_uiReturnValues;
	unsigned int    uiArgCount;
	unsigned int*    p_uiArgValues;
};

unsigned int GetNativeAddresFromName(char* a_sNative);
unsigned int GetNativeAddressFromHash(unsigned int a_iNativehash);
unsigned int CreateHash(char* Native);

void DRAW_RECT(float x, float y, float xsize, float ysize, int R, int G, int B, int A);

template<typename T>
T Invoke(char* a_sNative, unsigned int a_uiArgCount, ...);
#endif