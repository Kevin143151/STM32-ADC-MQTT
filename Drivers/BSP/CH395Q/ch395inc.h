/**
 ****************************************************************************************************
 * @file        ch395inc.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-6-17
 * @brief       ch395inc �������
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

#ifndef   __CH395INC_H__
#define   __CH395INC_H__


/* ********************************************************************************************************************* */
/* �������ͺͳ������� */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef     TRUE
#define     TRUE    1
#define     FALSE   0
#endif
#ifndef     NULL
#define     NULL    0
#endif

/* �������:
 * һ���������˳�����:
 * һ��������(���ڴ��ڷ�ʽ,������֮ǰ����Ҫ����ͬ����)
 * ���ɸ���������(������0��)
 * ���ɸ��������(������0��)
 * ��������������: CMDxy_NAME
 * ���е�x��y��������, x˵�������������ݸ���(�ֽ���), y˵������������ݸ���(�ֽ���), y�����W��ʾ��Ҫ�ȴ�����ִ�гɹ�
 * ��Щ�����ܹ�ʵ��0������ֽڵ����ݿ��д, ���ݿ鱾����ֽ���δ����������x��y֮��
 */
#define CMD01_GET_IC_VER                  0x01        /* ��ȡоƬ�Լ��̼��汾�� */

#define CMD31_SET_BAUDRATE                0x02        /* ���ڷ�ʽ: ���ô���ͨѶ������(�ϵ���߸�λ���Ĭ�ϲ�����Ϊ9600bps */

#define CMD00_ENTER_SLEEP                 0x03        /* ����˯��״̬ */

#define CMD00_RESET_ALL                   0x05        /* ִ��Ӳ����λ */

#define CMD11_CHECK_EXIST                 0x06        /* ����ͨѶ�ӿ��Լ�����״̬ */

#define CMD02_GET_GLOB_INT_STATUS_ALL     0x19        /* ��ȡȫ���ж�״̬��V44�汾�Ժ�ĳ�������������socket������Ҫ�ô������ȡȫ�����ж� */

#define CMD10_SET_PHY                     0x20        /* ����PHY��Ĭ��ΪAuto���Զ�Э�� */

#define CMD60_SET_MAC_ADDR                0x21        /* ����MAC��ַ ������CMD00H_INIT_CH395֮ǰ������� */

#define CMD40_SET_IP_ADDR                 0x22        /* ����IP��ַ ������CMD00H_INIT_CH395֮ǰ������� */

#define CMD40_SET_GWIP_ADDR               0x23        /* ��������IP��ַ ������CMD00H_INIT_CH395֮ǰ������� */

#define CMD40_SET_MASK_ADDR               0x24        /* �����������룬 ������CMD00H_INIT_CH395֮ǰ������� */

#define CMD90_SET_MAC_FILT                0x25        /* ����MAC���� ���Խ��й㲥���ಥ�ȹ��� */

#define CMD01_GET_PHY_STATUS              0x26        /* ��ȡPHY��ǰ״̬����Ͽ����ӣ�10/100M FULL/HALF */

#define CMD0W_INIT_CH395                  0x27        /* ��ʼ��CH395 */

#define CMD08_GET_UNREACH_IPPORT          0x28        /* ��ȡ���ɴ���Ϣ */

#define CMD01_GET_GLOB_INT_STATUS         0x29        /* ��ȡȫ���ж�״̬�����ֵΪ1S������������Ϊ0 */

#define CMD10_SET_RETRAN_COUNT            0x2A        /* ���Դ���������TCPģʽ����Ч */

#define CMD20_SET_RETRAN_PERIOD           0x2B        /* �������ڣ����ֵΪ20������TCPģʽ����Ч������������Ϊ0 */

#define CMD01_GET_CMD_STATUS              0x2C        /* ��ȡ����ִ��״̬ */

#define CMD06_GET_REMOT_IPP_SN            0x2D        /* ��ȡԶ�˵Ķ˿��Լ�IP��ַ����������TCP������ģʽ��ʹ�� */

#define CMD10_CLEAR_RECV_BUF_SN           0x2E        /* ������ջ�����  */

#define CMD12_GET_SOCKET_STATUS_SN        0x2F        /* ��ȡsocket n״̬ */

