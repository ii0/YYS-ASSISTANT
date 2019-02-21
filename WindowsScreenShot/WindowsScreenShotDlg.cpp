
// WindowsScreenShotDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WindowsScreenShot.h"
#include "WindowsScreenShotDlg.h"
#include "afxdialogex.h"
#include <windows.h>    
#include <psapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;
using namespace cv;
CStringList cStringList;
CString hwndSelectidName;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

extern BOOL m_threadStartOrStop;
extern int m_windowFlag;
extern Mat Invite, FightStart_ok, FightStart_no, PrepareAction, Erkounv, Jiaotu, Datianogou, FirstPortrait, Click2Continue, Challenge, FindSuper;
extern Mat wards, wards2, toFight, flush, VictoryLogo;
Mat cmpArray[] = { FightStart_ok, FightStart_no, PrepareAction, Erkounv, Jiaotu, Datianogou, FirstPortrait, Click2Continue, Invite, Invite };
/*   0��ʼս����ɫ  ��ʼս����ɫ    ׼��    3����Ů    ��ͼ   ���칷     6����   ʤ��logo   8����*/
extern LPARAM  FightStartPoint, PrepreActionPoint, RightPoint, FightPassPoint, ChallengePoint;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindowsScreenShotDlg �Ի���



CWindowsScreenShotDlg::CWindowsScreenShotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowsScreenShotDlg::IDD, pParent)
	, m_edit1(0)
	, m_edit2(0)
	, m_edit3(0)
	, m_edit4(0)
	, m_yysState(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDB_BITMAP2);
}

void CWindowsScreenShotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_YYSSTATE, m_yysState);
}

BEGIN_MESSAGE_MAP(CWindowsScreenShotDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SCREEN_SHOT, &CWindowsScreenShotDlg::OnBnClickedScreenShot)
	
	ON_BN_CLICKED(IDC_DOUHUN, &CWindowsScreenShotDlg::OnBnClickedDouhun)
	ON_BN_CLICKED(IDC_STOPALL, &CWindowsScreenShotDlg::OnBnClickedStopall)
	ON_BN_CLICKED(IDC_TRIHUN, &CWindowsScreenShotDlg::OnBnClickedTrihun)
	ON_BN_CLICKED(IDC_YULING, &CWindowsScreenShotDlg::OnBnClickedYuling)
//	ON_BN_CLICKED(IDC_OPENGL_TEST, &CWindowsScreenShotDlg::OnBnClickedOpenglTest)
//	ON_BN_CLICKED(IDC_ALT_PRTSCR, &CWindowsScreenShotDlg::OnBnClickedAltPrtscr)
ON_BN_CLICKED(IDC_SUPER_JUEXING, &CWindowsScreenShotDlg::OnBnClickedSuperJuexing)
ON_BN_CLICKED(IDC_UPDATE_HWNDLIST, &CWindowsScreenShotDlg::OnBnClickedUpdateHwndlist)
ON_BN_CLICKED(IDC_BUTTON_TUPO, &CWindowsScreenShotDlg::OnBnClickedButtonTupo)
END_MESSAGE_MAP()


// CWindowsScreenShotDlg ��Ϣ��������

BOOL CWindowsScreenShotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ����Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWindowsScreenShotDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWindowsScreenShotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWindowsScreenShotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT ThreadYuHun_TWO(LPVOID lpParm)
{
	
	HWND pWnd = ::FindWindowW(NULL, hwndSelectidName);
	if (pWnd == NULL) {
		m_threadStartOrStop = 0;
		AfxMessageBox(_T("���������"));
		return 0;
	}
	RECT rc;
	::GetWindowRect(pWnd, &rc);
	SetWindowPos(pWnd, HWND_BOTTOM, rc.left, rc.top, 830, 556, NULL);
	BOOL cmpResult[10] = { FALSE };
	CWindowsScreenShotDlg Cwssd = new CWindowsScreenShotDlg();
	Mat ScreenMat = Cwssd.GetRegionMat(pWnd);//���ȫͼ	
	Mat RegionMat;
	int acitonType = 0;
	int i;
	while (m_threadStartOrStop)
	{
		Sleep(200);
		ScreenMat = Cwssd.GetRegionMat(pWnd);
		for ( i = 1; i <= 2; i++)
		{
			RegionMat = Cwssd.GetRegionMatCut_2(ScreenMat, i);
			cmpResult[i - 1] = Cwssd.MatCompare(RegionMat, cmpArray[i - 1]);			
		}

		for (i = 7; i <= 9; i++)//������ĿС�֣�����֣�ֻ�ж��Ƿ����
		{
			RegionMat = Cwssd.GetRegionMatCut_2(ScreenMat, i);
			cmpResult[i - 1] = Cwssd.MatCompare(RegionMat, cmpArray[i - 1]);
		}
		for (i = 2; i <= 7; i++)
		{
			if (cmpResult[i])acitonType = i;
		}
		if (cmpResult[0] && ~cmpResult[8])acitonType = 1;
		if (cmpResult[1] && cmpResult[8])acitonType = 0;
		
		switch (acitonType)
		{
		case 1://����  ��ʼս��  ��
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightStartPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightStartPoint);
			Sleep(300);
			break;
		case 2://׼��
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, PrepreActionPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, PrepreActionPoint);
			Sleep(100);
			break;
		case 3://����Ů
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, RightPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, RightPoint);
			Sleep(2000);
			break;
		case 4://��ͼ
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, RightPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, RightPoint);
			Sleep(2000);
			break;
		case 5://���칷
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, RightPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, RightPoint);
			Sleep(2000);
			break;
		case 6://��������logo
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(200);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(400);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(400);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			break;
		case 7://����ʤ��logo
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(200);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			break;
		default:
			Sleep(rand() % 1500);
			break;
		}
		acitonType = 0;
		EmptyWorkingSet(GetCurrentProcess());
	}
	RegionMat.release();
	delete Cwssd;
	ScreenMat.release();
	EmptyWorkingSet(GetCurrentProcess());
	return 0;
}

