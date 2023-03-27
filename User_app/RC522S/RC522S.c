#include "rc522s.h"
#include "spi.h"
#include "main.h"
#include "RC522S.h"
#include "string.h"
#include "stdio.h"
#include "FreeRTOS.h"					//FreeRTOSʹ��
#include "task.h"
#include "queue.h"
#include "HX711.h"
#include "rtc.h"
#include "BC260Y.h"
#include "tim.h"
#include "semphr.h"
#include "work.h"
#include "ui.h"
//�����ź������
extern SemaphoreHandle_t MutexSemaphore;	//�����ź���

#define RS522_RST(N) HAL_GPIO_WritePin(RC522_RST_GPIO_Port, RC522_RST_Pin, N==1?GPIO_PIN_SET:GPIO_PIN_RESET)
#define RS522_NSS(N) HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, N==1?GPIO_PIN_SET:GPIO_PIN_RESET)

Pcd_Massage Pcd_Massage_Flag={0};
/**************************************************************************************
* �������ƣ�MFRC_Init
* ����������MFRC��ʼ��
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ˵    ����MFRC��SPI�ӿ�����Ϊ0~10Mbps
***************************************************************************************/
void MFRC_Init(void)
{
    RS522_NSS(1);
    RS522_RST(1);
}


/**************************************************************************************
* ��������: SPI_RW_Byte
* ��������: ģ��SPI��дһ���ֽ�
* ��ڲ���: -byte:Ҫ���͵�����
* ���ڲ���: -byte:���յ�������
*/
static uint8_t ret;  //��Щ������HAL���׼�ⲻͬ�ĵط�����д������
uint8_t SPI2_RW_Byte(uint8_t byte)
{
    HAL_SPI_TransmitReceive(&hspi2, &byte, &ret, 1, 10);//��byte д�룬������һ��ֵ����������ret
    return   ret;//�����byte �ĵ�ַ����ȡʱ�õ�Ҳ��ret��ַ��һ��ֻд��һ��ֵ10
}

/**************************************************************************************
* �������ƣ�MFRC_WriteReg
* ����������дһ���Ĵ���
* ��ڲ�����-addr:��д�ļĴ�����ַ
*           -data:��д�ļĴ�������
* ���ڲ�������
* �� �� ֵ����
* ˵    ������
*/
void MFRC_WriteReg(uint8_t addr, uint8_t data)
{
    uint8_t AddrByte;
    AddrByte = (addr << 1 ) & 0x7E; //�����ַ�ֽ�
    RS522_NSS(0);                   //NSS����
    SPI2_RW_Byte(AddrByte);         //д��ַ�ֽ�
    SPI2_RW_Byte(data);             //д����
    RS522_NSS(1);                   //NSS����
}

/**************************************************************************************
* �������ƣ�MFRC_ReadReg
* ������������һ���Ĵ���
* ��ڲ�����-addr:�����ļĴ�����ַ
* ���ڲ�������
* �� �� ֵ��-data:�����Ĵ���������
* ˵    ������
*/
uint8_t MFRC_ReadReg(uint8_t addr)
{
    uint8_t AddrByte, data;
    AddrByte = ((addr << 1 ) & 0x7E ) | 0x80;   //�����ַ�ֽ�
    RS522_NSS(0);                               //NSS����
    SPI2_RW_Byte(AddrByte);                     //д��ַ�ֽ�
    data = SPI2_RW_Byte(0x00);                  //������
    RS522_NSS(1);                               //NSS����
    return data;
}

/**************************************************************************************
* �������ƣ�MFRC_SetBitMask
* �������������üĴ�����λ
* ��ڲ�����-addr:�����õļĴ�����ַ
*           -mask:�����üĴ�����λ(��ͬʱ���ö��bit)
* ���ڲ�������
* �� �� ֵ����
* ˵    ������
***************************************************************************************/
void MFRC_SetBitMask(uint8_t addr, uint8_t mask)
{
    uint8_t temp;
    temp = MFRC_ReadReg(addr);                  //�ȶ��ؼĴ�����ֵ
    MFRC_WriteReg(addr, temp | mask);           //�������������д��Ĵ���
}

/**************************************************************************************
* �������ƣ�MFRC_ClrBitMask
* ��������������Ĵ�����λ
* ��ڲ�����-addr:������ļĴ�����ַ
*           -mask:������Ĵ�����λ(��ͬʱ������bit)
* ���ڲ�������
* �� �� ֵ����
* ˵    ������
***************************************************************************************/
void MFRC_ClrBitMask(uint8_t addr, uint8_t mask)
{
    uint8_t temp;
    temp = MFRC_ReadReg(addr);                  //�ȶ��ؼĴ�����ֵ
    MFRC_WriteReg(addr, temp & ~mask);          //�������������д��Ĵ���
}


