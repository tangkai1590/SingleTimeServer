#include "stdafx.h"
#include "NtpProtocl.h"
#include <iostream>  
#include <time.h>
#include <sys/timeb.h>  
#include <windows.h>
#ifndef ULONG64
#define unsigned long long ULONG64
#endif



ULONG64  hl64ton(ULONG64   host)
{
	ULONG64   ret = 0;
	ULONG   high, low;

	low = host & 0xFFFFFFFF;
	high = (host >> 32) & 0xFFFFFFFF;
	low = htonl(low);
	high = htonl(high);
	ret = low;
	ret <<= 32;
	ret |= high;
	return   ret;
}

NetworkUInt64 NtpProtocl::ms_n64ReferenceTimestamp;
NtpProtocl::NtpProtocl()
{
}


NtpProtocl::~NtpProtocl()
{
}

void NtpProtocl::GetNTPReplyPacket(NTPPacket* pPacket)
{
	pPacket->cwd = 0;
	pPacket->cwd = pPacket->cwd << 2;
	pPacket->cwd += 3;
	pPacket->cwd = pPacket->cwd << 3;
	pPacket->cwd += 4;

	pPacket->stratum = 0;
	pPacket->poll = 0;
	pPacket->precision = 0;

	pPacket->root_delay = 0;
	pPacket->root_dispersion = 0;

	pPacket->reference_identifier = 0x01010101;
	pPacket->reference_timestamp = ms_n64ReferenceTimestamp;
	pPacket->originate_timestamp = pPacket->transmit_timestamp;
	
	__time64_t curTime = 0;
	curTime  += n1970_1900_Seconds;

	long long time_last;
	time_last = time(NULL);     //总秒数  

	curTime = n1970_1900_Seconds + time_last;
	curTime = curTime << 32;

	FILETIME ft;
	LARGE_INTEGER li;
	int64_t tt = 0;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	// 从1601年1月1日0:0:0:000到1970年1月1日0:0:0:000的时间(单位100ns)
	#define EPOCHFILETIME   (116444736000000000UL)
	// 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
	tt = (li.QuadPart - EPOCHFILETIME) / 10;
	tt -= time_last * 1000 * 1000;
	tt *= 4294.967296;
	curTime += tt;
	pPacket->transmit_timestamp = curTime;
	pPacket->receive_timestamp = curTime;
	
}

int NtpProtocl::CheckPacket(NTPPacket* pPacket, int nLen)
{
	if (nLen == 0x30
		&& (pPacket->cwd&0x38) ==0x18)
	{
		return 3;
	}

	if (nLen == 0x30
		&& (pPacket->cwd & 0x38) == 0x30)
	{
		return 4;
	}
	return 0;
}

void NtpProtocl::GetNTPRequestPacket(NTPPacket* pPacket)
{
	pPacket->cwd = 0;
	pPacket->cwd = pPacket->cwd << 2;
	pPacket->cwd += 3;
	pPacket->cwd = pPacket->cwd << 3;
	pPacket->cwd += 3;

	pPacket->stratum = 0;
	pPacket->poll = 0;
	pPacket->precision = 0;

	pPacket->root_delay = 0;
	pPacket->root_dispersion = 0;
\
	__time64_t curTime = 0;
	curTime += n1970_1900_Seconds;

	long long time_last;
	time_last = time(NULL);     //总秒数  

	curTime = n1970_1900_Seconds + time_last;
	curTime = curTime << 32;

	FILETIME ft;
	LARGE_INTEGER li;
	int64_t tt = 0;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	// 从1601年1月1日0:0:0:000到1970年1月1日0:0:0:000的时间(单位100ns)
#define EPOCHFILETIME   (116444736000000000UL)
	// 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
	tt = (li.QuadPart - EPOCHFILETIME) / 10;
	tt -= time_last * 1000 * 1000;
	tt *= 4294.967296;
	curTime += tt;
	pPacket->transmit_timestamp = curTime;
}

void NtpProtocl::RecvNtpServerPacket(NTPPacket* pPacket)
{

}