UINT ThreadYuHun_THREE(LPVOID lpParm)
{

	HWND pWnd = ::FindWindowW(NULL, hwndSelectidName);
	if (pWnd == NULL) {
		m_threadStartOrStop = 0;
		AfxMessageBox(_T("���������"));
		return 0;
	}
	RECT rc;
	::GetWindowRect(pWnd, &rc);
	SetWindowPos(pWnd, HWND_BOTTOM, rc.left, rc.top, 830, 556, NULL);

	BOOL cmpResult[10] = { FALSE };
	CWindowsScreenShotDlg Cwssd = new CWindowsScreenShotDlg();
	Mat ScreenMat;//���ȫͼ	
	Mat RegionMat;
	int acitonType = 0;
	while (m_threadStartOrStop)
	{
		Sleep(300);
		ScreenMat = Cwssd.GetRegionMat(pWnd);
		for (int i = 1; i <= 2; i++)
		{
			RegionMat = Cwssd.GetRegionMatCut(ScreenMat, i);
			cmpResult[i - 1] = Cwssd.MatCompare(RegionMat, cmpArray[i - 1]);
			
		}
		for (int i = 7; i <= 10; i++)
		{
			RegionMat = Cwssd.GetRegionMatCut(ScreenMat, i);
			cmpResult[i - 1] = Cwssd.MatCompare(RegionMat, cmpArray[i - 1]);

		}
		for (int i = 2; i <= 7; i++)
		{
			if (cmpResult[i])acitonType = i;
		}
		if (cmpResult[0] && ~cmpResult[9])acitonType = 1;
		if (cmpResult[1] && cmpResult[9])acitonType = 0;
		
		switch (acitonType)
		{
		case 1://����  ��ʼս��  ��
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightStartPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightStartPoint);
			Sleep(300);
			break;
		case 2://׼��
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, PrepreActionPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, PrepreActionPoint);
			Sleep(100);
			break;
		case 3://����Ů
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, RightPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, RightPoint);
			Sleep(2000);
			break;
		case 4://��ͼ
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, RightPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, RightPoint);
			Sleep(2000);
			break;
		case 5://���칷
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, RightPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, RightPoint);
			Sleep(2000);
			break;
		case 6://��������logo
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(100);
			break;
		case 7://����ʤ��logo
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(100);
			break;
			//case 8://����2ren
			//	SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			//	SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			//	Sleep(100);
			//	break;
			//case 9://����3ren
			//	SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			//	SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			//	Sleep(100);
			//	break;
		default:
			Sleep(rand() % 700);
			break;
		}
		acitonType = 0;
		EmptyWorkingSet(GetCurrentProcess());
	}
	delete Cwssd;
	ScreenMat.release();
	RegionMat.release();
	EmptyWorkingSet(GetCurrentProcess());
	return 0;
}


UINT ThreadYuLing(LPVOID lpParm)//��ս
{
	HWND pWnd = ::FindWindowW(NULL, hwndSelectidName);
	if (pWnd == NULL) {
		m_threadStartOrStop = 0;
		AfxMessageBox(_T("���������"));
		return 0;
	}

	RECT rc;
	::GetWindowRect(pWnd, &rc);
	SetWindowPos(pWnd, HWND_BOTTOM, rc.left, rc.top, 830, 556, NULL);

	BOOL cmpResult[3] = { FALSE };
	CWindowsScreenShotDlg Cwssd = new CWindowsScreenShotDlg();
	Mat ScreenMat = Cwssd.GetRegionMat(pWnd);//���ȫͼ	
	Mat RegionMat;
	int actionType = 0;
	while (m_threadStartOrStop)
	{
		Sleep(200);
		ScreenMat = Cwssd.GetRegionMat(pWnd);

		RegionMat = ScreenMat(Rect(570,335,100,50));//��ս
		//imwrite("./��ս1.jpg", RegionMat);
		if (Cwssd.MatCompare(RegionMat, Challenge))
		{
			//AfxMessageBox(_T("��⵽�˼�⵽�ˣ�����"));
			actionType = 3;
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, ChallengePoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, ChallengePoint);
			Sleep(1000 * 10);
		}
		RegionMat = ScreenMat(Rect(166, 254, 60, 60));//����
		if (Cwssd.MatCompare(RegionMat, FirstPortrait))
		{
			actionType = 1;
		}
		RegionMat = ScreenMat(Rect(360, 470, 115, 30));//�����Ļ����
		//imwrite("./�����Ļ����.jpg",RegionMat);
		if (Cwssd.MatCompare(RegionMat, VictoryLogo))
		{
			actionType = 2;
		}
		switch (actionType)
		{
		case 1://��������logo
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(200);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(400);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(400);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			break;
		case 2://����ʤ��logo
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(200);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			break;
		case 3://��ս
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, ChallengePoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, ChallengePoint);
			Sleep(200);
			/*SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, ChallengePoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, ChallengePoint);
			Sleep(1000*30);*/ //�������ҵԭ��ʱ��һ����30������
			break;
		default:
			
			Sleep(rand() % 1000);
			break;
		}
		actionType = 0;
		EmptyWorkingSet(GetCurrentProcess());
	}
	RegionMat.release();
	delete Cwssd;
	ScreenMat.release();
	EmptyWorkingSet(GetCurrentProcess());
	return 0;
}

