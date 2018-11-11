/*һЩ���ñ�������*/
#ifndef NULL
#define NULL    ((void *)0)
#endif

// vs2017 minwindef.h
#if 0
//BCD=(HEX/10*16)+(HEX%10); //ʮ��������ת����BCD������
#define BCD(v1)					((UINT8)((v1/10)*0x10) + (UINT8)(v1%10))
//HEX=(BCD/16*10)+(BCD%16);//BCD��ת����ʮ��������
#define HEX(v1)					((UINT8)((v1/0x10)*10) + (UINT8)(v1%0x10))
//16 --> 8 x 2
#define HIBYTE(v1)              ((UINT8)((v1)>>8))                      //v1 is UINT16
#define LOBYTE(v1)              ((UINT8)((v1)&0xFF))
//8 x 2 --> 16
#define MAKEWORD(v1,v2)         ((((UINT16)(v1))<<8)+(UINT16)(v2))      //v1,v2 is UINT8
//8 x 4 --> 32
#define MAKELONG(v1,v2,v3,v4)   (UINT32)((v1<<32)+(v2<<16)+(v3<<8)+v4)  //v1,v2,v3,v4 is UINT8
//32 --> 16 x 2
#define YBYTE1(v1)              ((UINT16)((v1)>>16))                    //v1 is UINT32
#define YBYTE0(v1)              ((UINT16)((v1)&0xFFFF))
//32 --> 8 x 4
#define TBYTE3(v1)              ((UINT8)((v1)>>24))                     //v1 is UINT32
#define TBYTE2(v1)              ((UINT8)((v1)>>16))
#define TBYTE1(v1)              ((UINT8)((v1)>>8))
#define TBYTE0(v1)              ((UINT8)((v1)&0xFF))
#endif

//�õ����鳤��
#define GET_ARRAY_LEN(array,len) {len=(sizeof(array)/sizeof(array[0]));}

#define SET_BIT0        0x01
#define SET_BIT1        0x02
#define SET_BIT2        0x04
#define SET_BIT3        0x08
#define SET_BIT4        0x10
#define SET_BIT5        0x20
#define SET_BIT6        0x40
#define SET_BIT7        0x80
#define SET_BIT8        0x0100
#define SET_BIT9        0x0200
#define SET_BIT10       0x0400
#define SET_BIT11       0x0800
#define SET_BIT12       0x1000
#define SET_BIT13       0x2000
#define SET_BIT14       0x4000
#define SET_BIT15       0x8000

#define CLR_BIT0        0xFE
#define CLR_BIT1        0xFD
#define CLR_BIT2        0xFB
#define CLR_BIT3        0xF7
#define CLR_BIT4        0xEF
#define CLR_BIT5        0xDF
#define CLR_BIT6        0xBF
#define CLR_BIT7        0x7F

#define CLR_BIT8        0xFEFF
#define CLR_BIT9        0xFDFF
#define CLR_BIT10       0xFBFF
#define CLR_BIT11       0xF7FF
#define CLR_BIT12       0xEFFF
#define CLR_BIT13       0xDFFF
#define CLR_BIT14       0xBFFF
#define CLR_BIT15       0x7FFF




#define _DEBUG 1
#define _CHKDATA  0  //�Ƿ��ж�����


#define ON 			1
#define OFF 		0



#define HEAD 0xBD
#define LIFTHEAD 0xED
#define BLUE_HEAD 0x0D
#define RING_HEAD 0xCD
#define COMM_SIZE 160 //���ͳ���
#define COMM_LEN_MAX COMM_SIZE-10
#define COMM_FIX_SIZE 10  //���ڹ̶����ȵ��������ֽ���
#define MINCOMMSIZE      4   //��С�������Ϊ8
#define COMM_INFO_DEC_LEN 11 //��ȡ��Ϣλʱ����ȥ�ĳ���
#define COMM_MAX  3 //���10��
#define COMM_SENDMAX 3 //��෢��3��
#define FADDR_P 0x03  //���ͷ���ַλ��
#define TADDR_P 0x04  //���շ���ַλ��
#define IADDR_P 0x06  //�洢��Ϣ��λ��

#define MAXPORTIN 10
#define MAXPORTOUT 5

#define BUSWAIT 11 //�ڲ��ȴ�100MS
#define NETWAIT 101 //����ȴ�1S
#define ACKTIME 3 //��ʱ50MS���ҽ��лظ���������շ�������

#define AIR_WIND_NULL 0

#define WAITTIMEOUT 30 //���ڽ����ֽڳ�ʱ 10MS

#define REBOOTTIME 100 // ����100��󣬸�λһ�� 



typedef enum
{
  RATE_9600 = 0,
  RATE_115200
} BAUD_RATE;




/*******************************************************************************
˵���� ö��
*******************************************************************************/



typedef enum DIRECT
{
	DIR_SEND=0X01, //���͵�����
	DIR_ACK	       //�ظ�������
}DIRECT_ENUM;


/**************��������*******************/
typedef enum PC_COMM
{
	PC_START = 1,
	PC_KEY,
	PC_NET,
	PC_WIFI,
	PC_4G,
	PC_ID,
	PC_IC,
	PC_LOCK,
	PC_FC,
	PC_SD,
	PC_FLASH,
	PC_AUDIO_IN,
	PC_AUDIO_OUT,
	PC_QUIT,
	PC_EXIST,
	PC_I,
	PC_V,
	PC_BURN,
	PC_BURN_OVER,
	PC_POWER
}PC_COMM_ENUM;


/************************** �ṹ����� ********************/
typedef struct _COM_SETTING
{
	bool isOpen;
	UINT  port;
	UINT  baud;
	
}_COM_SETTING_STRU;


/**************ȫ�ֱ���*******************/
typedef struct LOCAL
{
	int init;

}LOCAL_STRU;



/**************�������ͽṹ��*******************/


typedef struct _SEND_COMM
{
	int isValid;
	int port;
	int SendNum; //���ʹ���
	int SendTime; //����ʱ�����
	int WaitTime; //�ȴ��ظ�ʱ��
	int circle; //ѭ����
	int len;   //���ͳ���
	int sum;
	int buf[COMM_SIZE];
}_SEND_COMM_STRU;


typedef struct _REC_COMM
{
	int isstart; //���յ�����
	int isover; //�Ƿ�ʱ
	int isupdate;//������
	int ack; //���յ��ظ�
	int accord; //�������͵�����
	int cnt;
	int p; //��ǰ���յ�ָ��
	int len;
	int direct;//����
	int type;
	int cmd;
	int circle;
	int fcs;
	int buf[COMM_SIZE];
}_REC_COMM_STRU;

typedef struct _WAIT_A20_ACK
{
	int start;
	int over;

	int cnt;
}_WAIT_A20_ACK_STRU;


#ifndef _GLOBAL_H
#define _GLOBAL_H

LOCAL_STRU Local;
_SEND_COMM_STRU Multi_comm_Buff[COMM_MAX];
_REC_COMM_STRU bus;
_WAIT_A20_ACK_STRU wait_ack;
_COM_SETTING_STRU set;

#else

extern LOCAL_STRU Local;
extern _SEND_COMM_STRU Multi_comm_Buff[COMM_MAX];
extern _REC_COMM_STRU bus;
extern _WAIT_A20_ACK_STRU wait_ack;
extern _COM_SETTING_STRU set;
#endif
