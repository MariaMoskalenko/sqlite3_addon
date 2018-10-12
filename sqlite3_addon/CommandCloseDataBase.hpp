#ifndef COMMANDCLOSEDATABASE_H
#define COMMANDCLOSEDATABASE_H
#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "ICommand.hpp"
#include "CSqlite3Wrapper.hpp"

class CommandQueue;

using std::string;
using std::function;
using std::shared_ptr;

class CommandCloseDataBase : public ICommand
{
public:
    CommandCloseDataBase(CommandQueue& parent, CSqlite3Wrapper& sqlWrapper);
    ~CommandCloseDataBase() override;
    void execute() override;
    CommandType getCommandType() override;
    int getResult() override;
    const std::string& getResultQuery() override;

private:
    CommandQueue& mParent;
    CSqlite3Wrapper& mSqlWrapper;
    int mResult;
    std::string mResultQuery;
};

inline CommandType CommandCloseDataBase::getCommandType()
{
    return CommandType::CommandCloseDataBase;
}

inline int CommandCloseDataBase::getResult()
{
    return mResult;
}

inline const std::string& CommandCloseDataBase::getResultQuery()
{
    return mResultQuery;
}

#endif
