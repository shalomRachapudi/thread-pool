#ifndef SYNC_QUEUE_H
#define SYNC_QUEUE_H

#include <queue>
#include <mutex>

template< typename T >
class SyncQueue {
public:
	void enqueue( T& t ) 
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_task_queue.push( t );
	}
	
	bool dequeue( T& t )
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		if ( m_task_queue.empty() )
			return false;
	
		t = std::move( m_task_queue.front() );
		m_task_queue.pop();
	
		return true;
	}
	
	int size( void )
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		return m_task_queue.size();
	}
	
	bool empty( void )
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		return m_task_queue.empty();
	}
	
private:
	std::queue< T > m_task_queue;
	std::mutex m_mutex;
};

#endif