/**
 ****************************************************************************************************
 * @file        ch395cmd.C
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-6-17
 * @brief       ch395����ӿ��ļ�
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

#include "./BSP/CH395Q/ch395inc.h"
#include "./BSP/CH395Q/ch395cmd.h"
#include "./BSP/CH395Q/ch395.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       ��λch395оƬ
 * @param       ��
 * @retval      ��
 */
void ch395_cmd_reset(void)
{
    ch395_write_cmd(CMD00_RESET_ALL);
    ch395_scs_hign;
}

/**
 * @brief       ʹch395����˯��״̬
 * @param       ��
 * @retval      ��
 */
void ch395_cmd_sleep(void)
{
    ch395_write_cmd(CMD00_ENTER_SLEEP);
    ch395_scs_hign;
}

/**
 * @brief       ��ȡоƬ�Լ��̼��汾�ţ�1�ֽڣ�����λ��ʾоƬ�汾��
 * @param       ��
 * @retval      1�ֽ�оƬ���̼��汾��
 */
uint8_t ch395_cmd_get_ver(void)
{
    uint8_t i;
    ch395_write_cmd(CMD01_GET_IC_VER);
    i = ch395_read_data();
    ch395_scs_hign;
    return i;
}

/**
 * @brief       ����������ڲ���Ӳ���Լ��ӿ�ͨѶ��
 * @param       1�ֽڲ�������
 * @retval      Ӳ��ok������ testdata��λȡ��
 */
uint8_t ch395_cmd_check_exist(uint8_t testdata)
{
    uint8_t i;

    ch395_write_cmd(CMD11_CHECK_EXIST);
    ch395_write_data(testdata);
    i = ch395_read_data();
    ch395_scs_hign;
    return i;
}

/**
 * @brief       ����phy����Ҫ����ch395 phyΪ100/10m ����ȫ˫����˫����ch395ĬΪ�Զ�Э�̡�
 * @param       �ο�phy �������/״̬
 * @retval      ��
 */
void ch395_cmd_set_phy(uint8_t phystat)
{
    ch395_write_cmd(CMD10_SET_PHY);
    ch395_write_data(phystat);
    ch395_scs_hign;
}

/**
 * @brief       ��ȡphy��״̬
 * @param       ��
 * @retval      ��ǰch395phy״̬���ο�phy����/״̬����
 */
uint8_t ch395_cmd_get_phy_status(void)
{
    uint8_t i;

    ch395_write_cmd(CMD01_GET_PHY_STATUS);
    i = ch395_read_data();
    ch395_scs_hign;
    return i;
}

/**
 * @brief       ��ȡȫ���ж�״̬���յ�������ch395�Զ�ȡ���жϣ�0x43�����°汾ʹ��
 * @param       ��
 * @retval      ���ص�ǰ��ȫ���ж�״̬
 */
uint8_t ch395_cmd_get_glob_int_status(void)
{
    uint8_t init_status;

    ch395_write_cmd(CMD01_GET_GLOB_INT_STATUS);
    init_status = ch395_read_data();
    ch395_scs_hign;
    return  init_status;
}

/**
 * @brief       ��ʼ��ch395оƬ
 * @param       ��
 * @retval      ����ִ�н��
 */
uint8_t ch395_cmd_init(void)
{
    uint8_t i = 0;
    uint8_t s = 0;

    ch395_write_cmd(CMD0W_INIT_CH395);
    ch395_scs_hign;

    while (1)
    {
        delay_ms(10);                          /* ��ʱ��ѯ������2MS���� */
        s = ch395_get_cmd_status();            /* ���ܹ���Ƶ����ѯ */

        if (s != CH395_ERR_BUSY)
        {
            break;         /* ���CH395оƬ����æ״̬ */
        }

        if (i++ > 200)
        {
            return CH395_ERR_UNKNOW; /* ��ʱ�˳�,��������Ҫ500MS����ִ����� */
        }
    }

    return s;
}

/**
 * @brief       ����ch395���ڲ����ʣ����ڴ���ģʽ����Ч
 * @param       baudrate ���ڲ�����
 * @retval      ��
 */
