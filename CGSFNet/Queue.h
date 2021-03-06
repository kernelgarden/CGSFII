#pragma once
#include <ace/Containers.h>
#include <assert.h>

#define INVALID_ID  0xffffffff
template<class T>
class Queue
{
public:
	void Push(T t)
	{
		m_Queue.enqueue_tail(t);
	}

/////////////////////////////////////////
// Description:
//   when the queue is empty, return false
/////////////////////////////////////////
	bool Pop(T& t)
	{
		return (-1 != m_Queue.dequeue_head(t));
	}

	bool Head(T*& t)
	{
		return (-1 != m_Queue.get(t));
	}

	int Size()
	{
		return m_Queue.size();
	}

protected:
	ACE_Unbounded_Queue<T>	m_Queue;
};

/////////////////////////
// Description:
//   Thread Safe Queue
////////////////////////
template<class T>
class TSQueueT
{
public:
	void Push(T t)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(_lock);
		_queue.enqueue_tail(t);
	}

	/////////////////////////////////////////
	// Description:
	//   when the queue is empty, return false
	/////////////////////////////////////////
	bool Pop(T& t)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(_lock);
		return (-1 != _queue.dequeue_head(t));
	}

	bool Head(T*& t)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(_lock);
		return (-1 != _queue.get(t));
	}

	int Size()
	{
		ACE_Guard<ACE_Thread_Mutex> guard(_lock);
		return _queue.size();
	}

protected:
	ACE_Unbounded_Queue<T>	_queue;
	ACE_Thread_Mutex				_lock;
};

class IDQueue
{
public:	
	void Init(int maxIdCount, int offset)
	{
		m_maxIdCount = maxIdCount;
		m_offset = offset;

		for (int i = 0; i < m_maxIdCount; ++i)
			m_idleIdQueue.Push(i + m_offset);
	}

	inline int Pop()
	{
		int id = INVALID_ID;
		m_idleIdQueue.Pop(id);
		
		return id;
	}


	inline void Push(int id)
	{
		if (IsValidId(id))
		{
			m_idleIdQueue.Push(id);
		}
		else
		{
			assert(0);
		}
	}

	inline bool IsValidId(int id) const
	{
		return (INVALID_ID != id) && (id >= m_offset) && (id < (m_maxIdCount + m_offset));
	}

private:
	int				m_offset;
	int				m_maxIdCount;
	TSQueueT<int>	m_idleIdQueue;
};