/**
 ****************************************************************************************************
 * @file        ch395_demo.c
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


/* 本地网络信息：IP地址、网关地址、子网掩码和MAC地址 */
uint8_t ch395_ipaddr[4]     = {192,168,1,10};
uint8_t ch395_gw_ipaddr[4]  = {192,168,1,1};
uint8_t ch395_ipmask[4]     = {255,255,255,0};
uint8_t ch395_macaddr[6]    = {0xB8,0xAE,0x1D,0x00,0x00,0x00};
/* 远程IP地址设置 */
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
 * @brief  初始化MQTT参数
 * @param  无
 * @retval 无
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
 * @brief  通过TCP方式发送数据到TCP服务器
 * @param  sock通道
 * @param  buf数据首地址
 * @param  buflen数据长度
 * @retval 小于0表示发送失败
 */
int ch395_transport_send_packet_buffer(int sock, unsigned char* buf, int buflen)
{
    ch395_send_data(sock, buf, buflen);
    return buflen;
}

/**
 * @brief  阻塞方式接受TCP服务器发送的数据
 * @param  buf数据存储首地址
 * @param  count数据缓冲区长度
 * @retval 小于0表示接收数据失败
 */
int ch395_transport_get_data(unsigned char *buf, int count)
{
    ch395_get_recv_data(0, count, buf);
    return count;
}

/**
 * @brief       显示实验信息
 * @param       无
 * @retval      无
 */
