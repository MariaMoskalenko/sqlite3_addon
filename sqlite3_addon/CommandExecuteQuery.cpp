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
    , mResultsQuery()
{
}

CommandExecuteQuery::~CommandExecuteQuery()
{
}

void CommandExecuteQuery::execute()
{
   if (!mQuery.empty()) {
      mResult = mSqlWrapper.executeQuery(mQuery.c_str());
      printf("Execute query result: %d\n", mResult);
      if(SQLITE_OK != mResult) {
          mResultQuery = mSqlWrapper.getErrorMessage();
          printf("Cannot execute query, error: %s\n", mResultQuery.c_str());
          printf("Original query: %s\n", mQuery.c_str());
      }
      else {
           mResultsQuery = mSqlWrapper.getResultVector();
           /*for (unsigned int i = 0; i < mResultsQuery.size(); i++ ) {
                printf("mResultsQuery: %s\n", mResultsQuery[i].c_str());
           }*/
      }
   }
   mParent.commandFinished(this);
   mSqlWrapper.cleanResults();
}
