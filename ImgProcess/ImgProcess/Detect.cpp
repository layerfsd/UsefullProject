#include "StdAfx.h"
#include "Detect.h"

using namespace cv;

CDetect::CDetect(void)
{
}


CDetect::~CDetect(void)
{
}

/*
* ���ڲ��� http://blog.csdn.net/jia20003/article/details/7459534
* (R, G, B) is classified as skin if
* R > 95 and G > 40 and B > 20and max{R, G, B} �C min{R, G, B} > 15 and |R-G| > 15
* and R > G and R > B
*/
BOOL CDetect::detectSkin(IplImage *pSrc)
{
    if(!pSrc)
        return FALSE;
    if(pSrc->nChannels != 3 && pSrc->nChannels != 4)
        return FALSE;
    unsigned char *pData = (unsigned char*)pSrc->imageData;
    int width = pSrc->width;
    int height = pSrc->height;
    int widthStep = pSrc->widthStep;
    int pixelsR, pixelsG, pixelsB;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            unsigned char *pTemp = pData+i*widthStep+j*pSrc->nChannels;
            pixelsB = pTemp[0];
            pixelsG = pTemp[1];
            pixelsR = pTemp[2];

            int max = MAX(pixelsR, MAX(pixelsG, pixelsB));
            int min = MIN(pixelsR, MIN(pixelsG, pixelsB));
            int rg = abs(pixelsR - pixelsG);
            if(pixelsR > 95 && pixelsG > 40 && pixelsB > 20 && pixelsG > 15 && 
                (max - min) > 15 && pixelsR > pixelsG && pixelsR > pixelsB) {
                    ;
            } else {
                pTemp[0]=0;
                pTemp[1]=0;
                pTemp[2]=0;
            }
        }
    }


    return TRUE;
}

/*
* http://www.cnblogs.com/tornadomeet/archive/2012/08/03/2621814.html http://blog.csdn.net/icvpr/article/details/8454439
* detectMultiScale(const Mat& img, CV_OUT vector<Rect>& foundLocations, double hitThreshold=0, Size winStride=Size(), 
*     Size padding=Size(), double scale=1.05, double finalThreshold=2.0, bool useMeanshiftGrouping = false)
* img: ����ͼƬ foundLocations: ��⵽Ŀ�������б� hitThreshold: �����ڲ�����Ϊ����Ŀ�����ֵ��Ҳ���Ǽ�⵽��������SVM���೬ƽ��ľ���
* winStride: ��������ÿ���ƶ��ľ��룬�����ǿ��ƶ��������� padding: ͼ������Ĵ�С scale: ����ϵ������������ÿ�����ӵı���
* finalThreshold: ����ֵ����У��ϵ������һ��Ŀ�걻������ڼ�����ʱ���ò�����ʱ�����˵������ã�Ϊ0ʱ��ʾ�����������
*/
BOOL CDetect::detectPedestrian(IplImage *pSrc)
{
    if(!pSrc)
        return FALSE;

    Mat mtx(pSrc);
    // 1. ����HOG����
    HOGDescriptor hog; // ����Ĭ�ϲ���

    // 2. ����SVM������
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());   // �����Ѿ�ѵ���õ����˼�������

    // 3. �ڲ���ͼ���ϼ����������
    std::vector<Rect> regions, found_filtered;
    hog.detectMultiScale(mtx, regions, 0, Size(8,8), Size(32,32), 1.05, 1);

#if 0
    // ��ʾ
    for (size_t i = 0; i < regions.size(); i++)
    {
        if(i%3==0)
            rectangle(mtx, regions[i], Scalar(0,0,255), 2);
        else if(i%3==1)
            rectangle(mtx, regions[i], Scalar(0,255,0), 2);
        else
            rectangle(mtx, regions[i], Scalar(255,0,0), 2);
    }
#else
    size_t i, j;
    for (i = 0; i < regions.size(); i++ )
    {
        Rect r = regions[i];

        //��������for������ҳ�����û��Ƕ�׵ľ��ο�r,������found_filtered��,�����Ƕ�׵�
        //��,��ȡ���������Ǹ����ο����found_filtered��
        for(j = 0; j <regions.size(); j++)
            if(j != i && (r&regions[j])==r)
                break;
        if(j == regions.size())
            found_filtered.push_back(r);
    }

    //��ͼƬimg�ϻ������ο�,��Ϊhog�����ľ��ο��ʵ�������Ҫ��΢��Щ,����������Ҫ
    //��һЩ����
    for(i = 0; i <found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(mtx, r.tl(), r.br(), Scalar(0, 255, 0), 3);
    }
#endif

    //imshow("hog", mtx);

    return TRUE;
}

