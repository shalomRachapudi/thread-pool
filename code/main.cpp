#include <iostream>
#include <cstdio>

#include "Client.h"

int main( int argc, char **argv )
{
	Client c( 15 );
	c.add_work( 400 );
	
	getchar(); // wait
	c.exit(); // exit
	return 0;
}