void ch395_cmd_set_uart_baud_rate(uint32_t baudrate)
{
    ch395_write_cmd(CMD31_SET_BAUDRATE);
    ch395_write_data((uint8_t)baudrate);
    ch395_write_data((uint8_t)((uint16_t)baudrate >> 8));
    ch395_write_data((uint8_t)(baudrate >> 16));
    uint8_t i = ch395_read_data();
    ch395_scs_hign;
}

/**
 * @brief       ��ȡ����ִ��״̬��ĳЩ������Ҫ�ȴ�����ִ�н��
 * @param       ��
 * @retval      ������һ������ִ��״̬
 */
uint8_t ch395_get_cmd_status(void)
{
    uint8_t i;

    ch395_write_cmd(CMD01_GET_CMD_STATUS);
    i = ch395_read_data();
    ch395_scs_hign;
    return i;
}

/**
 * @brief       ����ch395��ip��ַ
 * @param       ipaddr ָip��ַ
 * @retval      ��
 */
void ch395_cmd_set_ipaddr(uint8_t *ipaddr)
{
    uint8_t i;

    ch395_write_cmd(CMD40_SET_IP_ADDR);

    for (i = 0; i < 4; i++)
    {
        ch395_write_data(*ipaddr++);
    }

    ch395_scs_hign;
}

/**
 * @brief       ����ch395������ip��ַ
 * @param       ipaddr ָ������ip��ַ
 * @retval      ��
 */
void ch395_cmd_set_gw_ipaddr(uint8_t *gwipaddr)
{
    uint8_t i;

    ch395_write_cmd(CMD40_SET_GWIP_ADDR);

    for (i = 0; i < 4; i++)
    {
        ch395_write_data(*gwipaddr++);
    }

    ch395_scs_hign;
}

/**
 * @brief       ����ch395���������룬Ĭ��Ϊ255.255.255.0
 * @param       maskaddr ָ���������ַ
 * @retval      ��
 */
void ch395_cmd_set_maskaddr(uint8_t *maskaddr)
{
    uint8_t i;

    ch395_write_cmd(CMD40_SET_MASK_ADDR);

    for (i = 0; i < 4; i++)
    {
        ch395_write_data(*maskaddr++);
    }

    ch395_scs_hign;
}

/**
 * @brief       ����ch395��mac��ַ��
 * @param       mcaddr mac��ַָ��
 * @retval      ��
 */
void ch395_cmd_set_macaddr(uint8_t *amcaddr)
{
    uint8_t i;

    ch395_write_cmd(CMD60_SET_MAC_ADDR);

    for (i = 0; i < 6; i++)
    {
        ch395_write_data(*amcaddr++);
    }

    ch395_scs_hign;
    delay_ms(100);
}

/**
 * @brief       ��ȡch395��mac��ַ��
 * @param       amcaddr mac��ַָ��
 * @retval      ��
 */
void ch395_cmd_get_macaddr(uint8_t *amcaddr)
{
    uint8_t i;

    ch395_write_cmd(CMD06_GET_MAC_ADDR);

    for (i = 0; i < 6; i++)
    {
        *amcaddr++ = ch395_read_data();
    }

    ch395_scs_hign;
}

/**
 * @brief       ����mac���ˡ�
 * @param       filtype �ο� mac����
 * @param       table0 hash0
 * @param       table1 hash1
 * @retval      ��
 */
void ch395_cmd_set_macfilt(uint8_t filtype, uint32_t table0, uint32_t table1)
{
    ch395_write_cmd(CMD90_SET_MAC_FILT);
    ch395_write_data(filtype);
    ch395_write_data((uint8_t)table0);
    ch395_write_data((uint8_t)((uint16_t)table0 >> 8));
    ch395_write_data((uint8_t)(table0 >> 16));
    ch395_write_data((uint8_t)(table0 >> 24));

    ch395_write_data((uint8_t)table1);
    ch395_write_data((uint8_t)((uint16_t)table1 >> 8));
    ch395_write_data((uint8_t)(table1 >> 16));
    ch395_write_data((uint8_t)(table1 >> 24));
    ch395_scs_hign;
}

