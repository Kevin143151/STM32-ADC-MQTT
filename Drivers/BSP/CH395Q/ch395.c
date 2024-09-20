/**
 ****************************************************************************************************
 * @file        ch395q.c
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
 ****************************************************************************************************
 */

#include "./BSP/CH395Q/ch395.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/SPI/spi.h"


struct ch395q_t g_ch395q_sta;

/**
 * @brief       ch395_gpio��ʼ��
 * @param       ��
 * @retval      ��
 */
void ch395_gpio_init( void )
{
    GPIO_InitTypeDef gpio_init_struct;

    CH395_SCS_GPIO_CLK_ENABLE();     /* ʹ��SCSʱ�� */
    CH395_INT_GPIO_CLK_ENABLE();     /* ʹ��INTʱ�� */
    CH395_RST_GPIO_CLK_ENABLE();     /* ʹ��RSTʱ�� */

    /* SCS */
    gpio_init_struct.Pin = CH395_SCS_GPIO_PIN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;    /* ������� */
    HAL_GPIO_Init( CH395_SCS_GPIO_PORT, &gpio_init_struct );

    /* ��ʼ���ж����� */
    gpio_init_struct.Pin = CH395_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;        /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;            /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;  /* ���� */
    HAL_GPIO_Init( CH395_INT_GPIO_PORT, &gpio_init_struct );

    gpio_init_struct.Pin = CH395_RST_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;    /* ��� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;  /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;            /* ���� */
    HAL_GPIO_Init( CH395_RST_GPIO_PORT, &gpio_init_struct );
    HAL_GPIO_WritePin(CH395_RST_GPIO_PORT, CH395_RST_GPIO_PIN, GPIO_PIN_SET);
    delay_ms(20);
}

/**
 * @brief       Ӳ��SPI���������8��λ����
 * @param       d:��Ҫ���뵽ch395������
 * @retval      ��
 */
uint8_t ch395_read_write_byte( uint8_t data )
{
    uint8_t rxdata;
    rxdata = spi1_read_write_byte(data);       /* SPIд��һ��CH395Q���ݲ�����һ������ */
    return rxdata;                             /* �����յ������� */
}

/**
 * @brief       ��ch395д����
 * @param       ��Ҫд��ch395��������
 * @retval      ��
 */
void ch395_write_cmd( uint8_t mcmd )
{
    ch395_scs_hign;                         /* ��ֹCSԭ��Ϊ�ͣ��Ƚ�CS�ø� */
    ch395_scs_low;                          /* ���ʼ��CS���� */
    ch395_read_write_byte(mcmd);            /* SPI���������� */
    delay_ms(2);                            /* ��Ҫ��ʱ,��ʱ1.5uSȷ����д���ڲ�С��1.5uS */

}

/**
 * @brief       ��ch395д����
 * @param       ��Ҫд��ch395������
 * @retval      ��
 */
void ch395_write_data( uint8_t mdata )
{
    ch395_read_write_byte(mdata);           /* SPI�������� */
}

/**
 * @brief       ��ch395������
 * @param       ��
 * @retval      ���ض�ȡ������
 */
uint8_t ch395_read_data( void )
{
    uint8_t i;
    i = ch395_read_write_byte(0xff);        /* SPI������ */
    return i;
}

/**
 * @brief       ch395_keeplive_set ���ʱ����������
 * @param       ��
 * @retval      ��
 */
void ch395_keeplive_set(void)
{
    ch395_keeplive_cnt(DEF_KEEP_LIVE_CNT);
    ch395_keeplive_idle(DEF_KEEP_LIVE_IDLE);
    ch395_keeplive_intvl(DEF_KEEP_LIVE_PERIOD);
}

/**
 * @brief       ch395 socket����
 * @param       ch395_sokect��Socket������Ϣ
 * @retval      ��
 */
