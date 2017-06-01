/*
 * Logs the memory of this application to the datastore
 */
#include <fstream>
#include "task.hpp"
#include <sstream>

using namespace pts::task;

class MemoryTask : public PersistTask {
    public:
        MemoryTask() : PersistTask("memtask") {};
        float run() {
            std::ifstream statf("/proc/self/statm");
            if (statf.is_open()) {
                std::string line;
                std::getline(statf, line);
                std::istringstream memline(line);
                int mem;

                memline >> mem;
		
              
                statf.close();
                return mem;
            }
            throw "unable to open file";
            return -1;
        }
        
        ~MemoryTask(){}
};

REGISTER(MemoryTask,30);
