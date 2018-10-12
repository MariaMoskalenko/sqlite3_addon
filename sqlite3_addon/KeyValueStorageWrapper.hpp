#ifndef KEYVALUESTORAGEWRAPPER_H
#define KEYVALUESTORAGEWRAPPER_H
#pragma once

#include <nan.h>
#include <v8.h>
#include <node.h>
#include <stdio.h>
#include <string>
#include "CommandQueue.hpp"
#include "ICommand.hpp"

class KeyValueStorageWrapper: public Nan::ObjectWrap, public ICommandListener
{
public:
    static NAN_MODULE_INIT(Initialize);
    CommandQueue* GetWrapped();
    void notifyReady(ICommand* const command);

    typedef std::function<void(KeyValueStorageWrapper)> NotifyOpenEvent;
    NotifyOpenEvent onNotifyOpen;
    typedef std::function<void(KeyValueStorageWrapper)> NotifyExecuteEvent;
    NotifyExecuteEvent onNotifyExecute;
    typedef std::function<void(KeyValueStorageWrapper)> NotifyCloseEvent;
    NotifyCloseEvent onNotifyClose;

private:
    explicit KeyValueStorageWrapper(const Nan::FunctionCallbackInfo<v8::Value>& info);
    ~ KeyValueStorageWrapper();

    static NAN_METHOD(New);
    static NAN_METHOD(openDB);
    static NAN_METHOD(executeQuery);
    static NAN_METHOD(closeDB);

    static inline Nan::Persistent<v8::Function> & constructor() {
        static Nan::Persistent<v8::Function> mConstructor;
        return mConstructor;
    }

    CommandQueue mWrappedObject;
};
#endif
