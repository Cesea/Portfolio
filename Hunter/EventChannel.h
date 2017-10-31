#ifndef EVENT_CHANNEL_H
#define EVENT_CHANNEL_H

#include "EventHandlerQueue.h"
#include "EventQueue.h"

class EventChannel
{
public :
	template <typename EventType, class HandlerType>
	void Add(HandlerType &handler)
	{
		EventHandlerQueue<EventType>::Instance().Add(handler);
	}

	template <typename EventType, class HandlerType>
	void Remove(HandlerType &handler)
	{
		EventHandlerQueue<EventType>::Instance().Remove(handler);
	}

	template <typename	EventType>
	void Broadcast(const EventType &object)
	{
		EventHandlerQueue<EventType>::Instance().Broadcast(object);
	}

	template <typename EventType>
	void Queue(EventType *pEvent)
	{
		EventQueue<EventType>::Instance().Queue(pEvent);
	}

	template <typename EventType>
	void Update(float deltaTime)
	{
		std::list<EventType *> toHandleList = EventQueue<EventType>::Instance().CheckQueue(deltaTime);
		if (toHandleList.size() > 0)
		{
			for (std::list<EventType *>::iterator iter = toHandleList.begin(); iter != toHandleList.end(); ++iter)
			{
				Broadcast<EventType>(**iter);
				delete *iter;
			}
			toHandleList.clear();
		}
	}

	//template <typename EventType>
	//void Queue(const EventType &objet)
	//{
	//	EventHandlerQueue<EventType>::Instance().Queue(object);
	//}
	//void StepForward(float deltaTime)
	//{
	//}
	//template <typename EventType>
	//void Queue(const EventType &objet)
	//{
	//}
protected :

};


#endif