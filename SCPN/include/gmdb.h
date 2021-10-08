/**
* gmdb.h
*/

#ifndef __GMDB_H_
#define __GMDB_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
//#include <stdarg.h> /**< for indefinite parameter (...) */
#if defined(WIN32) || defined(__CYGWIN__)
#include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <vector>


#include <map>
#include <cstring>
//#include <hash_map>
using namespace std;

typedef struct marking_info
{
   int id;
   string name;
} marking_info;

typedef struct attr_name_type_info
{
   string name;
   SQLSMALLINT sql_c_type;
} attr_name_type_info;

typedef struct token_attribute
{
   string name;
   SQLSMALLINT sql_c_type;
   char value[SQL_MAX_MESSAGE_LENGTH];  // bindbuffer, should be casted to the real type, e.g. (int)value
} token_attribute;

typedef struct token_description
{
   int id;
   string place_name;
   vector<token_attribute*> attributes;
} token_description;

typedef struct token_description_list
{
   string tokentypename;
   vector<token_description*> token_descriptions;
} token_description_list;

typedef struct tokentype_description
{
   string tokentypename;
   vector<attr_name_type_info*> attributes;
} tokentype_description;

typedef struct marking_description
{
   string markingname;
   vector<string> placenames;
   vector<tokentype_description*> tokentypes;
} marking_description;


class GMDB
{

private:

/** private variables */

   SQLHENV V_OD_Env;  /** Handle ODBC environment */
   SQLHDBC V_OD_hdbc; /** Handle connection */


/********************** TEST *************************************
   typedef struct db_cache_key
   {
      string tablename;
      string columnname;
      int id;
   } db_cache_key;

//   struct db_cache_hash
//   {
//      size_t operator() (const db_cache_key &s) const
//      {
//         static hash<const char *> H;
//         stringstream idname;
//         idname << s.id;
//         return H((s.tablename+s.columnname+idname).c_str());
//      }
//   };

   struct db_cache_eqstr
   {
      bool operator()(const db_cache_key* k1, const db_cache_key* k2) const
      {
         return (k1->tablename == k2->tablename) && (k1->columnname == k2->columnname) && (k1->id == k2->id);
      }
   };

   typedef hash_map<db_cache_key*, int, db_cache_hash, db_cache_eqstr> db_int_cache;
**************************************************************/


/** private functions */
 //===> AH: made this functions public to have access to them
 //from manual transition code. This needs to be revised.
public:
   /**
   * AllocateStatementHandle
   *
   * Method for getting a statement handle.
   * A statement handle is needed for every new SQL query.
   * @param V_OD_hstmt SQL statement handle
   * @see SQLQuery(SQLHSTMT *V_OD_hstmt, const char *sqlquery)
   * @return error status (true: no error)
   */
   bool AllocateStatementHandle(SQLHSTMT *V_OD_hstmt);

   /**
   * SQLQuery
   *
   * Sends a SQL query to the database.
   * @param V_OD_hstmt SQL statment handle
   * @param sqlquery SQL query string
   * @see AllocateStatementHandle(SQLHSTMT *V_OD_hstmt);
   * @return error status (true: no error)
   */
   bool SQLQuery(SQLHSTMT *V_OD_hstmt, const char *sqlquery);

   /**
   * SQLGetTypeName
   *
   * Gets the real SQL name for the given SQL type.
   * Some types needs to be changed and the unsigned/signed types requires special cases.
   * @param sqltype SQL Type value which should be converted to string
   * @param sqltypename string reference for the resulting type string
   * @return error status (true: no error)
   */
   bool SQLGetTypeName(SQLSMALLINT sqltype, string &sqltypename);

   /**
   * SQLReadValue
   *
   * Reads a value from the specified table with column and id.
   * The given statement handle must be initialized and the correct sql type must be bound to the column 1.
   * The result will be written to the buffer bound to column 1 of the statement handle.
   * @param V_OD_hstmt Statement handle which is used for the SQL query
   * @param tablename the name of the table
   * @param columnname the name of the column
   * @param id identifier (column id) of the data in the table
   * @return error status (true: no error)
   */
   bool SQLReadValue(SQLHSTMT V_OD_hstmt, const char *tablename, const char *columnname, int id);

   /**
   * lcase
   *
   * Converts the given string to lower case.
   * @param text string which should be converted to lower case
   * @return new lower case string
   */
   static string lcase(string text);

public:
/** public functions */

   /**
   * GMDB
   *
   * Constructor
   */
   GMDB();

   /**
   * ~GMDB
   *
   * Destructor
   */
   ~GMDB();

