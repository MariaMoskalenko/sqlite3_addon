#ifndef COMMANDOPENDATABASE_H
#define COMMANDOPENDATABASE_H
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

class CommandOpenDataBase : public ICommand
{
public:
    CommandOpenDataBase(CommandQueue& parent, CSqlite3Wrapper& sqlWrapper,
                        const std::string& fileName);
    ~CommandOpenDataBase() override;
    void execute() override;
    CommandType getCommandType() override;
    int getResult() override;
    const std::string& getResultQuery() override;

private:
    CommandQueue& mParent;
    CSqlite3Wrapper& mSqlWrapper;
    int mResult;
    std::string mResultQuery;
    std::string mFileName;
};

inline CommandType CommandOpenDataBase::getCommandType()
{
    return CommandType::CommandOpenDataBase;
}

inline int CommandOpenDataBase::getResult()
{
    return mResult;
}

inline const std::string& CommandOpenDataBase::getResultQuery()
{
    return mResultQuery;
}

#endif
