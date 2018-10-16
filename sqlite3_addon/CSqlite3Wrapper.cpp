#include "CSqlite3Wrapper.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

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
    int error = 1;
    if (strstr(SQL, "SELECT") != nullptr) {
        error = sqlite3_prepare_v2(db, SQL, -1, &stmt, 0);
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
                            //printf("strStream: %s\n", strStream.str().c_str());
                            strBuffer.append(strStream.str());
                            strBuffer.append(" ");
                            strStream.str(std::string());
                            strStream.clear();
                            break;
                        }
                        case SQLITE_FLOAT:
                        {
                            strStream << sqlite3_column_double(stmt, i);
                            //printf("strStream: %s\n", strStream.str().c_str());
                            strBuffer.append(strStream.str());
                            strBuffer.append(" ");
                            strStream.str(std::string());
                            strStream.clear();
                            break;
                        }
                        case  SQLITE_BLOB:
                        {
                            //serialization, data buffer
                            break;
                        }
                        case SQLITE_NULL:
                        {
                            //no value
                            break;
                        }
                        default:
                            break;
                        }
                        //printf("strBuffer: %s; %d\n", strBuffer.c_str(), numCols);
                    }
                    strBuffer.append("\0");
                    mVecResults.push_back(strBuffer);
                }
            }
            if (SQLITE_OK!= error) {
               mErrorMessage = sqlite3_errmsg(db);
               printf("executeQuery step error (%d): %s\n", error, mErrorMessage.c_str());
               return(error);
            }
            /*for (unsigned int i = 0; i < mVecResults.size(); i++ ) {
                 printf("mVecResults: %s\n", mVecResults[i].c_str());
            }*/
            sqlite3_finalize(stmt);
    }
    else {
        if (strstr(SQL, "?") != nullptr) {
            printf("strstr contains ?\n");
            writeBlob(db, SQL, "sqlite.gif");
        }
        else {
            error = sqlite3_exec(db, SQL, 0, 0, 0);
            printf("error in sqlite3_exec: %d\n", error);
            if (SQLITE_OK != error) {
                mErrorMessage = sqlite3_errmsg(db);
            }
        }
    }
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

int CSqlite3Wrapper::writeBlob(sqlite3 *db, const char *sql, const char *filename)
{
    printf("writeBlob: enter\n");
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
       printf("writeBlob: error open the file\n");
       return SQLITE_ERROR;
    }
    file.seekg(0, std::ifstream::end);
    int size = file.tellg();
    file.seekg (0, file.beg);
    char* buffer = new char[size];
    file.read(buffer, size);
    file.close();
    printf("file is ready %d\n", size);
    int error = sqlite3_prepare(db, sql, -1, &stmt, 0);
    if (SQLITE_OK != error) {
        printf("sqlite3_prepare nok %d\n", error);
        mErrorMessage = sqlite3_errmsg(db);
        delete[] buffer;
        return error;
    }
    error = sqlite3_bind_int(stmt, 1, 1);
    error = sqlite3_bind_text(stmt, 2, "", 0, NULL);
    error = sqlite3_bind_int(stmt, 3, 1);
    error = sqlite3_bind_text(stmt, 4, "", 0, NULL);
    error = sqlite3_bind_double(stmt, 5, 5.00);
    error = sqlite3_bind_blob(stmt, 5, buffer, size, SQLITE_STATIC);
    if (SQLITE_OK != error) {
        printf("sqlite3_bind_blob nok %d\n", error);
        mErrorMessage = sqlite3_errmsg(db);
        delete[] buffer;
        return error;
    }
    error = sqlite3_step(stmt);
    if (SQLITE_DONE != error) {
        printf("sqlite3_step nok %d\n", error);
        mErrorMessage = sqlite3_errmsg(db);
        delete[] buffer;
        return error;
    }
    error = sqlite3_finalize(stmt);
    delete[] buffer;
    return error;
}
