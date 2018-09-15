#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include "async.h"
class WorkerThread {
public:
	WorkerThread( ASYNC *pool, const int id, bool status )
		: m_thread_pool ( pool ), m_id ( id ), m_status( status )
	{}
	
	void operator()() 
	{
		std::function< void() > f;
		bool dequeued = false;
				
		while( !m_thread_pool->m_quit ) {
			{ //making critical section as small as possible. Hence, a new scope.
				std::unique_lock< std::mutex > lock( m_thread_pool->m_cond_mutex );
				if ( m_thread_pool->m_queue.empty() )
					m_thread_pool->m_cond_lock.wait( lock );
					
				dequeued = m_thread_pool->m_queue.dequeue( f );
			}
			
			if ( dequeued ) {
				
				// update thread status to working (true)
				m_thread_pool->m_pool_state[ m_id ] = true;
				f();
			}
		}
		
		// update status to false. Thread is free.
		m_thread_pool->m_pool_state[ m_id ] = false;
	}
private:
	ASYNC *m_thread_pool;
	int m_id;
	bool m_status;
};

#endif