/**************************************************************************************
* �������ƣ�MFRC_CalulateCRC
* ������������MFRC����CRC���
* ��ڲ�����-pInData��������CRC���������
*           -len��������CRC��������ݳ���
*           -pOutData��CRC������
* ���ڲ�����-pOutData��CRC������
* �� �� ֵ����
* ˵    ������
***************************************************************************************/
void MFRC_CalulateCRC(uint8_t *pInData, uint8_t len, uint8_t *pOutData)
{
    //0xc1 1        2           pInData[2]
    uint8_t temp;
    uint32_t i;
    MFRC_ClrBitMask(MFRC_DivIrqReg, 0x04);                  //ʹ��CRC�ж�
    MFRC_WriteReg(MFRC_CommandReg, MFRC_IDLE);              //ȡ����ǰ�����ִ��
    MFRC_SetBitMask(MFRC_FIFOLevelReg, 0x80);               //���FIFO�����־λ
    for(i = 0; i < len; i++)                                //����CRC���������д��FIFO
    {
        MFRC_WriteReg(MFRC_FIFODataReg, *(pInData + i));
    }
    MFRC_WriteReg(MFRC_CommandReg, MFRC_CALCCRC);           //ִ��CRC����
    i = 100000;
    do
    {
        temp = MFRC_ReadReg(MFRC_DivIrqReg);                //��ȡDivIrqReg�Ĵ�����ֵ
        i--;
    }
    while((i != 0) && !(temp & 0x04));                      //�ȴ�CRC�������
    pOutData[0] = MFRC_ReadReg(MFRC_CRCResultRegL);         //��ȡCRC������
    pOutData[1] = MFRC_ReadReg(MFRC_CRCResultRegM);
}


/**************************************************************************************
* �������ƣ�MFRC_CmdFrame
* ����������MFRC522��ISO14443A��ͨѶ������֡����
* ��ڲ�����-cmd��MFRC522������
*           -pIndata��MFRC522���͸�MF1�������ݵĻ������׵�ַ
*           -InLenByte���������ݵ��ֽڳ���
*           -pOutdata�����ڽ���MF1��Ƭ�������ݵĻ������׵�ַ
*           -pOutLenBit��MF1���������ݵ�λ����
* ���ڲ�����-pOutdata�����ڽ���MF1��Ƭ�������ݵĻ������׵�ַ
*           -pOutLenBit������MF1����������λ���ȵ��׵�ַ
* �� �� ֵ��-status���������(MFRC_OK��MFRC_NOTAGERR��MFRC_ERR)
* ˵    ������
***************************************************************************************/
char MFRC_CmdFrame(uint8_t cmd, uint8_t *pInData, uint8_t InLenByte, uint8_t *pOutData, uint16_t *pOutLenBit)
{
    uint8_t lastBits;
    uint8_t n;
    uint32_t i;
    char status = MFRC_ERR;
    uint8_t irqEn   = 0x00;
    uint8_t waitFor = 0x00;

    /*�����������ñ�־λ*/
    switch(cmd)
    {
        case MFRC_AUTHENT:                  //Mifare��֤
            irqEn = 0x12;
            waitFor = 0x10;                 //idleIRq�жϱ�־
            break;
        case MFRC_TRANSCEIVE:               //���Ͳ���������
            irqEn = 0x77;
            waitFor = 0x30;                 //RxIRq��idleIRq�жϱ�־
            break;
    }

    /*��������֡ǰ׼��*/
    MFRC_WriteReg(MFRC_ComIEnReg, irqEn | 0x80);    //���ж�
    MFRC_ClrBitMask(MFRC_ComIrqReg, 0x80);          //����жϱ�־λSET1
    MFRC_WriteReg(MFRC_CommandReg, MFRC_IDLE);      //ȡ����ǰ�����ִ��
    MFRC_SetBitMask(MFRC_FIFOLevelReg, 0x80);       //���FIFO�����������־λ

    /*��������֡*/
    for(i = 0; i < InLenByte; i++)                  //д���������
    {
        MFRC_WriteReg(MFRC_FIFODataReg, pInData[i]);
    }
    MFRC_WriteReg(MFRC_CommandReg, cmd);            //ִ������
    if(cmd == MFRC_TRANSCEIVE)
    {
        MFRC_SetBitMask(MFRC_BitFramingReg, 0x80);  //��������
    }
    i = 300000;                                     //����ʱ��Ƶ�ʵ���,����M1�����ȴ�ʱ��25ms
    do
    {
        n = MFRC_ReadReg(MFRC_ComIrqReg);
        i--;
    }
    while((i != 0) && !(n & 0x01) && !(n & waitFor));     //�ȴ��������
    MFRC_ClrBitMask(MFRC_BitFramingReg, 0x80);          //ֹͣ����
    /*������յ�����*/
    if(i != 0)
    {
        if(!(MFRC_ReadReg(MFRC_ErrorReg) & 0x1B))
        {
            status = MFRC_OK;
            if(n & irqEn & 0x01)
            {
                status = MFRC_NOTAGERR;
            }
            if(cmd == MFRC_TRANSCEIVE)
            {
                n = MFRC_ReadReg(MFRC_FIFOLevelReg);
                lastBits = MFRC_ReadReg(MFRC_ControlReg) & 0x07;
                if (lastBits)
                {
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                }
                else
                {
                    *pOutLenBit = n * 8;
                }
                if(n == 0)
                {
                    n = 1;
                }
                if(n > MFRC_MAXRLEN)
                {
                    n = MFRC_MAXRLEN;
                }
                for(i = 0; i < n; i++)
                {
                    pOutData[i] = MFRC_ReadReg(MFRC_FIFODataReg);
                }
            }
        }
        else
        {
            status = MFRC_ERR;
        }
    }

    MFRC_SetBitMask(MFRC_ControlReg, 0x80);               //ֹͣ��ʱ������
    MFRC_WriteReg(MFRC_CommandReg, MFRC_IDLE);            //ȡ����ǰ�����ִ��
//printf("val=%d\r\n",status);
    return status;
}


