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
    imshow("连环画",gray);
    waitKey();
    imwrite("连环画.jpg",gray);
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
    cvThreshold(src, src, 120, 255, CV_THRESH_BINARY);	// 二值化
    cvNamedWindow("Source", 1);
    cvShowImage("Source", src);
    // 提取轮廓
    int contour_num = cvFindContours(src, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    cvZero(dst);		// 清空数组
    CvSeq *_contour = contour; 
    double maxarea = 0;
    double minarea = 100;
    int m = 0;
    for( ; contour != 0; contour = contour->h_next )  
    {
        double tmparea = fabs(cvContourArea(contour));
        if(tmparea < minarea)
        {
            //cvSeqRemove(contour, 0); // 删除面积小于设定值的轮廓
            continue;
        }
        CvRect aRect = cvBoundingRect( contour, 0 ); 
        if ((aRect.width/aRect.height)<1)  
        {  
            //cvSeqRemove(contour, 0); //删除宽高比例小于设定值的轮廓
            continue;
        }  
        if(tmparea > maxarea)  
        {  
            maxarea = tmparea;
        }  
        m++;
        // 创建一个色彩值
        CvScalar ExtColor = CV_RGB( 0, 0, 255 );
        CvScalar HoleColor = CV_RGB(0, 255, 0);

        //max_level 绘制轮廓的最大等级。如果等级为0，绘制单独的轮廓。如果为1，绘制轮廓及在其后的相同的级别下轮廓
        //如果值为2，所有的轮廓。如果等级为2，绘制所有同级轮廓及所有低一级轮廓，诸此种种
        //如果值为负数，函数不绘制同级轮廓，但会升序绘制直到级别为abs(max_level)-1的子轮廓
        cvDrawContours(dst, contour, ExtColor, HoleColor, -1, 1, 8);	//绘制外部和内部的轮廓
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
/* 提取轮廓两种方法对比及绘制轮廓'最大等级'分析                         */  
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
    // 1、  
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
    // 2、  
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
    /* 经测试 run1 = 16.1431ms run2 = 15.8677ms (参考)
	   不过可以肯定这两中算法时间复杂度是相同的                                     */  
    /************************************************************************/  
      
    //////////////////////////////////////////////////////////////////////////  
    // 上述两种方法完成了对轮廓的提取,如想绘制轮廓都得配合cvDrawContours来使用  
    // 而cvDrawContours 函数第5个参数为 max_level 经查ICVL含义如下:  
    //  
    // 绘制轮廓的最大等级。如果等级为0，绘制单独的轮廓。如果为1，绘制轮廓及在其后的相同的级别下轮廓。  
    // 如果值为2，所有的轮廓。如果等级为2，绘制所有同级轮廓及所有低一级轮廓，诸此种种。如果值为负数，  
    // 函数不绘制同级轮廓，但会升序绘制直到级别为abs(max_level)-1的子轮廓。  
    //  
    // 相信好多读者初次都无法理解等级的含义,而且测试时候输入>=1 的整数效果几乎一样  
    // 只有提取轮廓时候的提取模式设为 CV_RETR_CCOMP CV_RETR_TREE 时这个参数才有意义  
    //  
    // 经查FindContours 函数里面这样介绍提取模式(mode)的这两个参数:  
    // CV_RETR_CCOMP - 提取所有轮廓，并且将其组织为两层的 hierarchy: 顶层为连通域的外围边界，次层为洞的内层边界。   
    // CV_RETR_TREE - 提取所有轮廓，并且重构嵌套轮廓的全部 hierarchy   
    //   
    // 下面用第一种方法进行测试  
  
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
        CV_RETR_EXTERNAL           //#1 需更改区域  
        );  
  
    cvZero(img_temp);
#if 0
    cvDrawContours(  
        img_temp,   
        first_contour,  
        cvScalar(100),
        cvScalar(100),  
        1                       //#2 需更改区域
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
    /* (1, 2) = (CV_RETR_CCOMP, 1)  如图1 
       (1, 2) = (CV_RETR_CCOMP, 2)  如图2 
       (1, 2) = (CV_RETR_TREE, 1)   如图3 
       (1, 2) = (CV_RETR_TREE, 2)   如图4 
       (1, 2) = (CV_RETR_TREE, 6)   如图5 
       经分析CV_RETR_CCOMP 只把图像分为两个层次,顶层和次层,一等级轮廓只匹配与其最接近 
       的内侧轮廓即2等级 
       CV_RETR_TREE 则从轮廓外到内按等级1 - n 全部分配         
       CV_RETR_LIST 全部轮廓均为1级                        */  
    /************************************************************************/  
  
    cvWaitKey();  
    cvReleaseImage(&img);  
    cvReleaseImage(&img_temp);  
    cvReleaseMemStorage(&mem_storage);  
    cvDestroyAllWindows();  
    return 0;  
}
#endif

