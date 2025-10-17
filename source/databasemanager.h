//Everest Ashley SID: 0457240

#include <iostream>
#include <mysqlx/xdevapi.h>
#include <mysql/jdbc.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <string>


using ::std::cout;
using ::std::endl;
using namespace ::mysqlx;


class databasemanager
{
public:
  databasemanager(); //connect to database


//functions
  void addtoTable(string tab, string val);
  // (t) table to add a row to, (v) values to insert in ('value1','value2') format 
  // or "('"+ name + "', '" + email + "')" format with variables

  string retString(string col, string tab, string specval); 
  // select statement without where statement
  
  string retStringW(string col, string tab, string val, string specval); 
  // (c)column/s to select, (t)table to select from, (v)value to select if using where 
  // ex select + c + from + t + where + v + ";";

  void updateTable(string tab, string setv, string cond);
  // ex string state = update + t + set + setv + where + cond + ";"

private:
  string statement;
  string table;
// values for easier sql queries
  string insert = "INSERT INTO ";
  string values = "VALUES ";
  string update =  "UPDATE ";
  string set = "SET ";
  string select = "SELECT ";
  string from = "FROM ";
  string where = "WHERE ";
  
};
