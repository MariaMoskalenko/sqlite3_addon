#include "CommandCloseDataBase.hpp"
#include "CommandQueue.hpp"

CommandCloseDataBase::CommandCloseDataBase(CommandQueue& parent, CSqlite3Wrapper& sqlWrapper)
    : mParent(parent)
    , mSqlWrapper(sqlWrapper)
    , mResult(0)
    , mResultQuery("")
{
}

CommandCloseDataBase::~CommandCloseDataBase()
{
}

void CommandCloseDataBase::execute()
{
   mResult = mSqlWrapper.closeDB();
   if(SQLITE_OK != mResult) {
       mResultQuery = mSqlWrapper.errorMessage;
       printf("Cannot close db, error: %s\n", mResultQuery.c_str());
   }
   mParent.commandFinished(this);
}