/**
 * @brief       ��ȡ���ɴ���Ϣ (ip,port,protocol type)
 * @param       list �����ȡ���Ĳ��ɴ�
     @arg       ��1���ֽ�Ϊ���ɴ���룬��ο� ���ɴ����(ch395inc.h)
     @arg       ��2���ֽ�Ϊip��Э������
     @arg       ��3-4�ֽ�Ϊ�˿ں�
     @arg       ��4-8�ֽ�Ϊip��ַ
 * @retval      ��
 */
void ch395_cmd_get_unreachippt(uint8_t *list)
{
    uint8_t i;

    ch395_write_cmd(CMD08_GET_UNREACH_IPPORT);

    for (i = 0; i < 8; i++)
    {
        *list++ = ch395_read_data();
    }

    ch395_scs_hign;
}

/**
 * @brief       ��ȡԶ�˵�ip�Ͷ˿ڵ�ַ��һ����tcp serverģʽ��ʹ��
 * @param       sockindex socket����
 * @param       list ����ip�Ͷ˿�
 * @retval      ��
 */
void ch395_cmd_get_remoteipp(uint8_t sockindex, uint8_t *list)
{
    uint8_t i;

    ch395_write_cmd(CMD06_GET_REMOT_IPP_SN);
    ch395_write_data(sockindex);

    for (i = 0; i < 6; i++)
    {
        *list++ = ch395_read_data();
    }

    ch395_scs_hign;
}

/**
 * @brief       ����socket n��Ŀ��ip��ַ
 * @param       sockindex socket����
 * @param       ipaddr ָ��ip��ַ
 * @retval      ��
 */
void ch395_set_socket_desip(uint8_t sockindex, uint8_t *ipaddr)
{
    ch395_write_cmd(CMD50_SET_IP_ADDR_SN);
    ch395_write_data(sockindex);
    ch395_write_data(*ipaddr++);
    ch395_write_data(*ipaddr++);
    ch395_write_data(*ipaddr++);
    ch395_write_data(*ipaddr++);
    ch395_scs_hign;
}

/**
 * @brief       ����socket ��Э������
 * @param       sockindex socket����,prottype Э������
 * @param       ��ο� socketЭ�����Ͷ���(ch395inc.h)
 * @retval      ��
 */
void ch395_set_socket_prot_type(uint8_t sockindex, uint8_t prottype)
{
    ch395_write_cmd(CMD20_SET_PROTO_TYPE_SN);
    ch395_write_data(sockindex);
    ch395_write_data(prottype);
    ch395_scs_hign;
}

/**
 * @brief       ����socket n��Э������
 * @param       sockindex socket����
 * @param       desprot 2�ֽ�Ŀ�Ķ˿�
 * @retval      ��
 */
void ch395_set_socket_desport(uint8_t sockindex, uint16_t desprot)
{
    ch395_write_cmd(CMD30_SET_DES_PORT_SN);
    ch395_write_data(sockindex);
    ch395_write_data((uint8_t)desprot);
    ch395_write_data((uint8_t)(desprot >> 8));
    ch395_scs_hign;
}

/**
 * @brief       ����socket n��Э������
 * @param       sockindex socket����
 * @param       desprot 2�ֽ�Դ�˿�
 * @retval      ��
 */
void ch395_set_socket_sourport(uint8_t sockindex, uint16_t surprot)
{
    ch395_write_cmd(CMD30_SET_SOUR_PORT_SN);
    ch395_write_data(sockindex);
    ch395_write_data((uint8_t)surprot);
    ch395_write_data((uint8_t)(surprot >> 8));
    ch395_scs_hign;
}

/**
 * @brief       ipģʽ�£�socket ip��Э���ֶ�
 * @param       sockindex socket����
 * @param       prototype iprawģʽ1�ֽ�Э���ֶ�
 * @retval      ��
 */
