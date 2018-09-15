#ifndef ASYNC_H
#define ASYNC_H

#include <vector>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <future>
#include <utility>

#include "sync_queue.h"

class ASYNC {
public:

	/**
	 * Constructor for ASYNC class. It initializes properties and calls init() to spawn threads.
	 * @param thread_pool_size maximum number of threads that can be created by the ASYNC class
	 * @param maximum number of request ASYNC object can handle before the incoming request gets dropped
	 */ 
	ASYNC( unsigned int thread_pool_size, unsigned int request_buffer_size )
	{
		/* initialize properties */
		m_quit = false;
		m_request_processed = 0;
		m_request_dropped = 0;
		m_pool_state.resize( thread_pool_size );
		m_thread_pool_size = thread_pool_size;
		m_request_buffer_size = request_buffer_size;
		m_threads.resize( thread_pool_size );
		
		init(); //spawn threads
	}
	
	/**
	 * async_wait creates a packaged_task and enqueues it to the task queue.
	 * @param sleep_time_in_ms number of milliseconds a thread should sleep before calling the callback
	 * @param void (*)(int) function pointer to the callback function with wait_time as its argument 
	 * 
	 * @return the return type is of type std::future.
	 */ 
    auto async_wait( int sleep_time_in_ms, void ( *callback )( int ) ) 
		-> std::future< decltype( callback( sleep_time_in_ms ) ) >;
	
	/**
	 * Returns the status of the pool.
	 */
	std::vector< bool > pool_state();
	
	/**
	 * Terminates the pool 'graciously'. It waits for every thread to complete its execution.
	 */
    void exit();
	
public: 
	/**
	 * TODO: getter or setters. Or better, make worker_thread an inner class of ASYNC and public properties as private.
	 */
	bool m_quit; // flag to end the thread pool
	unsigned int m_request_processed;
	unsigned int m_request_dropped;
	std::vector< bool > m_pool_state; // current pool state
	
	std::mutex m_cond_mutex;
	std::condition_variable m_cond_lock;
	std::vector< std::thread > m_threads; // thread pool
	SyncQueue< std::function< void() > > m_queue; // task queue or buffer
	
	

private: /* private properties and methods */
	unsigned int m_thread_pool_size;
	unsigned int m_request_buffer_size;	
	
	/**
	 * Spawn threads / creates a thread pool
	 */
	void init();
};

#endif