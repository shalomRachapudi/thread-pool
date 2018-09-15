#include <chrono>
#include <iostream>

#include "async.h"
#include "worker_thread.h"

void ASYNC::init() 
{
	for ( size_t i = 0; i < m_threads.size(); i++ )
		m_threads[ i ] = std::thread( WorkerThread( this, i, false ) );
}

auto ASYNC::async_wait( const int sleep_time_in_ms, void ( *callback )( int ) ) 
	-> std::future< decltype( callback( sleep_time_in_ms ) ) >
{
	// if the buffer is full, drop the incoming requests.
	if ( (unsigned) m_queue.size() == m_request_buffer_size ) {
		++m_request_dropped;
		return std::future< void >();
	}
	
	// create job for a thread - sleeps for given milliseconds, and then call the callback function / method
	std::function< void() > func = [ sleep_time_in_ms, callback ]() {
		std::this_thread::sleep_for( std::chrono::milliseconds( sleep_time_in_ms ) );
		callback( sleep_time_in_ms );
	};
	
	// wrap (packaged_task) the created job/func so that it can be invoked asynchronously.
	auto task = std::make_shared< std::packaged_task< decltype( callback( sleep_time_in_ms ) )() > >( func );
	
	// create a void function 
	std::function< void() > wrapper = [ task ]() {
		( *task )();
	};
		
	// add it to the task queue
	m_queue.enqueue( wrapper );
	
	// assign it to a thread if it is waiting
	m_cond_lock.notify_one();
	
	// return from future to avoid blocking of main thread
	// in this case the return type is std::future<void>.
	return task->get_future();
}

std::vector< bool > ASYNC::pool_state() 
{
	return m_pool_state;
}

void ASYNC::exit()
{
	m_quit = true;
	m_cond_lock.notify_all();
	
	for( size_t i = 0; i < m_threads.size(); i++ ) {
		if ( m_threads[ i ].joinable() )
			m_threads[ i ].join();
	}
}