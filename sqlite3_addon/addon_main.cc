#include <nan.h>
#include <node.h>
#include <v8.h>
#include "KeyValueStorageWrapper.hpp"

NAN_MODULE_INIT(Initialize)
{
   KeyValueStorageWrapper::Initialize(target);
}

NODE_MODULE(sqlite3_addon, Initialize)
