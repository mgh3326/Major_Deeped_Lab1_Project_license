// licenseDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "license.h"
#include "licenseDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

 // 구현입니다.
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


// ClicenseDlg 대화 상자



ClicenseDlg::ClicenseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LICENSE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ClicenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, PicOri);
}

BEGIN_MESSAGE_MAP(ClicenseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_FILEOPEN, &ClicenseDlg::OnFileOpen)
	ON_BN_CLICKED(IDCANCEL, &ClicenseDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_Open, &ClicenseDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_Exact, &ClicenseDlg::OnBnClickedExact)
	ON_BN_CLICKED(IDC_Mosaic, &ClicenseDlg::OnBnClickedMosaic)
	//   ON_WM_CLOSE()
END_MESSAGE_MAP()


// ClicenseDlg 메시지 처리기

BOOL ClicenseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);         // 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);      // 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void ClicenseDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void ClicenseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void ClicenseDlg::DisplayImage(Mat DispMat, bool bInOut)
{
	if (bInOut)
	{
		GetDlgItem(IDC_PICTURE)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = PicOri.GetDC();
		PicOri.GetClientRect(&rect);
	}

	IplImage* Ori = &IplImage(DispMat);
	IplImage* Resize = NULL;

	ResizeImage(Ori, &Resize, &rect);
	DisplayBitmap(m_pDC, rect, Resize);
}
void ClicenseDlg::clear() {
	PicOri.SetBitmap(NULL);

	Invalidate(FALSE);
	//HINSTANCE hAppInst = AfxGetInstanceHandle();
	//CRect rect;

	//PicOri.GetClientRect(&rect);

	//CDC *pDC = PicOri.GetDC();//Gets the item DC

	//HBITMAP hBitmap;
	//hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), "result.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	//CDC dc;
	//dc.CreateCompatibleDC(pDC);

	//BITMAP bmp;
	//GetObject(hBitmap, sizeof(BITMAP), &bmp);
	//SelectObject(dc.m_hDC, hBitmap); //make and select a DC out of hImage

	//pDC->SetStretchBltMode(COLORONCOLOR);
	//CRect bmpRect;
	//SetRect((LPRECT)&bmpRect, 0, 0, bmp.bmWidth, bmp.bmHeight);
	//int BmpWidth = (int)(bmpRect.right - bmpRect.left);
	//int BmpHeight = (int)(bmpRect.bottom - bmpRect.top);

	//StretchBlt(pDC->m_hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, dc.m_hDC, 0, 0, BmpWidth, BmpHeight, SRCCOPY);
}

void ClicenseDlg::DisplayBitmap(CDC* pDC, CRect rect, IplImage* DispIplImage)
{
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biWidth = DispIplImage->width;
	bitmapInfo.bmiHeader.biHeight = DispIplImage->height;

	if (DispIplImage->nChannels == 3)
	{
		IplImage* ReverseImg = cvCreateImage(CvSize(DispIplImage->width, DispIplImage->height), 8, 3);

		for (int h = 0; h < DispIplImage->height; h++)
		{
			for (int w = 0; w < DispIplImage->width; w++)
			{
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3] = DispIplImage->imageData[(DispIplImage->height - h)*DispIplImage->widthStep + w * 3];
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3 + 1] = DispIplImage->imageData[(DispIplImage->height - h)*DispIplImage->widthStep + w * 3 + 1];
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3 + 2] = DispIplImage->imageData[(DispIplImage->height - h)*DispIplImage->widthStep + w * 3 + 2];
			}
		}
		bitmapInfo.bmiHeader.biBitCount = DispIplImage->depth*DispIplImage->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom,
			0, 0, DispIplImage->width, DispIplImage->height, ReverseImg->imageData, &bitmapInfo,
			DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
	}
	else
	{
		IplImage* ReverseImg = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 1);

		for (int h = 0; h < DispIplImage->height; h++)
		{
			for (int w = 0; w < DispIplImage->width; w++)
			{
				ReverseImg->imageData[h*DispIplImage->widthStep + w] = DispIplImage->imageData[(DispIplImage->height - h)*DispIplImage->widthStep + w];
			}
		}

		IplImage* tmp = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 3);
		cvCvtColor(ReverseImg, tmp, CV_GRAY2BGR);

		bitmapInfo.bmiHeader.biBitCount = tmp->depth*tmp->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, tmp->width, tmp->height, tmp->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
		cvReleaseImage(&tmp);
	}
}

