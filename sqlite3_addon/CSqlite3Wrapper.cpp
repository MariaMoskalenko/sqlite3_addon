#include "CSqlite3Wrapper.hpp"

std::string CSqlite3Wrapper::queryResult = "";
std::string CSqlite3Wrapper::errorMessage = "";

extern "C" {
    static int callback(void *data, int argc, char **argv, char **azColName) {
        int i;
        for (i = 0; i < argc; i++) {
            CSqlite3Wrapper::queryResult.append(argv[i]);
            CSqlite3Wrapper::queryResult.append(" ");
        }
        CSqlite3Wrapper::queryResult.append("\n");
        //printf("callback: queryResult: %s\n", CSqlite3Wrapper::queryResult.c_str());
        return 0;
    }
}

CSqlite3Wrapper::CSqlite3Wrapper()
	: db(nullptr)
    , zErrMsg(0)
    , fileName("")
	, SQL("")
	, data("")
{
}

CSqlite3Wrapper::~CSqlite3Wrapper()
{
}

int CSqlite3Wrapper::openDB(const char * filename)
{
    fileName = filename;
    int error = sqlite3_open(filename, &db);
    if (SQLITE_OK != error) {
        errorMessage = sqlite3_errmsg(db);
        int res = sqlite3_close(db);
        printf("error in close %d\n", res);
	}
    return error;
}

int CSqlite3Wrapper::executeQuery(const char * input)
{
    if (nullptr != input) {
		SQL = input;
        //printf("SQL: %s\n", SQL);
	}
	else {
        printf("nullptr in query\n");
        return SQLITE_ERROR;
	}
    int error = sqlite3_exec(db, SQL, callback, (void*)data, &zErrMsg);
    //printf("queryResult: %d, %s\n", error, CSqlite3Wrapper::queryResult.c_str());
    if (SQLITE_OK != error) {
        errorMessage = zErrMsg;
        printf("errorMessage: %s\n", CSqlite3Wrapper::errorMessage.c_str());
		sqlite3_free(zErrMsg);
	}
    return error;
}

int CSqlite3Wrapper::closeDB()
{
    int error =  sqlite3_close(db);
    if (SQLITE_OK != error) {
        errorMessage = sqlite3_errmsg(db);
	}
    return error;
}