/**************************************************************************************
* �������ƣ�PCD_Reset
* ����������PCD��λ
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ˵    ������
***************************************************************************************/
void PCD_Reset(void)
{
    /*Ӳ��λ*/
    RS522_RST(1);//�õ���λ����
    HAL_Delay(2);
    RS522_RST(0);
    HAL_Delay(2);
    RS522_RST(1);
    HAL_Delay(2);

    /*��λ*/
    MFRC_WriteReg(MFRC_CommandReg, MFRC_RESETPHASE);
    HAL_Delay(2);

    /*��λ��ĳ�ʼ������*/
    MFRC_WriteReg(MFRC_ModeReg, 0x3D);              //CRC��ʼֵ0x6363
    MFRC_WriteReg(MFRC_TReloadRegL, 30);            //��ʱ����װֵ
    MFRC_WriteReg(MFRC_TReloadRegH, 0);
    MFRC_WriteReg(MFRC_TModeReg, 0x8D);             //��ʱ������
    MFRC_WriteReg(MFRC_TPrescalerReg, 0x3E);        //��ʱ��Ԥ��Ƶֵ
    MFRC_WriteReg(MFRC_TxAutoReg, 0x40);            //100%ASK

    PCD_AntennaOff();                               //������
    HAL_Delay(2);
    PCD_AntennaOn();                                //������
}


/**************************************************************************************
* �������ƣ�PCD_AntennaOn
* ������������������,ʹ��PCD���������ز��ź�
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ˵    ����ÿ�ο�����ر�����֮��Ӧ������1ms�ļ��
***************************************************************************************/
void PCD_AntennaOn(void)
{
    uint8_t temp;
    temp = MFRC_ReadReg(MFRC_TxControlReg);
    if (!(temp & 0x03))
    {
        MFRC_SetBitMask(MFRC_TxControlReg, 0x03);
    }
}


/**************************************************************************************
* �������ƣ�PCD_AntennaOff
* �����������ر�����,ʧ��PCD���������ز��ź�
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ˵    ����ÿ�ο�����ر�����֮��Ӧ������1ms�ļ��
***************************************************************************************/
void PCD_AntennaOff(void)
{
    MFRC_ClrBitMask(MFRC_TxControlReg, 0x03);
}


/***************************************************************************************
* �������ƣ�PCD_Init
* ������������д����ʼ��
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ˵    ������
***************************************************************************************/
void PCD_Init(void)
{
    MFRC_Init();      //MFRC�ܽ�����
    PCD_Reset();      //PCD��λ  ����ʼ������
    PCD_AntennaOff(); //�ر�����
    PCD_AntennaOn();   //��������
}


