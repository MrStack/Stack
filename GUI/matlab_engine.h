#pragma once

#include"engine.h"
#include<string.h>
#include<map>
#include<string>
#include<iostream>
#include<vector>
#include<tchar.h>

using std::vector;

//#define _CONSOLE_   //控制台模式

#ifdef _CONSOLE_ //注意：传入参数必须为字符串
#define PRINT(x) std::cout<<(x)<<endl//控制台模式的消息输出函数
#else
#include<Windows.h>
#define PRINT(x) MessageBox((HWND)nullptr,(x),_T("Matlab"),MB_OK)//非控制台模式的消息输出函数
#endif

using std::map;
using std::string;
using std::cout;
using std::endl;

inline int PArgCheck(void const* p);//检测指针参数是否合法
inline int SetClassName(const char* pName, mxArray* pArray);//设置类变量名称

class CMatlabBase
{
private:
	Engine* m_pMatlabEngine;//Matlab引擎指针
	map<string,mxArray*> m_pWorkMatrix;//在工作区可以保存多个二维矩阵变量
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
		for (auto& iter{ std::begin(m_pWorkMatrix) }; iter != std::end(m_pWorkMatrix); iter++)//删除map中的所有的数据
		{
			free(iter->second);
		}
		engClose(m_pMatlabEngine);
		m_pMatlabEngine = nullptr;
	}

	int AddVariable(const char* pVarName, mxArray* pArray);
	int AddVariable(const char* pVarName, double pArray[], size_t ArraySize);//添加列向量。本函数仅支持实数，后面可以增加通用性来支持复数
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