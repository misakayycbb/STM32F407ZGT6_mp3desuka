/*
******************************************************************************
* @file    recorder.c
* @author  fire
* @version V1.0
* @date    2015-xx-xx
* @brief   WM8978�������ܲ���+mp3����
******************************************************************************
* @attention
*
* ʵ��ƽ̨:Ұ��  STM32 F407 ������  
* ��̳    :http://www.firebbs.cn
* �Ա�    :https://fire-stm32.taobao.com
*
******************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include "Bsp/usart/bsp_debug_usart.h"
#include "Bsp/systick/bsp_SysTick.h"
#include "Bsp/wm8978/bsp_wm8978.h"
#include "ff.h" 
#include "App/mp3Player.h"
#include "mp3dec.h"

/* �Ƽ�ʹ�����¸�ʽmp3�ļ���
 * �����ʣ�44100Hz
 * ��  ����2
 * �����ʣ�320kbps
 */

/* ������������Ƶ����ʱ�������������Ҫ������СΪ2304*16/8�ֽ�(16ΪPCM����Ϊ16λ)��
 * �������Ƕ���MP3BUFFER_SIZEΪ2304��ʵ�����������ΪMP3BUFFER_SIZE*2���ֽ�
 */
#define MP3BUFFER_SIZE  2304		
#define INPUTBUF_SIZE   3000	

static HMP3Decoder		Mp3Decoder;			/* mp3������ָ��	*/
static MP3FrameInfo		Mp3FrameInfo;		/* mP3֡��Ϣ  */
static MP3_TYPE mp3player;         /* mp3�����豸 */
static uint8_t Isread=0;           /* DMA������ɱ�־ */
static uint8_t bufflag=0;          /* ���ݻ�����ѡ���־ */

uint8_t inputbuf[INPUTBUF_SIZE]={0};        /* �������뻺������1940�ֽ�Ϊ���MP3֡��С  */
static short outbuffer[2][MP3BUFFER_SIZE];  /* ���������������Ҳ��I2S�������ݣ�ʵ��ռ���ֽ�����RECBUFFER_SIZE*2 */

FIL file;											/* file objects */
FRESULT result; 
UINT bw;            					/* File R/W count */

/* �������ļ��ڵ��õĺ������� */
void MP3Player_I2S_DMA_TX_Callback(void);

/**
  * @brief   MP3��ʽ��Ƶ����������
  * @param  ��
  * @retval ��
  */