UINT ThreadsSuperJuexing(LPVOID lpParm)//����������
{
	HWND pWnd = ::FindWindowW(NULL, hwndSelectidName);
	if (pWnd == NULL) {
		m_threadStartOrStop = 0;
		AfxMessageBox(_T("���������"));
		return 0;
	}
	RECT rc;
	::GetWindowRect(pWnd, &rc);
	SetWindowPos(pWnd, HWND_BOTTOM, rc.left, rc.top, 830, 556, NULL);
	BOOL cmpResult[3] = { FALSE };
	CWindowsScreenShotDlg Cwssd = new CWindowsScreenShotDlg();
	Mat ScreenMat = Cwssd.GetRegionMat(pWnd);//���ȫͼ	
	Mat RegionMat;
	int actionType = 0;
	while (m_threadStartOrStop)
	{
		Sleep(200);
		ScreenMat = Cwssd.GetRegionMat(pWnd);

		RegionMat = ScreenMat(Rect(570, 335, 100, 50));//��ս
		//imwrite("./��ս1.jpg", RegionMat);
		if (Cwssd.MatCompare(RegionMat, Challenge))
		{
			//AfxMessageBox(_T("��⵽�˼�⵽�ˣ�����"));
			actionType = 3;
		}
		RegionMat = ScreenMat(Rect(166, 254, 60, 60));//����
		if (Cwssd.MatCompare(RegionMat, FirstPortrait))
		{
			actionType = 1;
		}
		RegionMat = ScreenMat(Rect(360, 470, 115, 30));//�����Ļ����
		//imwrite("./�����Ļ����.jpg",RegionMat);
		if (Cwssd.MatCompare(RegionMat, VictoryLogo))
		{
			actionType = 2;
		}
		RegionMat = ScreenMat(Rect(1, 188, 50, 50));//���ֳ�����
		if (Cwssd.MatCompare(RegionMat, FindSuper))
		{
			actionType = 4;
		}

		switch (actionType)
		{
		case 1://��������logo
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(200);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(400);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(400);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			break;
		case 2://����ʤ��logo
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(200);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			break;
		case 3://��ս
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, ChallengePoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, ChallengePoint);
			Sleep(200);
			/*SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, ChallengePoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, ChallengePoint);
			Sleep(1000*60);*/
			break;
		case 4:
			m_threadStartOrStop = FALSE;
			AfxMessageBox(_T("���ֳ�����"));
			break;
		default:
			Sleep(rand() % 1500);
			Sleep(rand() % 1500);
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, FightPassPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, FightPassPoint);
			Sleep(rand() % 1500);
			break;
		}
		actionType = 0;
		EmptyWorkingSet(GetCurrentProcess());
	}
	RegionMat.release();
	delete Cwssd;
	ScreenMat.release();
	EmptyWorkingSet(GetCurrentProcess());
	return 0;
}

UINT ThreadTuPo(LPVOID lpParm){//
	// 1����ȡ�����ͼ
	// 2������ģ��ִ��
		// ��1����ģ��ƥ��  ���ƥ��λ��
		// ��2������ ���ƶ�ƥ��
		// ��3������ģ��ִ�в�ͬ�Ĳ���
	HWND pWnd = ::FindWindowW(NULL, hwndSelectidName);
	if (pWnd == NULL) {
		m_threadStartOrStop = 0;
		AfxMessageBox(_T("���������"));
		return 0;
	}
	RECT rc;
	::GetWindowRect(pWnd, &rc);
	SetWindowPos(pWnd, HWND_BOTTOM, rc.left, rc.top, 830, 556, NULL);
	BOOL cmpResult[3] = { FALSE };
	CWindowsScreenShotDlg Cwssd = new CWindowsScreenShotDlg();
	Mat ScreenMat, RegionMat;//���ȫͼ	
	Mat templates[] = { wards, toFight, VictoryLogo, wards2 };
	bool preStart = false;
	bool isFightStart = false;
	bool isFightEnd = false;
	Rect rect;
	Rect region = Rect(0,0,763,370);
	LPARAM clickPoint;
	int fightCount = 30;
	while (m_threadStartOrStop && fightCount>0)
	{
		Sleep(1000);

		ScreenMat = Cwssd.GetRegionMat(pWnd);
		rect = Cwssd.findTargetPosition(ScreenMat(region), templates[0]);//���ģ��ƥ�䷽��
		clickPoint = MAKELPARAM(rect.x + rect.width / 2, rect.y + rect.height / 2);
		RegionMat = ScreenMat(rect);
		if (!preStart && Cwssd.MatCompare2(RegionMat, templates[0], 0.2)) {
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, clickPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, clickPoint);
			preStart = true;
			Sleep(2000);
		}
		ScreenMat = Cwssd.GetRegionMat(pWnd);
		rect = Cwssd.findTargetPosition(ScreenMat(region), templates[3]);//���ģ��ƥ�䷽��
		clickPoint = MAKELPARAM(rect.x + rect.width / 2, rect.y + rect.height / 2);
		RegionMat = ScreenMat(rect);
		if (!preStart && Cwssd.MatCompare2(RegionMat, templates[3], 0.2)) {
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, clickPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, clickPoint);
			preStart = true;
			Sleep(2000);
		}


		ScreenMat = Cwssd.GetRegionMat(pWnd);
		rect = Cwssd.findTargetPosition(ScreenMat(Rect(0, 0, 763, 470)), templates[1]);//���ģ��ƥ�䷽��
		clickPoint = MAKELPARAM(rect.x + rect.width * 166 / 232, rect.y + rect.height / 2);
		RegionMat = ScreenMat(rect);
		if (preStart && Cwssd.MatCompare2(RegionMat, templates[1], 0.3)) {
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, clickPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, clickPoint);
			fightCount++;
			isFightStart = true;
			Sleep(2000);
		}

		ScreenMat = Cwssd.GetRegionMat(pWnd);
		rect = Cwssd.findTargetPosition(ScreenMat(Rect(0, 0, 763, 470)), templates[2]);//���ģ��ƥ�䷽��
		clickPoint = MAKELPARAM(rect.x + rect.width / 2, rect.y + rect.height / 2);
		RegionMat = ScreenMat(rect);
		if ( Cwssd.MatCompare2(RegionMat, templates[2], 0.2)) {
			SendMessage(pWnd, WM_LBUTTONDOWN, MK_LBUTTON, clickPoint);
			SendMessage(pWnd, WM_LBUTTONUP, NULL, clickPoint);
			preStart = false;
			isFightEnd = true;
			isFightStart = false;
			Sleep(1000);
		}

	}
	delete Cwssd;
	RegionMat.release();
	ScreenMat.release();
	EmptyWorkingSet(GetCurrentProcess());
	return 0;
}

