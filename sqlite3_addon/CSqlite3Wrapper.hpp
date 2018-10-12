#ifndef CSQLITE3WRAPPER_H
#define CSQLITE3WRAPPER_H

#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

    static std::string queryResult;
    static std::string errorMessage;

private:
	sqlite3 *db;
	char *zErrMsg;
    const char *fileName;
	const char* SQL;
	const char* data;
};

#endif