   /**
   * DBCreateDB
   *
   * Creates a database and edit the odbc.ini
   * This function contains the functions DBConnect and
   * DBCloseConnect. So don't use the functions before
   * calling DBCreateDB.
   * @param dbName the name of the new database. Note: The name of the database and the DSN name are the same! You connect with the DSN name to the database!
   * @param driver the driver name e.g. "myodbc"
   * @see DBDropDB(char *dbName)
   * @return error status (0: no error)
   */
   bool DBCreateDB(char *dbName, char *driver);

   /**
   * DBDropDB
   *
   * Drops a database in the database system,
   * deletes DSN name and erases the entry in /etc/odbc.ini
   * @param dbName the name of the database
   * @see DBCreateDB(char *dbName, char *driver)
   * @return error status (0: no error)
   */
   bool DBDropDB(char *dbName);

   /**
   * DBConnect
   *
   * Opens a connect to the database dbName. The dbName is the ODBC-DSN.
   * The DSN name is the same as the name of the database.
   * @param dbName the name of the database/DSN
   * @param user username for database access
   * @param psswd password for database access
   * @return error status (true: no error)
   * @see DBCloseConnect()
   */
   bool DBConnect(const char *dbName, const char *user, const char *psswd);

   /**
   * DBCloseConnect
   *
   * Closes the connection to the database.
   * @see DBConnect(char *dbName,char *user,char *psswd)
   */
   void DBCloseConnect();

   /**
   * DBGetMarkings
   *
   * Reads all markings (with id and name) from the database.
   * All tables are connected with exactly one marking, therefore each marking is a state of a net.
   * @param markingnames Vector of markingnames which will be filled
   * @return error status (true: no error)
   * @see DBCreateMarking(marking_description *newmarking)
   */
   bool DBGetMarkings(vector<marking_info*> markingnames);

   /**
   * DBCreateMarking
   *
   * Creates a new marking with the given name in the database and creates all neccessary tables.
   * This method searches for a free id in the "markings" table and creates a new entry with this id and the given name.
   * Then it creates all tables for places and tokentypes with a prefix "<<id>>_" which means "3_" if id=3.
   * Tables for each marking are:
   *    places
   *    tokens_<<tokentype>>
   *    objects_<<tokentype>>
   * @param newmarking structure with information about the marking name and all places and tokentypes
   * @return error status (true: no error)
   * @see DBGetMarkings(vector<marking_info*> markingnames)
   */
   bool DBCreateMarking(marking_description *newmarking);

   /**
   * DBReadTokens
   *
   * Reads all tokens for the given tokentype from database.
   * The method creates a dynamic SQL query for the tokentype using all attribute types.
   * For each attribute a buffer will be created which will get the results.
   * Additionally the fixed attributes like id and place_id will be assigned.
   * The following SQL query contains the attribute names in the same order as the assigned types.
   * All results (tokens) are fetched and inserted in the tokens list.
   * @param netid marking id for which tokens should be read
   * @param tokentypename name of the tokentype
   * @param typeinfos information about the tokentype attributes (name and type)
   * @param tokens list of tokens in which all tokens will be added in terms of token descriptions
   * @return error status (true: no error)
   * @see DBGetMarkings(vector<marking_info*> markingnames)
   */
   bool DBReadTokens(int netid, const char *tokentypename, vector<attr_name_type_info*> *typeinfos, token_description_list *tokens);

   /**
   * DBReadFrequencies
   *
   * Reads all frequencies from table "Passenger".
   * @return map with all frequencies with the id
   */
   map<int, double> DBReadFrequencies();

   /**
   * DBCreateTbl
   *
   * Creates a table. First you have to connect to a database with DBConnect.
   * Note: Every table must have a column "Id" of tye INT (%i)
   * @param tblName the name of the table you would like to create
   * @param types  "%s" VARCHAR[200] NOT NULL
   *               "%i" INT NOT NULL
   *               "%f" FLOAT NOT NULL
   *               "%d" DOUBLE NOT NULL
   *               "%b" ENUM('False','True') NOT NULL
   *               "%a" DATE NOT NULL {"yyyy-mm-dd"}
   *               "%m" TIME NOT NULL {"hh-mm-ss"}
   *               "%t" DATETIME NOT NULL {"yyyy-mm-dd hh-mm-ss"}
   * @param ... char *name - Name of the columns
   * @see DBDropTbl(char *tblName)
   * @return error status (0: no error)
   */
   bool DBCreateTbl(char *tblName, char *types, ...);