#define CMD11_GET_INT_STATUS_SN           0x30        /* ��ȡsocket n���ж�״̬ */

#define CMD50_SET_IP_ADDR_SN              0x31        /* ����socket n��Ŀ��IP��ַ */

#define CMD30_SET_DES_PORT_SN             0x32        /* ����socket n��Ŀ�Ķ˿� */

#define CMD30_SET_SOUR_PORT_SN            0x33        /* ����socket n��Դ�˿� */

#define CMD20_SET_PROTO_TYPE_SN           0x34        /* ����socket n��Э������ */

#define CMD1W_OPEN_SOCKET_SN              0x35        /* ��socket n */

#define CMD1W_TCP_LISTEN_SN               0x36        /* socket n�������յ������socket n���������ģʽ������TCPģʽ��Ч */

#define CMD1W_TCP_CONNECT_SN              0x37        /* socket n���ӣ��յ������socket n����ͻ���ģʽ������TCPģʽ��Ч */

#define CMD1W_TCP_DISNCONNECT_SN          0x38        /* socket n�Ͽ����ӣ��յ������socket n�Ͽ��������ӣ�����TCPģʽ��Ч */

#define CMD30_WRITE_SEND_BUF_SN           0x39        /* ��socket n������д������ */

#define CMD12_GET_RECV_LEN_SN             0x3B        /* ��ȡsocket n�������ݵĳ��� */

#define CMD30_READ_RECV_BUF_SN            0x3C        /* ��ȡsocket n���ջ��������� */

#define CMD1W_CLOSE_SOCKET_SN             0x3D        /* �ر�socket n */

#define CMD20_SET_IPRAW_PRO_SN            0x3E        /* ��IP RAW�£�����socket n��IP��Э������ */

#define CMD01_PING_ENABLE                 0x3F        /* ����/�ر�PING */

#define CMD06_GET_MAC_ADDR                0x40        /* ��ȡMAC��ַ */

#define CMD10_DHCP_ENABLE                 0x41        /* DHCPʹ�� */

#define CMD01_GET_DHCP_STATUS             0x42        /* ��ȡDHCP״̬ */

#define CMD014_GET_IP_INF                 0x43        /* IP,�������룬���� */

#define CMD00_PPPOE_SET_USER_NAME         0x44        /* ����PPPOE�û��� */

#define CMD00_PPPOE_SET_PASSWORD          0x45        /* �������� */

#define CMD10_PPPOE_ENABLE                0x46        /* PPPOEʹ�� */

#define CMD01_GET_PPPOE_STATUS            0x47        /* ��ȡpppoe״̬ */

#define CMD20_SET_TCP_MSS                 0x50        /* ����TCP MSS */

#define CMD20_SET_TTL                     0x51        /* ����TTL��TTL���ֵΪ128 */

#define CMD30_SET_RECV_BUF                0x52        /* ����SOCKET���ջ����� */

#define CMD30_SET_SEND_BUF                0x53        /* ����SOCKET���ͻ����� */

#define CMD10_SET_MAC_RECV_BUF            0x54        /* ����MAC���ջ����� */

#define CMD40_SET_FUN_PARA                0x55        /* ���ù��ܲ��� */

#define CMD40_SET_KEEP_LIVE_IDLE          0x56        /* ����KEEPLIVE���� */

#define CMD40_SET_KEEP_LIVE_INTVL         0x57        /* ���ü��ʱ�� */

#define CMD10_SET_KEEP_LIVE_CNT           0x58        /* ���Դ��� */

#define CMD20_SET_KEEP_LIVE_SN            0X59        /* ����socket nkeeplive����*/

#define CMD00_EEPROM_ERASE                0xE9        /* ����EEPROM*/

#define CMD30_EEPROM_WRITE                0xEA        /* дEEPROM */

#define CMD30_EEPROM_READ                 0xEB        /* ��EEPROM */

#define CMD10_READ_GPIO_REG               0xEC        /* ��GPIO�Ĵ��� */

#define CMD20_WRITE_GPIO_REG              0xED        /* дGPIO�Ĵ��� */

