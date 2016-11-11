#include"stdafx.h"
#include"matlab_engine.h"

inline int PArgCheck(void const* p)
{
	if (!p)
	{
		PRINT(_T("Parameter fault."));
		
		return -1;
	}

	return 0;
}

inline int SetClassName(const char* pName, mxArray* pArray)
{
	if (mxSetClassName(pArray, pName))
	{
		PRINT(_T("Error setting class name."));
		return -1;
	}

	return 0;
}

int CMatlabBase::AddVariable(const char* pVarName, mxArray* pArray)
{
	PArgCheck(pArray);
	PArgCheck(pVarName);

	engPutVariable(m_pMatlabEngine, pVarName, pArray);
	m_pWorkMatrix.insert(std::make_pair(pVarName, pArray));

	return 0;
}
int CMatlabBase::AddVariable(const char* pVarName, double pArray[], size_t ArraySize)//添加列向量。本函数仅支持实数，后面可以增加通用性来支持复数
{
	PArgCheck(pArray);
	PArgCheck(pVarName);

	mxArray* pInArray{ mxCreateDoubleMatrix(1, (ArraySize), mxREAL) };
	PArgCheck(pInArray);

	memcpy(mxGetPr(pInArray), pArray, ArraySize * sizeof(double));
	engPutVariable(m_pMatlabEngine, pVarName, pInArray);
	m_pWorkMatrix.insert(std::make_pair(pVarName, pInArray));

	return 0;
}
int CMatlabBase::AddVariable(const char* pVarName, vector<double>& Vector)
{
	PArgCheck(pVarName);

	mxArray* pInArray{ mxCreateDoubleMatrix(1,Vector.size(),mxREAL) };
	PArgCheck(pInArray);

	double* Buffer{ new double[Vector.size()] };//需要一个缓冲区来赋值
	for (size_t loop{}; loop<Vector.size(); loop++)
	{
		*(Buffer + loop) = Vector[loop];
	}

	memcpy(mxGetPr(pInArray), Buffer, Vector.size() * sizeof(double));
	engPutVariable(m_pMatlabEngine, pVarName, pInArray);
	m_pWorkMatrix.insert(std::make_pair(pVarName, pInArray));

	return 0;
}

void CMatlabBase::Plot(char const* pVarName)
{
	//map<string, mxArray*>::iterator iter{ m_pWorkMatrix.find(pVarName) };
	char* pString = new char[strlen(pVarName) + 6 + 1]{};//6为plot()六个字符，1为最后的结尾符
	memcpy(pString, "plot(", 5);
	memcpy(pString + 5, pVarName, strlen(pVarName));
	memcpy(pString + 5 + strlen(pVarName), ")", 1);
	engEvalString(m_pMatlabEngine, pString);
}
void CMatlabBase::Plot(char const* pVar1Name, char const* pVar2Name)
{
	char* pString = new char[strlen(pVar1Name) + strlen(pVar2Name) + 7 + 1]{};//7为两个变量的分割符逗号
	memcpy(pString, "plot(", 5);
	memcpy(pString + 5, pVar1Name, strlen(pVar1Name));
	memcpy(pString + 5 + strlen(pVar1Name), ",", 1);
	memcpy(pString + 5 + strlen(pVar1Name) + 1, pVar2Name, strlen(pVar2Name));
	memcpy(pString + 5 + strlen(pVar1Name) + 1 + strlen(pVar2Name), ")", 1);
	engEvalString(m_pMatlabEngine, pString);
}