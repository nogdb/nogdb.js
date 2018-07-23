#include <nan.h>

#include "classFilter.hpp"
#include "condition.hpp"
#include "context.hpp"
#include "record.hpp"
#include "txn.hpp"
#include "class.hpp"
#include "property.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "db.hpp"
#include "traverse.hpp"

NAN_METHOD(newRecord) {
    info.GetReturnValue().Set(Record::NewInstance());
}

NAN_METHOD(newClassFilter) {
    if (info.Length() == 1 && info[0]->IsArray())
    {
        info.GetReturnValue().Set(ClassFilter::NewInstance(info[0]));
    }
    else
    {
        return Nan::ThrowError(Nan::New("new ClassFilter() - invalid arugment(s)").ToLocalChecked());
    }
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

NAN_METHOD(newTxn) {
    v8::Local<v8::FunctionTemplate> contextType = Nan::New<v8::FunctionTemplate>(Context::constructor);
    if (info.Length() == 2 && contextType->HasInstance(info[0]->ToObject()) && info[1]->IsString())
    {
        info.GetReturnValue().Set(Txn::NewInstance(info[0],info[1]));
    }
    else
    {
        return Nan::ThrowError(Nan::New("new Txn() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_MODULE_INIT(InitModule)
{
    target->Set(Nan::New("newRecord").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(newRecord)->GetFunction());

    target->Set(Nan::New("newClassFilter").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(newClassFilter)->GetFunction());

    target->Set(Nan::New("newCondition").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(newCondition)->GetFunction());
    
    target->Set(Nan::New("newContext").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(newContext)->GetFunction());

    target->Set(Nan::New("newTxn").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(newTxn)->GetFunction());

    ClassFilter::Init(target);
    Condition::Init(target);
    Context::Init(target);
    Record::Init(target);
    Txn::Init(target);
    
    Class::Init(target);
    Property::Init(target);
    Vertex::Init(target);
    Edge::Init(target);
    Db::Init(target);
    Traverse::Init(target);
}

NODE_MODULE(nogdb_js, InitModule);