void mp3PlayerDemo(const char *mp3file)
{
	uint8_t *read_ptr=inputbuf;
	uint32_t frames=0;
	int err=0, i=0, outputSamps=0;	
	int	read_offset = 0;				/* ��ƫ��ָ�� */
	int	bytes_left = 0;					/* ʣ���ֽ��� */	
	
	mp3player.ucFreq=I2S_AudioFreq_Default;
	mp3player.ucStatus=STA_IDLE;
	mp3player.ucVolume=40;
	
	result=f_open(&file,mp3file,FA_READ);
	if(result!=FR_OK)
	{
		printf("Open mp3file :%s fail!!!->%d\r\n",mp3file,result);
		result = f_close (&file);
		return;	/* ֹͣ���� */
	}
	printf("��ǰ�����ļ� -> %s\n",mp3file);
	
	//��ʼ��MP3������
	Mp3Decoder = MP3InitDecoder();	
	if(Mp3Decoder==0)
	{
		printf("��ʼ��helix������豸\n");
		return;	/* ֹͣ���� */
	}
	printf("��ʼ����...\n");
	
	Delay_ms(10);	/* �ӳ�һ��ʱ�䣬�ȴ�I2S�жϽ��� */
	wm8978_Reset();		/* ��λWM8978����λ״̬ */

	/* ����WM8978оƬ������ΪDAC�����Ϊ���� */
	wm8978_CfgAudioPath(DAC_ON, EAR_LEFT_ON | EAR_RIGHT_ON);

	/* ����������������ͬ���� */
	wm8978_SetOUT1Volume(mp3player.ucVolume);

	/* ����WM8978��Ƶ�ӿ�Ϊ�����ֱ�׼I2S�ӿڣ�16bit */
	wm8978_CfgAudioIF(I2S_Standard_Phillips, 16);
	
	/*  ��ʼ��������I2S  */
	I2S_Stop();
	I2S_GPIO_Config();
	I2Sx_Mode_Config(I2S_Standard_Phillips,I2S_DataFormat_16b,mp3player.ucFreq);	
	I2S_DMA_TX_Callback=MP3Player_I2S_DMA_TX_Callback;
	I2Sx_TX_DMA_Init((uint16_t *)outbuffer[0],(uint16_t *)outbuffer[1],MP3BUFFER_SIZE);	
	
	bufflag=0;
	Isread=0;
	
	mp3player.ucStatus = STA_PLAYING;		/* ����״̬ */
  result=f_read(&file,inputbuf,INPUTBUF_SIZE,&bw);
	if(result!=FR_OK)
	{
		printf("��ȡ%sʧ�� -> %d\r\n",mp3file,result);
		MP3FreeDecoder(Mp3Decoder);
		return;
	}
	read_ptr=inputbuf;
	bytes_left=bw;
	/* ����������ѭ���� */
	while(mp3player.ucStatus == STA_PLAYING)
	{
		//Ѱ��֡ͬ�������ص�һ��ͬ���ֵ�λ��
		read_offset = MP3FindSyncWord(read_ptr, bytes_left);			
		//û���ҵ�ͬ����
		if(read_offset < 0)																		
		{
			result=f_read(&file,inputbuf,INPUTBUF_SIZE,&bw);
			if(result!=FR_OK)
			{
				printf("��ȡ%sʧ�� -> %d\r\n",mp3file,result);
				break;
			}
			read_ptr=inputbuf;
			bytes_left=bw;
			continue;
		}
		
		read_ptr += read_offset;					//ƫ����ͬ���ֵ�λ��
		bytes_left -= read_offset;				//ͬ����֮������ݴ�С	
		if(bytes_left < 1024)							//��������
		{
			/* ע������ط���Ϊ���õ���DMA��ȡ������һ��Ҫ4�ֽڶ���  */
			i=(uint32_t)(bytes_left)&3;									//�ж϶�����ֽ�
			if(i) i=4-i;														//��Ҫ������ֽ�
			memcpy(inputbuf+i, read_ptr, bytes_left);	//�Ӷ���λ�ÿ�ʼ����
			read_ptr = inputbuf+i;										//ָ�����ݶ���λ��
			//��������
			result = f_read(&file, inputbuf+bytes_left+i, INPUTBUF_SIZE-bytes_left-i, &bw);
			bytes_left += bw;										//��Ч��������С
		}
		//��ʼ���� ������mp3����ṹ�塢������ָ�롢��������С�������ָ�롢���ݸ�ʽ
		err = MP3Decode(Mp3Decoder, &read_ptr, &bytes_left, outbuffer[bufflag], 0);					
		frames++;	
		//������
		if (err != ERR_MP3_NONE)									
		{
			switch (err)
			{
				case ERR_MP3_INDATA_UNDERFLOW:
					printf("ERR_MP3_INDATA_UNDERFLOW\r\n");
					result = f_read(&file, inputbuf, INPUTBUF_SIZE, &bw);
					read_ptr = inputbuf;
					bytes_left = bw;
					break;		
				case ERR_MP3_MAINDATA_UNDERFLOW:
					/* do nothing - next call to decode will provide more mainData */
					printf("ERR_MP3_MAINDATA_UNDERFLOW\r\n");
					break;		
				default:
					printf("UNKNOWN ERROR:%d\r\n", err);		
					// ������֡
					if (bytes_left > 0)
					{
						bytes_left --;
						read_ptr ++;
					}	
					break;
			}
			Isread=1;
		}
		else		//�����޴���׼�������������PCM
		{
			MP3GetLastFrameInfo(Mp3Decoder, &Mp3FrameInfo);		//��ȡ������Ϣ				
			/* �����DAC */
			outputSamps = Mp3FrameInfo.outputSamps;							//PCM���ݸ���
			if (outputSamps > 0)
			{
				if (Mp3FrameInfo.nChans == 1)	//������
				{
					//������������Ҫ����һ�ݵ���һ������
					for (i = outputSamps - 1; i >= 0; i--)
					{
						outbuffer[bufflag][i * 2] = outbuffer[bufflag][i];
						outbuffer[bufflag][i * 2 + 1] = outbuffer[bufflag][i];
					}
					outputSamps *= 2;
				}//if (Mp3FrameInfo.nChans == 1)	//������
			}//if (outputSamps > 0)
			
			/* ���ݽ�����Ϣ���ò����� */
			if (Mp3FrameInfo.samprate != mp3player.ucFreq)	//������ 
			{
				mp3player.ucFreq = Mp3FrameInfo.samprate;
				
				printf(" \r\n Bitrate       %dKbps", Mp3FrameInfo.bitrate/1000);
				printf(" \r\n Samprate      %dHz", mp3player.ucFreq);
				printf(" \r\n BitsPerSample %db", Mp3FrameInfo.bitsPerSample);
				printf(" \r\n nChans        %d", Mp3FrameInfo.nChans);
				printf(" \r\n Layer         %d", Mp3FrameInfo.layer);
				printf(" \r\n Version       %d", Mp3FrameInfo.version);
				printf(" \r\n OutputSamps   %d", Mp3FrameInfo.outputSamps);
				printf("\r\n");
				//I2S_AudioFreq_Default = 2��������֡��ÿ�ζ�Ҫ������
				if(mp3player.ucFreq >= I2S_AudioFreq_Default)	
				{
					//���ݲ������޸�I2S����
					I2Sx_Mode_Config(I2S_Standard_Phillips,I2S_DataFormat_16b,mp3player.ucFreq);
					I2Sx_TX_DMA_Init((uint16_t *)outbuffer[0],(uint16_t *)outbuffer[1],outputSamps);
				}
				I2S_Play_Start();
			}
		}//else ��������
		
		if(file.fptr==file.fsize) 		//mp3�ļ���ȡ��ɣ��˳�
		{
			printf("END\r\n");
			break;
		}	
		
		while(Isread==0)
		{
		}
		Isread=0;
	}
	I2S_Stop();
	mp3player.ucStatus=STA_IDLE;
	MP3FreeDecoder(Mp3Decoder);
	f_close(&file);	
}

/* DMA��������жϻص����� */
/* �����������Ѿ�������ɣ���Ҫ�л��������������»��������ݲ��� 
   ͬʱ��ȡWAV�ļ�������䵽�Ѳ�������  */
void MP3Player_I2S_DMA_TX_Callback(void)
{
	if(I2Sx_TX_DMA_STREAM->CR&(1<<19)) //��ǰʹ��Memory1����
	{
		bufflag=0;                       //���Խ����ݶ�ȡ��������0
	}
	else                               //��ǰʹ��Memory0����
	{
		bufflag=1;                       //���Խ����ݶ�ȡ��������1
	}
	Isread=1;                          // DMA������ɱ�־
}


/***************************** (END OF FILE) *********************************/
