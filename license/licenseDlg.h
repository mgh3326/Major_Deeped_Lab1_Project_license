// licenseDlg.h: 헤더 파일
//

#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;


// ClicenseDlg 대화 상자
class ClicenseDlg : public CDialogEx
{
	// 생성입니다.
public:
	ClicenseDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LICENSE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	unsigned char** R;
	unsigned char** G;
	unsigned char** B;
	int Height, Width;
	bool isImageOpened;

	String FileName;
public:
	CStatic PicOri;

	Mat Ori_Img,Proc_Img;
	CRect rect;
	CDC* m_pDC;

	Rect r;
	void DisplayImage(Mat DispMat, bool bInOut);
	void DisplayBitmap(CDC* pDC, CRect rect, IplImage* DispIplImage);
	void ResizeImage(IplImage* src, IplImage** dst, CRect* rect);
	Mat FindPlate(String path);
	Mat FindEdge(Mat img_gray, int th_high, int th_low);

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedExact();
	afx_msg void OnBnClickedMosaic();
	afx_msg void OnBnClickedDel();
};

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal);
void MemFree2D(unsigned char** Mem, int nHeight);