uint8_t ch395q_socket_config(ch395_socket * ch395_sokect)
{
    if (ch395_sokect == NULL)
    {
        return 0;
    }
    
    if (g_ch395q_sta.dhcp_status == DHCP_UP)                                    /* DHCP��ȡ�ɹ�״̬ */
    {
        ch395_sokect->net_info.ip[0] = g_ch395q_sta.ipinf_buf[0];
        ch395_sokect->net_info.ip[1] = g_ch395q_sta.ipinf_buf[1];
        ch395_sokect->net_info.ip[2] = g_ch395q_sta.ipinf_buf[2];
        ch395_sokect->net_info.ip[3] = g_ch395q_sta.ipinf_buf[3];
        
        ch395_sokect->net_info.gwip[0] = g_ch395q_sta.ipinf_buf[4];
        ch395_sokect->net_info.gwip[1] = g_ch395q_sta.ipinf_buf[5];
        ch395_sokect->net_info.gwip[2] = g_ch395q_sta.ipinf_buf[6];
        ch395_sokect->net_info.gwip[3] = g_ch395q_sta.ipinf_buf[7];
        
        ch395_sokect->net_info.mask[0] = g_ch395q_sta.ipinf_buf[8];
        ch395_sokect->net_info.mask[1] = g_ch395q_sta.ipinf_buf[9];
        ch395_sokect->net_info.mask[2] = g_ch395q_sta.ipinf_buf[10];
        ch395_sokect->net_info.mask[3] = g_ch395q_sta.ipinf_buf[11];
        
        ch395_sokect->net_info.dns1[0] = g_ch395q_sta.ipinf_buf[12];
        ch395_sokect->net_info.dns1[1] = g_ch395q_sta.ipinf_buf[13];
        ch395_sokect->net_info.dns1[2] = g_ch395q_sta.ipinf_buf[14];
        ch395_sokect->net_info.dns1[3] = g_ch395q_sta.ipinf_buf[15];
        
        ch395_sokect->net_info.dns2[0] = g_ch395q_sta.ipinf_buf[16];
        ch395_sokect->net_info.dns2[1] = g_ch395q_sta.ipinf_buf[17];
        ch395_sokect->net_info.dns2[2] = g_ch395q_sta.ipinf_buf[18];
        ch395_sokect->net_info.dns2[3] = g_ch395q_sta.ipinf_buf[19];
    }
    else                                                                      /* DHCP��ȡʧ��״̬�����þ�̬IP��ַ��Ϣ */
    {
        ch395_cmd_set_ipaddr(ch395_sokect->net_config.ipaddr);                /* ����CH395��IP��ַ */
        ch395_cmd_set_gw_ipaddr(ch395_sokect->net_config.gwipaddr);           /* �������ص�ַ */
        ch395_cmd_set_maskaddr(ch395_sokect->net_config.maskaddr);            /* �����������룬Ĭ��Ϊ255.255.255.0*/
        ch395_cmd_init();
        delay_ms(10);
    }
    
    ch395_cmd_set_macaddr(ch395_sokect->net_config.macaddr);                  /* ����MAC��ַ */

    memcpy(&g_ch395q_sta.socket[ch395_sokect->socket_index].config, ch395_sokect, sizeof(ch395_socket));
    
    switch(ch395_sokect->proto)
    {
        case CH395Q_SOCKET_UDP:
            /* socket ΪUDPģʽ */
            ch395_set_socket_desip(ch395_sokect->socket_index, ch395_sokect->des_ip);                                           /* ����socket 0Ŀ��IP��ַ */
            ch395_set_socket_prot_type(ch395_sokect->socket_index,  PROTO_TYPE_UDP);                                            /* ����socket 0Э������ */
            ch395_set_socket_desport(ch395_sokect->socket_index, ch395_sokect->des_port);                                       /* ����socket 0Ŀ�Ķ˿� */
            ch395_set_socket_sourport(ch395_sokect->socket_index, ch395_sokect->sour_port);                                     /* ����socket 0Դ�˿� */
            g_ch395q_sta.ch395_error(ch395_open_socket(ch395_sokect->socket_index));                                            /* ����Ƿ�ɹ� */
            break;
        case CH395Q_SOCKET_TCP_CLIENT:
            /* socket ΪTCPClientģʽ */
            ch395_keeplive_set();                                                                                               /* �������� */
            ch395_set_socket_desip(ch395_sokect->socket_index, ch395_sokect->des_ip);                                           /* ����socket 0Ŀ��IP��ַ */
            ch395_set_socket_prot_type(ch395_sokect->socket_index,  PROTO_TYPE_TCP);                                            /* ����socket 0Э������ */
            ch395_set_socket_desport(ch395_sokect->socket_index, ch395_sokect->des_port);                                       /* ����socket 0Ŀ�Ķ˿� */
            ch395_set_socket_sourport(ch395_sokect->socket_index, ch395_sokect->sour_port);                                     /* ����socket 0Դ�˿� */
            g_ch395q_sta.ch395_error(ch395_open_socket(ch395_sokect->socket_index));                                            /* ���sokect�Ƿ�򿪳ɹ� */
            g_ch395q_sta.ch395_error(ch395_tcp_connect(ch395_sokect->socket_index));                                            /* ���tcp�����Ƿ�ɹ� */
            break;
        case CH395Q_SOCKET_TCP_SERVER:
            /* socket ΪTCPServerģʽ */
            ch395_set_socket_desip(ch395_sokect->socket_index, ch395_sokect->des_ip);                                           /* ����socket 0Ŀ��IP��ַ */
            ch395_set_socket_prot_type(ch395_sokect->socket_index,  PROTO_TYPE_TCP);                                            /* ����socket 0Э������ */
            ch395_set_socket_sourport(ch395_sokect->socket_index, ch395_sokect->sour_port);                                     /* ����socket 0Դ�˿� */
            g_ch395q_sta.ch395_error(ch395_open_socket(ch395_sokect->socket_index));                                            /* ���sokect�Ƿ�򿪳ɹ� */
            g_ch395q_sta.ch395_error(ch395_tcp_listen(ch395_sokect->socket_index));                                             /* ����tcp���� */
            break;
        case CH395Q_SOCKET_MAC_RAW:
            ch395_set_socket_prot_type(ch395_sokect->socket_index,  PROTO_TYPE_MAC_RAW);                                        /* ����socket 0Э������ */
            g_ch395q_sta.ch395_error(ch395_open_socket(ch395_sokect->socket_index));                                            /* ���sokect�Ƿ�򿪳ɹ� */
            break;
    }
    
    return 1;
}
 
