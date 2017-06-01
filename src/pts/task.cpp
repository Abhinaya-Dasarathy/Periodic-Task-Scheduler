

/*
 * task.cpp
 *
 *  Created on: May 27, 2017
 *      Author: abhi2
 */

#include "task.hpp"
#include <chrono>
#include <ctime>

using namespace pts::task;

TaskFactory& TaskFactory::Get() {
    static TaskFactory factory;
    return factory;
}

void TaskFactory::Register(TaskRegistrarInterface *registrar,
        std::string name, unsigned int sec) {
    registry[name] = std::make_pair(registrar, sec);
}

TaskInterface* TaskFactory::GetTask(std::string name) {
    TaskRegistrarInterface* registrar;
    auto entry = registry.find(name);
    if (entry == registry.end()) {
        throw std::out_of_range("Task Not Foud");
    }
    return entry->second.first->GetTask();
}

unsigned int TaskFactory::GetTaskInterval(std::string name) {
    TaskRegistrarInterface* registrar;
    auto entry = registry.find(name);
    if (entry == registry.end()) {
        throw std::out_of_range("Task Not Foud");
    }
    return entry->second.second;
}

std::vector<std::string> TaskFactory::GetTaskNames() {
    std::vector<std::string> vs;
    for (auto &mod : registry) {
        vs.push_back(mod.first);
    }
    return vs;
}

void PersistTask::persist(float& data) {
    if (!db->do_query("INSERT INTO " + store + " VALUES (" +
                std::to_string(data) + "," + "datetime('now')" + ")")) {
        throw "failed to insert in store";
    }
}



