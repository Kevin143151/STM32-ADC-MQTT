/**
 ****************************************************************************************************
 * @file        ch395_demo.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-6-17
 * @brief       CH395 OneNET ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200417
 * ��һ�η���
 *
 ****************************************************************************************************
 */
 
#ifndef __CH395_DEMO_H
#define __CH395_DEMO_H
#include "./SYSTEM/sys/sys.h"
#include "MQTTPacket.h"


/* �û���Ҫ�����豸��Ϣ�������º궨���е���Ԫ������ */
#define USER_DEVICE_NAME "MQTT"                                         /* �豸�� */
#define USER_PRODUCT_ID  "366007"                                       /* ��ƷID */
#define USER_ACCESS_KEY  "qlWudWg/3ANGVQLeHGfAu0Eh8J7CWgozfOpljI+Gy8k=" /* ��Ʒ��Կ */
#define USER_DEVICE_ID   "617747917"                                    /* ��Ʒ�豸ID */
#define USER_KEY         "QyxIRiJNQG5wPmEmMGY8QGVsRUtIZDtVUGI0eCQ1V3A=" /* �豸��Կ */
/* ��������Ҫonenet�ṩ��token�������ó� */
#define PASSWORD "version=2018-10-31&res=products%2F366007%2Fdevices%2FMQTT&et=1672735919&method=md5&sign=qI0pgDJnICGoPdhNi%2BHtfg%3D%3D"

/* ���²����ĺ궨��̶�������Ҫ�޸ģ�ֻ�޸��Ϸ��Ĳ������� */
#define HOST_NAME           "open.iot.10086.cn"  /*onenet���� */
#define DEVICE_SUBSCRIBE    "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/dp/post/json/+"          /* ���� */
#define DEVICE_PUBLISH      "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/dp/post/json"            /* ���� */
#define SERVER_PUBLISH      "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/cmd/request/+"    /* �������·����� */

#define CH395_SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CH395_CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define CH395_READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CH395_CLEAR_REG(REG)        ((REG) = (0x0))

#define CH395_WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define CH395_READ_REG(REG)         ((REG))

#define CH395_MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define vPOSITION_VAL(VAL)     (__CLZ(__RBIT(VAL))) 


typedef struct
{
    char    pro_id[10];
    char    access_key[48];
    char    dev_name[64 + 1];
    char    dev_id[16];
    char    key[48];
} onenet_info_t;

typedef struct ch395q_mqtt_t
{
    
    struct
    {
        int g_rc;
        int g_subcount;
        int granted_qos;
        unsigned char g_sessionpresent;
        unsigned char g_connack_rc;
        unsigned short g_submsgid;
        int g_len;
        int g_req_qos;
        uint8_t g_msgtypes; 
    }mqtt_argument;

    struct
    {
        float g_temp;                                       /* �¶�ֵ */
        float g_humid;                                      /* ʪ��ֵ */
    }send_data;
    
}ch395q_mqtt;

void ch395_demo(void);      /* ���̲��� */

#endif
