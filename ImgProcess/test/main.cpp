#if 0
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

int main()
{
    Mat src = imread("test.jpg",1);
    int width=src.cols;
    int heigh=src.rows;
    RNG rng;
    Mat img(src.size(),CV_8UC3);
    for (int y=0; y<heigh; y++)
    {
        uchar* P0  = src.ptr<uchar>(y);
        uchar* P1  = img.ptr<uchar>(y);
        for (int x=0; x<width; x++)
        {
            float B=P0[3*x];
            float G=P0[3*x+1];
            float R=P0[3*x+2];
            float newB=abs(B-G+B+R)*G/256;
            float newG=abs(B-G+B+R)*R/256;
            float newR=abs(G-B+G+R)*R/256;
            if(newB<0)newB=0;
            if(newB>255)newB=255;
            if(newG<0)newG=0;
            if(newG>255)newG=255;
            if(newR<0)newR=0;
            if(newR>255)newR=255;
            P1[3*x] = (uchar)newB;
            P1[3*x+1] = (uchar)newG;
            P1[3*x+2] = (uchar)newR;
        }
    }
    Mat gray;
    cvtColor(img,gray,CV_BGR2GRAY);
    normalize(gray,gray,255,0,CV_MINMAX);
    imshow("������",gray);
    waitKey();
    imwrite("������.jpg",gray);
    return 1;
}
#else
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

//#pragma comment(lib, "cv.lib")
//#pragma comment(lib, "cxcore.lib")
//#pragma comment(lib, "highgui.lib")

/*int main( int argc, char** argv )  
{
    IplImage* src = cvLoadImage("test3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contour = NULL;
    cvThreshold(src, src, 120, 255, CV_THRESH_BINARY);	// ��ֵ��
    cvNamedWindow("Source", 1);
    cvShowImage("Source", src);
    // ��ȡ����
    int contour_num = cvFindContours(src, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    cvZero(dst);		// �������
    CvSeq *_contour = contour; 
    double maxarea = 0;
    double minarea = 100;
    int m = 0;
    for( ; contour != 0; contour = contour->h_next )  
    {
        double tmparea = fabs(cvContourArea(contour));
        if(tmparea < minarea)
        {
            //cvSeqRemove(contour, 0); // ɾ�����С���趨ֵ������
            continue;
        }
        CvRect aRect = cvBoundingRect( contour, 0 ); 
        if ((aRect.width/aRect.height)<1)  
        {  
            //cvSeqRemove(contour, 0); //ɾ����߱���С���趨ֵ������
            continue;
        }  
        if(tmparea > maxarea)  
        {  
            maxarea = tmparea;
        }  
        m++;
        // ����һ��ɫ��ֵ
        CvScalar ExtColor = CV_RGB( 0, 0, 255 );
        CvScalar HoleColor = CV_RGB(0, 255, 0);

        //max_level �������������ȼ�������ȼ�Ϊ0�����Ƶ��������������Ϊ1��������������������ͬ�ļ���������
        //���ֵΪ2�����е�����������ȼ�Ϊ2����������ͬ�����������е�һ���������������
        //���ֵΪ����������������ͬ�������������������ֱ������Ϊabs(max_level)-1��������
        cvDrawContours(dst, contour, ExtColor, HoleColor, -1, 1, 8);	//�����ⲿ���ڲ�������
    }  
    contour = _contour;
    int count = 0;
    for(; contour != 0; contour = contour->h_next)
    {  
        count++;
        double tmparea = fabs(cvContourArea(contour));
        if (tmparea == maxarea)  
        {  
            CvScalar color = CV_RGB( 255, 0, 0);
            //cvDrawContours(dst, contour, color, color, -1, 1, 8);
        }  
    }  
    printf("The total number of contours is:%d", count);
    cvNamedWindow("Components", 1);
    cvShowImage("Components", dst);
    cvWaitKey(0);
    cvDestroyWindow("Source");
    cvReleaseImage(&src);
    cvDestroyWindow("Components");
    cvReleaseImage(&dst);

    return 0;
}*/


/************************************************************************/  
/* ��ȡ�������ַ����Աȼ���������'���ȼ�'����                         */  
/************************************************************************/ 
#include "cv.h"  
#include "highgui.h"  
  
