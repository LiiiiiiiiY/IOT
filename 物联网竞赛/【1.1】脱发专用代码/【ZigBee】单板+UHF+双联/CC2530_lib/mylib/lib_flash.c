#include "lib_flash.h"


/***************************************************************
* ����: ����FLASH�еĵ�pageҳ
****************************************************************/
void Flash_Erase(uint8 page)
{
    FADDRH = page << 1;
    FCTL |= 0x01;        //����ҳ�����
    while (FCTL & 0x80);  //�ȴ��������
}

/***************************************************************
* ����: ��buf�е�length���ֽ�д��FLASH��pageҳoffset��ʼ��λ�� 
****************************************************************/
uint8 DmaDesc[8]={0,0,0x62,0x73,0,0,0x12,0x42};//����DMA������
void Flash_Write(uint8 page, uint16 offset, uint8 *buf, uint16 length)
{
    FADDRH = (page << 1)|((offset >> 10) & 1);  //����FLASH��ַ
    FADDRL = (offset >> 2) & 0xFF;
    DmaDesc[0] = (uint16)buf >> 8;            //�������ݵ�ַ
    DmaDesc[1] = (uint16)buf & 0xFF; 
    DmaDesc[4] = (length >> 8) & 0x1F;        //�����ֽ���
    DmaDesc[5] = length & 0xFC; 
   
    DMA0CFGH = (uint16)DmaDesc >> 8;    //����DMAͨ��0��������ַ
    DMA0CFGL = (uint16)DmaDesc & 0xFF;
    DMAIRQ = 0x01;                      //����DMAͨ��0
    DMAARM = 0x01;                      //ʹDMAͨ��0���빤��״̬
    FCTL |=  0x02;                      //��ʼDMAд�����
    while (FCTL & 0x80);                //�ȴ�д�����
}

/***************************************************************
*����:��FLASH��pageҳoffset��ʼ��length���ֽڶ���buf��
****************************************************************/
void Flash_Read(uint8 page, uint16 offset, uint8 *buf, uint16 length)
{
    uint8 *pData = (uint8 *)(offset + (page & 0xF) * 2048 + 0x8000);
    uint8 memctr = MEMCTR;                           //�ݴ浱ǰ�洢��ӳ��״̬
    MEMCTR = (MEMCTR & 0xF8)|((page >> 4) & 7);   //������FLASHӳ�䵽XDATA
    while (length--) *buf++ = *pData++;           //��ȡ����
    MEMCTR = memctr;                              //�ظ��洢��ӳ��״̬
}