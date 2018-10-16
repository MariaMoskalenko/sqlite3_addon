#include "CommandOpenDataBase.hpp"
#include "CommandQueue.hpp"

CommandOpenDataBase::CommandOpenDataBase(CommandQueue& parent
                                         ,CSqlite3Wrapper& sqlWrapper
                                         ,const std::string& fileName)
    : mParent(parent)
    , mSqlWrapper(sqlWrapper)
    , mResult(0)
    , mResultQuery("")
    , mFileName(fileName)
{
}

CommandOpenDataBase::~CommandOpenDataBase()
{
}

void CommandOpenDataBase::execute()
{
   mResult = mSqlWrapper.openDB(mFileName.c_str());
   if (SQLITE_DONE != mResult && SQLITE_OK != mResult) {
       mResultQuery = mSqlWrapper.getErrorMessage();
       printf("Cannot open db, error: %s\n", mResultQuery.c_str());
   }
   mParent.commandFinished(this);
   mSqlWrapper.cleanResults();
}