void ClicenseDlg::ResizeImage(IplImage* src, IplImage** dst, CRect* rect)
{
	int ndst_width = 0, ndst_height = 0;

	if (src->width > src->height) {
		ndst_width = rect->Width();
		ndst_height = (src->height*rect->Width()) / src->width;
	}
	else {
		ndst_width = (src->width*rect->Height()) / src->height;
		ndst_height = rect->Height();
	}

	(*dst) = cvCreateImage(cvSize(ndst_width, ndst_height), IPL_DEPTH_8U, src->nChannels);

	cvResize(src, (*dst));
	rect->right = rect->left + ndst_width;
	rect->bottom = rect->top + ndst_height;
}

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nlnitVal)
{
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++)
	{
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nlnitVal, sizeof(unsigned char)*nWidth);
	}
	return rtn;
}

void MemFree2D(unsigned char** Mem, int nHeight)
{
	for (int n = 0; n < nHeight; n++)
	{
		delete[] Mem[n];
	}
	delete[] Mem;
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR ClicenseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void ClicenseDlg::OnFileOpen()
{
	CFileDialog dlg(TRUE);

	if (isImageOpened)
	{
		if (R != NULL)
		{
			MemFree2D(R, Height);
		}
		if (G != NULL)
		{
			MemFree2D(G, Height);
		}
		if (B != NULL)
		{
			MemFree2D(B, Height);
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.GetFileExt() != "jpg"&&dlg.GetFileExt() != "JPG"&&dlg.GetFileExt() != "png"&&dlg.GetFileExt() != "PNG")
		{
			MessageBox("JPG 또는 PNG 파일이 아닙니다.");
			return;
		}

		FileName = dlg.GetFileTitle();

		if (dlg.GetFileExt() == "jpg" || dlg.GetFileExt() == "JPG" || dlg.GetFileExt() == "png" || dlg.GetFileExt() == "PNG")
		{
			Ori_Img = imread((const char*)dlg.GetPathName());
			Height = Ori_Img.rows;
			Width = Ori_Img.cols;

			if (Ori_Img.channels() == 3)
			{
				R = MemAlloc2D(Height, Width, 0);
				G = MemAlloc2D(Height, Width, 0);
				B = MemAlloc2D(Height, Width, 0);

				for (int h = 0; h < Height; h++)
				{
					for (int w = 0; w < Width; w++)
					{
						Vec3b RGB = Ori_Img.at<Vec3b>(h, w);
						R[h][w] = RGB[2];
						G[h][w] = RGB[1];
						B[h][w] = RGB[0];
					}
				}
				//nFormat = FORMAT_RGB;
			}
			isImageOpened = true;
			clear();
			DisplayImage(Ori_Img, true);
		}
	}
}
Mat ClicenseDlg::CannyEdge(Mat img_gray, int th_high, int th_low)
{
	Mat img_edge;
	img_edge.create(img_gray.size(), img_gray.type());



	int i, j;

	int width = img_gray.cols;
	int height = img_gray.rows;

	unsigned char *pEdge = new uchar[width*height];

	int dx, dy, mag, slope, direction;
	int index, index2;

	const int fbit = 10;
	const int tan225 = 424;       // tan25.5 << fbit, 0.4142
	const int tan675 = 2472;      // tan67.5 << fbit, 2.4142

	const int CERTAIN_EDGE = 255;
	const int PROBABLE_EDGE = 100;

	bool bMaxima;

	int *mag_tbl = new int[width*height];
	int *dx_tbl = new int[width*height];
	int *dy_tbl = new int[width*height];

	unsigned char **stack_top, **stack_bottom;
	stack_top = new unsigned char*[width*height];
	stack_bottom = stack_top;
#define CANNY_PUSH(p)    *(p) = CERTAIN_EDGE, *(stack_top++) = (p)
#define CANNY_POP()      *(--stack_top)

	for (i = 0; i < width*height; i++) {
		mag_tbl[i] = 0;
	}

	// Sobel Edge Detection
	for (i = 1; i < height - 1; i++) {
		index = i * width;
		for (j = 1; j < width - 1; j++) {
			index2 = index + j;
			// -1 0 1
			// -2 0 2
			// -1 0 1
			dx = ((int)img_gray.at<uchar>(i - 1, j - 1) * (-1)) + ((int)img_gray.at<uchar>(i - 1, j + 1) * (1))
				+ ((int)img_gray.at<uchar>(i, j - 1) * (-2)) + ((int)img_gray.at<uchar>(i, j + 1) * (2))
				+ ((int)img_gray.at<uchar>(i + 1, j - 1) * (-1)) + ((int)img_gray.at<uchar>(i + 1, j + 1) * (1));

			// -1 -2 -1
			//  0  0  0
			//  1  2  1
			dy = ((int)img_gray.at<uchar>(i - 1, j - 1) * (-1)) + ((int)img_gray.at<uchar>(i - 1, j) * (-2)) + ((int)img_gray.at<uchar>(i - 1, j + 1) * (-1))
				+ ((int)img_gray.at<uchar>(i + 1, j - 1) * (1)) + ((int)img_gray.at<uchar>(i + 1, j) * (2)) + ((int)img_gray.at<uchar>(i + 1, j + 1) * (1));

			mag = abs(dx) + abs(dy);     // magnitude
			//mag = sqrtf(dx*dx + dy*dy);

			dx_tbl[index2] = dx;
			dy_tbl[index2] = dy;
			mag_tbl[index2] = mag;              // store the magnitude in the table
		}   // for(j)
	}   // for(i)

	for (i = 1; i < height - 1; i++) {
		index = i * width;
		for (j = 1; j < width - 1; j++) {
			index2 = index + j;

			mag = mag_tbl[index2];              // retrieve the magnitude from the table

			// if the magnitude is greater than the lower threshold
			if (mag > th_low) {

				// determine the orientation of the edge
				dx = dx_tbl[index2];
				dy = dy_tbl[index2];

				if (dx != 0) {
					slope = (dy << fbit) / dx;

					if (slope > 0) {
						if (slope < tan225)
							direction = 0;
						else if (slope < tan675)
							direction = 1;
						else
							direction = 2;
					}
					else {
						if (-slope > tan675)
							direction = 2;
						else if (-slope > tan225)
							direction = 3;
						else
							direction = 0;
					}
				}
				else
					direction = 2;

				bMaxima = true;
				// perform non-maxima suppression
				if (direction == 0) {
					if (mag < mag_tbl[index2 - 1] || mag < mag_tbl[index2 + 1])
						bMaxima = false;
				}
				else if (direction == 1) {
					if (mag < mag_tbl[index2 + width + 1] || mag < mag_tbl[index2 - width - 1])
						bMaxima = false;
				}
				else if (direction == 2) {
					if (mag < mag_tbl[index2 + width] || mag < mag_tbl[index2 - width])
						bMaxima = false;
				}
				else { // if(direction == 3)
					if (mag < mag_tbl[index2 + width - 1] || mag < mag_tbl[index2 - width + 1])
						bMaxima = false;
				}

				if (bMaxima) {
					if (mag > th_high) {
						pEdge[index2] = CERTAIN_EDGE;           // the pixel does belong to an edge
						*(stack_top++) = (unsigned char*)(pEdge + index2);
					}
					else
						pEdge[index2] = PROBABLE_EDGE;          // the pixel might belong to an edge
				}
			}

		}   // for(j)
	}   // for(i)

	while (stack_top != stack_bottom) {
		unsigned char* p = CANNY_POP();

		if (*(p + 1) == PROBABLE_EDGE)
			CANNY_PUSH(p + 1);
		if (*(p - 1) == PROBABLE_EDGE)
			CANNY_PUSH(p - 1);
		if (*(p + width) == PROBABLE_EDGE)
			CANNY_PUSH(p + width);
		if (*(p - width) == PROBABLE_EDGE)
			CANNY_PUSH(p - width);
		if (*(p - width - 1) == PROBABLE_EDGE)
			CANNY_PUSH(p - width - 1);
		if (*(p - width + 1) == PROBABLE_EDGE)
			CANNY_PUSH(p - width + 1);
		if (*(p + width - 1) == PROBABLE_EDGE)
			CANNY_PUSH(p + width - 1);
		if (*(p + width + 1) == PROBABLE_EDGE)
			CANNY_PUSH(p + width + 1);
	}

	for (i = 0; i < width*height; i++)
		if (pEdge[i] != CERTAIN_EDGE)
			pEdge[i] = 0;

	for (i = 0; i < height; i++) {
		index = i * width;
		for (j = 0; j < width; j++) {
			index2 = index + j;
			img_edge.at<uchar>(i, j) = pEdge[index2];
		}
	}

	delete[] mag_tbl;
	delete[] dx_tbl;
	delete[] dy_tbl;
	delete[] stack_bottom;

	return img_edge;
}

Mat ClicenseDlg::FindPlate(String path)
{
	Mat out;
	Mat image, image2, image3, drawing;  //  Make images.
	Rect rect, temp_rect;  //  Make temporary rectangles.
	vector<vector<Point> > contours;  //  Vectors for 'findContours' function.
	vector<Vec4i> hierarchy;

	double ratio, delta_x, delta_y, gradient;  //  Variables for 'Snake' algorithm.
	int number_select, plate_select = -1, plate_width, count, friend_count = 0, number_count = 0, plate_count = 0;

	image = imread(path);  //  Load an image file.   // 4가 조금 안됨

	/*if (image.rows < 500 || image.cols < 500)
	{
		cout << "Image is too small" << endl;
		out.resize(0);
		return out;
	}*/
	if (image.rows > 1000)
	{
		resize(image, image, Size(), 1000.0 / (double)image.rows, 1000.0 / (double)image.rows);
	}

	//imshow("Original", image);

	//waitKey(0);

	image.copyTo(image2);  //  Copy to temporary images.
	image.copyTo(image3);  //  'image2' - to preprocessing, 'image3' - to 'Snake' algorithm.

	cvtColor(image2, image2, CV_BGR2GRAY);  //  Convert to gray image.


	///////////////////////////////////////


	vector<vector<Point> > tempContours;  //  Vectors for 'findContours' function.
	vector<Vec4i> tempHierarchy;
	Mat tempImage, drawing2;
	image2.copyTo(tempImage);
	drawing2 = Mat::zeros(tempImage.size(), CV_8UC3);
	//Canny(tempImage, tempImage, 100, 300, 3); 
	tempImage = CannyEdge(tempImage, 300, 100);
	//Canny(tempImage, tempImage, 100, 300, 3);  //  Getting edges by Canny algorithm.

	findContours(tempImage, tempContours, tempHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point());
	vector<vector<Point> > tempContours_poly(tempContours.size());
	vector<Rect> tempBoundRect(tempContours.size());
	vector<Rect> tempBoundRect2(tempContours.size());

	for (int i = 0; i < tempContours.size(); i++) {
		approxPolyDP(Mat(tempContours[i]), tempContours_poly[i], 1, true);
		tempBoundRect[i] = boundingRect(Mat(tempContours_poly[i]));
	}

	for (int i = 0; i < tempContours.size(); i++) {
		ratio = (double)tempBoundRect[i].height / (double)tempBoundRect[i].width;

		//  Filtering rectangles height/width ratio, and size.

		if ((ratio <= 4) && (ratio >= 0.5) && (tempBoundRect[i].area() >= 100)) {
			drawContours(drawing2, tempContours, i, Scalar(0, 255, 255), 1, 8, tempHierarchy, 0, Point());
			rectangle(drawing2, tempBoundRect[i].tl(), tempBoundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);


			//  Include only suitable rectangles.
			tempBoundRect2[number_count++] = tempBoundRect[i];
		}
	}
	if (tempBoundRect2.size() > 2000)
		blur(image2, image2, Size(5, 5));
	else
		blur(image2, image2, Size(3, 3));
	cout << tempBoundRect2.size() << " < con size" << endl;
	//imshow("Original->Gray", image2);
	//waitKey(0);

	//////////////////////////////////////


	number_count = 0;
	Canny(image2, image2, 100, 300, 3);  //  Getting edges by Canny algorithm.
	//imshow("Original->Gray->Canny", image2);
	//waitKey(0);


	//  Finding contours.

	findContours(image2, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point());
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Rect> boundRect2(contours.size());
	vector<Rect> boundRect3(contours.size());

	//  Bind rectangle to every rectangle.

	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	drawing = Mat::zeros(image2.size(), CV_8UC3);

	for (int i = 0; i < contours.size(); i++) {
		ratio = (double)boundRect[i].height / boundRect[i].width;

		//  Filtering rectangles height/width ratio, and size.

		if ((ratio <= 4) && (ratio >= 0.5) && (boundRect[i].area() >= 100)) {
			drawContours(drawing, contours, i, Scalar(0, 255, 255), 1, 8, hierarchy, 0, Point());
			rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);


			//  Include only suitable rectangles.
			boundRect2[number_count++] = boundRect[i];
		}

		// find candidate plate
		if ((ratio <= 0.5) && (ratio >= 0.1) && (boundRect[i].area() >= 100)) {
			drawContours(drawing, contours, i, Scalar(255, 255, 0), 1, 8, hierarchy, 0, Point());
			rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 255, 0), 1, 8, 0);


			//  Include only suitable rectangles.
			boundRect3[plate_count++] = boundRect[i];
		}
	}

	boundRect2.resize(number_count);  //  Resize refinery rectangle array.
	boundRect3.resize(plate_count);  //  Resize refinery rectangle array.
	//imshow("Original->Gray->Canny->Contours&Rectangles", drawing);
	//waitKey(0);

	//  Bubble Sort accordance with X-coordinate.

	for (int i = 0; i < boundRect2.size() - 1; i++) {
		for (int j = 0; j < (boundRect2.size() - i - 1); j++) {
			if (boundRect2[j].tl().x > boundRect2[j + 1].tl().x) {
				temp_rect = boundRect2[j];
				boundRect2[j] = boundRect2[j + 1];
				boundRect2[j + 1] = temp_rect;
			}
		}
	}

	for (int i = 0; i < boundRect2.size(); i++) {
		rectangle(image3, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0, 255, 0), 1, 8, 0);
		count = 0;

		//  Snake moves to right, for eating his freind.

		for (int j = i + 1; j < boundRect2.size(); j++) {
			delta_x = abs(boundRect2[j].tl().x - boundRect2[i].tl().x);
			if (delta_x > 150)  //  Can't eat snake friend too far ^-^.
				break;
			delta_y = abs(boundRect2[j].tl().y - boundRect2[i].tl().y);
			//  If delta length is 0, it causes a divide-by-zero error.
			if (delta_x == 0) {
				delta_x = 1;
			}
			if (delta_y == 0) {
				delta_y = 1;
			}

			gradient = delta_y / delta_x;  //  Get gradient.
			//cout << gradient << endl;

			if (gradient < 0.25) {  //  Can eat friends only on straight line.
				count += 1;
			}

		}

		//  Find the most full snake.

		if (count > friend_count) {
			bool isInPlate = false;
			for (int k = 0; k < boundRect3.size(); k++)
			{
				if ((boundRect3[k].tl().x <= boundRect2[i].tl().x) && ((boundRect3[k].tl().x + boundRect3[k].width) >= (boundRect2[i].tl().x + boundRect2[i].width))
					&& (boundRect3[k].tl().y <= boundRect2[i].tl().y) && ((boundRect3[k].tl().y + boundRect3[k].height) >= (boundRect2[i].tl().y + boundRect2[i].height)))
				{
					plate_select = k;
					isInPlate = true;
				}
			}

			if (isInPlate)
			{
				number_select = i;  //  Save most full snake number.
				friend_count = count;  //  Renewal number of friends hunting.
				rectangle(image3, boundRect2[number_select].tl(), boundRect2[number_select].br(), Scalar(255, 0, 0), 1, 8, 0);
				plate_width = delta_x;  //  Save the last friend ate position.
			}

		}                           //  It's similar to license plate width, Right?

	}

	//  Drawing most full snake friend on the image.

	rectangle(image3, boundRect2[number_select].tl(), boundRect2[number_select].br(), Scalar(0, 0, 255), 2, 8, 0);
	line(image3, boundRect2[number_select].tl(), Point(boundRect2[number_select].tl().x + plate_width, boundRect2[number_select].tl().y), Scalar(0, 0, 255), 1, 8, 0);
	//imshow("Rectangles on original", image3);

	//waitKey(0);

	// find optimized plate
	int minArea = 210000000;
	for (int k = 0; k < boundRect3.size(); k++)
	{
		if ((boundRect3[k].tl().x <= boundRect2[number_select].tl().x) && ((boundRect3[k].tl().x + boundRect3[k].width) >= (boundRect2[number_select].tl().x + boundRect2[number_select].width))
			&& (boundRect3[k].tl().y <= boundRect2[number_select].tl().y) && ((boundRect3[k].tl().y + boundRect3[k].height) >= (boundRect2[number_select].tl().y + boundRect2[number_select].height)))
		{
			if (boundRect3[k].area() < minArea)
			{
				plate_select = k;
				minArea = boundRect3[k].area();
			}
		}
	}


	//  Shows license plate, and save image file.

	if (plate_select < 0)
		cout << "not find the license plate" << endl;
	else
		imshow("Region of interest", image(boundRect3[plate_select]));
	//imshow("Region of interest", image(Rect(boundRect2[number_select].tl().x - 20, boundRect2[number_select].tl().y - 20, plate_width + 40, plate_width*0.3)));
	//waitKey(0);



	//imwrite("/home/ubuntu01/Desktop/Plates/1-1.JPG", image(Rect(boundRect2[number_select].tl().x - 20, boundRect2[number_select].tl().y - 20, plate_width + 40, plate_width*0.3)));
	image.copyTo(out);
	rectangle(out, boundRect3[plate_select].tl(), boundRect3[plate_select].br(), Scalar(0, 0, 255), 3, 8, 0);
	Rect rr(boundRect3[plate_select].tl(), boundRect3[plate_select].br());
	r = rr;
	imshow("dd", out);
	waitKey(0);
	r.tl().x;
	return out;


}

void ClicenseDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void ClicenseDlg::OnBnClickedOpen()
{
	CFileDialog dlg(TRUE);

	if (isImageOpened)
	{
		if (R != NULL)
		{
			MemFree2D(R, Height);
		}
		if (G != NULL)
		{
			MemFree2D(G, Height);
		}
		if (B != NULL)
		{
			MemFree2D(B, Height);
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.GetFileExt() != "jpg"&&dlg.GetFileExt() != "JPG"&&dlg.GetFileExt() != "png"&&dlg.GetFileExt() != "PNG")
		{
			MessageBox("JPG 또는 PNG 파일이 아닙니다.");
			return;
		}

		FileName = (const char*)dlg.GetPathName();

		if (dlg.GetFileExt() == "jpg" || dlg.GetFileExt() == "JPG" || dlg.GetFileExt() == "png" || dlg.GetFileExt() == "PNG")
		{
			PicOri.SetBitmap(NULL);
			Ori_Img = imread(FileName);
			Height = Ori_Img.rows;
			Width = Ori_Img.cols;

			if (Ori_Img.channels() == 3)
			{
				R = MemAlloc2D(Height, Width, 0);
				G = MemAlloc2D(Height, Width, 0);
				B = MemAlloc2D(Height, Width, 0);

				for (int h = 0; h < Height; h++)
				{
					for (int w = 0; w < Width; w++)
					{
						Vec3b RGB = Ori_Img.at<Vec3b>(h, w);
						R[h][w] = RGB[2];
						G[h][w] = RGB[1];
						B[h][w] = RGB[0];
					}
				}
				//nFormat = FORMAT_RGB;
			}
			isImageOpened = true;
			DisplayImage(Ori_Img, true);
		}
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ClicenseDlg::OnBnClickedExact()
{
	/*Mat last = FindPlate("test/8.jpg");
	if (last.size().area > 0)
		imshow("last", last);
	else
		MessageBox("이미지가 너무 작습니다.");
*/
	Mat Exac_Img;
	// Exac_Img = 번호판추출하는함수();
	Exac_Img = FindPlate(FileName);

	imshow("Display frame", Exac_Img);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ClicenseDlg::OnBnClickedMosaic()
{
	Mat Mos_Img, Ch_Img;
	Ori_Img.copyTo(Mos_Img);
	Ori_Img.copyTo(Ch_Img);
	resize(Ch_Img, Ch_Img, Size(Ch_Img.cols / 15, Ch_Img.rows / 15), 0, 0, CV_INTER_NN);
	resize(Ch_Img, Ch_Img, Size(Ch_Img.cols * 15, Ch_Img.rows * 15), 0, 0, CV_INTER_NN);

	for (int h = r.tl().y; h < r.height + r.tl().y; h++)
	{
		for (int w = r.tl().x; w < r.tl().x + r.width; w++)
		{
			Mos_Img.at<Vec3b>(h, w)[0] = Ch_Img.at<Vec3b>(h, w)[0];
			Mos_Img.at<Vec3b>(h, w)[1] = Ch_Img.at<Vec3b>(h, w)[1];
			Mos_Img.at<Vec3b>(h, w)[2] = Ch_Img.at<Vec3b>(h, w)[2];
		}
	}
	//resize(Mos_Img, Mos_Img, Size(Mos_Img.cols * 2, Mos_Img.rows * 2), 0, 0, CV_INTER_NN);
	imshow("모자이크", Mos_Img);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}