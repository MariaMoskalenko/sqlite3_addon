#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H
#pragma once

#include <string.h>
#include <functional>
#include <algorithm>
#include <vector>
#include <mutex>

#include "ICommand.hpp"
#include "CommandOpenDataBase.hpp"
#include "CommandCloseDataBase.hpp"
#include "CommandExecuteQuery.hpp"

using std::function;
using std::string;
using std::vector;
using std::shared_ptr;

typedef std::vector<ICommandListener *> commandListeners;
typedef commandListeners::iterator commandListenersIterator;

class CommandQueue
{
public:
   CommandQueue();
   ~CommandQueue();

   void addCommandOpenDataBase(const std::string& fileName);
   void addCommandCloseDataBase();
   void addCommandExecuteQuery(const std::string& query);
   void commandFinished(ICommand* const command);
   void registerListener(ICommandListener* const ptr);
   void unRegisterListener(ICommandListener* const ptr);
   CommandQueue& self();

private:
    void processCommands();
    void addCommand(ICommand* const command);

	std::vector<ICommand*> mQueueCommands;
	bool mIsCommandRunning;
    commandListeners mListeners;
    std::mutex mSyncObj;
    CSqlite3Wrapper mSqlWrapper;
};

inline CommandQueue& CommandQueue::self() { return *this; }

#endif