/***************************************************************************************
* �������ƣ�PCD_Request
* ����������Ѱ��
* ��ڲ����� -RequestMode��Ѷ����ʽ
*                             PICC_REQIDL��Ѱ��������δ��������״̬
*                 PICC_REQALL��Ѱ��������ȫ����
*               -pCardType�����ڱ��濨Ƭ����
* ���ڲ�����-pCardType����Ƭ����
*                               0x4400��Mifare_UltraLight
*                       0x0400��Mifare_One(S50)
*                       0x0200��Mifare_One(S70)
*                       0x0800��Mifare_Pro(X)
*                       0x4403��Mifare_DESFire
* �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
* ˵    ������
***************************************************************************************/
char PCD_Request(uint8_t RequestMode, uint8_t *pCardType)
{
    char status;
    uint16_t unLen;
    uint8_t CmdFrameBuf[MFRC_MAXRLEN];

    MFRC_ClrBitMask(MFRC_Status2Reg, 0x08);//���ڲ��¶ȴ�����
    MFRC_WriteReg(MFRC_BitFramingReg, 0x07); //�洢ģʽ������ģʽ���Ƿ��������͵�
    MFRC_SetBitMask(MFRC_TxControlReg, 0x03);//���õ����ź�13.56MHZ

    CmdFrameBuf[0] = RequestMode;

    status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 1, CmdFrameBuf, &unLen);

    if((status == PCD_OK) && (unLen == 0x10))
    {
        *pCardType = CmdFrameBuf[0];
        *(pCardType + 1) = CmdFrameBuf[1];
    }
//		printf("read=%x\r\n",status);
    return status;
}


/***************************************************************************************
* �������ƣ�PCD_Anticoll
* ��������������ͻ,��ȡ����
* ��ڲ�����-pSnr�����ڱ��濨Ƭ���к�,4�ֽ�
* ���ڲ�����-pSnr����Ƭ���к�,4�ֽ�
* �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
* ˵    ������
***************************************************************************************/
char PCD_Anticoll(uint8_t *pSnr)
{
    char status;
    uint8_t i, snr_check = 0;
    uint16_t  unLen;
    uint8_t CmdFrameBuf[MFRC_MAXRLEN];

    MFRC_ClrBitMask(MFRC_Status2Reg, 0x08);
    MFRC_WriteReg(MFRC_BitFramingReg, 0x00);
    MFRC_ClrBitMask(MFRC_CollReg, 0x80);

    CmdFrameBuf[0] = PICC_ANTICOLL1;
    CmdFrameBuf[1] = 0x20;

    status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 2, CmdFrameBuf, &unLen);

    if(status == PCD_OK)
    {
        for(i = 0; i < 4; i++)
        {
            *(pSnr + i)  = CmdFrameBuf[i];
            snr_check ^= CmdFrameBuf[i];
        }
        if(snr_check != CmdFrameBuf[i])
        {
            status = PCD_ERR;
        }
    }

    MFRC_SetBitMask(MFRC_CollReg, 0x80);
	//	printf("x=%d\r\n",status);
    return status;
}


/***************************************************************************************
* �������ƣ�PCD_Select
* ����������ѡ��
* ��ڲ�����-pSnr����Ƭ���к�,4�ֽ�
* ���ڲ�������
* �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
* ˵    ������
***************************************************************************************/
char PCD_Select(uint8_t *pSnr)
{
    char status;
    uint8_t i;
    uint16_t unLen;
    uint8_t CmdFrameBuf[MFRC_MAXRLEN];

    CmdFrameBuf[0] = PICC_ANTICOLL1;
    CmdFrameBuf[1] = 0x70;
    CmdFrameBuf[6] = 0;
    for(i = 0; i < 4; i++)
    {
        CmdFrameBuf[i + 2] = *(pSnr + i);
        CmdFrameBuf[6]  ^= *(pSnr + i);
    }
    MFRC_CalulateCRC(CmdFrameBuf, 7, &CmdFrameBuf[7]);

    MFRC_ClrBitMask(MFRC_Status2Reg, 0x08);

    status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 9, CmdFrameBuf, &unLen);

    if((status == PCD_OK) && (unLen == 0x18))
    {
        status = PCD_OK;
    }
    else
    {
        status = PCD_ERR;
    }
	//	printf("card=%d\r\n",status);
    return status;
}


