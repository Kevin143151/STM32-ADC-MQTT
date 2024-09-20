/**
 ****************************************************************************************************
 * @file        ch395_demo.c
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
 ****************************************************************************************************
 */

#include "./APP/ch395_demo.h"
#include "./MALLOC/malloc.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/CH395Q/ch395.h"
#include "MQTTPacket.h"
#include "token.h"
#include "string.h"


/* ����������Ϣ��IP��ַ�����ص�ַ�����������MAC��ַ */
uint8_t ch395_ipaddr[4]     = {192,168,1,10};
uint8_t ch395_gw_ipaddr[4]  = {192,168,1,1};
uint8_t ch395_ipmask[4]     = {255,255,255,0};
uint8_t ch395_macaddr[6]    = {0xB8,0xAE,0x1D,0x00,0x00,0x00};
/* Զ��IP��ַ���� */
uint8_t ch395_des_ipaddr[4] = {183, 230, 40, 96};
static uint8_t socket0_send_buf[200] = {0};
static uint8_t socket0_recv_buf[1024];
ch395_socket cha95_sockct_sta[8];

ch395q_mqtt ch395q_mq_config;
MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
MQTTString receivedTopic;
MQTTString topicString = MQTTString_initializer;
onenet_info_t g_onenet_info = {  "", "", "", "", ""};
uint8_t g_subscribe_state = 0;

int g_payloadlen_in;
unsigned char *g_payload_in;
unsigned short g_msgid = 1;
unsigned char g_dup;
int g_qos;
unsigned char g_retained = 0;

/**
 * @brief  ��ʼ��MQTT����
 * @param  ��
 * @retval ��
 */
void ch395q_mqtt_confing(void)
{
    ch395q_mq_config.mqtt_argument.granted_qos = 0;
    ch395q_mq_config.mqtt_argument.g_connack_rc = 0;
    ch395q_mq_config.mqtt_argument.g_len = 0;
    ch395q_mq_config.mqtt_argument.granted_qos = 0;
    ch395q_mq_config.mqtt_argument.g_msgtypes= CONNECT;
    ch395q_mq_config.mqtt_argument.g_rc = 0;
    ch395q_mq_config.mqtt_argument.g_req_qos = 1;
    ch395q_mq_config.mqtt_argument.g_sessionpresent = 0;
    ch395q_mq_config.mqtt_argument.g_subcount = 0;
    ch395q_mq_config.mqtt_argument.g_submsgid = 0;
    ch395q_mq_config.send_data.g_humid = 0;
    ch395q_mq_config.send_data.g_temp = 0;
}

/**
 * @brief  ͨ��TCP��ʽ�������ݵ�TCP������
 * @param  sockͨ��
 * @param  buf�����׵�ַ
 * @param  buflen���ݳ���
 * @retval С��0��ʾ����ʧ��
 */
int ch395_transport_send_packet_buffer(int sock, unsigned char* buf, int buflen)
{
    ch395_send_data(sock, buf, buflen);
    return buflen;
}

/**
 * @brief  ������ʽ����TCP���������͵�����
 * @param  buf���ݴ洢�׵�ַ
 * @param  count���ݻ���������
 * @retval С��0��ʾ��������ʧ��
 */
int ch395_transport_get_data(unsigned char *buf, int count)
{
    ch395_get_recv_data(0, count, buf);
    return count;
}

/**
 * @brief       ��ʾʵ����Ϣ
 * @param       ��
 * @retval      ��
 */
