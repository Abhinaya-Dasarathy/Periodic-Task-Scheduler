/*
 * sqlite.cpp
 *
 *  Created on: May 27, 2017
 *      Author: abhi2
 */

#include "sqlite.hpp"
#include <iostream>
//#include <sqlite3.h>

using namespace pts::sqlite;



bool Sqlite3Datastore::connect() {

	  int rc = sqlite3_open(dbname.c_str(), &db );
    if (rc) {
        sqlite3_close(db);
        return false;
    }
    return true;
}

bool Sqlite3Datastore::disconnect() {
    sqlite3_close(db);
    return true;
}

bool Sqlite3Datastore::do_query(std::string query) {
    char *err;
    int rc = sqlite3_exec(db, query.c_str(), NULL, 0, &err);
    if (rc != SQLITE_OK) {
    	throw "do_query failed";
        //dblogger->error("do_query failed with error: " + std::string(err));
        sqlite3_free(err);
        return false;
    }
    return true;
}

DatastoreInterface* DatastoreFactory::getDatastore() {
    if (_backend == "sqlite3") {
        return new Sqlite3Datastore("datastore.db");
    } else {
        throw "db driver not implemented";
    }
    return nullptr;
}