/* Э������ */
#define PROTO_TYPE_IP_RAW                 0           /* IP��ԭʼ���� */
#define PROTO_TYPE_MAC_RAW                1           /* MAC��ԭʼ���� */
#define PROTO_TYPE_UDP                    2           /* UDPЭ������ */
#define PROTO_TYPE_TCP                    3           /* TCPЭ������ */

/* PHY �������/״̬ */
#define PHY_DISCONN                       (1<<0)      /* PHY�Ͽ� */
#define PHY_10M_FLL                       (1<<1)      /* 10Mȫ˫�� */
#define PHY_10M_HALF                      (1<<2)      /* 10M��˫�� */
#define PHY_100M_FLL                      (1<<3)      /* 100Mȫ˫�� */
#define PHY_100M_HALF                     (1<<4)      /* 100M��˫�� */
#define PHY_AUTO                          (1<<5)      /* PHY�Զ�ģʽ��CMD10H_SET_PHY */

/* CH395 MAC���� */
#define MAC_FILT_RECV_BORADPKT            (1<<0)      /* ʹ�ܽ��չ㲥�� */
#define MAC_FILT_RECV_ALL                 (1<<1)      /* ʹ�ܽ����������ݰ� */
#define MAC_FILT_RECV_MULTIPKT            (1<<2)      /* ʹ�ܽ��նಥ�� */
#define MAC_FILT_RECV_ENABLE              (1<<3)      /* ʹ�ܽ��� */
#define MAC_FILT_SEND_ENABLE              (1<<4)      /* ʹ�ܷ��� */

/* �ж�״̬ */
/* ����ΪGLOB_INT�������״̬ */
#define GINT_STAT_UNREACH                 (1<<0)      /* ���ɴ��ж� */
#define GINT_STAT_IP_CONFLI               (1<<1)      /* IP��ͻ */
#define GINT_STAT_PHY_CHANGE              (1<<2)      /* PHY״̬�ı� */
#define GINT_STAT_DHCP                    (1<<3)      /* PHY״̬�ı� */
#define GINT_STAT_SOCK0                   (1<<4)      /* socket0 �����ж� */
#define GINT_STAT_SOCK1                   (1<<5)      /* socket1 �����ж� */
#define GINT_STAT_SOCK2                   (1<<6)      /* socket2 �����ж� */
#define GINT_STAT_SOCK3                   (1<<7)      /* socket3 �����ж� */
#define GINT_STAT_SOCK4                   (1<<8)      /* scoket4 �����ж� */
#define GINT_STAT_SOCK5                   (1<<9)      /* scoket5 �����ж� */
#define GINT_STAT_SOCK6                   (1<<10)     /* scoket6 �����ж� */
#define GINT_STAT_SOCK7                   (1<<11)     /* scoket7 �����ж� */

/* ����ΪSn_INT�������״̬ */
#define SINT_STAT_SENBUF_FREE             (1<<0)      /* ���ͻ��������� */
#define SINT_STAT_SEND_OK                 (1<<1)      /* ���ͳɹ� */
#define SINT_STAT_RECV                    (1<<2)      /* socket�˿ڽ��յ����ݻ��߽��ջ�������Ϊ�� */
#define SINT_STAT_CONNECT                 (1<<3)      /* ���ӳɹ�,TCPģʽ�²������ж� */
#define SINT_STAT_DISCONNECT              (1<<4)      /* ���ӶϿ�,TCPģʽ�²������ж� */
#define SINT_STAT_TIM_OUT                 (1<<6)      /* ARP��TCPģʽ�»ᷢ�����ж� */

