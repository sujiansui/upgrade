/*一些常用变量定义*/
#ifndef NULL
#define NULL    ((void *)0)
#endif

// vs2017 minwindef.h
#if 0
//BCD=(HEX/10*16)+(HEX%10); //十六进制数转换成BCD码数据
#define BCD(v1)					((UINT8)((v1/10)*0x10) + (UINT8)(v1%10))
//HEX=(BCD/16*10)+(BCD%16);//BCD码转换成十六进制数
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

//得到数组长度
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
#define _CHKDATA  0  //是否判断数据


#define ON 			1
#define OFF 		0


#define COMNULL 0xff
#define HEAD 0xBD
#define LIFTHEAD 0xED
#define BLUE_HEAD 0x0D
#define RING_HEAD 0xCD
#define COMM_SIZE 160 //发送长度
#define COMM_LEN_MAX COMM_SIZE-10
#define COMM_FIX_SIZE 10  //串口固定长度的数据总字节数
#define MINCOMMSIZE      4   //最小的命令长度为8
#define COMM_INFO_DEC_LEN 11 //获取信息位时，减去的长度
#define COMM_MAX  3 //最多10组
#define COMM_SENDMAX 3 //最多发送3遍
#define FADDR_P 0x03  //发送方地址位置
#define TADDR_P 0x04  //接收方地址位置
#define IADDR_P 0x06  //存储信息的位置

#define MAXPORTIN 10
#define MAXPORTOUT 5

#define BYTEWAIT 100 //字节超时100MS
#define ACKWAIT 3000 //等待回复1S
#define ACKTIME 3 //延时50MS左右进行回复，避免接收方处理不来

#define AIR_WIND_NULL 0

#define WAITTIMEOUT 30 //串口接收字节超时 10MS

#define REBOOTTIME 100 // 工作100秒后，复位一次 



typedef enum
{
  RATE_9600 = 0,
  RATE_115200
} BAUD_RATE;




/*******************************************************************************
说明： 枚举
*******************************************************************************/



typedef enum DIRECT
{
	DIR_SEND=0X01, //发送的数据
	DIR_ACK	       //回复的数据
}DIRECT_ENUM;


/**************串口数据*******************/
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


/************************** 结构体变量 ********************/
typedef struct _COM_SETTING
{
	bool isOpen;
	UINT  port;
	UINT  baud;
	
}_COM_SETTING_STRU;


/**************全局变量*******************/
typedef struct LOCAL
{
	int init;

}LOCAL_STRU;



/**************其他类型结构体*******************/


typedef struct _SEND_COMM
{
	unsigned char isValid;
	unsigned char port;
	unsigned char SendNum; //发送次数
	unsigned char SendTime; //发送时间计数
	unsigned char WaitTime; //等待回复时间
	unsigned char circle; //循环码
	unsigned char len;   //发送长度
	unsigned char sum;
	unsigned char buf[COMM_SIZE];
}_SEND_COMM_STRU;


typedef struct _REC_COMM
{
	unsigned char start; //接收到数据
	unsigned char cnt;

	int p; //当前接收的指针
	unsigned char len;
	unsigned char direct;//方向
	unsigned char type;
	unsigned char cmd;
	unsigned char circle;
	unsigned char fcs;
	unsigned char buf[COMM_SIZE];
}_REC_COMM_STRU;

typedef struct _WAIT_A20_ACK
{
	bool start;
	int over;
	int cnt;
}_WAIT_A20_ACK_STRU;

typedef struct _WAIT_BYTE_ACK
{
	bool start;
	int over;
	int cnt;
}_WAIT_BYTE_ACK_STRU;

typedef struct _FILE
{
	unsigned long len;//用于存储BIN文件总字节数
	unsigned char buf[65536];//用于储存从BIN文件获取的数据
}_FILE_STRU;

#ifndef _GLOBAL_H
#define _GLOBAL_H

LOCAL_STRU Local;
_SEND_COMM_STRU Multi_comm_Buff[COMM_MAX];
_REC_COMM_STRU com_rec;
_WAIT_A20_ACK_STRU com_ack;
_WAIT_BYTE_ACK_STRU com_byte;
_COM_SETTING_STRU com_set;
_FILE_STRU up_file;
#else

extern LOCAL_STRU Local;
extern _SEND_COMM_STRU Multi_comm_Buff[COMM_MAX];
extern _REC_COMM_STRU com_rec;
extern _WAIT_A20_ACK_STRU com_ack;
extern _WAIT_BYTE_ACK_STRU com_byte;
extern _COM_SETTING_STRU com_set;
extern _FILE_STRU up_file;
#endif
