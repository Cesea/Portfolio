#ifndef EVENT_HANDLER_QUEUE_H
#define EVENT_HANDLER_QUEUE_H

#include "EventBridge.h"

#include <list>
#include <queue>
#include <memory>

class EventChannel;

template <typename EventType>
class EventHandlerQueue
{
private :
	friend class EventChannel;

	typedef EventHandler<EventType> EventHandlerType;

	typedef std::shared_ptr<EventHandlerType> EventHandlerPtr;

	typedef std::list<EventHandlerPtr> HandlerList;
	typedef typename HandlerList::iterator HandlerIterator;
	typedef typename HandlerList::const_iterator ConstHandlerIterator;

	HandlerList _handlerList;

	static EventHandlerQueue &Instance()
	{
		static EventHandlerQueue<EventType> instance;
		return instance;
	}

	template <class HandlerType>
	void Add(HandlerType &handler)
	{
		bool handlerExist = false;
		for (HandlerIterator iter = _handlerList.begin(); iter != _handlerList.end(); ++iter)
		{
			PointSame<HandlerType> pointToSame(handler);
			if (pointToSame(*iter))
			{
				handlerExist = true;
			}
		}
		if (!handlerExist)
		{
			_handlerList.push_back(EventHandlerPtr(new EventBridge<EventType, HandlerType>(handler)));
		}
	}

	void Broadcast(const EventType &object)
	{
		ConstHandlerIterator constIter;
		for (ConstHandlerIterator iter = _handlerList.begin(); iter != _handlerList.end();)
		{		
			constIter = iter++;
			(*constIter)->Handle(object);
		}
	}

	template <class HandlerType>
	class PointSame
	{
	public :
		typedef EventBridge<EventType, HandlerType> BridgeType;

		const HandlerType &handler;

		PointSame(const HandlerType &handler)
			:handler(handler)
		{
		}

		bool operator()(EventHandlerPtr ptr)
		{
			return ((*std::static_pointer_cast<BridgeType>(ptr)) == handler);
		}
	};

	template <class HandlerType>
	void Remove(const HandlerType &handler)
	{
		PointSame<HandlerType> ptr(handler);
		_handlerList.remove_if(ptr);
	}

};

#endif