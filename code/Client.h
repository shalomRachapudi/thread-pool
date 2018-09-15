#ifndef CLIENT_H
#define CLIENT_H

#include "async.h"

class Client {
public:

	/**
	 * A constructor for Client
	 * @param num_reg specifies initial number of requests to be generated.
	 * 
	 * Calls Client(unsigned int, unsigned int, unsigned int)
	 */	
	Client( unsigned int num_req ) : Client( 10, 6, num_req ) {}
	
	/**
	 * A constructor for Client
	 * @param num_threads number of threads
	 * @param max_buf_size maximum buffer size
	 * @param num_reg specifies initial number of requests to be generated.
	 */
	Client( unsigned int num_threads, unsigned int max_buf_size, unsigned int num_req );
    
    /**
     * Destructor
     */
    ~Client() { delete m_async; }
	
	/**
	 * Delegates a job to the pool
	 * @param wait_time time in milliseconds
	 */
	void add_work( int wait_time );
	
	/**
	 * This is the callback method that a thread calls when it wakes up from sleep
	 * @param wait_time time in milliseconds
	 * 
	 * The callback method is declared as static to avoid pointer to member function.
	 */
	static void callback( int wait_time );
	
	/**
	 * Prints pool status
	 */
	void pool_status();
	
	/**
	 * Terminates the pool
	 */
	void exit();
	
		
private:
	static ASYNC *m_async; // pool object
	
	void init( const unsigned int num_of_requests ); //initializer
};
#endif
