#ifndef _THEFOX_NET_MSGQUEUE_MQMANAGER_H_
#define _THEFOX_NET_MSGQUEUE_MQMANAGER_H_

#include <map>
#include <base/MutexLock.h>
#include <net/msgqueue/QueueTuple.h>

namespace thefox
{

class MqManager
{
public:
	MqManager()
	{}
	~MqManager()
	{}

	void pushMsg(const TcpConnectionPtr &sender, gpb::Message *msg)
	{
		MutexLockGuard lock(_mutex);
		QueueTupleMap::iterator it = _queues.find(queueName);
		if (it != _queues.end()) {
			it->second->push(sender, msg);
		} else {
			QueueTuplePtr queue(new QueueTuple(queueName));
			queue->push(sender, msg);
			_queues[queueName] = queue;
		}
	}

	MsgBoxPtr popMsg()
	{
		MutexLockGuard lock(_mutex);
		QueueTupleMap::iterator it = _queues.find(queueName);
		if (it != _queues.end()) {
			return it->second->pop();
		} else {
			return NULL;
		}
	}
	
private:
	THEFOX_DISALLOW_EVIL_CONSTRUCTORS(MqManager);
	QueueTuple _messages;
	Thread

};

typedef std::shared_ptr<MqManager> MqManagerPtr;

}// namespace thefox

#endif // _THEFOX_NET_MSGQUEUE_MQMANAGER_H_
