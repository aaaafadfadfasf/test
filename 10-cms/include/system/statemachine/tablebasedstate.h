#ifndef TABLE_BASED_STATE_H
#define TABLE_BASED_STATE_H

#ifdef _WIN32
#pragma warning (disable : 4786) 
#endif

#include <map>
#include <cassert>

class CStateResult
{
public:
    CStateResult() : m_bIsStateChanged(0),m_bIsEventDealed(0)
    {
        
    }
public:
    bool m_bIsStateChanged;
    bool m_bIsEventDealed;
};

namespace PrivateClasses
{
   inline unsigned long MakeLong(unsigned short wLoWord,unsigned short wHiWord)   
   {   
       return (unsigned long)((wHiWord << 16) | (wLoWord &0xffff));   
   }  
   /**
   *״̬ת�������key
   *��state��event���ؼ���.
   */
   class CTransitionRuleKey
   {
   public:
       CTransitionRuleKey(unsigned short wCurrentState,unsigned short wEvent):m_wCurrentState(wCurrentState),m_wEvent(wEvent)
       {
       }
       bool operator < (const CTransitionRuleKey& anotherKey)const
       {
           return MakeLong(m_wEvent,m_wCurrentState) < MakeLong(anotherKey.m_wEvent,anotherKey.m_wCurrentState);
       }
       bool operator == (const CTransitionRuleKey& anotherKey)const
       {
           return MakeLong(m_wEvent,m_wCurrentState) == MakeLong(anotherKey.m_wEvent,anotherKey.m_wCurrentState);
       }
   private:
       unsigned short m_wCurrentState;
       unsigned short m_wEvent;
   };
   
   /**
   *״̬ת������
   *������:��ǰ״̬,�����¼�,��һ״̬,ִ�ж���,״̬ת������.
   *����״̬ת������ʱ,ֻҪ�յ��¼�����������ת��.����ֻ����������ʱ��ת��״̬,ִ�ж���.
   *����һ��Ͷ��Ϸ��5��,��ô������5���������˲�������GameOver.
   */
   template <class ActionClass,class Parameters>
       class CTransitionRule
   {
   public:
       typedef void (ActionClass::*PAction)(Parameters&);
       typedef bool (ActionClass::*PCondition)(Parameters&);

       CTransitionRule(unsigned short wCurrentState,
           unsigned short wEvent,
           unsigned short wNextState,
           PAction pAction,
           PCondition pCondition = 0)
           : m_wCurrentState(wCurrentState),
           m_wEvent(wEvent),
           m_wNextState(wNextState),
           m_pAction(pAction),
           m_pCondition(pCondition)
       {}
       unsigned short m_wCurrentState;
       unsigned short m_wEvent;
       unsigned short m_wNextState;
       PAction m_pAction;
       PCondition m_pCondition;
   };
}

/**
*��������״̬��
*/
template <class ActionClass,class Parameters>
class CTableBasedState
{
   public:       
       typedef typename PrivateClasses::CTransitionRule<ActionClass,Parameters>::PAction PAction;
       typedef typename PrivateClasses::CTransitionRule<ActionClass,Parameters>::PCondition PCondition;

       CTableBasedState() : m_wCurrentState(0)
       {           
       }
       
       /**
       *����ת������.
       *@param wCurrentState ��ǰ״̬
       *@param wEvent �����¼�
       *@param wNextState �л�����һ״̬
       *@param pAction ִ�д˶���
       *@param pCondition ֻ��pCondition��������ʱ����״̬�л�,��ִ�� pAction.\n
       *һ��Ͷ��Ϸ��5��,��ô������5���������˲�������GameOver.
       */
       void AddRule(unsigned short wCurrentState,unsigned short wEvent,unsigned short wNextState,PAction pAction = 0,PCondition pCondition = 0)
       {
           using PrivateClasses::CTransitionRuleKey;
           using PrivateClasses::CTransitionRule;
           bool bInsert = m_transtitionRuleTable.insert(
               std::make_pair(CTransitionRuleKey(wCurrentState,wEvent),CTransitionRule<ActionClass,Parameters>(wCurrentState,wEvent,wNextState,pAction,pCondition))
               ).second;
           
           if(!bInsert)
           {
               assert(0 && "don't insert duplicated CTransitionRule\n");
           }
       }
       
       /**
       *�����¼�.�ڲ�����״̬�л�.
       *
       *@return �޷�����ʱ����false,˵����ǰ״̬���ܴ������Ϣ.
       */
       CStateResult DealEvent(ActionClass* pActionObj,unsigned short wEvent,Parameters& parameters)
       {
           CStateResult result;
           result.m_bIsEventDealed = false;
           result.m_bIsStateChanged = false;

           using PrivateClasses::CTransitionRuleKey;
           using PrivateClasses::CTransitionRule;
           typename TransitionRuleTable::iterator iter = m_transtitionRuleTable.find(CTransitionRuleKey(m_wCurrentState,wEvent));
           if(iter != m_transtitionRuleTable.end())
           {
               result.m_bIsEventDealed = true;
               const CTransitionRule<ActionClass,Parameters>& transitionRule = iter->second;
               assert(pActionObj && "you must set action object\n");
               if(pActionObj)
               {                   
                   if(transitionRule.m_pCondition && transitionRule.m_pCondition != NULL)
                   {
                       bool bChangeState = (pActionObj->*transitionRule.m_pCondition)(parameters);
                       if(!bChangeState)
                       {//����Ҫ�л�״̬,ֱ�ӷ���                           
                           result.m_bIsStateChanged = false;
                           return result;
                       }
                   }
                   result.m_bIsStateChanged = m_wCurrentState != transitionRule.m_wNextState;
                   m_wCurrentState = transitionRule.m_wNextState;
                   if (transitionRule.m_pAction!= NULL)
                   {   
                       (pActionObj->*transitionRule.m_pAction)(parameters);
                   }                   
               }               
               return result;
           }
           
           return result;
       }
       void SetCurrentState(unsigned short wState)
       {
           m_wCurrentState = wState;
       }
       unsigned short GetCurrentState()const
       {
           return m_wCurrentState;
       }
   private:
       typedef std::map<PrivateClasses::CTransitionRuleKey,PrivateClasses::CTransitionRule<ActionClass,Parameters> > TransitionRuleTable;
       TransitionRuleTable m_transtitionRuleTable;
       unsigned short m_wCurrentState;
};

#endif


//end of file

