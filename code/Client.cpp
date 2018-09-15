#include <iostream>
#include <random>
#include "Client.h"

ASYNC *Client::m_async = nullptr; 

Client::Client( unsigned int num_threads, unsigned int max_buf_size, unsigned int num_req )
{ 
	m_async = new ASYNC( num_threads, max_buf_size );
	init( num_req ); 
}

void Client::init( const unsigned int num_of_requests )
{
	/* 
	 * The random number generator code is taken from  
	 * https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
	 */
	std::mt19937 rng;
	rng.seed( std::random_device()() );
    std::uniform_int_distribution< std::mt19937::result_type > dist( 100, 50000 );
	
	for ( size_t i = 0; i < num_of_requests; i++ ) {
		auto ms = dist( rng );
		add_work( ms );
	}
}

void Client::add_work( int wait_time ) 
{
	m_async->async_wait( wait_time, &callback );
}


void Client::callback( int wait_time ) 
{
	std::cout << "The process waited for :: " << wait_time << "\t"
			  << "Requests Processed :: " << ++m_async->m_request_processed << "\t"
			  << "Requests Dropped :: " << m_async->m_request_dropped << std::endl;
}

void Client::pool_status()
{
	std::vector< bool > status = m_async->pool_state();
	for ( size_t i = 0; i < status.size(); i++ ) 
		std::cout << std::boolalpha << status[ i ] << " ";
	
	std::cout << std::endl;
}

void Client::exit() {
	m_async->exit();
}
