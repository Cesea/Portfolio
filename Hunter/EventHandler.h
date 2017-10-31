#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H


template <class> 
class EventHandlerQueue;

template <class EventType>
class EventHandler
{
private :
	friend class EventHandlerQueue<EventType>;

	virtual void Handle(const EventType &object) = 0;
};

#endif
