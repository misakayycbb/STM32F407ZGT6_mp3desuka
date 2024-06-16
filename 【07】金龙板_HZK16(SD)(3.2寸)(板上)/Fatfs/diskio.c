/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

//#include "Config.h"
#include "diskio.h"
#include "sddriver.h"
/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
#define BlockSize            512 /* Block Size in Bytes [�����block�����ǡ���������SD����ÿ������Ϊ512�ֽ�]*/
#define BufferWordsSize      (BlockSize >> 2)	//128������ο�MAIN�еĽ��ͣ���128��32λ�Ļ����������512���ֽ�
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
    BYTE state;
	u8 i=0;

    if(drv)
    {
        return STA_NOINIT;  //��֧�ִ���0�Ĳ���
    }

    state = SD_Init();
	do {
    	state = SD_Init();
    }while(state&&(i++<20))	;
    if(state == STA_NODISK)
    {
        return STA_NODISK;
    }
    else if(state != 0)
    {
        return STA_NOINIT;  //�������󣺳�ʼ��ʧ��
    }
    else
    {
        return RES_OK;           //��ʼ���ɹ�
    }
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
    if(drv)
    {
        return STA_NOINIT;  //��֧�ִ���0����
    }

    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	u8 res=0;
    if (drv || !count)
    {    
        return RES_PARERR;  /*��֧�ֵ����̲�����count���ܵ���0�����򷵻ز�������*/
    }  
	
    if(count==1)            /*1��sector�Ķ�����  */    
    {                                                
        res = SD_ReadSingleBlock(sector<<9 ,(&buff[0]));;      
    }                                                
    else                    /*���sector�Ķ����� */    
    {                                                
        res = SD_ReadMultiBlock(sector<<9,(&buff[0]),count);
    }
	                                                
    /* ������ֵ����sddriver.c�ķ���ֵת��ff.c�ķ���ֵ */
    if(res == 0x00)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	u8 res;

    if (drv || !count)
    {    
        return RES_PARERR;  /*  ��֧�ֵ����̲�����count���ܵ���0�����򷵻ز�������  */
    }

    // ��д����
    if(count == 1)
    {
        res = SD_WriteSingleBlock(sector<<9, buff);
    }
    else
    {
        res = SD_WriteMultiBlock(sector<<9, buff, count);
    }
    // ����ֵת��
    if(res == 0)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT res;
    if (drv)
    {    
        return RES_PARERR;  /*��֧�ֵ����̲������������� */
    }
    
    /*FATFSĿǰ�汾���账��CTRL_SYNC��GET_SECTOR_COUNT��GET_BLOCK_SIZ��������*/
    switch(ctrl)
    {
	    case CTRL_SYNC:
	        SD_CS_ENABLE();
	        if(SD_WaitReady()==0)
	        {
	            res = RES_OK;
	        }
	        else
	        {
	            res = RES_ERROR;
	        }
	        SD_CS_DISABLE();
	        break;
	        
	    case GET_BLOCK_SIZE:
	        *(WORD*)buff = 512;
	        res = RES_OK;
	        break;
	
	    case GET_SECTOR_COUNT:
	        *(DWORD*)buff = SD_GetCapacity();
	        res = RES_OK;
	        break;

	    default:
	        res = RES_PARERR;
	    	break;
    }

    return res;
}


/*-----------------------------------------------------------------------*/
/* User defined function to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{   
    return 0;
}

//�õ��ļ�Calendar��ʽ�Ľ�������,��DWORD get_fattime (void) ��任							


