#include "sddriver.h"

/* Private variables ---------------------------------------------------------*/
u8  SD_Type=0;

void SPI_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef	 SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/*----------------------SPI�˿�����---------------------------------*/
	/*SPI_SCK SPI_MOSI �����������*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  

	/*SPI_MISO ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  


	/*SD_CS �������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*ʧ��SD��*/
	SD_CS_DISABLE();

	/*----------------------SPI1����-----------------------------------*/
	/* ��ʼ�� */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	/*����ʱ���ȶ�״̬�Ǹߵ�ƽ*/
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;/* ʱ�ӻ�Ծ���أ�����*/
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; /*SPI������Ԥ��Ƶ*/
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /*��λ��ǰ*/ 
	SPI_InitStructure.SPI_CRCPolynomial = 7; /*the polynomial used for the CRC calculation*/
	SPI_Init(SPI1, &SPI_InitStructure);

	/* SPI1ʹ�� */
	SPI_Cmd(SPI1, ENABLE);
}

void SPI_SetSpeed(u8 SpeedSet)
{
    SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	/*����ʱ���ȶ�״̬�Ǹߵ�ƽ*/
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;/* ʱ�ӻ�Ծ���أ�����*/
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /*��λ��ǰ*/ 
	SPI_InitStructure.SPI_CRCPolynomial = 7; /*the polynomial used for the CRC calculation*/
    /*����ٶ���������0�������ģʽ����0�����ģʽ*/
    if(SpeedSet==SPI_SPEED_LOW)
    {
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    }
    else
    {
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    }
	SPI_Init(SPI1, &SPI_InitStructure);
}

/*******************************************************************************
* Function Name  : SPI_ReadWriteByte
* Description    : SPI��дһ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
* Input          : u8 TxData �����͵���
* Output         : None
* Return         : u8 RxData �յ�����
*******************************************************************************/
u8 SPI_ReadWriteByte(u8 TxData)
{
    u8 RxData = 0;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);	/*�ȴ����ͻ������� */
	SPI_I2S_SendData(SPI1, TxData);	               /*��һ���ֽ�*/

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);	 /*�ȴ����ݽ���	 */
	RxData = SPI_I2S_ReceiveData(SPI1);	        /*���ؽ��յ�������*/

    return (u8)RxData;
}

/*******************************************************************************
* Function Name  : SD_SendCommand
* Description    : ��SD������һ������
* Input          : u8 cmd   ���� 
*                  u32 arg  �������
*                  u8 crc   crcУ��ֵ
* Output         : None
* Return         : u8 r1 SD�����ص���Ӧ
*******************************************************************************/
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc)
{
    unsigned char r1;
    unsigned char Retry = 0;

    SPI_ReadWriteByte(0xff);
    //Ƭѡ���õͣ�ѡ��SD��
    SD_CS_ENABLE();

    //����
    SPI_ReadWriteByte(cmd | 0x40);                         //�ֱ�д������
    SPI_ReadWriteByte(arg >> 24);
    SPI_ReadWriteByte(arg >> 16);
    SPI_ReadWriteByte(arg >> 8);
    SPI_ReadWriteByte(arg);
    SPI_ReadWriteByte(crc);
    
    //�ȴ���Ӧ����ʱ�˳�
    while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
    {
        Retry++;
        if(Retry > 200)
        {
            break;
        }
    }
    //�ر�Ƭѡ
    SD_CS_DISABLE();
    //�������϶�������8��ʱ�ӣ���SD�����ʣ�µĹ���
    SPI_ReadWriteByte(0xFF);

    //����״ֵ̬
    return r1;
}

