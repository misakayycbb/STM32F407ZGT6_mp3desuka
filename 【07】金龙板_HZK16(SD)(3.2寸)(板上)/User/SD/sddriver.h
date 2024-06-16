#ifndef __SDDRIVER_H
#define __SDDRIVER_H
#include "stm32f10x_conf.h"

#define SD_CS_ENABLE()      GPIO_ResetBits(GPIOA, GPIO_Pin_4)   /*ʹ��SD��*/
#define SD_CS_DISABLE()     GPIO_SetBits(GPIOA, GPIO_Pin_4)     /*ʧ��SD��*/

#define SPI_SPEED_LOW     0
#define SPI_SPEED_HIGH	  1

/* Private define ------------------------------------------------------------*/
/* SD�����Ͷ��� */
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4

/* SD�������ݽ������Ƿ��ͷ����ߺ궨�� */
#define NO_RELEASE      0
#define RELEASE         1

/* SD��ָ��� */
#define CMD0    0       //����λ
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define ACMD23  23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define ACMD41  41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00

/* SPI���������� ����256��Ƶ */
uint8_t SD_Init(void);
void SPI_Configuration(void);
void SPI_SetSpeed(u8 SpeedSet);
u8 SPI_ReadWriteByte(u8 TxData);
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc);
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc);

u8 SD_ReadSingleBlock(u32 sector, u8 *buffer);
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count);
u8 SD_WriteSingleBlock(u32 sector, const u8 *data);
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count);

u8 SD_WaitReady(void);
u32 SD_GetCapacity(void);
u8 SD_GetCSD(u8 *csd_data);
u8 SD_ReceiveData(u8 *data, u16 len, u8 release);



#endif