void ch395_set_socket_ipraw_proto(uint8_t sockindex, uint8_t prototype)
{
    ch395_write_cmd(CMD20_SET_IPRAW_PRO_SN);
    ch395_write_data(sockindex);
    ch395_write_data(prototype);
    ch395_scs_hign;
}

/**
 * @brief       ����/�ر� ping
 * @param       senable :0 / 1, ���庬������:
 *   @arg       1:  ����ping
 *   @arg       0:  �ر�ping
 * @retval      ��
 */
void ch395_enable_ping(uint8_t enable)
{
    ch395_write_cmd(CMD01_PING_ENABLE);
    ch395_write_data(enable);
    ch395_scs_hign;
}

/**
 * @brief       ���ͻ�����д����
 * @param       sockindex socket����
 * @param       databuf  ���ݻ�����
 * @param       len   ����
 * @retval      ��
 */
void ch395_send_data(uint8_t sockindex, uint8_t *databuf, uint16_t len)
{
    uint16_t i;

    ch395_write_cmd(CMD30_WRITE_SEND_BUF_SN);
    ch395_write_data((uint8_t)sockindex);
    ch395_write_data((uint8_t)len);
    ch395_write_data((uint8_t)(len >> 8));

    for (i = 0; i < len; i++)
    {
        ch395_write_data(*databuf++);
    }

    ch395_scs_hign;
}

/**
 * @brief       ��ȡ���ջ���������
 * @param       sockindex socket����
 * @retval      ���ؽ��ջ�������Ч����
 */
uint16_t ch395_get_recv_length(uint8_t sockindex)
{
    uint16_t i;

    ch395_write_cmd(CMD12_GET_RECV_LEN_SN);
    ch395_write_data((uint8_t)sockindex);
    i = ch395_read_data();
    i = (uint16_t)(ch395_read_data() << 8) + i;
    ch395_scs_hign;
    return i;
}

/**
 * @brief       ������ջ�����
 * @param       sockindex socket����
 * @retval      ��
 */
void ch395_clear_recv_buf(uint8_t sockindex)
{
    ch395_write_cmd(CMD10_CLEAR_RECV_BUF_SN);
    ch395_write_data((uint8_t)sockindex);
    ch395_scs_hign;
}

/**
 * @brief       ��ȡ���ջ���������
 * @param       sockindex socket����
 * @param       len  ����
 * @param       pbuf  ������
 * @retval      ��
 */
void ch395_get_recv_data(uint8_t sockindex, uint16_t len, uint8_t *pbuf)
{
    uint16_t i;

    if (!len)return;

    ch395_write_cmd(CMD30_READ_RECV_BUF_SN);
    ch395_write_data(sockindex);
    ch395_write_data((uint8_t)len);
    ch395_write_data((uint8_t)(len >> 8));
    delay_us(1);

    for (i = 0; i < len; i++)
    {
        *pbuf = ch395_read_data();
        pbuf++;
    }

    ch395_scs_hign;
}

/**
 * @brief       �������Դ���
 * @param       count ����ֵ�����Ϊ20��
 * @retval      ��
 */
void ch395_cmd_set_retry_count(uint8_t count)
{
    ch395_write_cmd(CMD10_SET_RETRAN_COUNT);
    ch395_write_data(count);
    ch395_scs_hign;
}

/**
 * @brief       ������������
 * @param       period �������ڵ�λΪ���룬���1000ms
 * @retval      ��
 */
void ch395_cmd_set_retry_period(uint16_t period)
{
    ch395_write_cmd(CMD10_SET_RETRAN_COUNT);
    ch395_write_data((uint8_t)period);
    ch395_write_data((uint8_t)(period >> 8));
    ch395_scs_hign;
}

/**
 * @brief       ��ȡsocket
 * @param       sockindex socket����
 * @retval      socket n��״̬��Ϣ����1�ֽ�Ϊsocket �򿪻��߹ر�,��2�ֽ�Ϊtcp״̬
 */