int main()  
{  
    IplImage* img = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);  
    IplImage* img_temp = cvCreateImage(cvGetSize(img), 8, 1);  
      
    cvThreshold(img, img, 128, 255, CV_THRESH_BINARY);  
      
    CvMemStorage* mem_storage = cvCreateMemStorage(0);  
    CvSeq *first_contour = NULL, *c = NULL;  
      
    
    //////////////////////////////////////////////////////////////////////////  
    // 1��  
    cvNamedWindow("contour1");  
    cvCopyImage(img, img_temp);  
	double t = (double)cvGetTickCount();
    cvFindContours(img_temp, mem_storage, &first_contour);  
    cvZero(img_temp);  
    cvDrawContours(  
        img_temp,   
        first_contour,  
        cvScalar(100),  
        cvScalar(100),  
        1  
        );  
	t = (double)cvGetTickCount() - t; 
    cvShowImage("contour1", img_temp);  
  
    printf("run1 = %gms\n", t/(cvGetTickFrequency()*1000.));  
      
    cvClearMemStorage(mem_storage);  
      
    
    //////////////////////////////////////////////////////////////////////////  
    // 2��  
    cvNamedWindow("contour2");  
    cvCopyImage(img, img_temp);  
	t = (double)cvGetTickCount();
    CvContourScanner scanner = cvStartFindContours(img_temp, mem_storage);  
    while (cvFindNextContour(scanner));  
    first_contour = cvEndFindContours(&scanner);  
      
    cvZero(img_temp);  
    cvDrawContours(  
        img_temp,   
        first_contour,  
        cvScalar(100),  
        cvScalar(100),  
        1  
        );  
	t = (double)cvGetTickCount() - t; 
    cvShowImage("contour2", img_temp);  
	
	printf("run2 = %gms\n", t/(cvGetTickFrequency()*1000.));  
      
    cvClearMemStorage(mem_storage);  
    cvReleaseImage(&img);  
    cvReleaseImage(&img_temp);  
  
    cvWaitKey();  

    /************************************************************************/  
    /* ������ run1 = 16.1431ms run2 = 15.8677ms (�ο�)
	   �������Կ϶��������㷨ʱ�临�Ӷ�����ͬ��                                     */  
    /************************************************************************/  
      
    //////////////////////////////////////////////////////////////////////////  
    // �������ַ�������˶���������ȡ,������������������cvDrawContours��ʹ��  
    // ��cvDrawContours ������5������Ϊ max_level ����ICVL��������:  
    //  
    // �������������ȼ�������ȼ�Ϊ0�����Ƶ��������������Ϊ1��������������������ͬ�ļ�����������  
    // ���ֵΪ2�����е�����������ȼ�Ϊ2����������ͬ�����������е�һ��������������֡����ֵΪ������  
    // ����������ͬ�������������������ֱ������Ϊabs(max_level)-1����������  
    //  
    // ���źö���߳��ζ��޷����ȼ��ĺ���,���Ҳ���ʱ������>=1 ������Ч������һ��  
    // ֻ����ȡ����ʱ�����ȡģʽ��Ϊ CV_RETR_CCOMP CV_RETR_TREE ʱ���������������  
    //  
    // ����FindContours ������������������ȡģʽ(mode)������������:  
    // CV_RETR_CCOMP - ��ȡ�������������ҽ�����֯Ϊ����� hierarchy: ����Ϊ��ͨ�����Χ�߽磬�β�Ϊ�����ڲ�߽硣   
    // CV_RETR_TREE - ��ȡ���������������ع�Ƕ��������ȫ�� hierarchy   
    //   
    // �����õ�һ�ַ������в���  
  
    cvNamedWindow("contour_test");  
    cvNamedWindow("contour_raw");  
    img = cvLoadImage("test.jpg", CV_LOAD_IMAGE_GRAYSCALE);  
    cvShowImage("contour_raw", img);  
    cvThreshold(img, img, 128, 255, CV_THRESH_BINARY);  
    img_temp = cvCloneImage(img);  
    cvFindContours(  
        img_temp,   
        mem_storage,   
        &first_contour,  
        sizeof(CvContour),  
        CV_RETR_EXTERNAL           //#1 ���������  
        );  
  
    cvZero(img_temp);
#if 0
    cvDrawContours(  
        img_temp,   
        first_contour,  
        cvScalar(100),
        cvScalar(100),  
        1                       //#2 ���������
        );
#else
    int nCount = 0;
    for(CvSeq *pContour = first_contour; pContour != NULL; pContour = pContour->h_next){
        double tmparea = fabs(cvContourArea(pContour));
        if(tmparea < 2000)   
        {
            //cvClearSeq(pContour);
            continue;
        }
        cvDrawContours(img_temp, pContour, cvScalar(100), cvScalar(100), 0);
        nCount++;
    }
    printf("draw count: %d\n", nCount);
#endif
    cvShowImage("contour_test", img_temp);  
    /************************************************************************/  
    /* (1, 2) = (CV_RETR_CCOMP, 1)  ��ͼ1 
       (1, 2) = (CV_RETR_CCOMP, 2)  ��ͼ2 
       (1, 2) = (CV_RETR_TREE, 1)   ��ͼ3 
       (1, 2) = (CV_RETR_TREE, 2)   ��ͼ4 
       (1, 2) = (CV_RETR_TREE, 6)   ��ͼ5 
       ������CV_RETR_CCOMP ֻ��ͼ���Ϊ�������,����ʹβ�,һ�ȼ�����ֻƥ��������ӽ� 
       ���ڲ�������2�ȼ� 
       CV_RETR_TREE ��������⵽�ڰ��ȼ�1 - n ȫ������         
       CV_RETR_LIST ȫ��������Ϊ1��                        */  
    /************************************************************************/  
  
    cvWaitKey();  
    cvReleaseImage(&img);  
    cvReleaseImage(&img_temp);  
    cvReleaseMemStorage(&mem_storage);  
    cvDestroyAllWindows();  
    return 0;  
}
#endif