/***************************************************************************************
* �������ƣ�PCD_AuthState
* ������������֤��Ƭ����
* ��ڲ�����-AuthMode����֤ģʽ
*                   PICC_AUTHENT1A����֤A����
*                   PICC_AUTHENT1B����֤B����
*           -BlockAddr�����ַ(0~63)
*           -pKey������
*           -pSnr����Ƭ���к�,4�ֽ�
* ���ڲ�������
* �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
* ˵    ������֤����ʱ,������Ϊ��λ,BlockAddr����������ͬһ�������������
***************************************************************************************/
char PCD_AuthState(uint8_t AuthMode, uint8_t BlockAddr, uint8_t *pKey, uint8_t *pSnr)
{
    char status;
    uint16_t unLen;
    uint8_t i, CmdFrameBuf[MFRC_MAXRLEN];
    CmdFrameBuf[0] = AuthMode;
    CmdFrameBuf[1] = BlockAddr;
    for(i = 0; i < 6; i++)
    {
        CmdFrameBuf[i + 2] = *(pKey + i);
    }
    for(i = 0; i < 4; i++)
    {
        CmdFrameBuf[i + 8] = *(pSnr + i);
    }

    status = MFRC_CmdFrame(MFRC_AUTHENT, CmdFrameBuf, 12, CmdFrameBuf, &unLen);
    if((status != PCD_OK) || (!(MFRC_ReadReg(MFRC_Status2Reg) & 0x08)))
    {
        status = PCD_ERR;
    }
	//	printf("ms=%d\r\n",status);
    return status;
}


/***************************************************************************************
* �������ƣ�PCD_WriteBlock
* ������������MF1�����ݿ�
* ��ڲ�����-BlockAddr�����ַ
*           -pData: ���ڱ����д�������,16�ֽ�
* ���ڲ�������
* �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
* ˵    ������
***************************************************************************************/
char PCD_WriteBlock(uint8_t BlockAddr, uint8_t *pData)
{
    char status;
    uint16_t unLen;
    uint8_t i, CmdFrameBuf[MFRC_MAXRLEN];

    CmdFrameBuf[0] = PICC_WRITE;
    CmdFrameBuf[1] = BlockAddr;
    MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

    status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

    if((status != PCD_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A))
    {
        status = PCD_ERR;
    }
    if(status == PCD_OK)
    {
        for(i = 0; i < 16; i++)
        {
            CmdFrameBuf[i] = *(pData + i);
        }
        MFRC_CalulateCRC(CmdFrameBuf, 16, &CmdFrameBuf[16]);

        status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 18, CmdFrameBuf, &unLen);

        if((status != PCD_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A))
        {
            status = PCD_ERR;
        }
    }
    return status;
}


/***************************************************************************************
* �������ƣ�PCD_ReadBlock
* ������������MF1�����ݿ�
* ��ڲ�����-BlockAddr�����ַ
*           -pData: ���ڱ������������,16�ֽ�
* ���ڲ�����-pData: ���ڱ������������,16�ֽ�
* �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
* ˵    ������
***************************************************************************************/
char PCD_ReadBlock(uint8_t BlockAddr, uint8_t *pData)
{
    char status;
    uint16_t unLen;
    uint8_t i, CmdFrameBuf[MFRC_MAXRLEN];

    CmdFrameBuf[0] = PICC_READ;
    CmdFrameBuf[1] = BlockAddr;
    MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);
    printf("addr=%d,crc=%d",CmdFrameBuf[1],CmdFrameBuf[2]);
    status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);
    if((status == PCD_OK) && (unLen == 0x90))
    {
        for(i = 0; i < 16; i++)
        {
            *(pData + i) = CmdFrameBuf[i];
        }
    }
    else
    {
        status = PCD_ERR;
    }
//	printf("read3=%d\r\n",status);
    return status;
}


