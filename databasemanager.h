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


class UserInfo
{
public:
  databasemanager(string t, string st); //do statement st with table t
  

private:
  string statement;
  string table;
  
  
};
