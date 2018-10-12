#include "KeyValueStorageWrapper.hpp"

using namespace Nan;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Value;

static std::string resExec = "";
static int resExecError = 0;

std::string toStdString(v8::Local<v8::String> str) {
   return std::string(*v8::String::Utf8Value(str));
}

v8::Local<v8::String> fromStdString(std::string str) {
   return Nan::New(str.data()).ToLocalChecked();
}

KeyValueStorageWrapper::KeyValueStorageWrapper(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    mWrappedObject.registerListener(this);
}

KeyValueStorageWrapper::~KeyValueStorageWrapper()
{
    mWrappedObject.unRegisterListener(this);
}

CommandQueue* KeyValueStorageWrapper::GetWrapped()
{
    return &mWrappedObject;
}

void KeyValueStorageWrapper::notifyReady(ICommand* const command)
{
    printf("notifyReady cmdId=%d\n", command->getCommandType());
    resExecError = command->getResult();
    resExec = command->getResultQuery();
    printf("notifyReady resExecError=%d, resExec=%s\n", resExecError, resExec.c_str());
    switch(command->getCommandType()) {
    case CommandType::CommandOpenDataBase:
        onNotifyOpen;
        break;
    case CommandType::CommandExecuteQuery:
        onNotifyExecute;
        break;
    case CommandType::CommandCloseDataBase:
        onNotifyClose;
        break;
    default:
        break;
    }
}

NAN_MODULE_INIT(KeyValueStorageWrapper::Initialize)
{
    printf("KeyValueStorageWrapper::Initialize\n");
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("KeyValueStorageWrapper").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    SetPrototypeMethod(tpl, "openDB", openDB);
    SetPrototypeMethod(tpl, "executeQuery", executeQuery);
    SetPrototypeMethod(tpl, "closeDB", closeDB);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Set(target, Nan::New("CommandQueue").ToLocalChecked()
        , GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(KeyValueStorageWrapper::New)
{
    printf("KeyValueStorageWrapper::New\n");
    if (info.IsConstructCall()) {
        KeyValueStorageWrapper* wrapper = new  KeyValueStorageWrapper(info);
        wrapper->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
        printf("info.IsConstructCall=%d\n", info.IsConstructCall());
    }
    else
    {
        const int lenght = info.Length();
        Local<Value>* argv = new Local<Value>[lenght];
        for(int i = 0; i < lenght; i++)
        {
            argv[i] = info[i];
            printf("adding argv[%d] %s\n", i, toStdString(argv[i]->ToString()).c_str());
        }
        Local<Function> constr = Nan::New(constructor());
        info.GetReturnValue().Set(Nan::NewInstance(constr, lenght, argv).ToLocalChecked());
        delete[] argv;
    }
}

NAN_METHOD(KeyValueStorageWrapper::openDB)
{
    KeyValueStorageWrapper* wrapper = Nan::ObjectWrap::Unwrap<KeyValueStorageWrapper>(info.This());
    CommandQueue* wrappedObject = wrapper->GetWrapped();
    if (wrappedObject != nullptr) {
         std::string fileName(toStdString(info[0]->ToObject()->ToString()).c_str());
         wrappedObject->addCommandOpenDataBase(fileName);
         auto result = std::bind(wrapper->onNotifyOpen, wrapper, std::placeholders::_1, std::placeholders::_2);
         printf("addCommandOpenDataBase result: %s, %d\n", resExec.c_str(), resExecError);
         info.GetReturnValue().Set(fromStdString(resExec));
    }
}

NAN_METHOD(KeyValueStorageWrapper::executeQuery)
{
   KeyValueStorageWrapper* wrapper = Nan::ObjectWrap::Unwrap<KeyValueStorageWrapper>(info.This());
   CommandQueue* wrappedObject = wrapper->GetWrapped();
   if (wrappedObject != nullptr) {
        printf("addCommandExecuteQuery query: %s\n", toStdString(info[0]->ToObject()->ToString()).c_str());
        wrappedObject->addCommandExecuteQuery(toStdString(info[0]->ToObject()->ToString()));
        auto result  = std::bind(wrapper->onNotifyExecute, wrapper, std::placeholders::_1, std::placeholders::_2);
        printf("addCommandExecuteQuery result: %d\n", resExecError);
        info.GetReturnValue().Set(fromStdString(resExec));
   }
}

NAN_METHOD(KeyValueStorageWrapper::closeDB)
{
   KeyValueStorageWrapper* wrapper = Nan::ObjectWrap::Unwrap<KeyValueStorageWrapper>(info.This());
   CommandQueue* wrappedObject = wrapper->GetWrapped();
   if (wrappedObject != nullptr) {
        wrappedObject->addCommandCloseDataBase();
        auto result  = std::bind(wrapper->onNotifyClose, wrapper, std::placeholders::_1, std::placeholders::_2);
        printf("addCommandCloseDataBase result: %s, %d\n", resExec.c_str(), resExecError);
        info.GetReturnValue().Set(fromStdString(resExec));
   }
}
