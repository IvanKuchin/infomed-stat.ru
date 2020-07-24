#include "cmysql.h"
#include "cexception.h"

int CMysqlSkel::InitDB(const char *dbName, const char *user = "root", const char *pass = "" )
{
    MESSAGE_DEBUG("", "", "start (db/login: " + dbName + "/" + user + ")");

    db = mysql_init( NULL );
    if( db == NULL )
    {
        MESSAGE_ERROR("", "", "failed to allocate memory when init MYSQL");
        return(-1);
    }

// --- that ma fix 

    db = mysql_real_connect(db, DB_HOST, user, pass, dbName, 0, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS);

    if( db == NULL )
    {
        MESSAGE_ERROR("", "", "Failed to connect to MYQSL server (mysql_real_connect return NULL)");
        return(-1);
    }

    MESSAGE_DEBUG("", "", "finish");

    return(0);
}

void CMysqlSkel::CloseDB( void )
{
    MESSAGE_DEBUG("", "", "start");

    if( db ) mysql_close( db );

    MESSAGE_DEBUG("", "", "finish");
}

// --- Run Insert query only with autoincrement field
// --- Input: string
// --- Output: 0 - error or no AUTO_INCREMENT_FIELD
//              >0 - success
unsigned long CMysqlSkel::InsertQueryDB(const string &query)
{
    int             r;
    my_ulonglong    resultAutoIncrement = 0;

    MESSAGE_DEBUG("", "", "start (query [" + query + "])");

    r = mysql_query( db, query.c_str());
    if( r )
    {
        MESSAGE_ERROR("", "", "failed insert query (" + query + ") " + mysql_error(db));
    }
    else
    {
        if (mysql_store_result(db) == 0 &&
            mysql_field_count(db) == 0 &&
            mysql_insert_id(db) != 0)
        {
            resultAutoIncrement = mysql_insert_id(db);
        }
        else
        {
            MESSAGE_ERROR("", "", "failed insert query " + query + ": (returned > 0 cols) or (resultSet != NULL) or (table missing AUTO_INCREMENT_FIELD)");
        }
    }

    MESSAGE_DEBUG("", "", "finish (return [" + to_string((unsigned long)resultAutoIncrement) + "])");

    return (unsigned long)resultAutoIncrement;
}

// --- Run MySQL query
// --- Input: string
// --- Output:     NULL - resultSet is empty OR error
//             not NULL - resultSet
// --- Remark: no error indication
MYSQL_RES *CMysqlSkel::QueryDB(const string &query)
{
    MYSQL_RES   *result;
    int         r;

    MESSAGE_DEBUG("", "", "start (query [" + query + "])");

    r = mysql_query( db, query.c_str());
    if( r )
    {
        MESSAGE_ERROR("", "", "failed query (" + query + ") " + mysql_error(db));

        return NULL;
    }
    else
    {
    	result = mysql_store_result( db );
    	numRows = mysql_affected_rows( db );
    	if( numRows == 0 )
    	{
        	if( result ) mysql_free_result( result );
    	    result = NULL;
    	}
    	else
    	{
    	    if(result)
    	    {
        		numFields = mysql_num_fields( result );
        		fieldsInfo = mysql_fetch_fields( result );
    	    }
    	    else
    	    {
        		numFields = 0;
        		result = NULL;
    	    }
    	}
    }
    return(result);
}

bool CMysqlSkel::isError(void)
{
    return strlen(mysql_error(db));
}

const char *CMysqlSkel::GetErrorMessage(void)
{
    return mysql_error(db);
}

int CMysqlSkel::FieldsIndex(const char *fieldName )
{
    for(auto i = 0u; i < numFields; i++ )
    	if( strcmp( fieldName, fieldsInfo[i].name ) == 0 )
            return(i);
    return(-1);
}

auto CMysqlSkel::GetColName(unsigned int idx) -> string
{
    auto    result = ""s;

    if(idx < numFields) result = fieldsInfo[idx].name;

    return result;
}

void CMysqlSkel::FreeResultSet()
{
    if( resultSet )
    {
    	mysql_free_result( resultSet );
    	resultSet = NULL;
    }
}

char *CMysqlSkel::ResultValue( MYSQL_RES *result, unsigned int row, const char *name )
{
    auto            fi = FieldsIndex( name );

    if(fi < 0)
    {
        MESSAGE_ERROR("", "", "DB-field [" + name + "] doesn't exists");

        throw CException("error db");
    }

    return ResultValue(result, row, fi);
}

char *CMysqlSkel::ResultValue( MYSQL_RES *result, unsigned int row, int fi )
{
    MYSQL_ROW       fr;

    if(fi < 0)
    {
        MESSAGE_ERROR("", "", "field index can't be negative");

        throw CException("error db");
    }

    mysql_data_seek(result, row);
    fr = mysql_fetch_row(result);

    if(fr)
    {
        if(!fr[fi])
        {
            MESSAGE_ERROR("", "", "mysql result value(row " + to_string(row) + ", index " + to_string(fi) + ") is NULL");
        }
        return(fr[fi] ? fr[fi] : (char *)"");
    }
    else
    {
        MESSAGE_ERROR("", "", "mysql_fetch_row( " + to_string(row) + " ) returned NULL")

    	return(NULL);
    }
}

MYSQL_ROW CMysqlSkel::NextFetch(MYSQL_RES *result)
{
	currentRow = mysql_fetch_row(result);
	if(!currentRow)
	{
        MESSAGE_ERROR("", "", "fail calling mysql_fetch_row()");
        return NULL;
	}
	return currentRow;
}

char *CMysqlSkel::ResultValueFast(unsigned int fi)
{
	if(currentRow)
    	return(currentRow[fi]);
	else
	{
        MESSAGE_ERROR("", "", "fail extracting info from currentRow( " + to_string(fi) + " )");
    	return(NULL);
	}
}

int CMysqlSkel::ResultRows( MYSQL_RES *result )
{
    return numRows;
}



CMysql::CMysql()
{
    resultSet = NULL;
}

CMysql::CMysql(const char *dbName, const char *login, const char *pass)
{
    resultSet = NULL;
    InitDB(dbName, login, pass);
}

int CMysql::Connect(const char *dbName, const char *login, const char *pass)
{
    auto    result = InitDB(dbName, login, pass);

    if(result == 0)
    {
        // --- sucessfull init
        Query("set names " + DB_CHARSET);
    }

    return result;
}

int CMysql::Query(const string &query)
{
    FreeResultSet();
    resultSet = QueryDB(query);
    if(resultSet != NULL) {
       return AffectedRows();
    }
    return 0;
}

unsigned long CMysql::InsertQuery(const string &query)
{
    FreeResultSet();
    return InsertQueryDB(query);
}

string CMysql::Get(int row, const string &name)
{
    return Get(row, name.c_str());
}

string CMysql::Get(int row, const char *name)
{
    if(resultSet)
        return ResultValue(resultSet, row, name);
    return(NULL);
}

string CMysql::Get(int row, int col)
{
    if(resultSet)
        return ResultValue(resultSet, row, col);
    return(NULL);
}

MYSQL_ROW CMysql::NextRow(void)
{
    return NextFetch(resultSet);
}

char *CMysql::GetFast(int index)
{
    return ResultValueFast(index);
}


int CMysql::CountFields()
{
    return numFields;
}

int CMysql::AffectedRows()
{
    return numRows;
}

CMysql::~CMysql()
{
    FreeResultSet();
    CloseDB();
}

