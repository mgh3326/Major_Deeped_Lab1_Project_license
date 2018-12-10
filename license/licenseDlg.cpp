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
	ON_BN_CLICKED(IDCANCEL, &ClicenseDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_Open, &ClicenseDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_Exact, &ClicenseDlg::OnBnClickedExact)
	ON_BN_CLICKED(IDC_Mosaic, &ClicenseDlg::OnBnClickedMosaic)
	//   ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_Del, &ClicenseDlg::OnBnClickedDel)
END_MESSAGE_MAP()


// ClicenseDlg 메시지 처리기

BOOL ClicenseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	SetIcon(m_hIcon, TRUE);        
	SetIcon(m_hIcon, FALSE);      

	return TRUE;  
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

void ClicenseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

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
	else
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

HCURSOR ClicenseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
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
	Mat input_image, temp_image, rectangle_image, drawing;  //  Mat 객체들을 생성 해둡니다.
	Rect rect, temp_rect;  // Rect 객체들을 생성 해둡니다.
	vector<vector<Point> > contours;  //   contours 를 담을 벡터입니다.
	vector<Vec4i> hierarchy;

	double ratio, delta_x, delta_y, gradient;  // 변수 생성
	int number_select = -1, plate_select = -1, plate_width, count, friend_count = 0, number_count = 0, plate_count = 0;// 변수 생성 및 초기화

	input_image = imread(path);  // 이미지를 입력 받습니다.

	if (input_image.cols < 500)//너무 작은 이미지를 제외 하도록 하였습니다.
	{
		cout << "Image is too small" << endl;
		out.resize(0);
		return out;
	}
	if (input_image.rows > 1000)//너무 큰 이미지는 시간이 너무 오래 걸려서 resize 해주었습니다.
	{
		resize(input_image, input_image, Size(), 1000.0 / (double)input_image.rows, 1000.0 / (double)input_image.rows);
	}

	//imshow("Original", input_image);

	//waitKey(0);

	input_image.copyTo(temp_image);  //  전처리를 위해 복사합니다.
	input_image.copyTo(rectangle_image);  // 사각형을 그릴 이미지를 생성합니다.

	cvtColor(temp_image, temp_image, CV_BGR2GRAY);  //  흑백 이미지로 바꿉니다.




	vector<vector<Point> > tempContours;  // Contours를 담을 벡터입니다.
	vector<Vec4i> tempHierarchy;
	Mat tempImage, drawing2;
	temp_image.copyTo(tempImage);
	drawing2 = Mat::zeros(tempImage.size(), CV_8UC3);
	tempImage = CannyEdge(tempImage, 300, 100); // 엣지를 검출합니다.

	findContours(tempImage, tempContours, tempHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point());//엣지 내에서 객체를 검출합니다.
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

		//if ((ratio <= 4) && (ratio >= 0.5) && (tempBoundRect[i].area() >= 100)) {
		drawContours(drawing2, tempContours, i, Scalar(0, 255, 255), 1, 8, tempHierarchy, 0, Point());
		rectangle(drawing2, tempBoundRect[i].tl(), tempBoundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);


		//  Include only suitable rectangles.
		tempBoundRect2[number_count++] = tempBoundRect[i];
		//}
	}
	//imshow("temp", drawing2);
	if (tempBoundRect2.size() > 2800)//적절한 blur의 크기를 tempBoundRect2.size() 기준으로 선택합니다.
		blur(temp_image, temp_image, Size(5, 5));
	else
		blur(temp_image, temp_image, Size(3, 3));
	cout << tempBoundRect2.size() << " < con size" << endl;
	//imshow("Original->Gray", temp_image);
	//waitKey(0);



	number_count = 0;

	temp_image = CannyEdge(temp_image, 300, 100);// 엣지를 검출합니다.
	//imshow("Original->Gray->Canny", temp_image);
	//waitKey(0);


	//  Finding contours.

	findContours(temp_image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point());//엣지 내에서 객체를 탐지합니다.
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Rect> boundRect2(contours.size());
	vector<Rect> boundRect3(contours.size());

	//  Bind rectangle to every rectangle.

	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	drawing = Mat::zeros(temp_image.size(), CV_8UC3);

	for (int i = 0; i < contours.size(); i++) {
		ratio = (double)boundRect[i].height / boundRect[i].width;

		//  Filtering rectangles height/width ratio, and size.

		if ((ratio <= 4) && (ratio >= 0.6) && (boundRect[i].area() >= 100)) {//번호판의 크기와 ratio에 맞도록 조건문으로 번호판 만을 boundRect2에 담도록 하였습니다.
			drawContours(drawing, contours, i, Scalar(0, 255, 255), 1, 8, hierarchy, 0, Point());
			rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);


			//  Include only suitable rectangles.
			boundRect2[number_count++] = boundRect[i];
		}

		// find candidate plate
		if ((ratio <= 0.6) && (ratio >= 0.1) && (boundRect[i].area() >= 100)) {//번호판의 크기와 ratio에 맞도록 조건문으로 번호판 만을 boundRect2에 담도록 하였습니다.
			drawContours(drawing, contours, i, Scalar(255, 255, 0), 1, 8, hierarchy, 0, Point());
			rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 255, 0), 1, 8, 0);


			//  Include only suitable rectangles.
			boundRect3[plate_count++] = boundRect[i];
		}
	}

	boundRect2.resize(number_count);  //number_count에 맞게 resize 해줍니다.
	boundRect3.resize(plate_count);  // plate_count에 맞게 resize 해줍니다.
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
		rectangle(rectangle_image, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0, 255, 0), 1, 8, 0);
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
				rectangle(rectangle_image, boundRect2[number_select].tl(), boundRect2[number_select].br(), Scalar(255, 0, 0), 1, 8, 0);
				plate_width = delta_x;  //  Save the last friend ate position.
			}

		}                           //  It's similar to license plate width, Right?

	}
	//waitKey(0);
	//  Drawing most full snake friend on the input_image.
	if (number_select == -1)
	{
		cout << "not find the license plate" << endl;
		out.resize(0);
		return out;
	}

	rectangle(rectangle_image, boundRect2[number_select].tl(), boundRect2[number_select].br(), Scalar(0, 0, 255), 2, 8, 0);
	line(rectangle_image, boundRect2[number_select].tl(), Point(boundRect2[number_select].tl().x + plate_width, boundRect2[number_select].tl().y), Scalar(0, 0, 255), 1, 8, 0);
	//imshow("Rectangles on original", rectangle_image);

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


	//  Shows license plate, and save input_image file.

	if (plate_select < 0)//번호판을 찾지 못했을때 예외처리 하는 부분입니다.
	{
		cout << "not find the license plate" << endl;
		out.resize(0);
		return out;
	}
	//else
	   //imshow("Region of interest", input_image(boundRect3[plate_select]));
	//imshow("Region of interest", input_image(Rect(boundRect2[number_select].tl().x - 20, boundRect2[number_select].tl().y - 20, plate_width + 40, plate_width*0.3)));
	//waitKey(0);



	//imwrite("/home/ubuntu01/Desktop/Plates/1-1.JPG", input_image(Rect(boundRect2[number_select].tl().x - 20, boundRect2[number_select].tl().y - 20, plate_width + 40, plate_width*0.3)));
	Rect rr(boundRect3[plate_select].tl(), boundRect3[plate_select].br());
	r = rr;
	input_image.copyTo(out);
	rectangle(out, boundRect3[plate_select].tl(), boundRect3[plate_select].br(), Scalar(0, 0, 255), 3, 8, 0);
	//imshow("dd", out);
	//waitKey(0);

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
			Ori_Img = imread(FileName);
			Proc_Img = imread("re.PNG");
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
			}
			isImageOpened = true;
			DisplayImage(Ori_Img, true);
		}
	}
}


void ClicenseDlg::OnBnClickedExact()
{
	Mat Exac_Img;
	// Exac_Img = 번호판추출하는함수();
	Exac_Img = FindPlate(FileName);

	if (Exac_Img.size().area() > 0)
		imshow("license", Exac_Img(r));
	else
		MessageBox("이미지가 작거나 번호판을 찾지 못했습니다.");
}


void ClicenseDlg::OnBnClickedMosaic()
{
	Mat Mos_Img, Ch_Img;
	Ori_Img.copyTo(Mos_Img);
	Ori_Img.copyTo(Ch_Img);
	if (Ori_Img.rows > 1000)
	{
		resize(Mos_Img, Mos_Img, Size(), 1000.0 / (double)Ori_Img.rows, 1000.0 / (double)Ori_Img.rows);
		resize(Ch_Img, Ch_Img, Size(), 1000.0 / (double)Ori_Img.rows, 1000.0 / (double)Ori_Img.rows);
	}

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
	imshow("모자이크", Mos_Img);
}

void ClicenseDlg::OnBnClickedDel()
{
	DisplayImage(Proc_Img, false);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
