#include "CommandQueue.hpp"
#include "KeyValueStorageWrapper.hpp"

CommandQueue::CommandQueue()
	: mQueueCommands()
	, mIsCommandRunning(false)
    , mSqlWrapper()
{
}

CommandQueue::~CommandQueue()
{
}

void CommandQueue::addCommand(ICommand* const command) {
     if (nullptr != command) {
        printf("addCommand cmdId=%d\n", command->getCommandType());
        mSyncObj.lock();
        mQueueCommands.push_back(command);
        mSyncObj.unlock();
        printf("addCommand size=%zd\n", mQueueCommands.size());
        processCommands();
     }
}

void CommandQueue::processCommands()
{
   printf("processCommands mIsCommandRunning=%d\n", mIsCommandRunning);
   if (!mIsCommandRunning) {
       if(mQueueCommands.size() != 0) {
          ICommand* cmd = mQueueCommands.back();
          if (nullptr != cmd) {
             printf("process cmdId=%d\n", cmd->getCommandType());
             mIsCommandRunning = true;
             cmd->execute();
             printf("processeeded cmdId=%d\n", cmd->getCommandType());
             printf("processCommands size=%zd\n", mQueueCommands.size());
          }
       }
   }
}

void CommandQueue::commandFinished(ICommand* const command)
{
    if (nullptr != command) {
        printf("commandFinished cmdId=%d\n", (int)command->getCommandType());
        commandListenersIterator it = mListeners.begin();
        for ( ; it != mListeners.end(); it++) {
            (*it)->notifyReady(command);
        }
        mSyncObj.lock();
        auto iter = find(mQueueCommands.begin(), mQueueCommands.end(), command);
        //printf("going to erase\n");
        if (iter != mQueueCommands.end()) {
           printf("erased command cmdId=%d\n", (int)(*iter)->getCommandType());
           mQueueCommands.erase(iter);
           mIsCommandRunning = false;
           processCommands();
        }
        else {
            printf("no such command\n");
        }
        mSyncObj.unlock();
    }
}

void CommandQueue::addCommandOpenDataBase(const std::string& fileName)
{
    CommandOpenDataBase* command = new CommandOpenDataBase(self(), mSqlWrapper, fileName);
    addCommand(command);
}

void CommandQueue::addCommandCloseDataBase()
{
    CommandCloseDataBase* command = new CommandCloseDataBase(self(), mSqlWrapper);
    addCommand(command);
}

void CommandQueue::addCommandExecuteQuery(const std::string& query)
{
    CommandExecuteQuery* command = new CommandExecuteQuery(self(), mSqlWrapper, query);
    addCommand(command);
}

void CommandQueue::registerListener(ICommandListener* const ptr)
{
   if (nullptr != ptr) {
       mListeners.push_back(ptr);
   }
}

void CommandQueue::unRegisterListener(ICommandListener* const ptr)
{
   if (nullptr != ptr) {
      commandListenersIterator it = find(mListeners.begin(), mListeners.end(), ptr);
      if (it != mListeners.end()) {
         mListeners.erase(it);
      }
  }
}


