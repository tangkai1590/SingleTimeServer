#include "stdafx.h"
#include "Nema0183Protocl.h"


Nema0183Protocl::Nema0183Protocl()
{
}


Nema0183Protocl::~Nema0183Protocl()
{
}

int Nema0183Protocl::ParseData(void* pData, int nLen, void* pLeftData, int& nLeftLen)
{
	char* pchData = (char*)pData;
	char* pszMessageEnd = "\r\n";
	char* pszMessageBegin = "$";
	char* pszUnitDel = ",";
	char* pszUnitDel2 = "*";
	
	NemaMessage message;
	NemaUnitData messageUnit;
	messageUnit.clear();
	message.clear();

	int nMessageBeginPos = -1;
	int nUnitBeginPos = -1;

	// 开始分隔报文
	for (int i = 0; i < nLen; i++)
	{
		if (::memcmp(pszMessageBegin, pchData+i, 1) == 0) // Find the begin
		{
			nMessageBeginPos = i;
		}

		if (::memcmp(pszMessageEnd, pchData + i, 2) == 0) // Find the end
		{
			 if (nMessageBeginPos != -1 && i > nMessageBeginPos)
			 {
				 message.clear();
				 for (int j = nMessageBeginPos; j < i; j++)
				 {			 
					 if (::memcmp(pszUnitDel, pchData + j, 1) == 0
						 || ::memcmp(pszUnitDel2, pchData + j, 1) == 0) // Find the unit delimiter
					 {
						 nUnitBeginPos = j + 1;
						 message.push_back(messageUnit);
						 messageUnit.clear();
					 }
					 else
					 {
						 messageUnit.push_back(pchData[j]);
					 }

					 if (j == i - 1)
					 {
						 message.push_back(messageUnit);
						 messageUnit.clear();
					 }
				 }
				 m_currentData.push_back(message);
				 nMessageBeginPos = -1;
			 }

			 if (i == nLen - 2)
			 {
				 break;
			 }
		}

		if (i == nLen - 1 && nMessageBeginPos != -1 && pLeftData != nullptr)
		{	
			::memcpy_s(pLeftData, nLeftLen, pchData + nMessageBeginPos, nLen - nMessageBeginPos);
			nLeftLen = nLen - nMessageBeginPos;
		}
	}
	return 0;
}

int Nema0183Protocl::MessageNumber()
{
	return m_currentData.size();
}

Nema0183Protocl::NemaMessage& Nema0183Protocl::GetMessage(int nNum)
{
	return m_currentData[nNum];
}

void Nema0183Protocl::ClearData()
{
	m_currentData.clear();
}
