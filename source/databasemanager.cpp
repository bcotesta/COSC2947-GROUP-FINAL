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
    con = driver->connect("tcp://127.0.0.1:3306", "root", "keypick1"); 
  // connect to driver instance
  
    con->setSchema("bankdatabase"); 
   //Database that holds all tables
    }
  catch (sql::SQLException &e){ //catch for errors
    std::cerr << "SQL Error: " << e.what() << std::endl;
}

  void databasemanager::addtoTable(string tab, string val)
  {
    string statement = this.insert + t + " " + this.values + val; 
    //create statement to execute
    
     sql::ResultSet* res = stmt->executeQuery(statement); 
    //execute query 
    
  }

string databasemanager::retString(string col, string tab, string specval)
{
  std::string tempstring; 
  //string that will be returned;
  
  this.statement = this.select + col + " " + this.from + t; 
  //uses the statement variable to make a string statement for execution
  
  sql::ResultSet* res = stmt->executeQuery(this.statement); 
  //executes the statement assigns value to res (result set)
  
  tempstring = res->getstring(specval); 
  //searches returned statement for the specific value wanted.

  delete res;  //clean up otherwise it can cause some issues with multiple res and stmt
  delete stmt;
  
  return tempstring;
}

string databasemanager::retStringW(string col, string tab, string val, string specval)
{ //select statement with where 
  std::string tempstring; 
  //same comments and functions as retString
  this.statement = this.select + col + " " + this.from + tab + " " + this.where + val ; 
  //except here it uses the WHERE statement to give a more specific search
  
  sql::ResultSet* res = stmt->executeQuery(this.statement);
  tempstring = res->getstring(specval);

  delete res;  //clean up 
  delete stmt;
  
  return tempstring;
}

void databasemanager::UpdateTable(string tab, string setv, string cond)
{
  this.statement = this.update + tab + " " + this.set + setv + " " + this.where + cond; 
  //create statement for execution with UPDATE, SET, and WHERE
  //example UPDATE userinfo SET email = 'testemail' name = 'testname' WHERE userID = 1;
  //will update values for userID 1
  sql::ResultSet* res = stmt->executeQuery(this.statement);
  //Executes statement
  //no return needed
}