   /**
   * DBDropTbl
   *
   * Drops a table. First you have to connect to a database with DBConnect
   * @param tblName the name of the table you would like to drop
   * @see DBDCreateTbl(char *tblName, char *types, ...)
   * @return error status (0: no error)
   */
   bool DBDropTbl(char *tblName);

   /**
   * DBUpdateStr
   *
   * Writes string data to a SQL VARCHAR column
   * @param newStr new value
   * @param colName the name of the column
   * @param tblName the name of the table where you would like to update the data
   * @param id row id of the data in table tblname
   * @return error status (0: no error)
   */
   bool DBUpdateStr(char *newStr, char *colName, char *tblName, int id);

   /**
   * DBUpdateInt
   *
   * Writes integer data to a SQL INT column
   * @param newInt new value
   * @param colName the name of the column
   * @param tblName the name of the table where you would like to update the data
   * @param id row id of the data in table tblname
   * @return error status (0: no error)
   */
   bool DBUpdateInt(int newInt, char *colName, char *tblName, int id);

   /**
   * DBUpdateFloat
   *
   * Writes float data to a SQL FLOAT column
   * @param newFlt new value
   * @param colName the name of the column
   * @param tblName the name of the table where you would like to update the data
   * @param id row id of the data in table tblname
   * @return error status (0: no error)
   */
   bool DBUpdateFloat(float newFlt, char *colName, char *tblName, int id);

   /**
   * DBUpdateDouble
   *
   * Writes double data to a SQL DOUBLE column
   * @param newDbl new value
   * @param colName the name of the column
   * @param tblName the name of the table where you would like to update the data
   * @param id row id of the data in table tblname
   * @return error status (0: no error)
   */
   bool DBUpdateDouble(double newDbl, char *colName, char *tblName, int id);

   /**
   * DBUpdateBool
   *
   * Writes bool data to a SQL ENUM('False','True') column
   * @param newBool new value
   * @param colName the name of the column
   * @param tblName the name of the table where you would like to update the data
   * @param id row id of the data in table tblname
   * @return error status (0: no error)
   */
   bool DBUpdateBool(bool newBool, char *colName, char *tblName, int id);

   /**
   * DBUpdateDate
   *
   * Writes date data to a SQL DATE column. Format: "yyyy-mm-dd"
   * @param newDate new value
   * @param colName the name of the column
   * @param tblName the name of the table where you would like to update the data
   * @param id row id of the data in table tblname
   * @return error status (0: no error)
   */
   bool DBUpdateDate(char *newDate, char *colName, char *tblName, int id); /**< Format "yyyy-mm-dd" */

   /**
   * DBUpdateTime
   *
   * Writes time data to a SQL TIME column. Format: "hh-mm-ss"
   * @param newTime new value
   * @param colName the name of the column
   * @param tblName the name of the table where you would like to update the data
   * @param id row id of the data in table tblname
   * @return error status (0: no error)
   */
   bool DBUpdateTime(char *newTime, char *colName, char *tblName, int id); /**< Format "hh-mm-ss" */

   /**
   * DBUpdateDateTime
   *
   * Writes datetime data to a SQL DATETIME column. Format: "yyyy-mm-dd hh-mm-ss"
   * @param newDateTime new value
   * @param colName the name of the column
   * @param tblName the name of the table where you would like to update the data
   * @param id row id of the data in table tblname
   * @return error status (0: no error)
   */
   bool DBUpdateDateTime(char *newDateTime, char *colName, char *tblName, int id); /**< Format "yyyy-mm-dd hh-mm-ss" */

   /**
   * DBReadStr
   *
   * Reads string data from the specified table with column and id.
   * @param result string reference for result string
   * @param tablename the name of the table
   * @param columnname the name of the column
   * @param id identifier (column id) of the data in the table
   * @return error status (true: no error)
   */
   bool DBReadStr(string &result, const char *tablename, const char *columnname, int id);

   /**
   * DBReadInt
   *
   * Reads integer data from the specified table with column and id.
   * @param result pointer for integer result
   * @param tablename the name of the table
   * @param columnname the name of the column
   * @param id identifier (column id) of the data in the table
   * @return error status (true: no error)
   */
   bool DBReadInt(int *result, const char *tablename, const char *columnname, int id);

   /**
   * DBReadFloat
   *
   * Reads float data from the specified table with column and id.
   * @param result pointer for float result
   * @param tablename the name of the table
   * @param columnname the name of the column
   * @param id identifier (column id) of the data in the table
   * @return error status (true: no error)
   */
   bool DBReadFloat(float *result, const char *tablename, const char *columnname, int id);

