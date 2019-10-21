/*****************************************************************************
ģ����      : SNMP����ģ��
�ļ���      : snmpadp_event.h
����ļ�    : snmpadp_event.cpp
�ļ�ʵ�ֹ���: 
����        : ��  ��
�汾        : V0.1  Copyright(C) 2005-2006 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2005/12/22  0.1         ��  ��      ����
******************************************************************************/ 
#ifndef _SNMPADP_EVENT_H_
#define _SNMPADP_EVENT_H_

#define EV_PRINT_INST_STATUS        1000

//�������������Ϣ
#define EV_SUBAGENT_POWERON         1001

//�豸��ע����Ϣ
#define EV_AGENT_REG_REQ            1100
//�豸��ע��Ӧ��
#define EV_AGENT_REG_ACK            1101
//�豸��ע���Ӧ��
#define EV_AGENT_REG_NACK           1102

//
#define EV_CALLAGENT_REQ            1104
//
#define EV_CALLAGENT_ACK           1105
//
#define EV_CALLAGENT_NACK          1106
//
#define EV_CALLAGENTCB_NTF         1107

//
#define EV_SETHBTIME_CMD           1108


#define EV_SETMODULE_HB_NTF         1109

//
#define EV_AGENTSTATUS_REQ          1115
//
#define EV_AGENTSTATUS_ACK          1116
//
#define EV_AGENTSTATUS_NACK         1117


//
#define EV_SNMPADP_POWERON         1120
//
#define CONNECT_MGR_TIMER          1121
//
#define REGISTER_TIMER             1122
//
#define EV_3AS_SNMPADP_INIT         1130


#define EV_MANAGER_AGENT_REQ        1295
#define EV_AGENT_MANAGER_ACK        1296
#define EV_AGENT_MANAGER_NACK       1297
#define EV_AGENT_MANAGER_NTF        1298

//�豸����֪ͨ
#define EV_AGENT_DISCONNECTED_NTF   2000

//��ӡnormal��Initʵ����ϸ��Ϣ
#define EV_PRINT_INST_ALL_STATUS	2001

//ceshiyong
#define EV_PRINT_IDLE_NUM			2003

#endif //_SNMPADP_EVENT_H_
