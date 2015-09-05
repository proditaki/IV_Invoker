#include "main.h"


SYS_MODULE_INFO( GTA_IV_Invoker, 0, 1, 1);
SYS_MODULE_START( _GTA_IV_Invoker_prx_entry );

SYS_LIB_DECLARE_WITH_STUB( LIBNAME, SYS_LIB_AUTO_EXPORT, STUBNAME );
SYS_LIB_EXPORT( _GTA_IV_Invoker_export_function, LIBNAME );

sys_ppu_thread_t	g_MainThreadID;

struct NativeArg g_NativeArg;
unsigned int g_pArgs[16];

unsigned int GetNativeAddressFromHash(unsigned int a_iNativehash)
{
	unsigned int l_uiHashTablePointer = 0x17D1438;
	unsigned int l_uiModulator = *(unsigned int*)0x17D143C;
	unsigned int result;

	int l_iIndexer = a_iNativehash % l_uiModulator;
	unsigned int l_uiHashTableAddress = *(unsigned int *)l_uiHashTablePointer;
	unsigned int l_uiNatveHash = a_iNativehash;
	for (unsigned int i = *(unsigned int *)(l_uiHashTableAddress + 8 * l_iIndexer); i > 1; i = *(unsigned int *)(l_uiHashTableAddress + 8 * l_iIndexer))
	{
		if (i == a_iNativehash)
			break;
		l_uiNatveHash = (l_uiNatveHash >> 1) + 1;
		l_iIndexer = (l_uiNatveHash + l_iIndexer) % l_uiModulator;
	}
	if (*(unsigned int *)(l_uiHashTableAddress + 8 * l_iIndexer) <= 1 || (result = *(unsigned int *)(l_uiHashTableAddress + 8 * l_iIndexer + 4)) == 0)
	{
		if (l_uiModulator)
		{
			unsigned int l_uiModulator2 = *(unsigned int *)(l_uiModulator + 4);
			if (l_uiModulator2)
			{
				l_iIndexer = a_iNativehash % l_uiModulator2;
				int l_iIndexer2 = *(unsigned int *)l_uiModulator;
				l_uiNatveHash = a_iNativehash;
				for (unsigned int j = *(unsigned int *)(*(unsigned int *)l_uiModulator + 8 * (a_iNativehash % l_uiModulator2)); j > 1; j = *(unsigned int *)(l_iIndexer2 + 8 * l_iIndexer))
				{
					if (j == a_iNativehash)
						break;
					l_uiNatveHash = (l_uiNatveHash >> 1) + 1;
					l_iIndexer = (l_uiNatveHash + l_iIndexer) % l_uiModulator2;
				}
			}
		}
		//printf("Native function 0x%08x not found.", a_iNativehash);
		result = 0;
	}
	//printf("hash: 0x%08x address: 0x%08x\n", a_iNativehash, result);
	return result;
}

unsigned int GetNativeAddresFromName(char* a_sNative)
{
	if (strlen(a_sNative) > 0)
	{
		return GetNativeAddressFromHash(CreateHash(a_sNative));
	}

}

template<typename T>
T Invoke(char* a_sNative, unsigned int a_uiArgCount, ...)
{
	T* Result;
	va_list l_VaList;
	unsigned int l_uiIndex;
	unsigned int l_uiReadCount = 0;

	//printf("Native name: %s\n", a_sNative);
	unsigned int l_uiNativeAddress = GetNativeAddresFromName(a_sNative);

	//printf("Native Hash: 0x%08x\n", CreateHash(a_sNative));

	if (l_uiNativeAddress > 0)
	{
		va_start(l_VaList, a_uiArgCount);
		for (l_uiIndex = 0; l_uiIndex < 16; l_uiIndex++)
		{
			if (l_uiIndex < a_uiArgCount)
				g_pArgs[l_uiIndex] = va_arg(l_VaList, unsigned int);
			else
				g_pArgs[l_uiIndex] = 0;
		}
		va_end(l_VaList);

		((void(*)(struct NativeArg*))l_uiNativeAddress)(&g_NativeArg);

		Result = (T*)&g_pArgs[l_uiReadCount];

		l_uiReadCount += sizeof(T);
		if ((sizeof(T) % 4) != 0)
			l_uiReadCount += 4 - (sizeof(T) % 4);

		return *(T*)Result;
	}
	return 0;
}

int float_int(float f)
{
	return *((int*)&f);
}

unsigned int CreateHash(char* Native)
{
	unsigned int joaat = 0xA39BD0;
	return ((unsigned int(*)(char* native))&joaat)(Native);
}

void MainThread(uint64_t)
{
	g_NativeArg.p_uiArgValues = g_pArgs;
	g_NativeArg.p_uiReturnValues = g_pArgs;
	g_NativeArg.uiArgCount = 0;

	int lastCounter = 0, lastvalue = 0;
	sys_timer_sleep(140);
	while(true)
	{
		/*
		if (lastCounter != *((int*)0x17D1444) || lastvalue != *((int*)0x17D143C))
		{
			lastCounter = *((int*)0x17D1444);
			lastvalue = *((int*)0x17D143C);
			printf("Hash Table: %d NativeCounter: %d 0x17D143C: %d\n", *((int*)0x17D1438), *((int*)0x17D1444), *((int*)0x17D143C));
		}
		else if (*(unsigned int*)0x10020000 != 0)
		{
			GetNativeAddressFromHash(*(unsigned int*)0x10020000);
			*(unsigned int*)0x10020000 = 0;
		}
		*/
		DRAW_RECT(0.5, 0.52, 0.5, 0.2, 255, 100, 50, 150);
		//Invoke<int>("DRAW_RECT", 8, float_int(0.5), float_int(0.52), float_int(0.5), float_int(0.2), 255, 100, 50, 150);
		//printf("player online: %d", Invoke<int>(0xF49F20, 0));
		sys_timer_usleep(20000);
	}
}

void DRAW_RECT(float x, float y, float xsize, float ysize, int R, int G, int B, int A)
{
	Invoke<int>("DRAW_RECT", 8, float_int(x), float_int(y), float_int(xsize), float_int(ysize), R, G, B, A);
}

// An exported function is needed to generate the project's PRX stub export library
extern "C" int _GTA_IV_Invoker_export_function(void)
{
    return CELL_OK;
}

extern "C" int _GTA_IV_Invoker_prx_entry(void)
{
	if (sys_ppu_thread_create(&g_MainThreadID, MainThread, 0, 1000, 2048, 0, "Proditaki GTA V Hook") != CELL_OK)
	{
		printf("Unable to create the Thread !");
	}
    return SYS_PRX_RESIDENT;
}
