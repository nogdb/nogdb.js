#include <nan.h>

#include "context.hpp"
#include "txn.hpp"
#include "condition.hpp"
#include "record.hpp"

NAN_METHOD(newRecord) {
    info.GetReturnValue().Set(Record::NewInstance());
}

NAN_METHOD(newCondition) {
    if (info.Length() == 1 && info[0]->IsString())
    {
        info.GetReturnValue().Set(Condition::NewInstance(info[0]));
    }
    else
    {
        return Nan::ThrowError(Nan::New("new Condition() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(newContext) {
    if (info.Length() == 1 && info[0]->IsString())
    {
        info.GetReturnValue().Set(Context::NewInstance(info[0]));
    }
    else
    {
        return Nan::ThrowError(Nan::New("new Context() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_MODULE_INIT(InitModule)
{
    target->Set(Nan::New("newRecord").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(newRecord)->GetFunction());

    target->Set(Nan::New("newCondition").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(newCondition)->GetFunction());
    
    target->Set(Nan::New("newContext").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(newContext)->GetFunction());

    Condition::Init(target);
    Context::Init(target);
    Record::Init(target);
}

NODE_MODULE(nogdb_js, InitModule);