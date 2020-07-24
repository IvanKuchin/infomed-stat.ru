#ifndef __CMYSQL__H__
#define	__CMYSQL__H__

#include <mysql.h>
#include <string>

using namespace std;

#include "localy.h"
#include "clog.h"

class CMysqlSkel
{
	private:
		MYSQL_ROW       currentRow;

	protected:
		MYSQL			*db;
		MYSQL_RES		*resultSet;
		MYSQL_FIELD		*fieldsInfo;
		unsigned int	numRows;
		unsigned int	numFields;

		int				FieldsIndex(const char *fieldName);
		MYSQL_ROW		NextFetch(MYSQL_RES *result);
		char*			ResultValueFast(unsigned int fi);

	public:
		struct			MysqlError {};

						CMysqlSkel( void ): db(NULL), fieldsInfo(NULL), numRows(0), numFields(0) {};
		void			CloseDB( void );
		int				InitDB(const char *dbName, const char *user, const char *pass );
		MYSQL_RES*		QueryDB(const string &query );
		unsigned long	InsertQueryDB(const string &query );
		char*			ResultValue( MYSQL_RES *result, unsigned int row, const char *name );
		char*			ResultValue( MYSQL_RES *result, unsigned int row, int fi );
		int				ResultRows( MYSQL_RES *result );
		void			FreeResultSet();
		bool			isError();
		const char *	GetErrorMessage();
		auto			GetNumberOfCols()										{ return numFields; };
		auto			GetColName(unsigned int idx) -> string;

		virtual			~CMysqlSkel() {};
};

class CMysql : public CMysqlSkel
{
	public:
						CMysql();
						CMysql(const char *dbname, const char *login, const char *pass);

		int				Connect(const char *dbname, const char *login, const char *pass);
		int				Query(const string &query);
		unsigned long	InsertQuery(const string &query);
		string			Get(int rows, const string &field);
		string			Get(int rows, const char *field);
		string			Get(int rows, int col);
		MYSQL_ROW		NextRow(void);
		char*			GetFast(int col);
		int				CountFields();
		int				AffectedRows();

		virtual			~CMysql();
};

#endif
