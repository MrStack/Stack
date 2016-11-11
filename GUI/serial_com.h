#pragma once

#include<Windows.h>
#include<string.h>
#include<iostream>
#include<string>
#include<tchar.h>

//#define CONSOLE_MODE

#ifdef CONSOLE_MODE
#define MESSAGE(hWnd,lpText,lpCaption,uType) std::cout<<(lpText)<<std::endl
#else
#define MESSAGE(hWnd,lpText,lpCaption,uType) MessageBox((hWnd),(lpText),(lpCaption),(uType))
#endif

class CSerialCom
{
protected:
	HANDLE m_hSerialFile;//����HANDLE
	DCB m_SerialDcb;//�豸���ƿ�
	COMMTIMEOUTS m_TimeOut;//��ʱ����
	DWORD m_Error;//����
	HKEY m_PortKey;//ע����
	TCHAR m_KeyValueName[MAX_PATH];//��ֵ����
	TCHAR m_PortName[20];//�˿�����
	DWORD m_PortIndex;//�˿�����
	DWORD m_Type;

	char m_szReceiveBuffer[20001];//����������
	DWORD m_ReceiveLength;//���յ��ַ���
	char m_SendBuffer[8192 + 1];
	DWORD m_SendLength;
public:
	CSerialCom(void) :m_hSerialFile{}, m_SerialDcb{}, m_TimeOut{}, m_Error{}, m_KeyValueName{}, m_PortKey{}, m_PortIndex{}, m_PortName{}, m_Type{ REG_SZ }
	{
		memset(m_KeyValueName, 0, sizeof(WCHAR) * 33);
		memset(m_PortName, 0, sizeof(unsigned char) * 5);

		LSTATUS State{ RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\"), 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_READ, &m_PortKey) };

		if (State == ERROR_SUCCESS)
		{
			DWORD KeyValueNameCount{ MAX_PATH };
			DWORD ValueCount{ 20 };
			State = RegEnumValue(m_PortKey, m_PortIndex++, (LPWSTR)m_KeyValueName, &KeyValueNameCount, NULL, &m_Type, (LPBYTE)m_PortName, &ValueCount);
		}

		m_hSerialFile = CreateFile((LPCTSTR)m_PortName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);

		if (m_hSerialFile == (HANDLE)0xffffffffffffffff)
		{
			m_Error = GetLastError();
			MESSAGE(nullptr, TEXT("Can not open com."), TEXT("Error"), MB_OK);
		}

		m_SerialDcb.BaudRate = 115200;
		m_SerialDcb.ByteSize = 8;
		m_SerialDcb.StopBits = 1;
		m_SerialDcb.Parity = NOPARITY;
		m_SerialDcb.fBinary = TRUE;
		m_SerialDcb.fParity = TRUE;

		if (!SetCommState(m_hSerialFile, &m_SerialDcb))
		{
			MESSAGE(nullptr, TEXT("Can not set com attributes."), TEXT("Error"), MB_OK);
		}
		
		SetupComm(m_hSerialFile, 8192, 8192);//���ô��ڻ�������С
		PurgeComm(m_hSerialFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//�������

		memset(&m_TimeOut, 0, sizeof(m_TimeOut));//��m_TimeOut����
		m_TimeOut.ReadIntervalTimeout = 1000;//��ȡ���Ϊ1s
		SetCommTimeouts(m_hSerialFile, &m_TimeOut);//���ô��ڳ�ʱ
	}
	~CSerialCom()
	{
		CloseHandle(m_hSerialFile);
	}

	LPCTSTR GetComName(void)
	{
		return m_PortName;
	}

	char* ReadData(void)
	{
		ReadFile(m_hSerialFile, m_szReceiveBuffer, 20000, &m_ReceiveLength, nullptr);

		return m_szReceiveBuffer;
	}
};