void ch395_cmd_get_socket_status(uint8_t sockindex, uint8_t *status)
{
    ch395_write_cmd(CMD12_GET_SOCKET_STATUS_SN);
    ch395_write_data(sockindex);
    *status++ = ch395_read_data();
    *status++ = ch395_read_data();
    ch395_scs_hign;
}

/**
 * @brief       ��socket����������Ҫ�ȴ�ִ�гɹ�
 * @param       sockindex socket����
 * @retval      ����ִ�н��
 */
uint8_t  ch395_open_socket(uint8_t sockindex)
{
    uint8_t i = 0;
    uint8_t s = 0;
    ch395_write_cmd(CMD1W_OPEN_SOCKET_SN);
    ch395_write_data(sockindex);
    ch395_scs_hign;

    while (1)
    {
        delay_ms(5);                          /* ��ʱ��ѯ������2MS���� */
        s = ch395_get_cmd_status();           /* ���ܹ���Ƶ����ѯ */

        if (s != CH395_ERR_BUSY)
        {
            break;        /* ���CH395оƬ����æ״̬ */
        }

        if (i++ > 200)
        {
            return CH395_ERR_UNKNOW; /* ��ʱ�˳� */
        }
    }

    return s;
}

/**
 * @brief       �ر�socket��
 * @param       sockindex socket����
 * @retval      ����ִ�н��
 */
uint8_t  ch395_close_socket(uint8_t sockindex)
{
    uint8_t i = 0;
    uint8_t s = 0;
    ch395_write_cmd(CMD1W_CLOSE_SOCKET_SN);
    ch395_write_data(sockindex);
    ch395_scs_hign;

    while (1)
    {
        delay_ms(5);                            /* ��ʱ��ѯ������2MS���� */
        s = ch395_get_cmd_status();             /* ���ܹ���Ƶ����ѯ */

        if (s != CH395_ERR_BUSY)
        {
            break;          /* ���CH395оƬ����æ״̬ */
        }

        if (i++ > 200)
        {
            return CH395_ERR_UNKNOW;  /* ��ʱ�˳� */
        }
    }

    return s;
}

/**
 * @brief       tcp���ӣ�����tcpģʽ����Ч����������Ҫ�ȴ�ִ�гɹ�
 * @param       sockindex socket����
 * @retval      ����ִ�н��
 */
uint8_t ch395_tcp_connect(uint8_t sockindex)
{
    uint8_t i = 0;
    uint8_t s = 0;
    ch395_write_cmd(CMD1W_TCP_CONNECT_SN);
    ch395_write_data(sockindex);
    ch395_scs_hign;

    while (1)
    {
        delay_ms(5);                            /* ��ʱ��ѯ������2MS���� */
        s = ch395_get_cmd_status();             /* ���ܹ���Ƶ����ѯ */

        if (s != CH395_ERR_BUSY)
        {
            break;          /* ���CH395оƬ����æ״̬ */
        }

        if (i++ > 200)
        {
            return CH395_ERR_UNKNOW;  /* ��ʱ�˳� */
        }
    }

    return s;
}

/**
 * @brief       tcp����������tcpģʽ����Ч����������Ҫ�ȴ�ִ�гɹ�
 * @param       sockindex socket����
 * @retval      ����ִ�н��
 */
uint8_t ch395_tcp_listen(uint8_t sockindex)
{
    uint8_t i = 0;
    uint8_t s = 0;
    ch395_write_cmd(CMD1W_TCP_LISTEN_SN);
    ch395_write_data(sockindex);
    ch395_scs_hign;

    while (1)
    {
        delay_ms(5);                           /* ��ʱ��ѯ������2MS���� */
        s = ch395_get_cmd_status();            /* ���ܹ���Ƶ����ѯ */

        if (s != CH395_ERR_BUSY)
        {
            break;         /* ���CH395оƬ����æ״̬ */
        }

        if (i++ > 200)
        {
            return CH395_ERR_UNKNOW; /* ��ʱ�˳� */
        }
    }

    return s;
}

/**
 * @brief       tcp�Ͽ�������tcpģʽ����Ч����������Ҫ�ȴ�ִ�гɹ�
 * @param       sockindex socket����
 * @retval      ��
 */