/**
 * @brief       ����ʹ�ã���ʾ������룬��ͣ��
 * @param       ierror �������
 * @retval      ��
 */
void ch395_error(uint8_t ierror)
{
    if (ierror == CMD_ERR_SUCCESS)
    {
        return;          /* �����ɹ� */
    }
    
    printf("Error: %02X\r\n", (uint16_t)ierror);    /* ��ʾ���� */
    
    while ( 1 )
    {
        delay_ms(200);
        delay_ms(200);
    }
}

/**
 * @brief       CH395 PHY״̬
 * @param       phy_status��PHY״ֵ̬
 * @retval      ��
 */
void ch395_phy_status(uint8_t phy_status)
{
    switch (phy_status)
    {
        case PHY_DISCONN:
            printf("PHY DISCONN\r\n");
            break;
        case PHY_10M_FLL:
            printf("PHY 10M_FLL\r\n");
            break;
        case PHY_10M_HALF:
            printf("PHY 10M_HALF\r\n");
            break;
        case PHY_100M_FLL:
            printf("PHY 100M_FLL\r\n");
            break;
        case PHY_100M_HALF:
            printf("PHY 100M_HALF\r\n");
            break;
        default:
            printf("PHY AUTO\r\n");
            break;
    }

    delay_ms(1000);
}

/**
 * @brief      ����socket�ӿڵĽ����뷢�ͻ�����
 * @param      ��
 * @retval     ��
 */
void ch395_socket_r_s_buf_modify(void)
{

   ch395_set_socket_recv_buf(0,0,4);                                                            /* Socket 0 �����ջ�����4*512 = 2K�����ͻ�����2*512 = 1K*/
   ch395_set_socket_send_buf(0,4,2);  
  
   ch395_set_socket_recv_buf(1,6,4);                                                            /* Socket 1 */
   ch395_set_socket_send_buf(1,10,2);  
  
   ch395_set_socket_recv_buf(2,12,4);                                                           /* Socket 2 */
   ch395_set_socket_send_buf(2,16,2);  
  
   ch395_set_socket_recv_buf(3,18,4);                                                           /* Socket 3 */
   ch395_set_socket_send_buf(3,22,2);  
  
   ch395_set_socket_recv_buf(4,24,4);                                                           /* Socket 4 */
   ch395_set_socket_send_buf(4,28,2);  
  
   ch395_set_socket_recv_buf(5,30,4);                                                           /* Socket 5 */
   ch395_set_socket_send_buf(5,34,2);
  
   ch395_set_socket_recv_buf(6,36,4);                                                           /* Socket 6 */
   ch395_set_socket_send_buf(6,40,2);  
  
   ch395_set_socket_recv_buf(7,42,4);                                                           /* Socket 7 */
   ch395_set_socket_send_buf(7,46,2);  

}