void ch395_show_mesg(void)
{
    /* LCD显示实验信息 */
    lcd_show_string(10, 10, 220, 32, 32, "STM32", RED);
    lcd_show_string(10, 47, 220, 24, 24, "CH395Q OneNET", RED);
    lcd_show_string(10, 76, 220, 16, 16, "ATOM@ALIENTEK", RED);
    
    /* 串口输出实验信息 */
    printf("\n");
    printf("********************************\r\n");
    printf("STM32\r\n");
    printf("CH395Q OneNET\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

/**
 * @brief       例程测试
 * @param       无
 * @retval      无
 */
void ch395_demo(void)
{
    
    ch395_show_mesg();                                                                                                  /* 显示信息 */
    ch395q_mqtt_confing();
    
    do
    {
        ch395q_handler();
    }
    while (g_ch395q_sta.dhcp_status == DHCP_STA);                                                                       /* 获取DHCP */
    
    cha95_sockct_sta[0].socket_enable = CH395Q_ENABLE;                                                                  /* 使能socket对 */
    cha95_sockct_sta[0].socket_index = CH395Q_SOCKET_0;                                                                 /* 设置socket对 */
    memcpy(cha95_sockct_sta[0].des_ip, ch395_des_ipaddr, sizeof(cha95_sockct_sta[0].des_ip));                           /* 设置目标IP地址 */
    memcpy(cha95_sockct_sta[0].net_config.ipaddr, ch395_ipaddr, sizeof(cha95_sockct_sta[0].net_config.ipaddr));         /* 设置静态本地IP地址 */
    memcpy(cha95_sockct_sta[0].net_config.gwipaddr, ch395_gw_ipaddr, sizeof(cha95_sockct_sta[0].net_config.gwipaddr));  /* 设置静态网关IP地址 */
    memcpy(cha95_sockct_sta[0].net_config.maskaddr, ch395_ipmask, sizeof(cha95_sockct_sta[0].net_config.maskaddr));     /* 设置静态子网掩码地址 */
    memcpy(cha95_sockct_sta[0].net_config.macaddr, ch395_macaddr, sizeof(cha95_sockct_sta[0].net_config.macaddr));      /* 设置静态MAC地址 */
    cha95_sockct_sta[0].des_port = 1883;                                                                                /* 目标端口 */
    cha95_sockct_sta[0].sour_port = 5000;                                                                               /* 源端口 */
    cha95_sockct_sta[0].proto = CH395Q_SOCKET_TCP_CLIENT;                                                               /* 设置协议 */
    cha95_sockct_sta[0].send.buf = socket0_send_buf;                                                                    /* 发送数据 */
    cha95_sockct_sta[0].send.size = sizeof(socket0_send_buf);                                                           /* 发送数据大小 */
    cha95_sockct_sta[0].recv.buf =  socket0_recv_buf;                                                                   /* 接收数据缓冲区 */
    cha95_sockct_sta[0].recv.size = sizeof(socket0_recv_buf);                                                           /* 接收数据大小 */
    ch395q_socket_config(&cha95_sockct_sta[0]);                                                                         /* 配置socket参数 */

    char version[]  = "2018-10-31";
    unsigned int expiration_time    = 1956499200;
    char authorization_buf[160] = {0};
    
    memset(g_onenet_info.pro_id, 0, sizeof(g_onenet_info.pro_id));
    memset(g_onenet_info.access_key, 0, sizeof(g_onenet_info.access_key));
    memset(g_onenet_info.dev_name, 0, sizeof(g_onenet_info.dev_name));
    memset(g_onenet_info.dev_id, 0, sizeof(g_onenet_info.dev_id));
    memset(g_onenet_info.key, 0, sizeof(g_onenet_info.key));
    
    /* 把各个参数保存在g_onenet_info结构体的成员变量中 */
    strcpy(g_onenet_info.pro_id, USER_PRODUCT_ID);                          /* 产品ID */
    strcpy(g_onenet_info.access_key, USER_ACCESS_KEY);                      /* 产品密钥 */
    strcpy(g_onenet_info.dev_name, USER_DEVICE_NAME);                       /* 设备名称 */
    strcpy(g_onenet_info.dev_id, USER_DEVICE_ID);                           /* 产品设备ID */
    
    strcpy(g_onenet_info.key, USER_KEY);
    /* 设备密钥 */
    /* 根据这些参数进行解码，当然这个密码可以在token软件下解码 */
    onenet_authorization(version,
                         (char *)g_onenet_info.pro_id,
                         expiration_time,
                         (char *)g_onenet_info.key,
                         (char *)g_onenet_info.dev_name,
                         authorization_buf,
                         sizeof(authorization_buf),
                         0);

    data.clientID.cstring = (char *)g_onenet_info.dev_name; /* 设备名称 */;
    data.username.cstring = (char *)g_onenet_info.pro_id;   /* 产品ID */;
    data.password.cstring = (char *)authorization_buf;      /* 计算出来的密码 */;
    data.keepAliveInterval = 100;                           /* 保活时间 */

    unsigned char payload_out[200];
    int payload_out_len = 0;

    while(1)
    {
        if (g_ch395q_sta.switch_status == CONNECT_STAT && g_subscribe_state == 1)
        {
            ch395q_mq_config.send_data.g_temp = 30 + rand() % 10 + 1;    /* 温度的数据 */
            ch395q_mq_config.send_data.g_humid = 54.8 + rand() % 10 + 1; /* 湿度的数据 */
            sprintf((char *)payload_out, "{\"id\": 123,\"dp\": { \"temperatrue\": [{\"v\": %0.1f,}],\"power\": [{\"v\": %0.1f,}]}}", ch395q_mq_config.send_data.g_temp, ch395q_mq_config.send_data.g_humid);
            payload_out_len = strlen((char *)payload_out);
            topicString.cstring = DEVICE_PUBLISH;   /* 属性上报 发布 */
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

          case CONNECT:  /* 客户端发送服务器的连接操作 */
                ch395q_mq_config.mqtt_argument.g_len = MQTTSerialize_connect((unsigned char *)socket0_send_buf, sizeof(socket0_send_buf), &data);                                   /* 获取数据组长发送连接信息  */
                ch395q_mq_config.mqtt_argument.g_rc = ch395_transport_send_packet_buffer(CH395Q_SOCKET_0, (unsigned char *)socket0_send_buf, ch395q_mq_config.mqtt_argument.g_len); /* 发送返回发送数组长度 */

                if (ch395q_mq_config.mqtt_argument.g_rc == ch395q_mq_config.mqtt_argument.g_len)
                  printf("发送连接报文\r\n");
                else
                  printf("发送连接报文失败\r\n");

                ch395q_mq_config.mqtt_argument.g_msgtypes = 0;
                break;

          case CONNACK:  /* 服务器发送客户端确认连接请求 */
                printf("进入CONNACK状态\r\n");
                if (MQTTDeserialize_connack(&ch395q_mq_config.mqtt_argument.g_sessionpresent, 
                                            &ch395q_mq_config.mqtt_argument.g_connack_rc, 
                                            (unsigned char *)socket0_send_buf, 
                                            sizeof(socket0_send_buf)) != 1 || ch395q_mq_config.mqtt_argument.g_connack_rc != 0)  /* 收到回执 */
                {
                    printf("Unable to connect, return code %d\r\n", ch395q_mq_config.mqtt_argument.g_connack_rc);   /* 回执不一致，连接失败 */
                }
                else
                {
                    printf("MQTT is concet OK!\r\n");   /* 连接成功 */
                }
                
                ch395q_mq_config.mqtt_argument.g_msgtypes = SUBSCRIBE;   /* 连接成功 执行 订阅 操作 */
                break;

          case SUBSCRIBE:  /* 客户端发送到服务器的订阅操作 */
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

          case SUBACK:  /* 服务器发送到客户端的订阅确认 */
                ch395q_mq_config.mqtt_argument.g_rc = MQTTDeserialize_suback(&ch395q_mq_config.mqtt_argument.g_submsgid, 1, &ch395q_mq_config.mqtt_argument.g_subcount, &ch395q_mq_config.mqtt_argument.granted_qos, (unsigned char *)socket0_send_buf, sizeof(socket0_send_buf));  /* 有回执  QoS */
                ch395q_mq_config.mqtt_argument.g_msgtypes = 0;
                break;

            case PUBACK:   /* 发布成功 */
                ch395q_mq_config.mqtt_argument.g_msgtypes = 0;
                break;
            case PUBLISH:  /* 服务器的发布操作 */
                ch395q_mq_config.mqtt_argument.g_rc = MQTTDeserialize_publish(&g_dup, &g_qos, &g_retained, &g_msgid, &receivedTopic, &g_payload_in, &g_payloadlen_in,(unsigned char *)socket0_send_buf, sizeof(socket0_send_buf));  /* 服务器有推送信息 */
                printf("message arrived : %s\r\n", g_payload_in);
                break;
            default:
                break;

        }
        
        if (g_ch395q_sta.switch_status == DATA_RECV_STAT)
        {

            ch395q_mq_config.mqtt_argument.g_rc = MQTTPacket_read((unsigned char *)cha95_sockct_sta[0].send.buf, cha95_sockct_sta[0].recv.size, ch395_transport_get_data);  /*  轮询，读MQTT返回数据，*/
          
            if (ch395q_mq_config.mqtt_argument.g_rc > 0)  /* 如果有数据，进入相应状态。 */
            {
                ch395q_mq_config.mqtt_argument.g_msgtypes = ch395q_mq_config.mqtt_argument.g_rc;
            }
            g_ch395q_sta.switch_status = CONNECT_STAT;        /* 连接状态 */
        }
        
        ch395q_handler();
    }
}


