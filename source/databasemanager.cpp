//Everest Ashley SID: 0457240
// REFACTOR Brandon Cotesta 10/20/2025
#include "databasemanager.h"
#include <iostream>

databasemanager::databasemanager()
{
    sql::mysql::MySQL_Driver* driver;
    stmt = nullptr;
    connection = nullptr;

    try {
        driver = sql::mysql::get_driver_instance();
        connection = driver->connect("tcp://127.0.0.1:3306", "root", "keypick1");
        connection->setSchema("bankdatabase");
        stmt = connection->createStatement();
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
}

void databasemanager::addtoTable(std::string tab, std::string val)
{
    std::string statement = insert + tab + " " + values + val;
    //create statement to execute

    stmt->execute(statement); 
    //execute query - use execute() for INSERT statements
}

std::string databasemanager::retString(std::string col, std::string tab, std::string specval)
{
    std::string tempstring; 
    //string that will be returned;
  
    statement = select + col + " " + from + tab; 
    //uses the statement variable to make a string statement for execution
  
    sql::ResultSet* res = stmt->executeQuery(statement); 
    //executes the statement assigns value to res (result set)
  
    if (res->next()) {
        tempstring = res->getString(specval); 
        //searches returned statement for the specific value wanted.
    }

    delete res;  //clean up result set only
    // Don't delete stmt - it's a class member
  
    return tempstring;
}

std::string databasemanager::retStringW(std::string col, std::string tab, std::string val, std::string specval)
{ //select statement with where 
    std::string tempstring; 
    //same comments and functions as retString
    statement = select + col + " " + from + tab + " " + where + val; 
    //except here it uses the WHERE statement to give a more specific search
  
    sql::ResultSet* res = stmt->executeQuery(statement);
    
    if (res->next()) {
        tempstring = res->getString(specval);
    }

    delete res;  //clean up result set only
    // Don't delete stmt - it's a class member
  
    return tempstring;
}

// tab = table, setv = set values, cond = condition
void databasemanager::updateTable(std::string tab, std::string setv, std::string cond)
{
    statement = update + tab + " " + set + setv + " " + where + cond; 
    //create statement for execution with UPDATE, SET, and WHERE
    //example UPDATE userinfo SET email = 'testemail' name = 'testname' WHERE userID = 1;
    //will update values for userID 1
    
    stmt->execute(statement);
    //Executes statement - use execute() for UPDATE statements
    //no return needed
}

databasemanager::~databasemanager()
{
    if (stmt) {
        delete stmt;
        stmt = nullptr;
    }
    if (connection) {
        delete connection;
        connection = nullptr;
    }
}
