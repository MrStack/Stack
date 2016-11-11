#pragma once

#include<iostream>
#include<vector>

using std::vector;

#define POL_POSITIVE 1//极性正
#define POL_NEGATIVE 0//极性负

struct DataSource
{
	vector<double> AmplitudeIn;//幅值输入
	vector<double> AmplitudeOut;//幅值输出
	vector<double> Frequency;//频率
	vector<double> Phase;//相位
	vector<int>    Polarity;//极性
};

class CUnpaker
{
protected:
	char m_szBuffer[33];//处理每个逗号之间的数据缓冲区
	char m_szMainBuffer[20001];//总的缓冲区，包含了所有的数据；20001与串口内部定义的缓冲区大小一致
	DataSource m_UnpackedData;//已解包的数据
	
	char* FindFirstSemicolon(char* szInput)
	{
		while (*(szInput++) != ';')//需要测试此处是什么时候跳出循环的，szInput最终指向的是';'还是下一个数字
		{
			if (*szInput == '\0')
			{
				return nullptr;
			}
		}

		return szInput;
	}
	unsigned int FindNextToken(char* szInput)
	{
		unsigned int Index{};

		while (*(szInput++) != ',' && *((--szInput)++) != ';')
		{
			Index++;
		}

		return Index;
	}

public:
	CUnpaker(void) :m_szBuffer{}, m_szMainBuffer{}, m_UnpackedData{} {}
	~CUnpaker() = default;

	DataSource* UnpackData(char* szInput)
	{
		if (!szInput)
		{
			return nullptr;
		}
		
		//检测是否为空数组
		size_t Count0{};
		for (size_t loop{};loop<21; loop++)
		{
			if (szInput[loop] == 0)
			{
				Count0++;
			}
			if (Count0 == 20)
			{
				return nullptr;
			}
		}

		szInput = FindFirstSemicolon(szInput);//找到第一个分号，扔掉之前可能残缺的数据

		double TempDouble{};//临时存放缓存的double类型数据
		unsigned int StructIndex{};//索引结构体内部的变量，循环赋值
		unsigned int CopyCount{};

		while (FindFirstSemicolon(szInput))
		{
			if (StructIndex == 5)
			{
				StructIndex = 0;
			}
			CopyCount = FindNextToken(szInput);//寻找在下一个逗号或分号出现时中间的字符数量
			memcpy(m_szBuffer, szInput, CopyCount);//将两个分隔符之间的字符取出来
			szInput = szInput + CopyCount + 1;//将指针往后面推,+1是为了越过一个逗号
			TempDouble = atof(m_szBuffer);//字符转换为double
			switch (StructIndex)
			{
			case 0:
				m_UnpackedData.AmplitudeIn.push_back(TempDouble);
				break;
			case 1:
				m_UnpackedData.AmplitudeOut.push_back(TempDouble);
				break;
			case 2:
				m_UnpackedData.Frequency.push_back(TempDouble);
				break;
			case 3:
				m_UnpackedData.Phase.push_back(TempDouble);
				break;
			case 4:
				if (TempDouble < 1.1 && TempDouble > 0.9)
				{
					m_UnpackedData.Polarity.push_back(POL_POSITIVE);
				}
				else
				{
					m_UnpackedData.Polarity.push_back(POL_NEGATIVE);
				}
				break;
			}
			StructIndex++;
			if (m_UnpackedData.Polarity.size() == 200)//当解包够200组数据时退出
			{
				break;
			}
		}

		return &m_UnpackedData;
	}
	int GetProgress(void)
	{
		return static_cast<int>(m_UnpackedData.Polarity.size());
	}
};