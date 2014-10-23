#ifndef LP_ENGINERY_H
#define LP_ENGINERY_H
#include "LP_Advanced.h"

class LP_Reference
{
public:
	int maxV;     //����ٶ�
	int maxA;  //�����ٶ�
	int jumpV;  //�����ٶ�
	double cornerDistance;  //�սǾ���,ֵԽС�ս�Խ����ֵԽ�󣬹ս�ԽԲ��
};
class LP_PointSpeed
{
public:
	double px;
	double py;
	double pz;
	double vx;
	double vy;
	double vz;
};
struct NC_Data  
{
	long	N;					          //����κ�
	unsigned char    Code;				        //��������
	unsigned char    PlaneGroup;			    //Բ��ƽ��
	double 	x,y,z,R,cx,cy;	    //X��Y��Z���꣬Բ�ģ��뾶
	double  F;                  //�û�Ŀ���ٶ�
	double  v;					//�ٶ�Ԥ�����õ����յ��ٶ�
};
class LP_EngineryBuffer         //����ӹ����ݵĻ�����
{
public:
	void push(NC_Data& ncdata);
private:
	vector<NC_Data> data;
};
class LP_CAM_Handle           //һ��HandleΪͬһ��ɫ��һ��ͼ��
{
public:
	void push(LP_Graph * graph);
    inline	int getIndex();
	bool empty();
private:
	int index;               //�ӹ�˳�����������Ӧ����ɫ������
	LP_PointSpeed startP;             //����λ�ú������������ٶȡ�
	LP_PointSpeed endP;            //�յ��λ�ú������������ٶȡ�
	vector<LP_Graph *> graphics;
};
class LP_CAM_Group        //һ��GroupΪһ���ӹ��飬���а�����ͬ����ɫ��ͼ�Σ�����Ϊһ���ӹ���Ԫ��������ͼ�ε���ɫ˳�μӹ���Ϻ��ٽ�����һ���ӹ��顣
{
public:
	void push(LP_CAM_Handle *handle);
	LP_CAM_Handle* find(int index);
private:
	vector<LP_CAM_Handle *> handles;
};
class LP_Enginery
{
	LP_Enginery(); 
};
#endif