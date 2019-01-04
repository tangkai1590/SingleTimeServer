#pragma once
#include <stdio.h>
#include <vector>
class Nema0183Protocl
{
public:
	Nema0183Protocl();
	~Nema0183Protocl();

	typedef std::vector<char> NemaUnitData;
	typedef std::vector<NemaUnitData> NemaMessage;
	typedef std::vector<NemaMessage> NemaDataLsit;

	int ParseData(void* pData, int nLen, void* pLeftData, int& nLeftLen);
	int MessageNumber();
	NemaMessage& GetMessage(int nNum);

	void ClearData();

private:
	NemaDataLsit m_currentData;
};