/***************************************************************************************
* �������ƣ�PCD_Value
* ������������MF1�����ݿ�����ֵ����
* ��ڲ�����
*           -BlockAddr�����ַ
*           -pValue�����ֽ���ֵ��ֵ,��λ��ǰ
*           -mode����ֵ�����ģʽ
*                  PICC_INCREMENT����ֵ
*                PICC_DECREMENT����ֵ
* ���ڲ�������
* �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
* ˵    ������
***************************************************************************************/
char PCD_Value(uint8_t mode, uint8_t BlockAddr, uint8_t *pValue)
{
    //0XC1        1           Increment[4]={0x03, 0x01, 0x01, 0x01};
    char status;
    uint16_t unLen;
    uint8_t i, CmdFrameBuf[MFRC_MAXRLEN];

    CmdFrameBuf[0] = mode;
    CmdFrameBuf[1] = BlockAddr;
    MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

    status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

    if((status != PCD_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A))
    {
        status = PCD_ERR;
    }

    if(status == PCD_OK)
    {
        for(i = 0; i < 16; i++)
        {
            CmdFrameBuf[i] = *(pValue + i);
        }
        MFRC_CalulateCRC(CmdFrameBuf, 4, &CmdFrameBuf[4]);
        unLen = 0;
        status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 6, CmdFrameBuf, &unLen);
        if(status != PCD_ERR)
        {
            status = PCD_OK;
        }
    }

    if(status == PCD_OK)
    {
        CmdFrameBuf[0] = PICC_TRANSFER;
        CmdFrameBuf[1] = BlockAddr;
        MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

        status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

        if((status != PCD_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A))
        {
            status = PCD_ERR;
        }
    }
    return status;
}


/***************************************************************************************
* �������ƣ�PCD_BakValue
* ��������������Ǯ��(��ת��)
* ��ڲ�����-sourceBlockAddr��Դ���ַ
*                -goalBlockAddr   ��Ŀ����ַ
* ���ڲ�������
* �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
* ˵    ����ֻ����ͬһ��������ת��
***************************************************************************************/
char PCD_BakValue(uint8_t sourceBlockAddr, uint8_t goalBlockAddr)
{
    char status;
    uint16_t  unLen;
    uint8_t CmdFrameBuf[MFRC_MAXRLEN];

    CmdFrameBuf[0] = PICC_RESTORE;
    CmdFrameBuf[1] = sourceBlockAddr;
    MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);
    status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);
    if((status != PCD_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A))
    {
        status = PCD_ERR;
    }

    if(status == PCD_OK)
    {
        CmdFrameBuf[0] = 0;
        CmdFrameBuf[1] = 0;
        CmdFrameBuf[2] = 0;
        CmdFrameBuf[3] = 0;
        MFRC_CalulateCRC(CmdFrameBuf, 4, &CmdFrameBuf[4]);
        status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 6, CmdFrameBuf, &unLen);
        if(status != PCD_ERR)
        {
            status = PCD_OK;
        }
    }

    if(status != PCD_OK)
    {
        return PCD_ERR;
    }

    CmdFrameBuf[0] = PICC_TRANSFER;
    CmdFrameBuf[1] = goalBlockAddr;
    MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);
    status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);
    if((status != PCD_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A))
    {
        status = PCD_ERR;
    }

    return status;
}


/***************************************************************************************
* �������ƣ�PCD_Halt
* �������������Ƭ��������״̬
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
* ˵    ������
***************************************************************************************/
char PCD_Halt(void)
{
    char status;
    uint16_t unLen;
    uint8_t CmdFrameBuf[MFRC_MAXRLEN];

    CmdFrameBuf[0] = PICC_HALT;
    CmdFrameBuf[1] = 0;
    MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

    status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

    return status;
}


//RC522_Read_ID_Once������
//ֻ����ID��������֤��
char RC522_Read_ID_Once(void)
{
	char cStr [ 30 ], tStr[30];
    unsigned char snr, DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t ucArray_ID [ 5 ];   /*�Ⱥ���IC�������ͺ�UID(IC�����к�)*/                                                                                          
    char ucStatusReturn;     /*����״̬ */  

    /*Ѱ��*/
    if ( ( ucStatusReturn = PCD_Request ( PICC_REQALL, ucArray_ID ) ) != PCD_OK )  
    /*��ʧ���ٴ�Ѱ��*/
        ucStatusReturn = PCD_Request ( PICC_REQALL, ucArray_ID );
        if ( ucStatusReturn == 0  )
				{
                sprintf ( tStr, "The Card Type is: %02X%02X",
                                    ucArray_ID [ 0 ],
                                    ucArray_ID [ 1 ]);
                printf ( "%s\r\n",tStr ); 		
            /*����ײ�����ж��ſ������д��������Χʱ������ͻ���ƻ������ѡ��һ�Ž��в�����*/
            if ( PCD_Anticoll ( ucArray_ID ) == 0 )                                                                   
            {
                sprintf ( cStr, "The Card ID is: %02X%02X%02X%02X",
                                    ucArray_ID [ 0 ],
                                    ucArray_ID [ 1 ],
                                    ucArray_ID [ 2 ],
                                    ucArray_ID [ 3 ] );
                Pcd_Massage_Flag.Pcd_Card_ID= (u32)ucArray_ID [ 0 ]<<24 | (u32) ucArray_ID [ 1 ]<<16 |(u32) ucArray_ID [ 2 ]<<8 |(u32)ucArray_ID [ 3 ];
							//printf("����1��%x-%x-%x-%x\n",ucArray_ID[0],ucArray_ID[1],ucArray_ID[2],ucArray_ID[3]);
                printf ( "%s\r\n",cStr ); 
                // ѡ��
                if(PCD_Select(ucArray_ID)==0)
                {   
                    snr = 1;    // ������1
                    if(PCD_AuthState(PICC_AUTHENT1A, (snr*4+3), DefaultKey, ucArray_ID)==0)
                    {
                        return PCD_OK;
                    }
                    else
                        return PCD_ERR;
                }
                else
                    return PCD_ERR;
            }
            else
                return PCD_ERR;
		}
		else
    return PCD_NOTAGERR;
}