/**
 * @brief      ch395_tcp��ʼ��
 * @param      ��
 * @retval     ��
 */
void ch395_hardware_init(void)
{
    uint8_t i;
    ch395_gpio_init();
    spi1_init();

    g_ch395q_sta.ch395_error = ch395_error;
    g_ch395q_sta.ch395_phy_cb = ch395_phy_status;
    g_ch395q_sta.ch395_reconnection = ch395_reconnection;
    g_ch395q_sta.dhcp_status = DHCP_STA;
    
    i = ch395_cmd_check_exist(0x65);                                        /* ����������ڲ���Ӳ���Լ��ӿ�ͨѶ */

    if (i != 0x9a)
    {
        g_ch395q_sta.ch395_error(i);                                        /* ch395q������ */
    }
    
    ch395_cmd_reset();                                                      /* ��ch395q��λ */
    delay_ms(100);                                                          /* �������ȴ�100������ʱ */
    
    g_ch395q_sta.ch395_error(ch395_cmd_init());                             /* ��ʼ��ch395q���� */
    
    do
    {
        g_ch395q_sta.phy_status = ch395_cmd_get_phy_status();               /* ��ȡPHY״̬ */ 
        g_ch395q_sta.ch395_phy_cb(g_ch395q_sta.phy_status);                 /* �ж�˫��������ģʽ */ 
    }
    while(g_ch395q_sta.phy_status == PHY_DISCONN);
    
    g_ch395q_sta.version = ch395_cmd_get_ver();                             /* ��ȡ�汾 */
    printf("CH395VER : %2x\r\n", (uint16_t)g_ch395q_sta.version);
    
    i = ch395_dhcp_enable(1);                                               /* ����DHCP */
    g_ch395q_sta.ch395_error(i);                                            /* ch395q������ */
    
    delay_ms(1000);                                                         /* ch395q��ʼ����ʱ */
}

/**
 * @brief       CH395 socket �ж�,��ȫ���ж��б�����
 * @param       sockindex ��0~7��
 * @retval      ��
 */
void ch395_socket_interrupt(uint8_t sockindex)
{
    uint8_t  sock_int_socket;
    uint16_t rx_len = 0;
    
    sock_int_socket = ch395_get_socket_int(sockindex);      /* ��ȡsocket ���ж�״̬ */

    if (sock_int_socket & SINT_STAT_SENBUF_FREE)            /* ���ͻ��������У����Լ���д��Ҫ���͵����� */
    {
        
    }

    if (sock_int_socket & SINT_STAT_SEND_OK)                /* ��������ж� */
    {
        
    }

    if (sock_int_socket & SINT_STAT_RECV)                   /* �����ж� */
    {
        g_ch395q_sta.switch_status = DATA_RECV_STAT;
        g_ch395q_sta.socket[sockindex].config.recv.size = ch395_get_recv_length(sockindex);     /* ��ȡ��ǰ�����������ݳ��� */
        rx_len = g_ch395q_sta.socket[sockindex].config.recv.size;
        ch395_get_recv_data(sockindex, rx_len, g_ch395q_sta.socket[sockindex].config.recv.buf); /* ��ȡ���� */
        g_ch395q_sta.socket[sockindex].config.recv.buf[rx_len] = '\0';
        printf("%s", g_ch395q_sta.socket[sockindex].config.recv.buf);
        g_ch395q_sta.socket[sockindex].config.recv.recv_flag |= 0x04;
    }

    if (sock_int_socket & SINT_STAT_CONNECT)                /* �����жϣ�����TCPģʽ����Ч */
    {
        g_ch395q_sta.switch_status = CONNECT_STAT;
        
        if (g_ch395q_sta.socket[sockindex].config.proto == CH395Q_SOCKET_TCP_CLIENT)
        {
            ch395_set_keeplive(sockindex,1);                /* ��KEEPALIVE���ʱ�� */
            ch395_setttl_num(sockindex,60);                 /* ����TTL */
        }
    }

    if (sock_int_socket & SINT_STAT_DISCONNECT)             /* �Ͽ��жϣ�����TCPģʽ����Ч */
    {
        g_ch395q_sta.ch395_error(ch395_open_socket(g_ch395q_sta.socket[sockindex].config.socket_index));
        
        switch(g_ch395q_sta.socket[sockindex].config.proto)
        {
            case CH395Q_SOCKET_TCP_CLIENT:
                g_ch395q_sta.ch395_error(ch395_tcp_connect(g_ch395q_sta.socket[sockindex].config.socket_index));
                break;
            case CH395Q_SOCKET_TCP_SERVER:
                g_ch395q_sta.ch395_error(ch395_tcp_listen(g_ch395q_sta.socket[sockindex].config.socket_index));
                break;
            default:
                break;
        }
        delay_ms(200);                                      /* ��ʱ200MS���ٴ����ԣ�û�б�Ҫ����Ƶ������ */
    }

    if (sock_int_socket & SINT_STAT_TIM_OUT)                /* ��ʱ�жϣ�����TCPģʽ����Ч */
    {
        if (g_ch395q_sta.socket[sockindex].config.proto == CH395Q_SOCKET_TCP_CLIENT)
        {
            delay_ms(200);                                  /* ��ʱ200MS���ٴ����ԣ�û�б�Ҫ����Ƶ������ */
            g_ch395q_sta.ch395_error(ch395_open_socket(g_ch395q_sta.socket[sockindex].config.socket_index));
            g_ch395q_sta.ch395_error(ch395_tcp_connect(g_ch395q_sta.socket[sockindex].config.socket_index));
        }
    }
}

