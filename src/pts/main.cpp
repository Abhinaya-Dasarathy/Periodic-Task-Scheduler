/*
 * main.cpp
 *
 *  Created on: May 27, 2017
 *      Author: abhi2
 */

#include "scheduler.hpp"
#include <vector>
#include <iostream>

using namespace pts;

int main(int argc, char **argv) {
    Scheduler sched;
    task::TaskFactory &factory = task::TaskFactory::Get();
    std::vector<std::string> taskNames = factory.GetTaskNames();

    for (auto &name : taskNames) {
        task::TaskInterface *module = factory.GetTask(name);
        unsigned int interval = factory.GetTaskInterval(name);

        sched.runEvery(module, std::chrono::seconds(interval));
    }
    sched.start();
    return 0;
}
