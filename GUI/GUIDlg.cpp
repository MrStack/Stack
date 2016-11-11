
// GUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GUI.h"
#include "GUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGUIDlg dialog



CGUIDlg::CGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctlProgress);
	DDX_Control(pDX, IDC_LIST1, ctl_ListBox);
}

BEGIN_MESSAGE_MAP(CGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGUIDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CGUIDlg::OnNMCustomdrawProgress1)
	ON_BN_CLICKED(IDC_BUTTON1, &CGUIDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CGUIDlg message handlers

BOOL CGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_ctlProgress.SetRange32(0, 200);
	m_ctlProgress.SetStep(2);
	m_ctlProgress.SetPos(0);
	ctl_ListBox.InsertString(-1, COM.GetComName());
	UpdateData(false);

//	_beginthread([](void* pvoid) {while (true) { m_ctlProgress.SetPos(Unpacker.GetProgress()); } }, 0, nullptr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGUIDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	_beginthread([](void* pvoid)
	{
		size_t Count{};
		int Progress{};
		while (true) 
		{
			Progress = Unpacker.GetProgress();
			if (Progress == 0)
			{
				Count++;
				if (Count == 100)
				{
					return;
				}
			}
			m_ctlProgress.SetPos(Progress); 
			if (Unpacker.GetProgress() == 200)
			{
				return;
			}
		} 
	}, 0, nullptr);
	char* Buff{ nullptr };

	//测试用数据
	//if (fopen_s(&File, "D:\\data.txt", "r") == NOERROR)
	//{
	//	for (size_t loop{};; loop++)
	//	{
	//		CurrentCha = fgetc(File);
	//		if (CurrentCha != EOF)
	//		{
	//			Buff[loop] = CurrentCha;
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}
	//}
	//fclose(File);

	Buff = COM.ReadData();
	UnpackedData = Unpacker.UnpackData(Buff);
	if (!UnpackedData)
	{
		MessageBox(_T("No data received.Please check your connection."), _T("Error"), MB_OK);
	}
}


void CGUIDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CGUIDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if (!UnpackedData)
	{
		MessageBox(_T("Can not plot.Please receive data first."),_T("Error"), MB_OK);
		return;
	}
	MatlabCore.AddVariable("AmplitudeIn", UnpackedData->AmplitudeIn);
	MatlabCore.AddVariable("AmplitudeOut", UnpackedData->AmplitudeOut);
	MatlabCore.AddVariable("Frequency", UnpackedData->Frequency);
	MatlabCore.AddVariable("Phase", UnpackedData->Phase);

	m_ctlProgress.SetPos(0);

	MatlabCore.Eval("close()");
	MatlabCore.Eval("close()");

	//以下是绘制波特图

	MatlabCore.Eval("figure;subplot(2,1,1);");//绘制子图，第一个参数为总行数，第二个参数为总列数，第三个参数为当前所要绘制的子图序号
	MatlabCore.Eval("loglog(Frequency,AmplitudeOut,'linestyle','none','marker','.');");
	
	MatlabCore.Eval("title('Bode Diagram');");//设置标题
	MatlabCore.Eval("ylabel('Magnitude(dB)');");//设置y轴标签
	MatlabCore.Eval("axis([min(Frequency),max(Frequency),min(AmplitudeOut),max(AmplitudeOut)]);");//设置轴范围
	
	MatlabCore.Eval("subplot(2,1,2);");//绘图
	MatlabCore.Eval("loglog(Frequency,Phase,'linestyle','none','marker','.');");

	MatlabCore.Eval("ylabel('Phase(deg)');");//设置y轴标签
	MatlabCore.Eval("xlabel('Frequency(rad/s)');");//设置x轴标签
	MatlabCore.Eval("axis([min(Frequency),max(Frequency),min(Phase),max(Phase)]);");//设置轴范围

	//以下是绘制奈奎斯特图

	MatlabCore.Eval("figure;plot(AmplitudeOut.*cos(Phase),AmplitudeOut.*sin(Phase),'linestyle','none','marker','.');");//绘制nyquist图
	MatlabCore.Eval("xlabel('Real Axis');");//设置x轴坐标
	MatlabCore.Eval("ylabel('Imaginary Axis');");//设置y轴坐标
	MatlabCore.Eval("title('Nyquist Diagram');");//设置标题

//	MatlabCore.Eval("subplot(2,2,3)");
//	MatlabCore.Eval("loglog(Frequency,AmplitudeOut.^20)");//对数坐标绘图，第一个参数为x,第二个参数为y；因为纵轴为20倍log所以用了幂函数
}
