//Everest Ashley SID: 0457240
#include "databasemanager.h"
#include <iostream>


databasemanager::databasemanager():
{
  //establish connection 
  sql::mysql::MySQL_Driver* driver;
  sql::Connection* con;

try {
    int tempint = 0;
    driver = sql::mysql::get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "keypick1"); // connect to driver instance
    con->setSchema("bankdatabase"); //Database that holds all tables
    }
  catch (sql::SQLException &e){ //catch for errors
    std::cerr << "SQL Error: " << e.what() << std::endl;
}
}
