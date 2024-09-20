/**
 ****************************************************************************************************
 * @file        ch395_demo.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-6-17
 * @brief       CH395 OneNET 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200417
 * 第一次发布
 *
 ****************************************************************************************************
 */
 
#ifndef __CH395_DEMO_H
#define __CH395_DEMO_H
#include "./SYSTEM/sys/sys.h"
#include "MQTTPacket.h"


/* 用户需要根据设备信息完善以下宏定义中的三元组内容 */
#define USER_DEVICE_NAME "MQTT"                                         /* 设备名 */
#define USER_PRODUCT_ID  "366007"                                       /* 产品ID */
#define USER_ACCESS_KEY  "qlWudWg/3ANGVQLeHGfAu0Eh8J7CWgozfOpljI+Gy8k=" /* 产品密钥 */
#define USER_DEVICE_ID   "617747917"                                    /* 产品设备ID */
#define USER_KEY         "QyxIRiJNQG5wPmEmMGY8QGVsRUtIZDtVUGI0eCQ1V3A=" /* 设备密钥 */
/* 该密码需要onenet提供的token软件计算得出 */
#define PASSWORD "version=2018-10-31&res=products%2F366007%2Fdevices%2FMQTT&et=1672735919&method=md5&sign=qI0pgDJnICGoPdhNi%2BHtfg%3D%3D"

/* 以下参数的宏定义固定，不需要修改，只修改上方的参数即可 */
#define HOST_NAME           "open.iot.10086.cn"  /*onenet域名 */
#define DEVICE_SUBSCRIBE    "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/dp/post/json/+"          /* 订阅 */
#define DEVICE_PUBLISH      "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/dp/post/json"            /* 发布 */
#define SERVER_PUBLISH      "$sys/"USER_PRODUCT_ID"/"USER_DEVICE_NAME"/cmd/request/+"    /* 服务器下发命令 */

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
        float g_temp;                                       /* 温度值 */
        float g_humid;                                      /* 湿度值 */
    }send_data;
    
}ch395q_mqtt;

void ch395_demo(void);      /* 例程测试 */

#endif