uint8_t ch395_tcp_disconnect(uint8_t sockindex)
{
    uint8_t i = 0;
    uint8_t s = 0;
    ch395_write_cmd(CMD1W_TCP_DISNCONNECT_SN);
    ch395_write_data(sockindex);
    ch395_scs_hign;

    while (1)
    {
        delay_ms(5);                           /* ��ʱ��ѯ������2MS���� */
        s = ch395_get_cmd_status();               /* ���ܹ���Ƶ����ѯ */

        if (s != CH395_ERR_BUSY)
        {
            break;         /* ���CH395оƬ����æ״̬ */
        }

        if (i++ > 200)
        {
            return CH395_ERR_UNKNOW; /* ��ʱ�˳� */
        }
    }

    return s;
}

/**
 * @brief       ��ȡsocket n���ж�״̬
 * @param       sockindex   socket����
 * @retval      �ж�״̬
 */
uint8_t ch395_get_socket_int(uint8_t sockindex)
{
    uint8_t intstatus;
    ch395_write_cmd(CMD11_GET_INT_STATUS_SN);
    ch395_write_data(sockindex);
    delay_us(2);
    intstatus = ch395_read_data();
    ch395_scs_hign;
    return intstatus;
}

/**
 * @brief       �Զಥ��ַ����crc���㣬��ȡ��6λ��
 * @param       mac_addr   mac��ַ
 * @retval      ����crc32�ĸ�6λ
 */
uint8_t ch395_crcret_6bit(uint8_t *mac_addr)
{
    signed long perbyte;
    signed long perbit;
    const uint32_t poly = 0x04c11db7;
    uint32_t crc_value = 0xffffffff;
    uint8_t c;

    for ( perbyte = 0; perbyte < 6; perbyte ++ )
    {
        c = *(mac_addr++);

        for ( perbit = 0; perbit < 8; perbit++ )
        {
            crc_value = (crc_value << 1) ^ ((((crc_value >> 31)^c) & 0x01) ? poly : 0);
            c >>= 1;
        }
    }

    crc_value = crc_value >> 26;
    return ((uint8_t)crc_value);
}

/**
 * @brief       ����/ֹͣdhcp
 * @param       flag:0 / 1, ���庬������:
 *   @arg       1:����dhcp
 *   @arg       0��ֹͣdhcp
 * @retval      ִ��״̬
 */
uint8_t  ch395_dhcp_enable(uint8_t flag)
{
    uint8_t i = 0;
    uint8_t s;
    ch395_write_cmd(CMD10_DHCP_ENABLE);
    ch395_write_data(flag);
    ch395_scs_hign;

    while (1)
    {
        delay_ms(20);
        s = ch395_get_cmd_status();            /* ���ܹ���Ƶ����ѯ */

        if (s != CH395_ERR_BUSY)
        {
            break;         /* ���ch395оƬ����æ״̬ */
        }

        if (i++ > 200)
        {
            return CH395_ERR_UNKNOW; /* ��ʱ�˳� */
        }
    }

    return s;
}

/**
 * @brief       ��ȡdhcp״̬
 * @param       ��
 * @retval      dhcp״̬��0Ϊ�ɹ�������ֵ��ʾ����
 */
uint8_t ch395_get_dhcp_status(void)
{
    uint8_t status;
    ch395_write_cmd(CMD01_GET_DHCP_STATUS);
    status = ch395_read_data();
    ch395_scs_hign;
    return status;
}

/**
 * @brief       ��ȡip��������������ص�ַ
 * @param       sockindex socket����
 * @retval      12���ֽڵ�ip,������������ص�ַ
 */
void ch395_get_ipinf(uint8_t *addr)
{
    uint8_t i;
    ch395_write_cmd(CMD014_GET_IP_INF);

    for (i = 0; i < 20; i++)
    {
        *addr++ = ch395_read_data();
    }

    ch395_scs_hign;
}

