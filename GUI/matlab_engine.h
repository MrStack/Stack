#pragma once

#include"engine.h"
#include<string.h>
#include<map>
#include<string>
#include<iostream>
#include<vector>
#include<tchar.h>

using std::vector;

//#define _CONSOLE_   //����̨ģʽ

#ifdef _CONSOLE_ //ע�⣺�����������Ϊ�ַ���
#define PRINT(x) std::cout<<(x)<<endl//����̨ģʽ����Ϣ�������
#else
#include<Windows.h>
#define PRINT(x) MessageBox((HWND)nullptr,(x),_T("Matlab"),MB_OK)//�ǿ���̨ģʽ����Ϣ�������
#endif

using std::map;
using std::string;
using std::cout;
using std::endl;

inline int PArgCheck(void const* p);//���ָ������Ƿ�Ϸ�
inline int SetClassName(const char* pName, mxArray* pArray);//�������������

class CMatlabBase
{
private:
	Engine* m_pMatlabEngine;//Matlab����ָ��
	map<string,mxArray*> m_pWorkMatrix;//�ڹ��������Ա�������ά�������
public:
	explicit CMatlabBase(void) :m_pMatlabEngine{ engOpen(nullptr) }, m_pWorkMatrix{} 
	{
		if (!m_pMatlabEngine)
		{
			PRINT(_T("Fail to start Matlab engine."));
			exit(-1);
		}
		engSetVisible(m_pMatlabEngine, false);
	}
	~CMatlabBase()
	{
		for (auto& iter{ std::begin(m_pWorkMatrix) }; iter != std::end(m_pWorkMatrix); iter++)//ɾ��map�е����е�����
		{
			free(iter->second);
		}
		engClose(m_pMatlabEngine);
		m_pMatlabEngine = nullptr;
	}

	int AddVariable(const char* pVarName, mxArray* pArray);
	int AddVariable(const char* pVarName, double pArray[], size_t ArraySize);//�������������������֧��ʵ���������������ͨ������֧�ָ���
	int AddVariable(const char* pVarName, vector<double>& Vector);
	
	void NewFigure(void)
	{
		engEvalString(m_pMatlabEngine, "figure;");
	}
	void Plot(char const* pVarName);
	void Plot(char const* pVar1Name, char const* pVar2Name);

	void Eval(char const* pCommand)
	{
		engEvalString(m_pMatlabEngine, pCommand);
	}
};