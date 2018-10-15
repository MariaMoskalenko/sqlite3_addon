#ifndef CSQLITE3WRAPPER_H
#define CSQLITE3WRAPPER_H

#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
extern "C" {
    #include "sqlite3.h"
}

class CSqlite3Wrapper
{
public:
	CSqlite3Wrapper();
	~CSqlite3Wrapper();

    int openDB(const char * filename);
    int executeQuery(const char * input);
    int closeDB();

    const std::string& getResultString();
    const std::string& getErrorMessage();
    const std::vector<std::string>& getResultVector();
    void cleanResults();

private:
	sqlite3 *db;
    const char *fileName;
	const char* SQL;
    sqlite3_stmt *stmt;
    std::string mQueryResult;
    std::string mErrorMessage;
    std::vector<std::string> mVecResults;
};

inline const std::string& CSqlite3Wrapper::getResultString()
{
    return mQueryResult;
}

inline const std::string& CSqlite3Wrapper::getErrorMessage()
{
    return mErrorMessage;
}

inline const std::vector<std::string>& CSqlite3Wrapper::getResultVector()
{
    return mVecResults;
}

inline void CSqlite3Wrapper::cleanResults()
{
    mVecResults.clear();
    mQueryResult = "";
    mErrorMessage = "";
}

#endif
