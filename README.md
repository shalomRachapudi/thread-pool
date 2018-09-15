Thread Pool in C++
==================

My very own (bare-bones) implementation of thread pool written from scratch to understand the intricacies involved in thread scheduling. This implementation uses std::thread library. The implementation executes any number of sleep requests asynchronously. Every request to sleep is also accompanied by a callback function which is invoked when the sleep time is over.

Note: Although the `callback` function is defined in the source code, you can change it as per your requirement.

Getting the Source Code
=======================

To get the source code, clone the GitHub repository:

    $ git clone https://github.com/shalomRachapudi/thread-pool.git

Steps to run the Thread-pool
============================

    $ ./compile_and_run.sh
