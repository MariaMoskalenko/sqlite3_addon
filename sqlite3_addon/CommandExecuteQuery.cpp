#include "CommandExecuteQuery.hpp"
#include "CommandQueue.hpp"

CommandExecuteQuery::CommandExecuteQuery(CommandQueue& parent
                                         , CSqlite3Wrapper& sqlWrapper
                                         , const std::string& query)
    : mParent(parent)
    , mQuery(query)
    , mSqlWrapper(sqlWrapper)
    , mResult(0)
    , mResultQuery("")
{
}

CommandExecuteQuery::~CommandExecuteQuery()
{
}

void CommandExecuteQuery::execute()
{
   printf("Execute query: %s\n", mQuery.c_str());
   if (!mQuery.empty()) {
      mResult = mSqlWrapper.executeQuery(mQuery.c_str());
      printf("Execute query result: %d\n", mResult);
      if(SQLITE_OK != mResult) {
          mResultQuery = mSqlWrapper.errorMessage;
          printf("Cannot execute query, error: %s\n", mResultQuery.c_str());
      }
      else {
           mResultQuery = mSqlWrapper.queryResult;
      }
      printf("Execute query result: %s\n", mResultQuery.c_str());
   }
   mParent.commandFinished(this);
}