/* ��ȡ����״̬ */
#define CMD_ERR_SUCCESS                   0x00        /* ��������ɹ� */
#define CMD_RET_ABORT                     0x5F        /* �������ʧ�� */
#define CH395_ERR_BUSY                    0x10        /* æ״̬����ʾ��ǰ����ִ������ */
#define CH395_ERR_MEM                     0x11        /* �ڴ���� */
#define CH395_ERR_BUF                     0x12        /* ���������� */
#define CH395_ERR_TIMEOUT                 0x13        /* ��ʱ */
#define CH395_ERR_RTE                     0x14        /* ·�ɴ���*/
#define CH395_ERR_ABRT                    0x15        /* ����ֹͣ*/
#define CH395_ERR_RST                     0x16        /* ���Ӹ�λ */
#define CH395_ERR_CLSD                    0x17        /* ���ӹر�/socket �ڹر�״̬ */
#define CH395_ERR_CONN                    0x18        /* ������ */
#define CH395_ERR_VAL                     0x19        /* �����ֵ */
#define CH395_ERR_ARG                     0x1a        /* ����Ĳ��� */
#define CH395_ERR_USE                     0x1b        /* �Ѿ���ʹ�� */
#define CH395_ERR_IF                      0x1c        /* MAC����  */
#define CH395_ERR_ISCONN                  0x1d        /* ������ */
#define CH395_ERR_OPEN                    0X20        /* �Ѿ��� */
#define CH395_ERR_UNKNOW                  0xFA        /* δ֪���� */

/* PPP״̬ */
#define CH395_PPP_SUCCESS                 0           /* �ɹ� */
#define CH395_PPPERR_PARM                 1           /* ��Ч���� */
#define CH395_PPPERR_OPEN                 2           /* �޷���PPP�Ự */
#define CH395_PPPERR_DEVICE               3           /* ��Ч��PPP�豸 */
#define CH395_PPPERR_ALLOC                4           /* ��Դ����ʧ�� */
#define CH395_PPPERR_USER                 5           /* �û��ж� */
#define CH395_PPPERR_CONNECT              6           /* ���ӶϿ� */
#define CH395_PPPERR_AUTHFAIL             7           /* ��ս����ʧ�� */
#define CH395_PPPERR_PROTOCOL             8           /* ����Э��ʧ�� */
#define CH395_PPPERR_TIME_OUT             9           /* ��ʱʧ�� */
#define CH395_PPPERR_CLOSE                10          /* �ر�ʧ�� */

/* ���ɴ���� */
#define UNREACH_CODE_HOST                 0           /* �������ɴ� */
#define UNREACH_CODE_NET                  1           /* ���粻�ɴ� */
#define UNREACH_CODE_PROTOCOL             2           /* Э�鲻�ɴ� */
#define UNREACH_CODE_PROT                 3           /* �˿ڲ��ɴ� */
/* ����ֵ��ο�RFC792�ĵ� */

/* �����ͷ */
#define SER_SYNC_CODE1                    0x57        /* ��������ͬ����1 */
#define SER_SYNC_CODE2                    0xAB        /* ��������ͬ����2 */

/* TCP״̬ */
#define TCP_CLOSED                        0
#define TCP_LISTEN                        1
#define TCP_SYN_SENT                      2
#define TCP_SYN_RCVD                      3
#define TCP_ESTABLISHED                   4
#define TCP_FIN_WAIT_1                    5
#define TCP_FIN_WAIT_2                    6
#define TCP_CLOSE_WAIT                    7
#define TCP_CLOSING                       8
#define TCP_LAST_ACK                      9
#define TCP_TIME_WAIT                     10

/* GPIO�Ĵ�����ַ */
#define GPIO_DIR_REG                     0x80        /* �Ĵ�������Ĵ�����1�����;0:���� */
#define GPIO_IN_REG                      0x81        /* GPIO����Ĵ��� */
#define GPIO_OUT_REG                     0x82        /* GPIO����Ĵ��� */
#define GPIO_CLR_REG                     0x83        /* GPIO������: 0=keep, 1=clear */
#define GPIO_PU_REG                      0x84        /* GPIO����ʹ�ܼĴ��� */
#define GPIO_PD_REG                      0x85        /* GPIO����ʹ�ܼĴ��� */

/* ���ܲ��� */
#define FUN_PARA_FLAG_TCP_SERVER         (1<<1)      /* tcp server ������ģʽ��־��0X44�汾���Ժ�֧�� */
#define FUN_PARA_FLAG_LOW_PWR            (1<<2)      /* �ͺ���ģʽ */
#define SOCK_CTRL_FLAG_SOCKET_CLOSE      (1<<3)      /* CH395�������ر�Socket */
#define SOCK_DISABLE_SEND_OK_INT         (1<<4)      /* send ok�жϿ���λ��Ϊ1��ʾ�ر�send ok�ж� */

#ifdef __cplusplus
}
#endif
#endif