/*******************************************************************************
* Function Name  : SD_SendCommand_NoDeassert
* Description    : ��SD������һ������(�����ǲ�ʧ��Ƭѡ�����к������ݴ�����
* Input          : u8 cmd   ���� 
*                  u32 arg  �������
*                  u8 crc   crcУ��ֵ
* Output         : None
* Return         : u8 r1 SD�����ص���Ӧ
*******************************************************************************/
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc)
{
    unsigned char r1;
    unsigned char Retry = 0;

    //????????
    SPI_ReadWriteByte(0xff);
    //Ƭѡ���õͣ�ѡ��SD��
    SD_CS_ENABLE();

    //����
    SPI_ReadWriteByte(cmd | 0x40);                         //�ֱ�д������
    SPI_ReadWriteByte(arg >> 24);
    SPI_ReadWriteByte(arg >> 16);
    SPI_ReadWriteByte(arg >> 8);
    SPI_ReadWriteByte(arg);
    SPI_ReadWriteByte(crc);

    //�ȴ���Ӧ����ʱ�˳�
    while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
    {
        Retry++;
        if(Retry > 200)
        {
            break;
        }
    }
    //������Ӧֵ
    return r1;
}

uint8_t SD_Init(void)
{
	uint16_t  i;        /* ����ѭ������ */
    uint8_t   r1;       /* ���SD���ķ���ֵ	*/
    uint16_t  retry;    /* �������г�ʱ����	 */
    uint8_t   buff[6];
	uint16_t  temp;

	SPI_Configuration();    /* SD��Ӳ����ʼ�� */

	/*��ʼ������Ϊ����ģʽ
	  ��ΪSPI��ʼ��ʱ�ǵ��ٵģ�SD����ʼ����ɺ�����Ϊ���٣�
	  �еĿ�ֻҪ��ʼ��һ�ξ��У������ok�� 
	  ���еĿ���Ҫ��θ�λ 
	*/
    SPI_SetSpeed(SPI_SPEED_LOW);  
   
   /*�Ȳ���>74�����壬��SD���Լ���ʼ����� */
	for(i=0;i<50;i++)
	{
		temp = SPI_ReadWriteByte(0xFF);		  
	}
	/*-----------------SD����λ��idle��ʼ-----------------*/
    /*
	  ѭ����������CMD0��ֱ��SD������0x01,����IDLE״̬
      ��ʱ��ֱ���˳�
	*/
    retry = 0;
    do
    {
        r1 = SD_SendCommand(CMD0, 0, 0x95);	  /*����CMD0����SD������IDLE״̬ */
        retry++;
    }while((r1 != 0x01) && (retry<200));

    /*����ѭ���󣬼��ԭ�򣺳�ʼ���ɹ���or ���Գ�ʱ��*/
    if(retry==200)
    {
		printf("\r\n ��ʱ����  \r\n");
        return 1;   /* ��ʱ����1 */
    }
	printf("\r\n SD���Ѹ�λ��idle \r\n", temp);

	/*------------��ȡ��Ƭ��SD�汾��Ϣ����������Ӧ�ĳ�ʼ��----------------*/
    r1 = SD_SendCommand_NoDeassert(8, 0x1aa, 0x87);   
	printf("\r\n ��Ƭ��SD�汾��Ϣ��0x%x \r\n", r1);

	/*----------������v1.0��汾�ĳ�ʼ��-----------*/
    if(r1 == 0x05)    
    {
        //���ÿ�����ΪSDV1.0����������⵽ΪMMC�������޸�ΪMMC
        SD_Type = SD_TYPE_V1;

        //�����V1.0����CMD8ָ���û�к�������
        //Ƭѡ�øߣ�������������
        SD_CS_DISABLE();
        //�෢8��CLK����SD������������
        SPI_ReadWriteByte(0xFF);

        //-----------------SD����MMC����ʼ����ʼ-----------------

        //������ʼ��ָ��CMD55+ACMD41
        // �����Ӧ��˵����SD�����ҳ�ʼ�����
        // û�л�Ӧ��˵����MMC�������������Ӧ��ʼ��
        retry = 0;
        do
        {
            //�ȷ�CMD55��Ӧ����0x01���������
            r1 = SD_SendCommand(CMD55, 0, 0);
            if(r1 != 0x01)
            {
                return r1;  
            }
            //�õ���ȷ��Ӧ�󣬷�ACMD41��Ӧ�õ�����ֵ0x00����������200��
            r1 = SD_SendCommand(ACMD41, 0, 0);
            retry++;
        }while((r1!=0x00) && (retry<400));
        // �ж��ǳ�ʱ���ǵõ���ȷ��Ӧ
        // ���л�Ӧ����SD����û�л�Ӧ����MMC��
        
        //----------MMC�������ʼ��������ʼ------------
        if(retry==400)
        {
            retry = 0;
            //����MMC����ʼ�����û�в��ԣ�
            do
            {
                r1 = SD_SendCommand(1, 0, 0);
                retry++;
            }while((r1!=0x00)&& (retry<400));
            if(retry==400)
            {
                return 1;   //MMC����ʼ����ʱ
            }
            //д�뿨����
            SD_Type = SD_TYPE_MMC;
        }
        //----------MMC�������ʼ����������------------
        
        //����SPIΪ����ģʽ
        SPI_SetSpeed(1);

		SPI_ReadWriteByte(0xFF);
        
        //��ֹCRCУ��
        /*
		r1 = SD_SendCommand(CMD59, 0, 0x01);
        if(r1 != 0x00)
        {
            return r1;  //������󣬷���r1
        }
        */    
        //����Sector Size
        r1 = SD_SendCommand(CMD16, 512, 0xff);
        if(r1 != 0x00)
        {
            return r1;  //������󣬷���r1
        }
    }
    
    /*-------------------������V2.0���ĳ�ʼ��--------------------------*/
    /*������Ҫ��ȡOCR���ݣ��ж���SD2.0����SD2.0HC��*/
    else if(r1 == 0x01)
    {
		printf("\r\n ����V2.0�Ŀ� \r\n");

        //V2.0�Ŀ���CMD8�����ᴫ��4�ֽڵ����ݣ�Ҫ�����ٽ���������
        buff[0] = SPI_ReadWriteByte(0xFF);  //should be 0x00
        buff[1] = SPI_ReadWriteByte(0xFF);  //should be 0x00
        buff[2] = SPI_ReadWriteByte(0xFF);  //should be 0x01
        buff[3] = SPI_ReadWriteByte(0xFF);  //should be 0xAA
        SD_CS_DISABLE();

        //the next 8 clocks
        SPI_ReadWriteByte(0xFF);
        
        //�жϸÿ��Ƿ�֧��2.7V-3.6V�ĵ�ѹ��Χ
        if(buff[2]==0x01 && buff[3]==0xAA)
        {
            //֧�ֵ�ѹ��Χ�����Բ���
			printf("\r\n ֧�ֵ�ѹ��Χ�����Բ��� \r\n");
            retry = 0;
            //������ʼ��ָ��CMD55+ACMD41
    		do
    		{
    			r1 = SD_SendCommand(CMD55, 0, 0);
    			if(r1!=0x01)
    			{
    				return r1;
    			}
    			r1 = SD_SendCommand(ACMD41, 0x40000000, 0);
                if(retry>200)   
                {
                    return r1;  //��ʱ�򷵻�r1״̬
                }
            }while(r1!=0);
			printf("\r\n ��ʼ��ָ������ \r\n");
          
            //��ʼ��ָ�����ɣ���������ȡOCR��Ϣ

            //-----------����SD2.0���汾��ʼ-----------
            r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
            if(r1!=0x00)
            {
                return r1;  //�������û�з�����ȷӦ��ֱ���˳�������Ӧ��
            }
			printf("\r\n SD2.0���汾������� r1 = 0x%x \r\n", r1);
            //��OCRָ����󣬽�������4�ֽڵ�OCR��Ϣ
            buff[0] = SPI_ReadWriteByte(0xFF);
            buff[1] = SPI_ReadWriteByte(0xFF); 
            buff[2] = SPI_ReadWriteByte(0xFF);
            buff[3] = SPI_ReadWriteByte(0xFF);

            //OCR������ɣ�Ƭѡ�ø�
            SD_CS_DISABLE();
            SPI_ReadWriteByte(0xFF);

            //�����յ���OCR�е�bit30λ��CCS����ȷ����ΪSD2.0����SDHC
            //���CCS=1��SDHC   CCS=0��SD2.0
            if(buff[0]&0x40)    //���CCS
            {
                SD_Type = SD_TYPE_V2HC;
            }
            else
            {
                SD_Type = SD_TYPE_V2;
            }
            
            //����SPIΪ����ģʽ
            SPI_SetSpeed(SPI_SPEED_HIGH);  
        }
    }
	printf("\r\n ��ʼ�����������ص�r1 = 0x%x",r1);
    return r1;
}