/**
 * @brief       дgpio�Ĵ���
 * @param       regadd   �Ĵ�����ַ
 * @param       regval   �Ĵ���ֵ
 * @retval      ��
 */
void ch395_write_gpio_addr(uint8_t regadd, uint8_t regval)
{
    ch395_write_cmd(CMD20_WRITE_GPIO_REG);
    ch395_write_data(regadd);
    ch395_write_data(regval);
}

/**
 * @brief       ��gpio�Ĵ���
 * @param       regadd   �Ĵ�����ַ
 * @retval      �Ĵ�����ֵ
 */
uint8_t ch395_read_gpio_addr(uint8_t regadd)
{
    uint8_t i;
    ch395_write_cmd(CMD10_READ_GPIO_REG);
    ch395_write_data(regadd);
    delay_ms(1);
    i = ch395_read_data();
    return i;
}

/**
 * @brief       ����eeprom
 * @param       ��
 * @retval      ִ��״̬
 */
uint8_t ch395_eeprom_erase(void)
{
    uint8_t i;
    ch395_write_cmd(CMD00_EEPROM_ERASE);

    while (1)
    {
        delay_ms(20);
        i = ch395_get_cmd_status();

        if (i == CH395_ERR_BUSY)
        {
            continue;
        }

        break;
    }

    return i;
}

/**
 * @brief       дeeprom
 * @param       eepaddr  eeprom��ַ
 * @param       buf      ��������ַ
 * @param       len      ����
 * @retval      ��
 */
uint8_t ch395_eeprom_write(uint16_t eepaddr, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    ch395_write_cmd(CMD30_EEPROM_WRITE);
    ch395_write_data((uint8_t)(eepaddr));
    ch395_write_data((uint8_t)(eepaddr >> 8));
    ch395_write_data(len);

    while (len--)ch395_write_data(*buf++);

    while (1)
    {
        delay_ms(20);
        i = ch395_get_cmd_status();

        if (i == CH395_ERR_BUSY)
        {
            continue;
        }

        break;
    }

    return i;
}

/**
 * @brief       дeeprom
 * @param       eepaddr  eeprom��ַ
 * @param       buf      ��������ַ
 * @param       len      ����
 * @retval      ��
 */
void ch395_eeprom_read(uint16_t eepaddr, uint8_t *buf, uint8_t len)
{
    ch395_write_cmd(CMD30_EEPROM_READ);
    ch395_write_data((uint8_t)(eepaddr));
    ch395_write_data((uint8_t)(eepaddr >> 8));
    ch395_write_data(len);
    delay_ms(1);

    while (len--)
    {
        *buf++ = ch395_read_data();
    }
}

/**
 * @brief       ����tcp mssֵ
 * @param       tcpmss
 * @retval      ��
 */
void ch395_set_tcpmss(uint16_t tcpmss)
{
    ch395_write_cmd(CMD20_SET_TCP_MSS);
    ch395_write_data((uint8_t)(tcpmss));
    ch395_write_data((uint8_t)(tcpmss >> 8));
}

/**
 * @brief       ����socket���ջ�����
 * @param       sockindex  socket����,ַ,blknum
 * @param       startblk   ��ʼ��
 * @param       ��λ���������� ����λΪ512�ֽ�
 * @retval      ��
 */
void ch395_set_socket_recv_buf(uint8_t sockindex, uint8_t startblk, uint8_t blknum)
{
    ch395_write_cmd(CMD30_SET_RECV_BUF);
    ch395_write_data(sockindex);
    ch395_write_data(startblk);
    ch395_write_data(blknum);
}

/**
 * @brief       ����socket���ͻ�����
 * @param       sockindex  socket����
 * @param       startblk   ��ʼ��ַ
 * @param       blknum     ��λ����������
 * @retval      ��
 */
void ch395_set_socket_send_buf(uint8_t sockindex, uint8_t startblk, uint8_t blknum)
{
    ch395_write_cmd(CMD30_SET_SEND_BUF);
    ch395_write_data(sockindex);
    ch395_write_data(startblk);
    ch395_write_data(blknum);
}

