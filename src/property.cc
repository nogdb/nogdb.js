#include <memory>
#include <nan.h>
#include <nogdb/nogdb.h>

#include "txn.hpp"
#include "toV8type.hpp"

#include "property.hpp"

NAN_MODULE_INIT(Property::Init)
{
    v8::Handle<v8::Object> p = v8::Object::New(v8::Isolate::GetCurrent());

    Nan::SetMethod(p, "add", add);
    Nan::SetMethod(p, "alter", alter);
    Nan::SetMethod(p, "remove", remove);
    Nan::SetMethod(p, "createIndex", createIndex);
    Nan::SetMethod(p, "dropIndex", dropIndex);

    target->Set(Nan::New("Property").ToLocalChecked(), p);
}

NAN_METHOD(Property::add)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 4  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsString() 
                            && info[2]->IsString() && info[3]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        std::string className = *Nan::Utf8String(info[1]->ToString());
        std::string propertyName = *Nan::Utf8String(info[2]->ToString());
        std::string type = *Nan::Utf8String(info[3]->ToString());

        nogdb::PropertyType propertyType;
        if(type=="TINYINT")                 propertyType = nogdb::PropertyType::TINYINT;
        else if(type=="UNSIGNED_TINYINT")   propertyType = nogdb::PropertyType::UNSIGNED_TINYINT;
        else if(type=="SMALLINT")           propertyType = nogdb::PropertyType::SMALLINT;
        else if(type=="UNSIGNED_SMALLINT")  propertyType = nogdb::PropertyType::UNSIGNED_SMALLINT;
        else if(type=="INTEGER")            propertyType = nogdb::PropertyType::INTEGER;
        else if(type=="UNSIGNED_INTEGER")   propertyType = nogdb::PropertyType::UNSIGNED_INTEGER;
        else if(type=="BIGINT")             propertyType = nogdb::PropertyType::BIGINT;
        else if(type=="UNSIGNED_BIGINT")    propertyType = nogdb::PropertyType::UNSIGNED_BIGINT;
        else if(type=="TEXT")               propertyType = nogdb::PropertyType::TEXT;
        else if(type=="REAL")               propertyType = nogdb::PropertyType::REAL;
        else if(type=="BLOB")               propertyType = nogdb::PropertyType::BLOB;
        else Nan::ThrowError("propertyType Invalid");
        
        nogdb::PropertyDescriptor propD = nogdb::Property::add(*txn->base,className,propertyName,propertyType);
        info.GetReturnValue().Set(v8PropertyDescriptor(propD));
    }
    else
    {
        return Nan::ThrowError(Nan::New("Property.add() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Property::alter)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 4  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsString() 
                            && info[2]->IsString() && info[3]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        std::string className = *Nan::Utf8String(info[1]->ToString());
        std::string oldPropertyName = *Nan::Utf8String(info[2]->ToString());
        std::string newPropertyName = *Nan::Utf8String(info[3]->ToString());
        
        nogdb::Property::alter(*txn->base,className,oldPropertyName,newPropertyName);

        info.GetReturnValue().SetUndefined();
    }
    else
    {
        return Nan::ThrowError(Nan::New("Property.alter() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Property::remove)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 3  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsString() 
                            && info[2]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        std::string className = *Nan::Utf8String(info[1]->ToString());
        std::string propertyName = *Nan::Utf8String(info[2]->ToString());
        
        nogdb::Property::remove(*txn->base,className,propertyName);

        info.GetReturnValue().SetUndefined();
    }
    else
    {
        return Nan::ThrowError(Nan::New("Property.remove() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Property::createIndex)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 4  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsString() 
                            && info[2]->IsString() && info[3]->IsBoolean())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        std::string className = *Nan::Utf8String(info[1]->ToString());
        std::string propertyName = *Nan::Utf8String(info[2]->ToString());
        bool isUnique = info[3]->BooleanValue();

        nogdb::Property::createIndex(*txn->base,className,propertyName,isUnique);

        info.GetReturnValue().SetUndefined();
    }
    else
    {
        return Nan::ThrowError(Nan::New("Property.createIndex() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Property::dropIndex)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 3  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsString() 
                            && info[2]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        std::string className = *Nan::Utf8String(info[1]->ToString());
        std::string propertyName = *Nan::Utf8String(info[2]->ToString());

        nogdb::Property::dropIndex(*txn->base,className,propertyName);

        info.GetReturnValue().SetUndefined();
    }
    else
    {
        return Nan::ThrowError(Nan::New("Property.dropIndex() - invalid arugment(s)").ToLocalChecked());
    } 
}
