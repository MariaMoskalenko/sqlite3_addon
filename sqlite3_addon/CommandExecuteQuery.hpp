#ifndef COMMANDEXECUTEQUERY_H
#define COMMANDEXECUTEQUERY_H
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

class CommandExecuteQuery : public ICommand
{
public:
    CommandExecuteQuery(CommandQueue& parent, CSqlite3Wrapper& sqlWrapper, const std::string& query);
    ~CommandExecuteQuery() override;
    void execute() override;
    CommandType getCommandType() override;
    int getResult() override;
    const std::string& getResultQuery() override;

private:
    CommandQueue& mParent;
    std::string mQuery;
    CSqlite3Wrapper& mSqlWrapper;
    int mResult;
    std::string mResultQuery;
};

inline CommandType CommandExecuteQuery::getCommandType()
{
    return CommandType::CommandExecuteQuery;
}

inline int CommandExecuteQuery::getResult()
{
    return mResult;
}

inline const std::string& CommandExecuteQuery::getResultQuery()
{
    return mResultQuery;
}

#endif
