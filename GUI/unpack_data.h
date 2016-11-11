#pragma once

#include<iostream>
#include<vector>

using std::vector;

#define POL_POSITIVE 1//������
#define POL_NEGATIVE 0//���Ը�

struct DataSource
{
	vector<double> AmplitudeIn;//��ֵ����
	vector<double> AmplitudeOut;//��ֵ���
	vector<double> Frequency;//Ƶ��
	vector<double> Phase;//��λ
	vector<int>    Polarity;//����
};

class CUnpaker
{
protected:
	char m_szBuffer[33];//����ÿ������֮������ݻ�����
	char m_szMainBuffer[20001];//�ܵĻ����������������е����ݣ�20001�봮���ڲ�����Ļ�������Сһ��
	DataSource m_UnpackedData;//�ѽ��������
	
	char* FindFirstSemicolon(char* szInput)
	{
		while (*(szInput++) != ';')//��Ҫ���Դ˴���ʲôʱ������ѭ���ģ�szInput����ָ�����';'������һ������
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
		
		//����Ƿ�Ϊ������
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

		szInput = FindFirstSemicolon(szInput);//�ҵ���һ���ֺţ��ӵ�֮ǰ���ܲ�ȱ������

		double TempDouble{};//��ʱ��Ż����double��������
		unsigned int StructIndex{};//�����ṹ���ڲ��ı�����ѭ����ֵ
		unsigned int CopyCount{};

		while (FindFirstSemicolon(szInput))
		{
			if (StructIndex == 5)
			{
				StructIndex = 0;
			}
			CopyCount = FindNextToken(szInput);//Ѱ������һ�����Ż�ֺų���ʱ�м���ַ�����
			memcpy(m_szBuffer, szInput, CopyCount);//�������ָ���֮����ַ�ȡ����
			szInput = szInput + CopyCount + 1;//��ָ����������,+1��Ϊ��Խ��һ������
			TempDouble = atof(m_szBuffer);//�ַ�ת��Ϊdouble
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
			if (m_UnpackedData.Polarity.size() == 200)//�������200������ʱ�˳�
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