#ifndef EVENT_BRIDGE_H
#define EVENT_BRIDGE_H

#include "EventHandler.h"

template <class>
class EventHandlerQueue;

template <typename EventType, class HandlerType>
class EventBridge : public EventHandler<EventType>
{
	friend class EventHandlerQueue<EventType>;
public :
	bool operator== (const HandlerType &handler) const;
private :
	EventBridge(HandlerType &handler);
	void Handle(const EventType &object);
	HandlerType &_handler;

};

template<typename EventType, class HandlerType>
inline bool EventBridge<EventType, HandlerType>::operator==(const HandlerType & handler) const
{
	return ((&_handler) == (&handler));
}

template<typename EventType, class HandlerType>
inline EventBridge<EventType, HandlerType>::EventBridge(HandlerType & handler)
	:_handler(handler)
{
}

template<typename EventType, class HandlerType>
inline void EventBridge<EventType, HandlerType>::Handle(const EventType & object)
{
	_handler.Handle(object);
}

#endif