//�ȴ����뿪
char WaitCardOff(void)
{
	char          status;
  unsigned char	TagType[4];

	status = PCD_Request(PICC_REQALL, TagType);
		if(status==PCD_NOTAGERR)
    {
        status = PCD_Request(PICC_REQALL, TagType);
			if(status==PCD_NOTAGERR)
        {
            status = PCD_Request(PICC_REQALL, TagType);
						if(status==PCD_NOTAGERR)
            {
                return PCD_NOTAGERR;
            }
            else
                return PCD_OK;
        }
        else
            return PCD_OK;
    }
    else
        return PCD_OK;

}

/**
  * @Name    RC522S_task
  * @brief   ��������
  * @param   None
  * @retval
  * @author  
 **/

void RC522Stask(void* pvParameters )
{
    unsigned char pcd_addr;
    u8 buf[2]={0,0},Legal_status=0;
    u8 aval=0;
    	
	HX711_Massage.K=((((HX711_VAVDD)/HX711_BEARING)*HX711_GAIN)*HX711_24BIT)/HX711_BASE_VDD;      //����ת����Kֵ
    HX711_Massage.K/=HX711_K_VALUE;
	if(HX711_Massage.Base_Weight_Value==0)
	{
		Get_WeightBase();
	}

    for( ; ; ) 
    { 
//			 xSemaphoreTake(MutexSemaphore,portMAX_DELAY);	//��ȡ�����ź���
        Pcd_Massage_Flag.Have_A_Card=WaitCardOff();
        
        if(Pcd_Massage_Flag.Have_A_Card==PCD_NOTAGERR )        //��⿨�Ƿ��뿪
        {
            if(aval==0)
            {
                aval=1;
                ui_event_cb=surplus_change; //����һ��ʣ����ˢ���¼�
               // Pcd_Massage_Flag.Have_A_Card=PCD_ERR;
                Pcd_Massage_Flag.Pcd_Read_Flag=0;
                Pcd_Massage_Flag.Pcd_Write_Flag=0;
                Pcd_Massage_Flag.Pcd_Card_ID=0;                
                 Pcd_Massage_Flag.Card_Modal=0;
                Legal_status=0;
                readCard_delay=0;
                Motor_Working(0);
            }                
        }
       // else if(Pcd_Massage_Flag.Have_A_Card==PCD_OK)
       // {
       //     //aval=0;
       //     //Pcd_Massage_Flag.Have_A_Card=PCD_OK;
       // }
        else if(Pcd_Massage_Flag.Pcd_Read_Flag==0 && Pcd_Massage_Flag.Have_A_Card==PCD_OK && readCard_delay>=6)  //��⵽��֮���3S�ٶ�д��
        {
           
           if(RC522_Read_ID_Once()==PCD_OK) 		//����ID��������֤
           {
                //��д��
                pcd_addr=1;
				aval=0;
                printf("pcd_addr=%d",pcd_addr);
                if(PCD_ReadBlock((pcd_addr*4+0), buf)==0) 
                {
                     Pcd_Massage_Flag.Pcd_Read_Flag=1; 
                      Pcd_Massage_Flag.Pcd_Read_Card_Werght=((u16)buf[0])<<8 | (u16)buf[1];                   
                    //Pcd_Massage_Flag.Pcd_Read_Card_Werght=Weight_Decode(buf);     //�򵥼���
                    if(Pcd_Massage_Flag.Pcd_Read_Card_Werght!=1)
                    {                      
                        //Pcd_Massage_Flag.Pcd_Read_Card_Werght=((u16)buf[0])<<8 | (u16)buf[1];                   
                        printf("������ֵ�ǣ� %d\n",Pcd_Massage_Flag.Pcd_Read_Card_Werght);						
                    }
                }
								
           }                               
            if(Pcd_Massage_Flag.Pcd_Read_Flag==1 && Pcd_Massage_Flag.Pcd_Write_Flag==0 && Pcd_Massage_Flag.Card_Modal==0)
            {
                Get_Weight(); 
                if(Legal_status==0)
                {              
                    if(HX711_Massage.Write_To_Card_Weight>=Pcd_Massage_Flag.Pcd_Read_Card_Werght)       //�ж�ҩˮ�����Ƿ�һ�£�
                    {
                        if((HX711_Massage.Write_To_Card_Weight-Pcd_Massage_Flag.Pcd_Read_Card_Werght)>300)
                        {
                            Pcd_Massage_Flag.Pcd_Legal_Flag=2;
                            BC260_Massage.send_error=1;
                        }
                        else
                        {
                            Pcd_Massage_Flag.Pcd_Legal_Flag=1;
                            
                        }
                    }
                    else
                    {
                        if((Pcd_Massage_Flag.Pcd_Read_Card_Werght-HX711_Massage.Write_To_Card_Weight)>300)
                        {
                            Pcd_Massage_Flag.Pcd_Legal_Flag=2;
                             BC260_Massage.send_error=1;
                        }
                        else
                        {
                            Pcd_Massage_Flag.Pcd_Legal_Flag=1;
                            
                        }
                    }

                    //    if(Pcd_Massage_Flag.Pcd_Legal_Flag==1)
                    //    {
                    //        beep_on();
                    //        vTaskDelay(300);	
                    //        beep_off();
                    //         memset(&work_time,0,sizeof(work_time));//ÿ�����·��Ϻ�Ҫ��λ�����ж�
                    //    }
                    //    else
                    //    {
                    //        u8 num=0;
                    //        for(num=0;num<3;num++)
                    //        {
                    //            beep_on();
                    //            vTaskDelay(300);	
                    //            beep_off();
                    //            vTaskDelay(300);
                    //        }
                    //    }
                        
                }
                
            }
            if(Pcd_Massage_Flag.Pcd_Write_Flag==1 && Pcd_Massage_Flag.Pcd_Legal_Flag==1)//Pcd_Write_Flag��׼������ǰ��ֵ1
            {
                Write_Weight_To_Card();              
                ui_event_cb=surplus_change; //����һ��ʣ����ˢ���¼�
            }
            if(Pcd_Massage_Flag.Pcd_Legal_Flag==1 )
                {
                    Legal_status=1;   //״̬��1���ȿ��뿪����0                    
                    readCard_delay=0;
                }
            else if(Pcd_Massage_Flag.Pcd_Read_Flag==1 && Pcd_Massage_Flag.Card_Modal==1)//�����ֶ�д��
            {
                Get_Weight();
                
                 if(HX711_Massage.Write_To_Card_Weight>=Pcd_Massage_Flag.Pcd_Read_Card_Werght)       
                {
                    if((HX711_Massage.Write_To_Card_Weight-Pcd_Massage_Flag.Pcd_Read_Card_Werght)>50)
                    {
                        Write_Weight_To_Card();
                        Pcd_Massage_Flag.Pcd_Read_Flag=0;
                        Pcd_Massage_Flag.Card_Modal=0;
                    }
                }
                else
                {
                    if((Pcd_Massage_Flag.Pcd_Read_Card_Werght-HX711_Massage.Write_To_Card_Weight)>100)
                    {
                        Write_Weight_To_Card();
                        Pcd_Massage_Flag.Pcd_Read_Flag=0;
                        Pcd_Massage_Flag.Card_Modal=0;
                    }
                }
                      
            }

        }
				// if(Pcd_Massage_Flag.Have_A_Card==PCD_ERR)
				// {
						//if(aval==0)
						//{
						//	ui_event_cb=surplus_change; //����һ��ʣ����ˢ���¼�
						//	aval=1;								
						//}
			    //}
				//else
				//{
				//	if(aval==1)
				//	{
				//			ui_event_cb=surplus_change; //����һ��ʣ����ˢ���¼�
				//			aval=0;								
				//	}
				//}
					
				
//				 xSemaphoreGive(MutexSemaphore);					//�ͷ��ź���      
        vTaskDelay(500);	
    }		
}

