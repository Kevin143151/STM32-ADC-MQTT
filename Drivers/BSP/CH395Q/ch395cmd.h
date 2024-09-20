/**
 ****************************************************************************************************
 * @file        ch395cmd.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-6-17
 * @brief       ch395cmd ��������
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
 
#ifndef __CH395CMD_H__
#define __CH395CMD_H__
#include "./BSP/CH395Q/ch395inc.h"
#include "./SYSTEM/sys/sys.h"


void ch395_cmd_reset(void);                                                                             /* ��λ */

void ch395_cmd_sleep(void);                                                                             /* ˯�� */

uint8_t ch395_cmd_get_ver(void);                                                                        /* ��ȡоƬ���̼��汾�� */

uint8_t ch395_cmd_check_exist(uint8_t testdata);                                                        /* �������� */

void ch395_cmd_set_phy(uint8_t phystat);                                                                /* ����phy״̬  */

uint8_t ch395_cmd_get_phy_status(void);                                                                 /* ��ȡphy״̬ */

uint8_t ch395_cmd_get_glob_int_status(void);                                                            /* ��ȡch395ȫ���ж�״̬ */

uint8_t ch395_cmd_init(void);                                                                           /* ��ʼ��ch395 */

void ch395_cmd_set_uart_baud_rate(uint32_t baudrate);                                                   /* ���ò����� */

uint8_t ch395_get_cmd_status(void);                                                                     /* ��ȡ����ִ��״̬ */

void ch395_cmd_set_ipaddr(uint8_t *ipaddr);                                                             /* ����ch395��ip��ַ */

void ch395_cmd_set_gw_ipaddr(uint8_t *gwipaddr);                                                        /* ����ch395������ip��ַ */

void ch395_cmd_set_maskaddr(uint8_t *maskaddr);                                                         /* ������������ */

void ch395_cmd_set_macaddr(uint8_t *amcaddr);                                                           /* ����ch395��mac��ַ */

void ch395_cmd_get_macaddr(uint8_t *amcaddr);                                                           /* ��ȡmac��ַ */

void ch395_cmd_set_macfilt(uint8_t filtype, uint32_t table0, uint32_t table1);                          /* ����ch395��mac���� */

void ch395_cmd_get_unreachippt(uint8_t *list);                                                          /* ��ȡ���ɴ��ַ�Լ��˿� */

void ch395_cmd_get_remoteipp(uint8_t sockindex, uint8_t *list);                                         /* ��ȡԶ��ip�Ͷ˿ڣ�һ����tcp server��ʹ�� */

void ch395_set_socket_desip(uint8_t sockindex, uint8_t *ipaddr);                                        /* ����scoket n��Ŀ��ip��ַ */

void ch395_set_socket_prot_type(uint8_t sockindex, uint8_t prottype);                                   /* ����socket n��Э������ */

void ch395_set_socket_desport(uint8_t sockindex, uint16_t desprot);                                     /* ����socket n��Ŀ�Ķ˿� */

void ch395_set_socket_sourport(uint8_t sockindex, uint16_t surprot);                                    /* ����socket n��Դ�˿� */

void ch395_set_socket_ipraw_proto(uint8_t sockindex, uint8_t prototype);                                /* ��iprawģʽ�£�����socket n��ip��Э���ֶ� */

void ch395_set_recv_threslen(uint8_t sockindex, uint16_t len);                                          /* ����socket n�Ľ����жϷ�ֵ */

void ch395_send_data(uint8_t sockindex, uint8_t *databuf, uint16_t len);                                /* ��socket n�ķ��ͻ�����д���� */

uint16_t ch395_get_recv_length(uint8_t sockindex);                                                      /* ��ȡsocket n�Ľ��ճ��� */

void ch395_clear_recv_buf(uint8_t sockindex);                                                           /* ���socket n�Ľ��ջ����� */

void ch395_get_recv_data(uint8_t sockindex, uint16_t len, uint8_t *pbuf);                               /* ��ȡ�������� */

void ch395_cmd_set_retry_count(uint8_t count);                                                          /* ����������Դ��� */

void ch395_cmd_set_retry_period(uint16_t period);                                                       /* ��������������� ��λ ���� */

void ch395_cmd_get_socket_status(uint8_t sockindex, uint8_t *status) ;                                  /* ��ȡsocket n��״̬ */

uint8_t  ch395_open_socket(uint8_t sockindex);                                                          /* ��socket n*/

uint8_t  ch395_close_socket(uint8_t sockindex);                                                         /* �ر�socket n*/

uint8_t ch395_tcp_connect(uint8_t sockindex);                                                           /* tcp���� */

uint8_t ch395_tcp_listen(uint8_t sockindex);                                                            /* tcp���� */

uint8_t ch395_tcp_disconnect(uint8_t sockindex);                                                        /* tcp�Ͽ����� */

uint8_t ch395_get_socket_int(uint8_t sockindex);                                                        /* ��ȡsocket n���ж�״̬ */

uint8_t ch395_crcret_6bit(uint8_t *mac_addr);                                                           /* �ಥ��ַcrc32������hash���� function count = 36 */

void ch395_get_ipinf(uint8_t *addr);                                                                    /* ��ȡip��������������ص�ַ */

uint8_t ch395_get_dhcp_status(void);                                                                    /* ��ȡdhcp״̬ */

uint8_t  ch395_dhcp_enable(uint8_t flag);                                                               /* ����/ֹͣdhcp */

void ch395_write_gpio_addr(uint8_t regadd, uint8_t regval);                                             /* дgpio�Ĵ��� */

uint8_t ch395_read_gpio_addr(uint8_t regadd);                                                           /* ��gpio�Ĵ��� */

uint8_t ch395_eeprom_erase(void);                                                                       /* ����eeprom */

uint8_t ch395_eeprom_write(uint16_t eepaddr, uint8_t *buf, uint8_t len);                                /* дeeprom */

void ch395_eeprom_read(uint16_t eepaddr, uint8_t *buf, uint8_t len);                                    /* ��eeprom */

void ch395_set_tcpmss(uint16_t tcpmss);                                                                 /* ����tcp mssֵ */

void ch395_set_socket_recv_buf(uint8_t sockindex, uint8_t startblk, uint8_t blknum);                    /* ����socket���ջ����� */

void ch395_set_socket_send_buf(uint8_t sockindex, uint8_t startblk, uint8_t blknum);                    /* ����socket���ͻ����� */

void ch395_udp_send_data(uint8_t *buf, uint32_t len, uint8_t *ip, uint16_t port, uint8_t sockindex);    /* udp��ָ����ip�Ͷ˿ڷ������� */

void ch395_set_start_para(uint32_t mdata);                                                              /* ����ch395�������� */

uint16_t ch395_cmd_get_glob_int_status_all(void);                                                       /* ��ȡȫ���ж�״̬���յ�������ch395�Զ�ȡ���ж�,0x44�����ϰ汾ʹ�� */

void ch395_keeplive_idle(uint32_t idle);                                                                /* ����keeplive���� */

void ch395_keeplive_intvl(uint32_t intvl);                                                              /* ����keeplive���ʱ�� */

void ch395_keeplive_cnt(uint8_t cnt);                                                                   /* ����keepalive���Դ��� */

void ch395_set_keeplive(uint8_t sockindex, uint8_t cmd);                                                /* ����ttl */

void ch395_setttl_num(uint8_t sockindex, uint8_t ttlnum);                                               /* ����ttl */

#endif