void ch395_show_mesg(void)
{
    /* LCD��ʾʵ����Ϣ */
    lcd_show_string(10, 10, 220, 32, 32, "STM32", RED);
    lcd_show_string(10, 47, 220, 24, 24, "CH395Q OneNET", RED);
    lcd_show_string(10, 76, 220, 16, 16, "ATOM@ALIENTEK", RED);
    
    /* �������ʵ����Ϣ */
    printf("\n");
    printf("********************************\r\n");
    printf("STM32\r\n");
    printf("CH395Q OneNET\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

/**
 * @brief       ���̲���
 * @param       ��
 * @retval      ��
 */
void ch395_demo(void)
{
    
    ch395_show_mesg();                                                                                                  /* ��ʾ��Ϣ */
    ch395q_mqtt_confing();
    
    do
    {
        ch395q_handler();
    }
    while (g_ch395q_sta.dhcp_status == DHCP_STA);                                                                       /* ��ȡDHCP */
    
    cha95_sockct_sta[0].socket_enable = CH395Q_ENABLE;                                                                  /* ʹ��socket�� */
    cha95_sockct_sta[0].socket_index = CH395Q_SOCKET_0;                                                                 /* ����socket�� */
    memcpy(cha95_sockct_sta[0].des_ip, ch395_des_ipaddr, sizeof(cha95_sockct_sta[0].des_ip));                           /* ����Ŀ��IP��ַ */
    memcpy(cha95_sockct_sta[0].net_config.ipaddr, ch395_ipaddr, sizeof(cha95_sockct_sta[0].net_config.ipaddr));         /* ���þ�̬����IP��ַ */
    memcpy(cha95_sockct_sta[0].net_config.gwipaddr, ch395_gw_ipaddr, sizeof(cha95_sockct_sta[0].net_config.gwipaddr));  /* ���þ�̬����IP��ַ */
    memcpy(cha95_sockct_sta[0].net_config.maskaddr, ch395_ipmask, sizeof(cha95_sockct_sta[0].net_config.maskaddr));     /* ���þ�̬���������ַ */
    memcpy(cha95_sockct_sta[0].net_config.macaddr, ch395_macaddr, sizeof(cha95_sockct_sta[0].net_config.macaddr));      /* ���þ�̬MAC��ַ */
    cha95_sockct_sta[0].des_port = 1883;                                                                                /* Ŀ��˿� */
    cha95_sockct_sta[0].sour_port = 5000;                                                                               /* Դ�˿� */
    cha95_sockct_sta[0].proto = CH395Q_SOCKET_TCP_CLIENT;                                                               /* ����Э�� */
    cha95_sockct_sta[0].send.buf = socket0_send_buf;                                                                    /* �������� */
    cha95_sockct_sta[0].send.size = sizeof(socket0_send_buf);                                                           /* �������ݴ�С */
    cha95_sockct_sta[0].recv.buf =  socket0_recv_buf;                                                                   /* �������ݻ����� */
    cha95_sockct_sta[0].recv.size = sizeof(socket0_recv_buf);                                                           /* �������ݴ�С */
    ch395q_socket_config(&cha95_sockct_sta[0]);                                                                         /* ����socket���� */

    char version[]  = "2018-10-31";
    unsigned int expiration_time    = 1956499200;
    char authorization_buf[160] = {0};
    
    memset(g_onenet_info.pro_id, 0, sizeof(g_onenet_info.pro_id));
    memset(g_onenet_info.access_key, 0, sizeof(g_onenet_info.access_key));
    memset(g_onenet_info.dev_name, 0, sizeof(g_onenet_info.dev_name));
    memset(g_onenet_info.dev_id, 0, sizeof(g_onenet_info.dev_id));
    memset(g_onenet_info.key, 0, sizeof(g_onenet_info.key));
    
    /* �Ѹ�������������g_onenet_info�ṹ��ĳ�Ա������ */
    strcpy(g_onenet_info.pro_id, USER_PRODUCT_ID);                          /* ��ƷID */
    strcpy(g_onenet_info.access_key, USER_ACCESS_KEY);                      /* ��Ʒ��Կ */
    strcpy(g_onenet_info.dev_name, USER_DEVICE_NAME);                       /* �豸���� */
    strcpy(g_onenet_info.dev_id, USER_DEVICE_ID);                           /* ��Ʒ�豸ID */
    
    strcpy(g_onenet_info.key, USER_KEY);
    /* �豸��Կ */
    /* ������Щ�������н��룬��Ȼ������������token����½��� */
    onenet_authorization(version,
                         (char *)g_onenet_info.pro_id,
                         expiration_time,
                         (char *)g_onenet_info.key,
                         (char *)g_onenet_info.dev_name,
                         authorization_buf,
                         sizeof(authorization_buf),
                         0);

    data.clientID.cstring = (char *)g_onenet_info.dev_name; /* �豸���� */;
    data.username.cstring = (char *)g_onenet_info.pro_id;   /* ��ƷID */;
    data.password.cstring = (char *)authorization_buf;      /* ������������� */;
    data.keepAliveInterval = 100;                           /* ����ʱ�� */

    unsigned char payload_out[200];
    int payload_out_len = 0;

    while(1)
    {
        if (g_ch395q_sta.switch_status == CONNECT_STAT && g_subscribe_state == 1)
        {
            ch395q_mq_config.send_data.g_temp = 30 + rand() % 10 + 1;    /* �¶ȵ����� */
            ch395q_mq_config.send_data.g_humid = 54.8 + rand() % 10 + 1; /* ʪ�ȵ����� */
            sprintf((char *)payload_out, "{\"id\": 123,\"dp\": { \"temperatrue\": [{\"v\": %0.1f,}],\"power\": [{\"v\": %0.1f,}]}}", ch395q_mq_config.send_data.g_temp, ch395q_mq_config.send_data.g_humid);
            payload_out_len = strlen((char *)payload_out);
            topicString.cstring = DEVICE_PUBLISH;   /* �����ϱ� ���� */
            ch395q_mq_config.mqtt_argument.g_len = MQTTSerialize_publish((unsigned char *)socket0_send_buf, sizeof(socket0_send_buf), 0, 1, 0, 1, topicString, payload_out, payload_out_len);
            ch395q_mq_config.mqtt_argument.g_rc = ch395_transport_send_packet_buffer(CH395Q_SOCKET_0, (unsigned char *)socket0_send_buf, ch395q_mq_config.mqtt_argument.g_len);

            if (ch395q_mq_config.mqtt_argument.g_rc == ch395q_mq_config.mqtt_argument.g_len)
            {
                printf("PUBLISH Successfully\r\n");
            }
            else
            {
                printf("PUBLISH failed\r\n");
            }
        }
        
        delay_ms(1000);
        
        switch (ch395q_mq_config.mqtt_argument.g_msgtypes)
        {

          case CONNECT:  /* �ͻ��˷��ͷ����������Ӳ��� */
                ch395q_mq_config.mqtt_argument.g_len = MQTTSerialize_connect((unsigned char *)socket0_send_buf, sizeof(socket0_send_buf), &data);                                   /* ��ȡ�����鳤����������Ϣ  */
                ch395q_mq_config.mqtt_argument.g_rc = ch395_transport_send_packet_buffer(CH395Q_SOCKET_0, (unsigned char *)socket0_send_buf, ch395q_mq_config.mqtt_argument.g_len); /* ���ͷ��ط������鳤�� */

                if (ch395q_mq_config.mqtt_argument.g_rc == ch395q_mq_config.mqtt_argument.g_len)
                  printf("�������ӱ���\r\n");
                else
                  printf("�������ӱ���ʧ��\r\n");

                ch395q_mq_config.mqtt_argument.g_msgtypes = 0;
                break;

          case CONNACK:  /* ���������Ϳͻ���ȷ���������� */
                printf("����CONNACK״̬\r\n");
                if (MQTTDeserialize_connack(&ch395q_mq_config.mqtt_argument.g_sessionpresent, 
                                            &ch395q_mq_config.mqtt_argument.g_connack_rc, 
                                            (unsigned char *)socket0_send_buf, 
                                            sizeof(socket0_send_buf)) != 1 || ch395q_mq_config.mqtt_argument.g_connack_rc != 0)  /* �յ���ִ */
                {
                    printf("Unable to connect, return code %d\r\n", ch395q_mq_config.mqtt_argument.g_connack_rc);   /* ��ִ��һ�£�����ʧ�� */
                }
                else
                {
                    printf("MQTT is concet OK!\r\n");   /* ���ӳɹ� */
                }
                
                ch395q_mq_config.mqtt_argument.g_msgtypes = SUBSCRIBE;   /* ���ӳɹ� ִ�� ���� ���� */
                break;

          case SUBSCRIBE:  /* �ͻ��˷��͵��������Ķ��Ĳ��� */
                topicString.cstring = DEVICE_SUBSCRIBE;
                ch395q_mq_config.mqtt_argument.g_len = MQTTSerialize_subscribe((unsigned char *)socket0_send_buf, 
                                                                                sizeof(socket0_send_buf), 0, 1, 1,
                                                                                &topicString, 
                                                                                &ch395q_mq_config.mqtt_argument.g_req_qos);
                ch395q_mq_config.mqtt_argument.g_rc = ch395_transport_send_packet_buffer(CH395Q_SOCKET_0, (unsigned char *)socket0_send_buf, ch395q_mq_config.mqtt_argument.g_len);

                if (ch395q_mq_config.mqtt_argument.g_rc == ch395q_mq_config.mqtt_argument.g_len)
                {
                    printf("SUBSCRIBE Successfully\r\n");
                }
                else
                {
                    int t = 0;
                    
                    t ++;

                    if (t >= 10)
                    {
                        t = 0;
                        ch395q_mq_config.mqtt_argument.g_msgtypes = CONNECT;
                    }
                    else
                        ch395q_mq_config.mqtt_argument.g_msgtypes = SUBSCRIBE;
                    break;
                }
                ch395q_mq_config.mqtt_argument.g_msgtypes = 0;
                g_subscribe_state = 1;
                break;

          case SUBACK:  /* ���������͵��ͻ��˵Ķ���ȷ�� */
                ch395q_mq_config.mqtt_argument.g_rc = MQTTDeserialize_suback(&ch395q_mq_config.mqtt_argument.g_submsgid, 1, &ch395q_mq_config.mqtt_argument.g_subcount, &ch395q_mq_config.mqtt_argument.granted_qos, (unsigned char *)socket0_send_buf, sizeof(socket0_send_buf));  /* �л�ִ  QoS */
                ch395q_mq_config.mqtt_argument.g_msgtypes = 0;
                break;

            case PUBACK:   /* �����ɹ� */
                ch395q_mq_config.mqtt_argument.g_msgtypes = 0;
                break;
            case PUBLISH:  /* �������ķ������� */
                ch395q_mq_config.mqtt_argument.g_rc = MQTTDeserialize_publish(&g_dup, &g_qos, &g_retained, &g_msgid, &receivedTopic, &g_payload_in, &g_payloadlen_in,(unsigned char *)socket0_send_buf, sizeof(socket0_send_buf));  /* ��������������Ϣ */
                printf("message arrived : %s\r\n", g_payload_in);
                break;
            default:
                break;

        }
        
        if (g_ch395q_sta.switch_status == DATA_RECV_STAT)
        {

            ch395q_mq_config.mqtt_argument.g_rc = MQTTPacket_read((unsigned char *)cha95_sockct_sta[0].send.buf, cha95_sockct_sta[0].recv.size, ch395_transport_get_data);  /*  ��ѯ����MQTT�������ݣ�*/
          
            if (ch395q_mq_config.mqtt_argument.g_rc > 0)  /* ��������ݣ�������Ӧ״̬�� */
            {
                ch395q_mq_config.mqtt_argument.g_msgtypes = ch395q_mq_config.mqtt_argument.g_rc;
            }
            g_ch395q_sta.switch_status = CONNECT_STAT;        /* ����״̬ */
        }
        
        ch395q_handler();
    }
}


