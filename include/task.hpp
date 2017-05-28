/*
 * task.hpp
 *
 *  Created on: May 27, 2017
 *      Author: abhi
 */

#ifndef TASK_HPP_
#define TASK_HPP_


#include "sqlite.hpp"
#include <string>
#include <vector>
#include <map>

#pragma once

namespace pts {
namespace task {

class TaskInterface {
    public:
        virtual float run() = 0;
        virtual ~TaskInterface() {};
        virtual void persist(float &data) {};
};

class PersistTask : public TaskInterface {
    public:
        PersistTask(std::string name) : store(name) {
            pts::sqlite::DatastoreFactory factory("sqlite3");
            db = factory.getDatastore();
            if (!db->connect()) {
                throw "failed to connect to the database";
            }
            if (!db->do_query("CREATE TABLE IF NOT EXISTS " + store +
                    " (val REAL, "
                    " Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);")) {
                throw "failed to create table";
            }
        }
        ~PersistTask() {
            db->disconnect();
            delete db;
        }
        void persist(float& data);
    private:
        std::string store;
        pts::sqlite::DatastoreInterface *db;
};



class TaskRegistrarInterface {
    public:
        virtual TaskInterface* GetTask() = 0;
        virtual ~TaskRegistrarInterface() {};
};

class TaskFactory {
    public:

        static TaskFactory& Get();

        void Register(TaskRegistrarInterface *registrar, std::string name,
                        unsigned int sec);
        TaskInterface* GetTask(std::string name);
        unsigned int GetTaskInterval(std::string name);
        std::vector<std::string> GetTaskNames();

        TaskFactory(TaskFactory const&) = delete;
        void operator=(TaskFactory const&) = delete;

    private:
        TaskFactory() {}
        std::map<std::string, std::pair<TaskRegistrarInterface*, int>
            > registry;
};

template<class T>
class TaskRegistrar : public TaskRegistrarInterface {
    public:
        TaskRegistrar(std::string classname, unsigned int sec);

        TaskInterface* GetTask();
};


template<class T>
TaskRegistrar<T>::TaskRegistrar(std::string classname, unsigned int sec) {
    TaskFactory &factory = TaskFactory::Get();
    factory.Register(this, classname, sec);
}

template<class T>
TaskInterface* TaskRegistrar<T>::GetTask() {
    return new T();
}

}
}

#define REGISTER(CLASSNAME,sec) \
    namespace { \
        static pts::task::TaskRegistrar<CLASSNAME> \
        CLASSNAME##_registrar(#CLASSNAME,sec); \
    };



#endif /* TASK_HPP_ */
