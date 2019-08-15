/*
* event_msgqueue.h
*
* Libevent threaded message passing primitives header
*
* Andrew Danforth <acd@weirdness.net>, October 2006
*
* Copyright (c) Andrew Danforth, 2006
*
*/

/**
*  用做线程的消息队列. 依赖 libevent.
*/

#ifndef _EVENT_MSGQUEUE_H_
#define _EVENT_MSGQUEUE_H_

struct event_base;

namespace LibEvent{namespace Concurrency{
    
    
    struct event_msgqueue;
    
    class CQueueMsg;
    
    event_msgqueue* msgqueue_new(event_base *base,
        unsigned int max_size,
        void (*callback)(const CQueueMsg& msg, void * pContext),
        void *cbarg);
    
    int msgqueue_push(event_msgqueue *, const CQueueMsg& msg);
    unsigned int msgqueue_length(event_msgqueue *);
    void msgqueue_destroy(event_msgqueue*);
    
}}
#endif


//end of file


