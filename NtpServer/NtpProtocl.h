#pragma once

#define NTP_PCK_LEN 48
#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6

#define JAN_1970		0x83aa7e80 /* 1900年～1970年之间的时间秒数 */
#define NTPFRAC(x)     (4294 * (x) + ((1981 * (x)) >> 11))
#define USEC(x)         (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))

class NetworkUInt64
{
public:
	operator UINT64()
	{
		return htonll(nData);
	}

	const NetworkUInt64& operator = (UINT64 nValue)
	{
		nData = htonll(nValue);
		return *this;
	}
protected:
	UINT64  nData;
};

const static ULONGLONG n1970_1900_Seconds = 0x83aa7e80;
#define JAN_1970 0x83aa7e80 /* 1900年～1970年之间的时间秒数 */

#pragma pack(push,1)
struct NTPPacket
{
	//header 
	char cwd;
	char stratum : 8;
	char poll : 8;
	char precision : 8;

	unsigned int root_delay;
	unsigned int root_dispersion;
	unsigned int reference_identifier;

	//时间戳  
	NetworkUInt64 reference_timestamp; //T4  
	NetworkUInt64 originate_timestamp;  //T1  
	NetworkUInt64 receive_timestamp;    //T2  
	NetworkUInt64 transmit_timestamp;   //T3  
};

class NtpProtocl
{
public:
	NtpProtocl();
	~NtpProtocl();

public:
	static NetworkUInt64 ms_n64ReferenceTimestamp;

	void GetNTPReplyPacket(NTPPacket* pPacket);
	int CheckPacket(NTPPacket* pPacket, int nLen);

	void GetNTPRequestPacket(NTPPacket* pPacket);
	void RecvNtpServerPacket(NTPPacket* pPacket);
};

