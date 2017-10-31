#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <queue>

class EventChannel;

template <typename EventType>
class EventQueue
{
private :
	friend class EventChannel;

	static EventQueue &Instance()
	{
		static EventQueue<EventType> instance;
		return instance;
	}

	void Queue(EventType *event)
	{
		_eventQueue.push_back(event);
	}

	std::list<EventType *> CheckQueue(float deltaTime)
	{
		std::list<EventType *> result;
		if (_eventQueue.size() > 0)
		{
			std::list<EventType *>::iterator iter = _eventQueue.begin();

			while (iter != _eventQueue.end())
			{
				(*iter)->timeStamp -= deltaTime;
				if ((*iter)->timeStamp < 0.0f)
				{
					result.push_back(*iter);

					if (_eventQueue.size() > 1)
					{
						std::list<EventType *>::iterator next = ++iter;
						_eventQueue.erase(iter);
						iter = next;
					}
					else
					{
						_eventQueue.erase(iter);
						break;
					}
				}
				else
				{
					iter++;
				}
			}

			//for (std::list<EventType *>::iterator iter = _eventQueue.begin(); iter != _eventQueue.end(); ++iter)
			//{
			//}
		}
		return result;
	}

private :
	std::list<EventType *> _eventQueue;

};


#endif