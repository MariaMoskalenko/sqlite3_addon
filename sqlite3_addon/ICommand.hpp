#ifndef ICOMMAND_H
#define ICOMMAND_H
#pragma once

#include <string>
#include <vector>

class CommandQueue;

enum class CommandType
{
    Default = 0
    , CommandOpenDataBase
    , CommandExecuteQuery
    , CommandCloseDataBase
};

enum class CommandError
{
    SUCCESS = 0
    , ERROR_OPEN
    , ERROR_CLOSE
    , ERROR_EXECUTE
};

class ICommand
{
public:
   virtual ~ICommand() {}
   virtual void execute() = 0;
   virtual CommandType getCommandType() = 0;
   virtual int getResult() = 0;
   virtual const std::string& getResultQuery() = 0;
   virtual const std::vector<std::string> getResultsQuery() = 0;
};

class ICommandListener
{
public:
   virtual ~ICommandListener() {}
   virtual void notifyReady(ICommand* const command) = 0;
};

#endif
