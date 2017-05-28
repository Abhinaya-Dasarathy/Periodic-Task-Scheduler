/*
 * helloWorld.cpp
 *
 *  Created on: May 28, 2017
 *      Author: abhi2
 */

#include "task.hpp"


using namespace pts::task;


class PrintTask : public PersistTask {
    public:

        PrintTask() : PersistTask("printtask") {};
        // run methoed is executed by the scheduler every fixed
        // interval, the return value is logged to the datastore
        // along with a timestamp
        float run() {
           printf("hello world");
            return 0;
        }

        ~PrintTask(){}
};

// register to run this module at every 10 seconds;
REGISTER(PrintTask,10);



