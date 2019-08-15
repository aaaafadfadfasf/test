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
   *状态转换规则的key
   *以state和event做关键字.
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
   *状态转换规则
   *参数是:当前状态,输入事件,下一状态,执行动作,状态转换条件.
   *不填状态转换条件时,只要收到事件则作无条件转换.否则只在满足条件时才转换状态,执行动作.
   *例如一次投游戏币5个,那么条件是5个币用完了才能真正GameOver.
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
*表驱动的状态机
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
       *增加转换规则.
       *@param wCurrentState 当前状态
       *@param wEvent 输入事件
       *@param wNextState 切换到下一状态
       *@param pAction 执行此动作
       *@param pCondition 只在pCondition条件满足时才做状态切换,才执行 pAction.\n
       *一次投游戏币5个,那么条件是5个币用完了才能真正GameOver.
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
       *处理事件.内部进行状态切换.
       *
       *@return 无法处理时返回false,说明当前状态不能处理该消息.
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
                       {//不需要切换状态,直接返回                           
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