/*******************************************************************************
* Function Name  : SD_ReadSingleBlock
* Description    : ��SD����һ��block
* Input          : u32 sector ȡ��ַ��sectorֵ���������ַ�� 
*                  u8 *buffer ���ݴ洢��ַ����С����512byte�� 
* Output         : None
* Return         : u8 r1 
*                  0�� �ɹ�
*                  other��ʧ��
*******************************************************************************/
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer)
{
	u8 r1;

    //����Ϊ����ģʽ
    SPI_SetSpeed(SPI_SPEED_HIGH);
    
    if(SD_Type==SD_TYPE_V2HC)
    {
        sector = sector>>9;
    }

	r1 = SD_SendCommand(CMD17, sector, 0);//������
	
	if(r1 != 0x00)
    {
        return r1;
    }
    
    r1 = SD_ReceiveData(buffer, 512, RELEASE);
    if(r1 != 0)
    {
        return r1;   //�����ݳ���
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
* Function Name  : SD_ReadMultiBlock
* Description    : ��SD���Ķ��block
* Input          : u32 sector ȡ��ַ��sectorֵ���������ַ�� 
*                  u8 *buffer ���ݴ洢��ַ����С����512byte��
*                  u8 count ������count��block
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count)
{
    u8 r1;

    //����Ϊ����ģʽ
    SPI_SetSpeed(SPI_SPEED_HIGH);
    
    if(SD_Type==SD_TYPE_V2HC)
    {
        sector = sector>>9;
    }
    //SD_WaitReady();
    //�����������
	r1 = SD_SendCommand(CMD18, sector, 0);//������
	if(r1 != 0x00)
    {
        return r1;
    }
    //��ʼ��������
    do
    {
        if(SD_ReceiveData(buffer, 512, NO_RELEASE) != 0x00)
        {
            break;
        }
        buffer += 512;
    } while(--count);

    //ȫ��������ϣ�����ֹͣ����
    SD_SendCommand(CMD12, 0, 0);
    //�ͷ�����
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xFF);
    
    if(count != 0)
    {
        return count;   //���û�д��꣬����ʣ�����
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
* Function Name  : SD_WriteSingleBlock
* Description    : д��SD����һ��block
* Input          : u32 sector ������ַ��sectorֵ���������ַ�� 
*                  u8 *buffer ���ݴ洢��ַ����С����512byte�� 
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_WriteSingleBlock(u32 sector, const u8 *data)
{
    u8 r1;
    u16 i;
    u16 retry;

    //����Ϊ����ģʽ
    SPI_SetSpeed(SPI_SPEED_HIGH);

    //�������SDHC����������sector��ַ������ת����byte��ַ
    if(SD_Type==SD_TYPE_V2HC)
    {
        sector = sector>>9;
    }

    r1 = SD_SendCommand(CMD24, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1;  //Ӧ����ȷ��ֱ�ӷ���
    }
    
    //��ʼ׼�����ݴ���
    SD_CS_ENABLE();
    //�ȷ�3�������ݣ��ȴ�SD��׼����
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    //����ʼ����0xFE
    SPI_ReadWriteByte(0xFE);

    //��һ��sector������
    for(i=0;i<512;i++)
    {
        SPI_ReadWriteByte(*data++);
    }
    //��2��Byte��dummy CRC
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    
    //�ȴ�SD��Ӧ��
    r1 = SPI_ReadWriteByte(0xff);
    if((r1&0x1F)!=0x05)
    {
        SD_CS_DISABLE();
        return r1;
    }
    
    //�ȴ��������
    retry = 0;
    while(!SPI_ReadWriteByte(0xff))
    {
        retry++;
        if(retry>0xfffe)        //�����ʱ��д��û����ɣ������˳�
        {
            SD_CS_DISABLE();
            return 1;           //д�볬ʱ����1
        }
    }

    //д����ɣ�Ƭѡ��1
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xff);

    return 0;
}

/*******************************************************************************
* Function Name  : SD_WaitReady
* Description    : �ȴ�SD��Ready
* Input          : None
* Output         : None
* Return         : u8 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_WaitReady(void)
{
    u8 r1;
    u16 retry;
    retry = 0;
    do
    {
        r1 = SPI_ReadWriteByte(0xFF);
        if(retry==0xfffe)
        {
            return 1;
        }
    }while(r1!=0xFF);

    return 0;
}

/*******************************************************************************
* Function Name  : SD_GetCapacity
* Description    : ��ȡSD��������
* Input          : None
* Output         : None
* Return         : u32 capacity 
*                   0�� ȡ�������� 
*******************************************************************************/
u32 SD_GetCapacity(void)
{
    u8 csd[16];
    u32 Capacity;
    u8 r1;
    u16 i;
	u16 temp;

    //ȡCSD��Ϣ������ڼ��������0
    if(SD_GetCSD(csd)!=0)
    {
        return 0;
    }
       
    //���ΪSDHC�����������淽ʽ����
    if((csd[0]&0xC0)==0x40)
    {
        Capacity =  ((((u32)csd[8])<<8) + (u32)csd[9] +1)*(u32)1024;
    }
    else
    {
        /**
		�������Ϊ���ϰ汾 ��������
        *	formula of the capacity
        *	memory capacity = BLOCKNR * BLOCK_LEN
        *	BLOCKNR = (C_SIZE + 1)* MULT
        *	C_SIZE_MULT+2
        *	MULT = 2
        *	READ_BL_LEN
        *	BLOCK_LEN = 2
        **/

        //C_SIZE
    	i = csd[6]&0x03;
    	i<<=8;
    	i += csd[7];
    	i<<=2;
    	i += ((csd[8]&0xc0)>>6);
    
        //C_SIZE_MULT
    	r1 = csd[9]&0x03;
    	r1<<=1;
    	r1 += ((csd[10]&0x80)>>7);
    
        //BLOCKNR
    	r1+=2;
    	temp = 1;
    	while(r1)
    	{
    		temp*=2;
    		r1--;
    	}
    	Capacity = ((u32)(i+1))*((u32)temp);
    
        // READ_BL_LEN
    	i = csd[5]&0x0f;
        //BLOCK_LEN
    	temp = 1;
    	while(i)
    	{
    		temp*=2;
    		i--;
    	}
        //The final result
    	Capacity *= (u32)temp; 
    	//Capacity /= 512;
    }
    return (u32)Capacity;
}

/*******************************************************************************
* Function Name  : SD_GetCSD
* Description    : ��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
* Input          : u8 *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : u8 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
u8 SD_GetCSD(u8 *csd_data)
{
    u8 r1;

    //��CMD9�����CSD
    r1 = SD_SendCommand(CMD9, 0, 0xFF);
    if(r1 != 0x00)
    {
        return r1;  //û������ȷӦ�����˳�������
    }
    //����16���ֽڵ�����
    SD_ReceiveData(csd_data, 16, RELEASE);

    return 0;
}

/*******************************************************************************
* Function Name  : SD_ReceiveData
* Description    : ��SD���ж���ָ�����ȵ����ݣ������ڸ���λ��
* Input          : u8 *data(��Ŷ������ݵ��ڴ�>len)
*                  u16 len(���ݳ��ȣ�
*                  u8 release(������ɺ��Ƿ��ͷ�����CS�ø� 0�����ͷ� 1���ͷţ�
* Output         : None
* Return         : u8 
*                  0��NO_ERR
*                  other��������Ϣ
*******************************************************************************/
u8 SD_ReceiveData(u8 *data, u16 len, u8 release)
{
    u16 retry;
    u8 r1;

    // ����һ�δ���
    SD_CS_ENABLE();
    //�ȴ�SD������������ʼ����0xFE
    retry = 0;
    do
    {
        r1 = SPI_ReadWriteByte(0xFF);
        retry++;
        if(retry>2000)  //2000�εȴ���û��Ӧ���˳�����
        {
            SD_CS_DISABLE();
            return 1;
        }
    }while(r1 != 0xFE);
    //��ʼ��������
    while(len--)
    {
        *data = SPI_ReadWriteByte(0xFF);
        data++;
    }
    //������2��αCRC��dummy CRC��
    SPI_ReadWriteByte(0xFF);
    SPI_ReadWriteByte(0xFF);
    //�����ͷ����ߣ���CS�ø�
    if(release == RELEASE)
    {
        //�������
        SD_CS_DISABLE();
        SPI_ReadWriteByte(0xFF);
    }

    return 0;
}

/*******************************************************************************
* Function Name  : SD_WriteMultiBlock
* Description    : д��SD����N��block
* Input          : u32 sector ������ַ��sectorֵ���������ַ�� 
*                  u8 *buffer ���ݴ洢��ַ����С����512byte��
*                  u8 count д���block��Ŀ
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count)
{
    u8 r1;
    u16 i;

    //����Ϊ����ģʽ
    SPI_SetSpeed(SPI_SPEED_HIGH);

    //�������SDHC����������sector��ַ������ת����byte��ַ
    if(SD_Type == SD_TYPE_V2HC)
    {
        sector = sector>>9;
    }
    //���Ŀ�꿨����MMC��������ACMD23ָ��ʹ��Ԥ����
    if(SD_Type != SD_TYPE_MMC)
    {
        r1 = SD_SendCommand(ACMD23, count, 0x00);
    }
    //�����д��ָ��
    r1 = SD_SendCommand(CMD25, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1;  //Ӧ����ȷ��ֱ�ӷ���
    }
    
    //��ʼ׼�����ݴ���
    SD_CS_ENABLE();
    //�ȷ�3�������ݣ��ȴ�SD��׼����
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);

    //--------������N��sectorд���ѭ������
    do
    {
        //����ʼ����0xFC �����Ƕ��д��
        SPI_ReadWriteByte(0xFC);
    
        //��һ��sector������
        for(i=0;i<512;i++)
        {
            SPI_ReadWriteByte(*data++);
        }
        //��2��Byte��dummy CRC
        SPI_ReadWriteByte(0xff);
        SPI_ReadWriteByte(0xff);
        
        //�ȴ�SD��Ӧ��
        r1 = SPI_ReadWriteByte(0xff);
        if((r1&0x1F)!=0x05)
        {
            SD_CS_DISABLE();    //���Ӧ��Ϊ��������������ֱ���˳�
            return r1;
        }

        //�ȴ�SD��д�����
        if(SD_WaitReady()==1)
        {
            SD_CS_DISABLE();    //�ȴ�SD��д����ɳ�ʱ��ֱ���˳�����
            return 1;
        }

        //��sector���ݴ������
    }while(--count);
    
    //��������������0xFD
    r1 = SPI_ReadWriteByte(0xFD);
    if(r1==0x00)
    {
        count =  0xfe;
    }

    if(SD_WaitReady())
    {
        while(1)
        {
        }
    }
    
    //д����ɣ�Ƭѡ��1
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xff);

    return count;   //����countֵ�����д����count=0������count=1
}




