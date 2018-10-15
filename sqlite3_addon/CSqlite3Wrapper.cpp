#include "CSqlite3Wrapper.hpp"
#include <sstream>

CSqlite3Wrapper::CSqlite3Wrapper()
	: db(nullptr)
    , fileName("")
	, SQL("")
    , stmt(nullptr)
    , mQueryResult("")
    , mErrorMessage("")
    , mVecResults()
{
}

CSqlite3Wrapper::~CSqlite3Wrapper()
{
}

int CSqlite3Wrapper::openDB(const char * filename)
{
    fileName = filename;
    sqlite3_enable_shared_cache( 1 );
    int error = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|SQLITE_OPEN_FULLMUTEX, NULL);
    if (SQLITE_DONE != error && SQLITE_OK != error) {
        mErrorMessage = sqlite3_errmsg(db);
        int res = sqlite3_close(db);
        printf("error in close %d\n", res);
	}
    else {
        const char*  sqlStr = "PRAGMA default_cache_size = 2048;";
        error = sqlite3_prepare_v2(db, sqlStr, -1, &stmt, 0);
        if (SQLITE_DONE != error && SQLITE_OK != error) {
            mErrorMessage = sqlite3_errmsg(db);
            printf("openDB sqlite3_prepare_v2 error: %s %d\n", mErrorMessage.c_str(), error);
        }
        else {
           error = sqlite3_step(stmt);
           if (SQLITE_DONE!= error) {
               mErrorMessage = sqlite3_errmsg(db);
               printf("openDB sqlite3_step error (%d): %s\n", error, mErrorMessage.c_str());
               return(error);
           }
        }
        sqlite3_finalize(stmt);
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

    int error = sqlite3_prepare_v2(db, SQL, -1, &stmt, 0);
    if (SQLITE_DONE != error && SQLITE_OK != error) {
        mErrorMessage = sqlite3_errmsg(db);
        printf("executeQuery sqlite3_prepare_v2 error: %s %d\n", mErrorMessage.c_str(), error);
    }
    else {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::string strBuffer = "";
            std::stringstream strStream;
            int numCols = sqlite3_column_count(stmt);
            for (int i = 0; i < numCols; ++i)
            {
                switch (sqlite3_column_type(stmt, i))
                {
                case SQLITE3_TEXT:
                {
                    strBuffer.append((const char*)sqlite3_column_text(stmt, i));
                    strBuffer.append(" ");
                    break;
                }
                case SQLITE_INTEGER:
                {
                    strStream << sqlite3_column_int(stmt, i);
                    strBuffer.append(strStream.str());
                    strBuffer.append(" ");
                    break;
                }
                case SQLITE_FLOAT:
                {
                    strStream << sqlite3_column_double(stmt, i);
                    strBuffer.append(strStream.str());
                    strBuffer.append(" ");
                    break;
                }
                default:
                    break;
                }
                printf("strBuffer: %s; %d\n", strBuffer.c_str(), numCols);
            }
            strBuffer.append("\0");
            mVecResults.push_back(strBuffer);
        }
        if (SQLITE_OK!= error) {
           mErrorMessage = sqlite3_errmsg(db);
           printf("executeQuery step error (%d): %s\n", error, mErrorMessage.c_str());
           return(error);
        }
        for (unsigned int i = 0; i < mVecResults.size(); i++ ) {
             printf("mVecResults: %s\n", mVecResults[i].c_str());
        }
    }
    sqlite3_finalize(stmt);
    return error;
}

int CSqlite3Wrapper::closeDB()
{
    int error =  sqlite3_close(db);
    if (SQLITE_OK != error) {
        mErrorMessage = sqlite3_errmsg(db);
	}
    return error;
}
