/**
 ****************************************************************************************************
 * @file        ch395q.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-6-17
 * @brief       CH395 ��������
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

#ifndef __CH395_H
#define __CH395_H
#include "./BSP/CH395Q/ch395inc.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/CH395Q/ch395inc.h"
#include "./BSP/CH395Q/ch395cmd.h"
#include "./SYSTEM/delay/delay.h"
#include "string.h"
#include "stdio.h"


/******************************************************************************************/
/* ���� ���� */

#define CH395_SCS_GPIO_PORT                  GPIOG
#define CH395_SCS_GPIO_PIN                   GPIO_PIN_9
#define CH395_SCS_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)            /* PG��ʱ��ʹ�� */

#define CH395_INT_GPIO_PORT                  GPIOG
#define CH395_INT_GPIO_PIN                   GPIO_PIN_6
#define CH395_INT_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)            /* PG��ʱ��ʹ�� */

#define CH395_RST_GPIO_PORT                  GPIOD
#define CH395_RST_GPIO_PIN                   GPIO_PIN_7
#define CH395_RST_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)            /* PD��ʱ��ʹ�� */

/******************************************************************************************/
#define ch395_scs_low                        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_RESET)   /* SPIƬѡ��������͵�ƽ */
#define ch395_scs_hign                       HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET)     /* SPIƬѡ��������ߵ�ƽ */
#define ch395_sdo_pin                        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)                     /* ��ȡCH395��SPI����������ŵ�ƽ */
#define ch395_int_pin_wire                   HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_6)                     /* �ٶ�CH395��INT#����,���δ������ôҲ����ͨ����ѯ�����ж������SDO����״̬ʵ�� */


typedef struct ch395q_socket_t
{
    uint8_t socket_enable;                                          /* Socketʹ�� */
    uint8_t socket_index;                                           /* Socket��� */
    uint8_t proto;                                                  /* SocketЭ�� */
    uint8_t des_ip[4];                                              /* Ŀ��IP��ַ */
    uint16_t des_port;                                              /* Ŀ�Ķ˿� */
    uint16_t sour_port;                                             /* Դ�˿� */
    
    struct
    {
        uint8_t *buf;                                               /* ����ռ� */
        uint32_t size;                                              /* ����ռ��С */
    } send;                                                         /* ���ͻ��� */
    
    struct
    {
        uint8_t recv_flag;                                          /* �������ݱ�־λ */
        uint8_t *buf;                                               /* ����ռ� */
        uint32_t size;                                              /* ����ռ��С */
    } recv;                                                         /* ���ջ��� */                                                       /* ���ջ��� */
    
    struct
    {
        uint8_t ip[4];                                              /* IP��ַ */
        uint8_t gwip[4];                                            /* ����IP��ַ */
        uint8_t mask[4];                                            /* �������� */
        uint8_t dns1[4];                                            /* DNS������1��ַ */
        uint8_t dns2[4];                                            /* DNS������2��ַ */
    } net_info;                                                     /* ������Ϣ */
    
    struct
    {
        uint8_t ipaddr[4];                                          /* IP��ַ 32bit*/
        uint8_t gwipaddr[4];                                        /* ���ص�ַ 32bit*/
        uint8_t maskaddr[4];                                        /* �������� 32bit*/
        uint8_t macaddr[6];                                         /* MAC��ַ 48bit*/
    } net_config;                                                   /* ����������Ϣ */

} ch395_socket;

/* DHCP״̬ */
enum DHCP
{
    DHCP_UP = 0,                                                    /* DHCP��ȡ�ɹ�״̬ */
    DHCP_DOWN,                                                      /* DHCP��ȡʧ��״̬ */
    DHCP_STA,                                                       /* DHCP����״̬ */
};

enum START
{
    DATA_RECV_STAT = 0,
    CONNECT_STAT,
    DISCONNECT_STAT,
    TIM_OUT_STAT,
};

struct ch395q_t
{
    uint8_t version;                                                /* �汾��Ϣ */
    uint8_t phy_status;                                             /* PHY״̬ */
    uint8_t dhcp_status;                                            /* DHCP״̬ */
    uint8_t  ipinf_buf[20];                                         /* ��ȡIP��Ϣ */
    
    struct
    {
        ch395_socket config;                                        /* ������Ϣ */
    } socket[8];                                                    /* Socket״̬ */
    
    uint16_t switch_status;                                         /* ����״̬ */
    
    void (*ch395_error)(uint8_t i);                                 /* ch395q�����⺯�� */
    void (*ch395_phy_cb)(uint8_t phy_status);                       /* ch395q phy״̬�ص����� */
    void (*ch395_reconnection)(void);                               /* ch395q �������Ӻ��� */
};

extern struct ch395q_t g_ch395q_sta;

/* CH395Qģ��Socket��Ŷ��� */
#define CH395Q_SOCKET_0             0                               /* Socket 0 */
#define CH395Q_SOCKET_1             1                               /* Socket 1 */
#define CH395Q_SOCKET_2             2                               /* Socket 2 */
#define CH395Q_SOCKET_3             3                               /* Socket 3 */
#define CH395Q_SOCKET_4             4                               /* Socket 4 */
#define CH395Q_SOCKET_5             5                               /* Socket 5 */
#define CH395Q_SOCKET_6             6                               /* Socket 6 */
#define CH395Q_SOCKET_7             7                               /* Socket 7 */

/* ʹ�ܶ��� */
#define CH395Q_DISABLE              1                               /* ���� */
#define CH395Q_ENABLE               2                               /* ʹ�� */

/* CH395Qģ��SocketЭ�����Ͷ��� */
#define CH395Q_SOCKET_UDP           0                               /* UDP */
#define CH395Q_SOCKET_TCP_CLIENT    1                               /* TCP�ͻ��� */
#define CH395Q_SOCKET_TCP_SERVER    2                               /* TCP������ */
#define CH395Q_SOCKET_MAC_RAW       3                               /* MAC_RAW */

#define DEF_KEEP_LIVE_IDLE          (15*1000)                       /* ����ʱ�� */
#define DEF_KEEP_LIVE_PERIOD        (15*1000)                       /* ���Ϊ15�룬����һ��KEEPLIVE���ݰ� */
#define DEF_KEEP_LIVE_CNT           200

uint8_t  ch395_read_data(void ) ;
void ch395_write_cmd( uint8_t mcmd );
void ch395_write_data( uint8_t mdata );
void ch395q_handler(void);
void ch395_interrupt_handler(void);
void ch395_hardware_init(void);
uint8_t ch395q_socket_config(ch395_socket * ch395_sokect);
void ch395_reconnection(void);

#endif