   /**
   * DBReadDouble
   *
   * Reads double data from the specified table with column and id.
   * @param result pointer for double result
   * @param tablename the name of the table
   * @param columnname the name of the column
   * @param id identifier (column id) of the data in the table
   * @return error status (true: no error)
   */
   bool DBReadDouble(double *result, const char *tablename, const char *columnname, int id);

   /**
   * DBReadBool
   *
   * Reads boolean data from the specified table with column and id.
   * @param result pointer for boolean result
   * @param tablename the name of the table
   * @param columnname the name of the column
   * @param id identifier (column id) of the data in the table
   * @return error status (true: no error)
   */
   bool DBReadBool(bool *result, const char *tablename, const char *columnname, int id);

   /**
   * DBReadDateTime
   *
   * Reads date or/and time data from a SQL DATE, TIME or DATETIME column.
   * Format: "yyyy-mm-dd" or "hh-mm-ss" or "yyyy-mm-dd hh-mm-ss" depending on column type.
   * @param result pointer for date/time result
   * @param tablename the name of the table
   * @param columnname the name of the column
   * @param id identifier (column id) of the data in the table
   * @return error status (true: no error)
   */
   bool DBReadDateTime(string &result, const char *tablename, const char *columnname, int id);

   /**
   * DBInsertRow
   *
   * Inserts a complete new row in tblName. First you have to connect to a database with DBConnect
   * @param tblName the name of the table
   * @param types  "%s" VARCHAR[200] NOT NULL
   *               "%i" INT NOT NULL
   *               "%f" FLOAT NOT NULL
   *               "%d" DOUBLE NOT NULL
   *               "%b" ENUM('False','True') NOT NULL
   *               "%a" DATE NOT NULL {"yyyy-mm-dd"}
   *               "%m" TIME NOT NULL {"hh-mm-ss"}
   *               "%t" DATETIME NOT NULL {"yyyy-mm-dd hh-mm-ss"}
   * @param ... variables to bind to the SQL data
   * @see DBReadRow(char *tblName, char *types, ...)
   * @see DBUpdateRow(char *tblName, char *types, ...)
   * @see DBDeleteRow(char *tblName, int id)
   * @return error status (0: no error)
   */
   bool DBInsertRow(char *tblName, char *types, ...);

   /**
   * DBUpdateRow
   *
   * Writes a complete row. First you have to connect to a database with DBConnect
   * @param tblName the name of the table
   * @param types  "%s" VARCHAR[200] NOT NULL
   *               "%i" INT NOT NULL
   *               "%f" FLOAT NOT NULL
   *               "%d" DOUBLE NOT NULL
   *               "%b" ENUM('False','True') NOT NULL
   *               "%a" DATE NOT NULL {"yyyy-mm-dd"}
   *               "%m" TIME NOT NULL {"hh-mm-ss"}
   *               "%t" DATETIME NOT NULL {"yyyy-mm-dd hh-mm-ss"}
   * @param id id of the row in table tblname
   * @param ... variables to bind to the SQL data
   * @see DBInsertRow(char *tblName, char *types, int id, ...)
   * @see DBUpdateRow(char *tblName, char *types, int id, ...)
   * @see DBReadRow(char *tblName, char *types, int id, ...)
   * @return error status (0: no error)
   */
   bool DBUpdateRow(char *tblName, char *types, int id, ...);

   /**
   * DBReadRow
   *
   * Reads a complete row. First you have to connect to a database with DBConnect
   * @param tblName the name of the table
   * @param types  "%s" VARCHAR[200] NOT NULL
   *               "%i" INT NOT NULL
   *               "%f" FLOAT NOT NULL
   *               "%d" DOUBLE NOT NULL
   *               "%b" ENUM('False','True') NOT NULL
   *               "%a" DATE NOT NULL {"yyyy-mm-dd"}
   *               "%m" TIME NOT NULL {"hh-mm-ss"}
   *               "%t" DATETIME NOT NULL {"yyyy-mm-dd hh-mm-ss"}
   * @param id id of the row in table tblname
   * @param ... variables to bind to the SQL data
   * @see DBInsertRow(char *tblName, char *types, ...)
   * @see DBUpdateRow(char *tblName, char *types, int id, ...)
   * @return error status (0: no error)
   */
   bool DBReadRow(char *tblName, char *types, int id, ...);

   /**
   * DBDeleteRow
   *
   * Deletes a row with special id number
   * @param tblName the name of the table you would like to drop
   * @param id id of the row in table tblname
   * @see DBInsertRow(char *tblName, char *types, ...)
   * @return error status (0: no error)
   */
   bool DBDeleteRow(char *tblName, int id);

//   void DBExplode();

};

#endif