/**
 * @brief       udp��ָ����ip�Ͷ˿ڷ�������
 * @param       buf     : �������ݻ�����
 * @param       len     : �������ݳ���
 * @param       ip      : Ŀ��ip
 * @param       port    : Ŀ��˿�
 * @param       sockeid : socket����ֵ
 * @retval      ��
 */
void ch395_udp_send_data(uint8_t *buf, uint32_t len, uint8_t *ip, uint16_t port, uint8_t sockindex)
{
    ch395_set_socket_desip(sockindex, ip);   /* ����socket 0Ŀ��IP��ַ */
    ch395_set_socket_desport(sockindex, port);
    ch395_send_data(sockindex, buf, len);
}

/**
 * @brief       ����ch395��������
 * @param       mdata ���õĲ���
 * @retval      ��
 */
void ch395_set_start_para(uint32_t mdata)
{
    ch395_write_cmd(CMD40_SET_FUN_PARA);
    ch395_write_data((uint8_t)mdata);
    ch395_write_data((uint8_t)((uint16_t)mdata >> 8));
    ch395_write_data((uint8_t)(mdata >> 16));
    ch395_write_data((uint8_t)(mdata >> 24));
}

/**
 * @brief       ��ȡȫ���ж�״̬���յ�������ch395�Զ�ȡ���ж�,0x44�����ϰ汾ʹ��
 * @param       ��
 * @retval      ���ص�ǰ��ȫ���ж�״̬
 */
uint16_t ch395_cmd_get_glob_int_status_all(void)
{
    uint16_t init_status;
    ch395_write_cmd(CMD02_GET_GLOB_INT_STATUS_ALL);
    delay_us(2);
    init_status = ch395_read_data();
    init_status = (uint16_t)(ch395_read_data() << 8) + init_status;
    ch395_scs_hign;
    return  init_status;
}

/**
 * @brief       ����keepalive����
 * @param       sockindex socket��
 * @param       cmd 0���ر� 1������
 * @retval      ��
 */
void ch395_set_keeplive(uint8_t sockindex, uint8_t cmd)
{
    ch395_write_cmd(CMD20_SET_KEEP_LIVE_SN);
    ch395_write_data(sockindex);
    ch395_write_data(cmd);
}

/**
 * @brief       ����keepalive���Դ���
 * @param       cnt ���Դ�������
 * @retval      ��
 */
void ch395_keeplive_cnt(uint8_t cnt)
{
    ch395_write_cmd(CMD10_SET_KEEP_LIVE_CNT);
    ch395_write_data(cnt);
}

/**
 * @brief       ����keeplive����
 * @param       idle ����ʱ�䣨��λ��ms��
 * @retval      ��
 */
void ch395_keeplive_idle(uint32_t idle)
{
    ch395_write_cmd(CMD40_SET_KEEP_LIVE_IDLE);
    ch395_write_data((uint8_t)idle);
    ch395_write_data((uint8_t)((uint16_t)idle >> 8));
    ch395_write_data((uint8_t)(idle >> 16));
    ch395_write_data((uint8_t)(idle >> 24));
}

/**
 * @brief       ����keeplive���ʱ��
 * @param       intvl ���ʱ�䣨��λ��ms��
 * @retval      ��
 */
void ch395_keeplive_intvl(uint32_t intvl)
{
    ch395_write_cmd(CMD40_SET_KEEP_LIVE_INTVL);
    ch395_write_data((uint8_t)intvl);
    ch395_write_data((uint8_t)((uint16_t)intvl >> 8));
    ch395_write_data((uint8_t)(intvl >> 16));
    ch395_write_data((uint8_t)(intvl >> 24));
}

/**
 * @brief       ����ttl
 * @param       ssockindex socket��
 * @param       ttlnum:ttl��
 * @retval      ��
 */
void ch395_setttl_num(uint8_t sockindex, uint8_t ttlnum)
{
    ch395_write_cmd(CMD20_SET_TTL);
    ch395_write_data(sockindex);
    ch395_write_data(ttlnum);
}