/*
	screenPic ��ǰ��Ļ��ͼ
	templatePic ģ��ͼƬ
*/
Rect CWindowsScreenShotDlg::findTargetPosition(Mat screenPic, Mat templatePic) {

	int width = screenPic.cols - templatePic.cols + 1;//result����
	int height = screenPic.rows - templatePic.rows + 1;//result�߶�
	Mat result(height, width, CV_32FC1);//�������ӳ��ͼ��
	matchTemplate(screenPic, templatePic, result, CV_TM_CCOEFF_NORMED);
	normalize(result, result, 0, 1, NORM_MINMAX, -1);//��һ����0-1��Χ
	double minValue, maxValue;
	Point minLoc, maxLoc;
	minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);
	rectangle(screenPic, maxLoc, Point(maxLoc.x + templatePic.cols, maxLoc.y + templatePic.rows), Scalar(0, 255, 0), 2, 8);
	//imwrite("ʶ����.jpg", screenPic);//����ƥ�������ڵ���

	result.release();
	//���� ģ��ƥ�䵽��ԭͼ�����ڵ�λ��
	return Rect(maxLoc.x, maxLoc.y, templatePic.cols, templatePic.rows);
}

bool CWindowsScreenShotDlg::MatCompare(Mat screenPic, Mat templatePic)
{
	Mat screen64, template64;
	Mat screenPic64, templatePic64;
	cv::resize(screenPic, screenPic64, cv::Size(8, 8), 0, 0, cv::INTER_NEAREST);
	cv::resize(templatePic, templatePic64, cv::Size(8, 8), 0, 0, cv::INTER_NEAREST);
	cv::cvtColor(screenPic64, screen64, CV_BGR2GRAY);
	cv::cvtColor(templatePic64, template64, CV_BGR2GRAY);

	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];

	for (int i = 0; i < 8; i++)
	{
		uchar* data1 = screen64.ptr<uchar>(i);
		uchar* data2 = template64.ptr<uchar>(i);

		int tmp = i * 8;

		for (int j = 0; j < 8; j++)
		{
			int tmp1 = tmp + j;

			arr1[tmp1] = data1[j] / 4 * 4;//������ֵ��û�б仯�ģ����Ǵ����ֵת����int���ͣ����ǽ�4*4�������źã�ĿǰΪֹҲû��ɶ������
			arr2[tmp1] = data2[j] / 4 * 4;

			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	for (int i = 0; i < 64; i++)
	{
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
	{
		if (arr1[i] != arr2[i])
			++iDiffNum;
	}
	screen64.release();
	template64.release();
	screenPic64.release();
	templatePic64.release();
	if (iDiffNum < 20)
	{
		return true;
	}
	EmptyWorkingSet(GetCurrentProcess());
	return false;
}

bool CWindowsScreenShotDlg::MatCompare2(Mat screenPic, Mat templatePic, double rate)
{
	Mat screen64, template64;
	Mat screenPic64, templatePic64;
	cv::resize(screenPic, screenPic64, cv::Size(8, 8), 0, 0, cv::INTER_NEAREST);
	cv::resize(templatePic, templatePic64, cv::Size(8, 8), 0, 0, cv::INTER_NEAREST);
	cv::cvtColor(screenPic64, screen64, CV_BGR2GRAY);
	cv::cvtColor(templatePic64, template64, CV_BGR2GRAY);

	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];

	for (int i = 0; i < 8; i++)
	{
		uchar* data1 = screen64.ptr<uchar>(i);
		uchar* data2 = template64.ptr<uchar>(i);

		int tmp = i * 8;

		for (int j = 0; j < 8; j++)
		{
			int tmp1 = tmp + j;

			arr1[tmp1] = data1[j] / 4 * 4;//������ֵ��û�б仯�ģ����Ǵ����ֵת����int���ͣ����ǽ�4*4�������źã�ĿǰΪֹҲû��ɶ������
			arr2[tmp1] = data2[j] / 4 * 4;

			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	for (int i = 0; i < 64; i++)
	{
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
	{
		if (arr1[i] != arr2[i])
			++iDiffNum;
	}
	if ((iDiffNum)/64.0 > rate)
	{
		return false;
	}

	screen64.release();
	template64.release();
	screenPic64.release(); 
	templatePic64.release();
	EmptyWorkingSet(GetCurrentProcess());
	return true;
}
Mat CWindowsScreenShotDlg::GetRegionMat(HWND hWnd)
{

		extern int m_windowFlag;
		HDC hDC = ::GetWindowDC(hWnd);
		ASSERT(hDC);
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		ASSERT(hMemDC);
		RECT rc;
		::GetWindowRect(hWnd, &rc);
		BITMAP bitmap = { 0 };
		HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
		ASSERT(hBitmap);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBitmap);

		//::BitBlt(hMemDC,0,0,rc.right-rc.left,rc.bottom-rc.top,hDC,0,0,SRCCOPY);
		::PrintWindow(hWnd, hMemDC, 0);//��ȡ��ͼ

		::GetObject(hBitmap, sizeof(BITMAP), &bitmap);//�²�����ط���ͼƬ�Ѿ���ֵ��bitmap�����

		/*��bitmap��ʽͼƬת��ΪIplImage�����ڴ���*/
		int depth = (bitmap.bmBitsPixel == 1) ? IPL_DEPTH_1U : IPL_DEPTH_8U;
		int nChannels = (bitmap.bmBitsPixel == 1) ? 1 : bitmap.bmBitsPixel / 8;
		IplImage* img = cvCreateImage(cvSize(bitmap.bmWidth, bitmap.bmHeight), depth, nChannels);
		BYTE *pBuffer = new BYTE[bitmap.bmHeight*bitmap.bmWidth*nChannels];
		GetBitmapBits(hBitmap, bitmap.bmHeight*bitmap.bmWidth*nChannels, pBuffer);
		memcpy(img->imageData, pBuffer, bitmap.bmHeight*bitmap.bmWidth*nChannels);
		delete[]pBuffer;
		IplImage *dst = cvCreateImage(cvGetSize(img), img->depth, 3);
		cvCvtColor(img, dst, CV_BGRA2BGR);
		cvReleaseImage(&img);

		::SelectObject(hMemDC, hOldBmp);
		::DeleteObject(hBitmap);
		::DeleteObject(hMemDC);
		::ReleaseDC(hWnd, hDC);

		if (1 == m_windowFlag)
		{	
			Mat returnPic = cvarrToMat(dst, true)(Rect(0, 18, 856, 538));
			cvReleaseImage(&dst);
			return returnPic;
		}
		else
		{
			Mat returnPic = cvarrToMat(dst, true);
			cvReleaseImage(&dst);
			return returnPic;
		}


}
Mat CWindowsScreenShotDlg::GetRegionMatCut(Mat screenPic, int type)
{
	
	Rect rect;
	switch (type)
	{
	case 1://����  ��ʼս��  ��
		rect = Rect(614, 402, 125, 50);
		break;
	case 2://����  ��ʼս��  ��
		rect = Rect(614, 402, 125, 50);
		break;
	case 3://׼��
		rect = Rect(720, 370, 75, 45);
		break;
	case 4://����Ů
		rect = Rect(163, 122, 105, 90);
		break;
	case 5://����
		rect = Rect(581, 108, 90, 110);
		//imwrite("����---mmp.jpg", screenPic(rect));
		break;
	case 6://���칷
		rect = Rect(185, 118, 85, 98);
		break;
	case 7://��������logo
		rect = Rect(323, 211, 60, 60);//3��
		//rect = Rect(240, 207, 60, 60);//2��
		//rect = Rect(173, 260, 60, 60);//1��
		break;
	case 8://����ʤ��logo
		rect = Rect(360, 470, 115, 30);
		break;
	case 9://����1
		rect = Rect(432, 318, 62, 62);
		break;		
	case 10://����2
		rect = Rect(651, 318, 62, 62);
		break;		
	default:
		break;
	}
	//imwrite("screenPicrect.jpg", screenPic(rect));
	return screenPic(rect);

}

Mat CWindowsScreenShotDlg::GetRegionMatCut_2(Mat screenPic, int type)
{

	Rect rect;
	switch (type)
	{
	case 1://����  ��ʼս��  ��
		rect = Rect(614, 402, 125, 50);
		break;
	case 2://����  ��ʼս��  ��
		rect = Rect(614, 402, 125, 50);
		break;
	case 3://׼��
		rect = Rect(720, 370, 75, 45);
		break;
	case 4://����Ů
		rect = Rect(163, 122, 105, 90);
		break;
	case 5://����
		rect = Rect(581, 108, 90, 110);
		//imwrite("����---mmp.jpg", screenPic(rect));
		break;
	case 6://���칷
		rect = Rect(185, 118, 85, 98);
		break;
	case 7://��������logo
		//rect = Rect(323, 211, 60, 60);//3��
		rect = Rect(240, 207, 60, 60);//2��
		//rect = Rect(173, 260, 60, 60);//1��
		break;
	case 8://�����Ļ����
		rect = Rect(360, 470, 115, 30);
		break;
	case 9://����1
		rect = Rect(432, 318, 62, 62);
		break;
	case 10://����2
		rect = Rect(651, 318, 62, 62);
		break;
	default:
		break;
	}
	//imwrite("screenPicrect.jpg", screenPic(rect));
	return screenPic(rect);



}


void CWindowsScreenShotDlg::OnBnClickedScreenShot()
{

	int index = ((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetLBText(index, hwndSelectidName);
	HWND pWnd = ::FindWindowW(NULL, hwndSelectidName);

	if (pWnd != NULL) {
		LPCTSTR pFilePath = _T("./12.bmp");
		SaveHwndToBmpFile(pWnd, pFilePath);
	}
	else {
		AfxMessageBox(_T("δ�ҵ���ش���"));
	}
	//extern int m_windowFlag;
	//HWND pWnd = ::FindWindowW(NULL, _T("����ʦ - MuMuģ����"));
	//if (pWnd != NULL){
	//	m_windowFlag = 0;
	//}
	//else
	//{
	//	pWnd = ::FindWindowW(NULL, _T("ҹ��ģ����"));
	//	if (NULL != pWnd)
	//	{
	//		m_windowFlag = 1;
	//	}
	//	else
	//	{
	//		pWnd = ::FindWindowW(NULL, _T("��Ѷ�������֡���׼���桿"));
	//		if (NULL != pWnd){
	//			m_windowFlag = 2;
	//		}
	//		else{
	//			m_windowFlag = -1;
	//		}
	//	}
	//}
	//if (-1 != m_windowFlag){
	//LPCTSTR pFilePath = _T("./12.bmp");
	//SaveHwndToBmpFile(pWnd, pFilePath);
	////	HWND pWnd = ::FindWindowW(NULL, _T("����ʦ-������Ϸ"));
	//}
	//else
	//{
	//	AfxMessageBox(_T("δ�ҵ���ش���"));
	//}


}

void CWindowsScreenShotDlg::SaveHwndToBmpFile(HWND hWnd, LPCTSTR lpszPath)
{
	extern int m_windowFlag;
	HDC hDC = ::GetWindowDC(hWnd);
	ASSERT(hDC);

	HDC hMemDC = ::CreateCompatibleDC(hDC);	
	ASSERT(hMemDC);

	RECT rc;
	::GetWindowRect(hWnd, &rc);
 	m_edit1 = rc.left;//281,1084,101,627
 	m_edit2 = rc.right;
 	m_edit3 = rc.top;
 	m_edit4 = rc.bottom;
 	UpdateData(FALSE);
	//ͨ�����Եó���ͨ���ĸ����������Եó����Ͻǵ����ꡢ���ڴ�С���Ӷ����������ض�������������ض�ʱ����һ����ʾ״̬���Ӷ������жϡ�

	Mat bitmap_mat;
	BITMAP bitmap = { 0 };
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
	ASSERT(hBitmap);

	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBitmap);
	::PrintWindow(hWnd, hMemDC, 0);//��ȡ��ͼ
	::GetObject(hBitmap, sizeof(BITMAP), &bitmap);//�²�����ط���ͼƬ�Ѿ���ֵ��bitmap�����

	/*��bitmap��ʽͼƬת��ΪIplImage�����ڴ���*/
	int depth = (bitmap.bmBitsPixel == 1) ? IPL_DEPTH_1U : IPL_DEPTH_8U;
	int nChannels = (bitmap.bmBitsPixel == 1) ? 1 : bitmap.bmBitsPixel / 8;
	IplImage* img = cvCreateImage(cvSize(bitmap.bmWidth, bitmap.bmHeight), depth, nChannels);
	BYTE *pBuffer = new BYTE[bitmap.bmHeight*bitmap.bmWidth*nChannels];
	GetBitmapBits(hBitmap, bitmap.bmHeight*bitmap.bmWidth*nChannels, pBuffer);
	memcpy(img->imageData, pBuffer, bitmap.bmHeight*bitmap.bmWidth*nChannels);
	delete []pBuffer;
	IplImage *dst = cvCreateImage(cvGetSize(img), img->depth, 3);
	cvCvtColor(img, dst, CV_BGRA2BGR);
	cvReleaseImage(&img);

	bitmap_mat = cvarrToMat(dst);
	if (1 == m_windowFlag)
	{
		imwrite("./��ȡ.jpg", bitmap_mat(Rect(0, 18, 856, 538)));
	}
	else {
		imwrite("./��ȡ.jpg", bitmap_mat);
	}

	cvReleaseImage(&img);
	cvReleaseImage(&dst);
	bitmap_mat.release();
	//cv::Rect rect(581, 108, 90, 110);
	//Mat mat_cut = bitmap_mat(rect);
	//imwrite(".//����.jpg", mat_cut);
	//
	//cv::Rect rect2(446, 325, 62, 62);
	// mat_cut = bitmap_mat(rect2);
	//imwrite(".//����1.jpg", mat_cut);

	//cv::Rect rect3(671, 325, 62, 62);
	//mat_cut = bitmap_mat(rect3);
	//imwrite(".//����2.jpg", mat_cut);

	//cv::Rect rect4(163, 122, 105, 90);
	//Mat mat_cut = bitmap_mat(rect4);
	//imwrite(".//����Ů.jpg", mat_cut);

	//cv::Rect rect5(739, 381, 84, 46);
	//mat_cut = bitmap_mat(rect5);
	//imwrite(".//׼��.jpg", mat_cut);

	//cv::Rect rect6(157, 121, 120, 107);
	//mat_cut = bitmap_mat(rect6);
	//imwrite(".//��ͼ.jpg", mat_cut);

	//cv::Rect rect7(185, 118, 85, 98);
	//mat_cut = bitmap_mat(rect7);
	//imwrite(".//���칷.jpg", mat_cut);

	//cv::Rect rect8(173, 260, 60, 60);
	//mat_cut = bitmap_mat(rect8);
	//imwrite(".//��ʮ����1.jpg", mat_cut);

	//cv::Rect rect9(365, 369, 104, 67);
	//mat_cut = bitmap_mat(rect9);
	//imwrite(".//ʤ��logo.jpg", mat_cut);
	::SelectObject(hMemDC, hOldBmp);
	::DeleteObject(hBitmap);
	::DeleteObject(hMemDC);
	::ReleaseDC(hWnd, hDC);
}


void CWindowsScreenShotDlg::OnBnClickedDouhun()
{
	// ˫�˻�ʮ
	extern BOOL m_threadStartOrStop;
	GetDlgItem(IDC_DOUHUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_TRIHUN)->EnableWindow(FALSE);
	SetDlgItemText(IDC_YYSSTATE, _T("���˻�ʮ"));
	int index = ((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetLBText(index, hwndSelectidName);
	m_threadStartOrStop = 1;
	CWinThread *pThread_YuHun_Two = AfxBeginThread(ThreadYuHun_TWO, 0);
}


void CWindowsScreenShotDlg::OnBnClickedStopall()
{
	// ֹͣ
	extern BOOL m_threadStartOrStop;
	GetDlgItem(IDC_DOUHUN)->EnableWindow(TRUE);
	GetDlgItem(IDC_TRIHUN)->EnableWindow(TRUE);
	GetDlgItem(IDC_YULING)->EnableWindow(TRUE);
	GetDlgItem(IDC_SUPER_JUEXING)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_TUPO)->EnableWindow(TRUE);
	SetDlgItemText(IDC_YYSSTATE, _T("ֹͣ"));
	m_threadStartOrStop = FALSE;
}


void CWindowsScreenShotDlg::OnBnClickedTrihun()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	extern BOOL m_threadStartOrStop;
	int index = ((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetLBText(index, hwndSelectidName);
	GetDlgItem(IDC_TRIHUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DOUHUN)->EnableWindow(FALSE);
	SetDlgItemText(IDC_YYSSTATE, _T("���˻�ʮ"));
	m_threadStartOrStop = 1;
	CWinThread *pThread_YuHun_Three = AfxBeginThread(ThreadYuHun_THREE, 0);
}


void CWindowsScreenShotDlg::OnBnClickedYuling()
{
	extern BOOL m_threadStartOrStop;
	int index = ((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetLBText(index, hwndSelectidName);
	GetDlgItem(IDC_TRIHUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DOUHUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_YULING)->EnableWindow(FALSE);
	SetDlgItemText(IDC_YYSSTATE, _T("��ս"));
	m_threadStartOrStop = 1;
	AfxBeginThread(ThreadYuLing, 0);
}

void CWindowsScreenShotDlg::OnBnClickedSuperJuexing()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	extern BOOL m_threadStartOrStop;
	int index = ((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetLBText(index, hwndSelectidName);
	GetDlgItem(IDC_TRIHUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DOUHUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_YULING)->EnableWindow(FALSE);
	GetDlgItem(IDC_SUPER_JUEXING)->EnableWindow(FALSE);
	SetDlgItemText(IDC_YYSSTATE, _T("����������"));
	m_threadStartOrStop = 1;
	AfxBeginThread(ThreadsSuperJuexing, 0);
	
}




//void CWindowsScreenShotDlg::OnBnClickedOpenglTest()
//{
//	 opengl��ͼ
//	HWND pWnd = ::FindWindowW(NULL, _T("����ʦ-������Ϸ"));
//	HWND pWnd = ::FindWindowW(NULL, _T("ҹ��ģ����"));
//	HDC hdc = ::GetDC(pWnd);
//	RECT rc;
//	::GetWindowRect(pWnd, &rc);
//	unsigned char* screenShotBuf;
//	int width = rc.right - rc.left;
//	int height = rc.bottom - rc.top;
//	GLubyte* BMP_Header[54];
//	GLint i, j;
//	GLint PixelDataLength;
//	FILE* pDummyFile=fopen("bitmap1.bmp","rb");
//	FILE* pWritingFile=fopen("grab.bmp","wb");
//	GLubyte* pPixelData;
//	/*unsigned char header[54] = { 0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
//		54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 32, 0,
//		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//		0, 0, 0, 0
//	};
//	long file_size = (long)width * (long)height * 4 + 54;
//	header[2] = (unsigned char)(file_size & 0x000000ff);
//	header[3] = (file_size >> 8) & 0x000000ff;
//	header[4] = (file_size >> 16) & 0x000000ff;
//	header[5] = (file_size >> 24) & 0x000000ff;
//	header[18] = width & 0x000000ff;
//	header[19] = (width >> 8) & 0x000000ff;
//	header[20] = (width >> 16) & 0x000000ff;
//	header[21] = (width >> 24) & 0x000000ff;
//	header[22] = height & 0x000000ff;
//	header[23] = (height >> 8) & 0x000000ff;
//	header[24] = (height >> 16) & 0x000000ff;
//	header[25] = (height >> 24) & 0x000000ff;
//	char fname_bmp[128];*/
//
//
//	i = width * 3;
//	while (i % 4 != 0)++i;
//
//
//	PixelDataLength = i*height;
//	pPixelData = (GLubyte*)malloc(PixelDataLength);
//	if (0 == pPixelData)exit(0);
//
//
//	
//	HGLRC context = wglCreateContext(hdc);
//	if (NULL != hdc){
//		wglMakeCurrent(hdc, context);
//		screenShotBuf = new unsigned char[4 * width*height];
//		glPixelStorei(GL_UNPACK_ALIGNMENT,4);
//		glReadPixels(200,200,width,height,GL_RGBA,GL_UNSIGNED_BYTE,screenShotBuf);
//
//
//		/*fwrite(header, sizeof(unsigned char), (size_t)(long)width*height * 4, pWritingFile);
//		fwrite(pPixelData, sizeof(unsigned char), (size_t)(long)width*height * 4, pWritingFile);*/
//
//		fread(BMP_Header,sizeof(BMP_Header),1,pDummyFile);
//		fwrite(BMP_Header,sizeof(BMP_Header),1,pWritingFile);
//		i = width;
//		j = height;
//		fwrite(&i, sizeof(i), 1, pWritingFile);
//		fwrite(&j, sizeof(j), 1, pWritingFile);
//		fseek(pWritingFile,0,SEEK_END);
//		fwrite(pPixelData,PixelDataLength,1,pWritingFile);
//		
//
//	}
//	fclose(pDummyFile);
//	fclose(pWritingFile);
//	free(pPixelData);
//
//}
//void CWindowsScreenShotDlg::OnBnClickedAltPrtscr(){
//	AfxBeginThread(ThreadAltScreen, 0);
//
//}

//void CWindowsScreenShotDlg::AltPrtscr(HWND hWnd, LPCTSTR lpszPath)
//{
//	HWND hDesktop = ::GetDesktopWindow();
//	ASSERT(hDesktop);
//	if (NULL == hWnd)
//	{
//		hWnd = hDesktop;
//	}
//	RECT rect;
//	::GetWindowRect(hWnd, &rect);
//
//	int nWidht = rect.right - rect.left;
//	int nHeight = rect.bottom - rect.top;
//
//	HDC hSrcDC = ::GetWindowDC(hWnd);
//	ASSERT(hSrcDC);
//	HDC hMemDC = ::CreateCompatibleDC(hSrcDC);
//	ASSERT(hMemDC);
//	HBITMAP hBitmap = ::CreateCompatibleBitmap(hSrcDC, nWidht, nHeight);
//	ASSERT(hBitmap);
//	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);
//	::BitBlt(hMemDC, 0, 0, nWidht, nHeight, hSrcDC, 0, 0, SRCCOPY);
//	
//	BITMAP bitmap = { 0 };
//	::GetObject(hBitmap, sizeof(BITMAP), &bitmap);
//	BITMAPINFOHEADER bi = { 0 };
//	BITMAPFILEHEADER bf = { 0 };
//
//	CONST int nBitCount = 24;
//	bi.biSize = sizeof(BITMAPINFOHEADER);
//	bi.biWidth = bitmap.bmWidth;
//	bi.biHeight = bitmap.bmHeight;
//	bi.biPlanes = 1;
//	bi.biBitCount = nBitCount;
//	bi.biCompression = BI_RGB;
//	DWORD dwSize = ((bitmap.bmWidth * nBitCount + 31) / 32) * 4 * bitmap.bmHeight;
//
//	HANDLE hDib = GlobalAlloc(GHND, dwSize + sizeof(BITMAPINFOHEADER));
//	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
//	*lpbi = bi;
//
//	//::GetDIBits(hMemDC, hBitmap, 0, bitmap.bmHeight, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER), (BITMAPINFO*)lpbi, DIB_RGB_COLORS);
//	::glReadPixels(0, 0,(GLint)nWidht, (GLint)nHeight, GL_RGBA, GL_UNSIGNED_BYTE, &bitmap.bmBitsPixel);
//
//	int depth = (bitmap.bmBitsPixel == 1) ? IPL_DEPTH_1U : IPL_DEPTH_8U;
//	int nChannels = (bitmap.bmBitsPixel == 1) ? 1 : bitmap.bmBitsPixel / 8;
//	IplImage* img = cvCreateImage(cvSize(bitmap.bmWidth, bitmap.bmHeight), depth, nChannels);
//	BYTE *pBuffer = new BYTE[bitmap.bmHeight*bitmap.bmWidth*nChannels];
//	GetBitmapBits(hBitmap, bitmap.bmHeight*bitmap.bmWidth*nChannels, pBuffer);
//	memcpy(img->imageData, pBuffer, bitmap.bmHeight*bitmap.bmWidth*nChannels);
//	delete[]pBuffer;
//	IplImage *dst = cvCreateImage(cvGetSize(img), img->depth, 3);
//	cvCvtColor(img, dst, CV_BGRA2BGR);
//	cvReleaseImage(&img);
//
//	Mat bitmap_mat = Mat(dst);
//	imwrite(".//matxxx.jpg", bitmap_mat);
//	bitmap_mat.release();
//
//	GlobalUnlock(hDib);
//	GlobalFree(hDib);
//
//	::SelectObject(hMemDC, hOldBitmap);
//	::DeleteObject(hBitmap);
//	::DeleteDC(hMemDC);
//	::ReleaseDC(hWnd, hSrcDC);
//
//}
//UINT ThreadAltScreen(LPVOID lpParm){
//
//
//	HWND hWnd = ::FindWindowW(NULL, _T("����ʦ - MuMuģ����"));
//	SendMessage(hWnd, WM_KEYDOWN, VK_MENU, 0x000f0001);
//	SendMessage(hWnd, WM_KEYDOWN, VK_SNAPSHOT, 0x000f0001);
//	Sleep(100);
//	SendMessage(hWnd, WM_KEYUP, VK_SNAPSHOT, 0x000f0001);
//	SendMessage(hWnd, WM_KEYUP, VK_MENU, 0x000f0001);
//	return 0;
//}




void CWindowsScreenShotDlg::OnBnClickedUpdateHwndlist()
{	
	((CComboBox*)GetDlgItem(IDC_HWND_LIST))->ResetContent();
	cStringList.RemoveAll();
	UpdateData(TRUE);
	::EnumWindows((WNDENUMPROC)EnumWndProc, 0);
	POSITION position;
	for (int i = 0; i < cStringList.GetCount(); i++) {
		position = cStringList.FindIndex(i);
		CString temp = cStringList.GetAt(position);
		((CComboBox*)GetDlgItem(IDC_HWND_LIST))->AddString(temp);
	}
	UpdateData(TRUE);
}

BOOL CALLBACK CWindowsScreenShotDlg::EnumWndProc(HWND hwnd, LPARAM lParam) {
	BOOL bRet = TRUE;
	if (::IsWindowVisible(hwnd))
	{
		TCHAR szTitle[255];
		::GetWindowText(hwnd, szTitle, 255);
		CString  strTitle(szTitle);
		if (strTitle.Find(_T("����ʦ")) != -1) {
			cStringList.AddHead(strTitle);
			//bRet = FALSE;
		}
	}
	return bRet;
}
// ����ͻ��
void CWindowsScreenShotDlg::OnBnClickedButtonTupo()
{
	//�����������ȡ���
	int index = ((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_HWND_LIST))->GetLBText(index, hwndSelectidName);
	GetDlgItem(IDC_TRIHUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DOUHUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_YULING)->EnableWindow(FALSE);
	GetDlgItem(IDC_SUPER_JUEXING)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_TUPO)->EnableWindow(FALSE);
	SetDlgItemText(IDC_YYSSTATE, _T("ͻ��"));
	m_threadStartOrStop = 1;
	AfxBeginThread(ThreadTuPo, 0);
}