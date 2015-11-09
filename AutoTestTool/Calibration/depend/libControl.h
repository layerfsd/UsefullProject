#ifndef _LIBCONTROL_H
#define _LIBCONTROL_H

#ifdef ROBOTCONTROL_EXPORTS
#define CONTROL_API  __declspec(dllexport) 
#else
#define CONTROL_API  __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum Extend_CMD
{
	SGLFINGER_UP = 0,
	SGLFINGER_DOWN,
	DBLFINGER_UP,
	DBLFINGER_DOWN,
	DBLFINGER_CLOSE,
	DBLFINGER_OPEN,
	LASER_CLOSE,
	LASER_OPEN,
	
	KEY1_UP = 10,
	KEY1_DOWN,
	KEY2_UP,
	KEY2_DOWN,
	KEY3_UP,
	KEY3_DOWN,
	KEY4_UP,
	KEY4_DOWN,

	RESET_ROBOT = 30,
	SET_MARKPOINT,
	GOTO_MARKPOOINT,
};

/************************************************************************
** Initialize the control library [��ʼ�����ƿ�]
***********************************************************************/
CONTROL_API BOOL CONTROL_Init();

/************************************************************************
** Uninitialize the library [����ʼ�����ƿ�]
***********************************************************************/
CONTROL_API void CONTROL_Cleanup();

/************************************************************************
** connect the device [l�����豸]
** pchDevIP: Ip address of cam
** wDevPort: 34567 now
** pchUserName: admin now
** pchPassword: 123456 now
** return value: > 0 connect OK, else failed
***********************************************************************/
CONTROL_API LONG CONTROL_Connect(char *pchDevIP, WORD wDevPort, char *pchUserName, char *pchPassword);

/************************************************************************
** disconnect current connection
***********************************************************************/
CONTROL_API BOOL CONTROL_Disconnect(LONG lConnectID);

/************************************************************************
** preview global cam [ȫ������ͷԤ��]
** lConnectID: the return value of CONTROL_Connect
** return value: > 0 OK, else failed
***********************************************************************/
CONTROL_API LONG CONTROL_RealPlay(LONG lConnectID, HWND hWnd);

/************************************************************************
** move the specified position [�ƶ���]
** lConnectID: the return value of CONTROL_Connect
** lPosX: coordinate X
** lPosY: coordinate Y
** lPosZ: not support now
***********************************************************************/
CONTROL_API BOOL CONTROL_MoveTo(LONG lConnectID, LONG lPosX, LONG lPosY, LONG lPosZ);

/************************************************************************
** move the robot [�ƶ�]
** nFlage: 0 up 1 down 2 left 3 right 4 upleft 5 upright 6 downleft 7 down right
***********************************************************************/
CONTROL_API BOOL CONTROL_Move(LONG lConnectID, INT nFlag, DWORD dwSpeed, BOOL bStop);

/************************************************************************
** Click the device to be test [��������豸]
** lSpeed: speed of click 1~8
** lTimes: times of click
** lTouchTime: the keep time of finger to screen, to be implemented
** lUnTouchTime: the keep time of finger in zero, to be implemented
** �ú�����������ʹ��CONTROL_ClickEx
***********************************************************************/
CONTROL_API BOOL CONTROL_Click(LONG lConnectID, LONG lSpeed, LONG lTimes, LONG lTouchTime, LONG lUnTouchTime);

/************************************************************************
** Multi Click the device to be test [��������豸]
** 
** �ú�����������ʹ��CONTROL_ClickEx
***********************************************************************/
CONTROL_API BOOL CONTROL_MultiClick(LONG lConnectID, LONG lFrequency, LONG lDuration);

/************************************************************************
** �������Ŀ�/����
** type: 1 ���� 2 ���
** triggerMode: 1 �������� 2 ��������
** step: �˲�����0~7��
** caseIndex: �������
** clkFrequency: ���ƽ��
** delay: ������ʱ��x100ms��
** clkTimes: �������
** waveFrequency: ����Ƶ��
** waveTimes: ��������
***********************************************************************/
CONTROL_API BOOL CONTROL_ClickEx(LONG lConnectID, BYTE type, BYTE triggerMode, BYTE step, BYTE caseIndex, BYTE clkFrequency, BYTE delay, WORD clkTimes, WORD waveFrequency, WORD waveTimes);

/************************************************************************
** Slide the device to be test [���������豸]
** wStartX: start position of x
** wStartY: start Position of y
** wEndX: end position of x
** wEndY: end Position of y
***********************************************************************/
CONTROL_API BOOL CONTROL_Slide(LONG lConnectID, WORD wStartX, WORD wStartY, WORD wEndX, WORD wEndY, BYTE timeBefore, BYTE timeAfter, BYTE axisSpeed, BYTE fingerSpeed);

/************************************************************************
** rotate the platform [��תƽ̨]
** bClockwise: TRUE clockwise else anticlockwise
** dwSpeed: speed of rotate
** bStop: TRUE stop rotate else begin rotate
***********************************************************************/
CONTROL_API BOOL CONTROL_PlatformRotate(LONG lConnectID, BOOL bClockwise, DWORD dwSpeed, BOOL bStop);

/************************************************************************
** Query the position of robot [��ѯλ��]
** plPosX: return the X coordinate of robot
** plPosY: return the Y coordinate of robot
** pStatus: other status to be define
***********************************************************************/
CONTROL_API BOOL CONTROL_QueryPos(LONG lConnectID, LONG* plPosX, LONG *plPosY, INT* pStatus, LONG *plPosZ = NULL);

/************************************************************************
** extend control of robot [��λ����չ����]
** lCmd: 
** lReserve: reserve to use
***********************************************************************/
CONTROL_API BOOL CONTROL_Extend(LONG lConnectID, LONG lCmd, LONG lReserve);

/************************************************************************
** [������������]
** type: 0 ���� 1 ���
** triggerMode: 0 �������� 1 ��������
** step: �˲�����0~7��
** caseIndex: �������
** clkFrequency: ���ƽ��
** delay: ������ʱ��x100ms��
** clkTimes: �������
** waveFrequency: ����Ƶ��
** waveTimes: ��������
***********************************************************************/
//CONTROL_API BOOL CONTROL_Trigger(LONG lConnectID, BYTE channel0, BYTE channel1, BYTE channel2, BYTE channel3, LONG lReserve);
CONTROL_API BOOL CONTROL_Trigger(LONG lConnectID, BYTE type, BYTE triggerMode, BYTE step, BYTE caseIndex, BYTE clkFrequency, BYTE delay, WORD clkTimes, WORD waveFrequency, WORD waveTimes);

/************************************************************************
** [�����������]
** Ԥ��
***********************************************************************/
CONTROL_API BOOL CONTROL_SquareWave(LONG lConnectID, LONG lFlag, USHORT frequency1, USHORT frequency2, LONG lCount1, LONG lCount2, LONG lReserve);

#ifdef __cplusplus
}
#endif

#endif