/**
 * @brief       CH395ȫ���жϺ���
 * @param       ��
 * @retval      ��
 */
void ch395_interrupt_handler(void)
{
    uint16_t  init_status;
    uint8_t i;
    
    init_status = ch395_cmd_get_glob_int_status_all();

    if (init_status & GINT_STAT_UNREACH)                                    /* ���ɴ��жϣ���ȡ���ɴ���Ϣ */
    {
         ch395_cmd_get_unreachippt(g_ch395q_sta.ipinf_buf);
    }

    if (init_status & GINT_STAT_IP_CONFLI)                                  /* ����IP��ͻ�жϣ����������޸�CH395�� IP������ʼ��CH395 */
    {
        
    }

    if (init_status & GINT_STAT_PHY_CHANGE)                                 /* ����PHY�ı��ж� */
    {
        g_ch395q_sta.phy_status = ch395_cmd_get_phy_status();               /* ��ȡPHY״̬ */
    }

    if (init_status & GINT_STAT_DHCP)                                       /* ����DHCP�ж� */
    {

        i = ch395_get_dhcp_status();
        
        switch (i)
        {
            case DHCP_UP:
                ch395_get_ipinf(g_ch395q_sta.ipinf_buf);
                printf("IP:%02d.%02d.%02d.%02d\r\n", (uint16_t)g_ch395q_sta.ipinf_buf[0], (uint16_t)g_ch395q_sta.ipinf_buf[1], (uint16_t)g_ch395q_sta.ipinf_buf[2], (uint16_t)g_ch395q_sta.ipinf_buf[3]);
                printf("GWIP:%02d.%02d.%02d.%02d\r\n", (uint16_t)g_ch395q_sta.ipinf_buf[4], (uint16_t)g_ch395q_sta.ipinf_buf[5], (uint16_t)g_ch395q_sta.ipinf_buf[6], (uint16_t)g_ch395q_sta.ipinf_buf[7]);
                printf("Mask:%02d.%02d.%02d.%02d\r\n", (uint16_t)g_ch395q_sta.ipinf_buf[8], (uint16_t)g_ch395q_sta.ipinf_buf[9], (uint16_t)g_ch395q_sta.ipinf_buf[10], (uint16_t)g_ch395q_sta.ipinf_buf[11]);
                printf("DNS1:%02d.%02d.%02d.%02d\r\n", (uint16_t)g_ch395q_sta.ipinf_buf[12], (uint16_t)g_ch395q_sta.ipinf_buf[13], (uint16_t)g_ch395q_sta.ipinf_buf[14], (uint16_t)g_ch395q_sta.ipinf_buf[15]);
                printf("DNS2:%02d.%02d.%02d.%02d\r\n", (uint16_t)g_ch395q_sta.ipinf_buf[16], (uint16_t)g_ch395q_sta.ipinf_buf[17], (uint16_t)g_ch395q_sta.ipinf_buf[18], (uint16_t)g_ch395q_sta.ipinf_buf[19]);
                g_ch395q_sta.dhcp_status = DHCP_UP;
                break;
            default:
                g_ch395q_sta.dhcp_status = DHCP_DOWN;
                /* ����Ĭ��IP��ַ��Ϣ */
                printf("��̬IP��Ϣ.....................................\r\n");
                break;
        }
    }

    if (init_status & GINT_STAT_SOCK0)
    {
        ch395_socket_interrupt(CH395Q_SOCKET_0);                          /* ����socket 0�ж� */
    }

    if (init_status & GINT_STAT_SOCK1)
    {
        ch395_socket_interrupt(CH395Q_SOCKET_1);                          /* ����socket 1�ж� */
    }

    if (init_status & GINT_STAT_SOCK2)
    {
        ch395_socket_interrupt(CH395Q_SOCKET_2);                          /* ����socket 2�ж� */
    }

    if (init_status & GINT_STAT_SOCK3)
    {
        ch395_socket_interrupt(CH395Q_SOCKET_3);                          /* ����socket 3�ж� */
    }

    if (init_status & GINT_STAT_SOCK4)
    {
        ch395_socket_interrupt(CH395Q_SOCKET_4);                          /* ����socket 4�ж� */
    }

    if (init_status & GINT_STAT_SOCK5)
    {
        ch395_socket_interrupt(CH395Q_SOCKET_5);                          /* ����socket 5�ж� */
    }

    if (init_status & GINT_STAT_SOCK6)
    {
        ch395_socket_interrupt(CH395Q_SOCKET_6);                          /* ����socket 6�ж� */
    }

    if (init_status & GINT_STAT_SOCK7)
    {
        ch395_socket_interrupt(CH395Q_SOCKET_7);                          /* ����socket 7�ж� */
    }
}

