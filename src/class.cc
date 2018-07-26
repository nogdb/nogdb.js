#include <memory>
#include <nan.h>
#include <nogdb/nogdb.h>

#include "txn.hpp"

#include "class.hpp"

#include "toV8type.hpp"

NAN_MODULE_INIT(Class::Init)
{
    v8::Handle<v8::Object> c = v8::Object::New(v8::Isolate::GetCurrent());

    Nan::SetMethod(c, "create", create);
    Nan::SetMethod(c, "createExtend", createExtend);
    Nan::SetMethod(c, "drop", drop);
    Nan::SetMethod(c, "alter", alter);

    target->Set(Nan::New("Class").ToLocalChecked(), c);
}

NAN_METHOD(Class::create)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 3 && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsString() && info[2]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        std::string className = *Nan::Utf8String(info[1]->ToString());
        std::string type = *Nan::Utf8String(info[2]->ToString());

        nogdb::ClassType classType = nogdb::ClassType::VERTEX;
        if(type=="VERTEX")      classType = nogdb::ClassType::VERTEX;
        else if(type=="EDGE")   classType = nogdb::ClassType::EDGE;
        else Nan::ThrowError("ClassType Invalid");
        try {
            nogdb::ClassDescriptor classD = nogdb::Class::create(*txn->base, className, classType);
            info.GetReturnValue().Set(v8ClassDescriptor(classD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Class.create() - invalid arugment(s)").ToLocalChecked());
    }    
}

NAN_METHOD(Class::createExtend)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 3 && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsString() && info[2]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        std::string className = *Nan::Utf8String(info[1]->ToString());
        std::string superClass = *Nan::Utf8String(info[2]->ToString());
        try {
            nogdb::ClassDescriptor classD = nogdb::Class::createExtend(*txn->base, className, superClass);
            info.GetReturnValue().Set(v8ClassDescriptor(classD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Class.createExtend() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Class::drop)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 2 && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        std::string className = *Nan::Utf8String(info[1]->ToString());
        try {
            nogdb::Class::drop(*txn->base, className);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Class.drop() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Class::alter)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 3 && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsString() && info[2]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        std::string oldClassName = *Nan::Utf8String(info[1]->ToString());
        std::string newClassName = *Nan::Utf8String(info[2]->ToString());
        try {
            nogdb::Class::alter(*txn->base, oldClassName, newClassName);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
        
    }
    else
    {
        return Nan::ThrowError(Nan::New("Class.alter() - invalid arugment(s)").ToLocalChecked());
    } 
}