/**
 * @brief       CH395ȫ�ֹ�����
 * @param       ��
 * @retval      ��
 */
void ch395q_handler(void)
{
    if (ch395_int_pin_wire == 0)
    {
        ch395_interrupt_handler();                                       /* �жϴ����� */
    }
    
    g_ch395q_sta.ch395_reconnection();                                   /* ���PHY״̬������������ */
}

/**
 * @brief       ���PHY״̬������������
 * @param       ��
 * @retval      ��
 */
void ch395_reconnection(void)
{
    for (uint8_t socket_index = CH395Q_SOCKET_0 ; socket_index <= CH395Q_SOCKET_7 ; socket_index ++ )
    {
        if (g_ch395q_sta.phy_status == PHY_DISCONN && (g_ch395q_sta.dhcp_status == DHCP_UP || g_ch395q_sta.dhcp_status == DHCP_DOWN || g_ch395q_sta.dhcp_status == DHCP_STA))
        {
            if (g_ch395q_sta.socket[socket_index].config.socket_enable == CH395Q_ENABLE)
            {
                ch395_close_socket(g_ch395q_sta.socket[socket_index].config.socket_index);
                g_ch395q_sta.ch395_error(ch395_dhcp_enable(0));                                                                 /* ch395q������ */
                g_ch395q_sta.socket[socket_index].config.socket_enable = CH395Q_DISABLE;
                g_ch395q_sta.dhcp_status = DHCP_STA;
            }
        }
        else
        {
            if (g_ch395q_sta.phy_status != PHY_DISCONN && g_ch395q_sta.socket[socket_index].config.socket_enable == CH395Q_DISABLE)
            {
                if (g_ch395q_sta.dhcp_status == DHCP_STA)
                {
                    ch395_cmd_reset();                                                                                          /* ��ch395q��λ */
                    delay_ms(100);                                                                                              /* �������ȴ�100������ʱ */
                    ch395_cmd_init();
                    delay_ms(100);                                                                                              /* �������ȴ�100������ʱ */
                    ch395_socket_r_s_buf_modify();
//                    ch395_set_tcpmss(536);
//                    ch395_set_start_para(FUN_PARA_FLAG_TCP_SERVER | SOCK_CTRL_FLAG_SOCKET_CLOSE);
                    g_ch395q_sta.ch395_error(ch395_dhcp_enable(1));                                                             /* ����DHCP */
                }
                
                do
                {
                    if (ch395_int_pin_wire == 0)
                    {
                        ch395_interrupt_handler();                                                                              /* �жϴ����� */
                    }
                }
                while (g_ch395q_sta.dhcp_status == DHCP_STA);                                                                   /* ��ȡDHCP */
                
                switch(g_ch395q_sta.socket[socket_index].config.proto)
                {
                    case CH395Q_SOCKET_UDP:
                        /* socket ΪUDPģʽ */
                        ch395_set_socket_desip(socket_index, g_ch395q_sta.socket[socket_index].config.des_ip);                  /* ����socket 0Ŀ��IP��ַ */
                        ch395_set_socket_prot_type(socket_index,  PROTO_TYPE_UDP);                                              /* ����socket 0Э������ */
                        ch395_set_socket_desport(socket_index, g_ch395q_sta.socket[socket_index].config.des_port);              /* ����socket 0Ŀ�Ķ˿� */
                        ch395_set_socket_sourport(socket_index, g_ch395q_sta.socket[socket_index].config.sour_port);            /* ����socket 0Դ�˿� */
                        g_ch395q_sta.ch395_error(ch395_open_socket(socket_index));                                              /* ����Ƿ�ɹ� */
                        break;
                    case CH395Q_SOCKET_TCP_CLIENT:
                        /* socket ΪTCPClientģʽ */
                        ch395_keeplive_set();                                                                                   /* �������� */
                        ch395_set_socket_desip(socket_index, g_ch395q_sta.socket[socket_index].config.des_ip);                  /* ����socket 0Ŀ��IP��ַ */
                        ch395_set_socket_prot_type(socket_index,  PROTO_TYPE_TCP);                                              /* ����socket 0Э������ */
                        ch395_set_socket_desport(socket_index,g_ch395q_sta.socket[socket_index].config.des_port);               /* ����socket 0Ŀ�Ķ˿� */
                        ch395_set_socket_sourport(socket_index,g_ch395q_sta.socket[socket_index].config.sour_port);             /* ����socket 0Դ�˿� */
                        g_ch395q_sta.ch395_error(ch395_open_socket(socket_index));                                              /* ���sokect�Ƿ�򿪳ɹ� */
                        g_ch395q_sta.ch395_error(ch395_tcp_connect(socket_index));                                              /* ���tcp�����Ƿ�ɹ� */
                        break;
                    case CH395Q_SOCKET_TCP_SERVER:
                        /* socket ΪTCPServerģʽ */
                        ch395_set_socket_desip(socket_index, g_ch395q_sta.socket[socket_index].config.des_ip);                  /* ����socket 0Ŀ��IP��ַ */
                        ch395_set_socket_prot_type(socket_index,  PROTO_TYPE_TCP);                                              /* ����socket 0Э������ */
                        ch395_set_socket_sourport(socket_index, g_ch395q_sta.socket[socket_index].config.sour_port);            /* ����socket 0Դ�˿� */
                        g_ch395q_sta.ch395_error(ch395_open_socket(socket_index));                                              /* ���sokect�Ƿ�򿪳ɹ� */
                        g_ch395q_sta.ch395_error(ch395_tcp_listen(socket_index));                                               /* ����tcp���� */
                        break;
                    case CH395Q_SOCKET_MAC_RAW:
                        ch395_set_socket_prot_type(socket_index,  PROTO_TYPE_MAC_RAW);                                          /* ����socket 0Э������ */
                        g_ch395q_sta.ch395_error(ch395_open_socket(socket_index));                                              /* ���sokect�Ƿ�򿪳ɹ� */
                        break;
                    default:
                        ch395_set_socket_prot_type(socket_index,  PROTO_TYPE_TCP);
                        ch395_set_socket_sourport(socket_index, g_ch395q_sta.socket[socket_index].config.sour_port);                                                        /* ����socket 1~7Դ�˿� */
                        break;
                }
                g_ch395q_sta.socket[socket_index].config.socket_enable = CH395Q_ENABLE;
            }
        }
    }
}
