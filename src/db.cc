#include <nogdb/nogdb.h>

#include "txn.hpp"

#include "db.hpp"

#include "toV8type.hpp"

NAN_MODULE_INIT(Db::Init)
{
    v8::Handle<v8::Object> d = v8::Object::New(v8::Isolate::GetCurrent());

    Nan::SetMethod(d, "getRecord", getRecord);
    Nan::SetMethod(d, "getSchema", getSchema);
    Nan::SetMethod(d, "getDbInfo", getDbInfo);

    target->Set(Nan::New("Db").ToLocalChecked(), d);
}

NAN_METHOD(Db::getRecord)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 2 && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);
        try {
            nogdb::Record record = nogdb::Db::getRecord(*txn->base, recordD);
            info.GetReturnValue().Set(v8Record(record));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Db.getRecord() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Db::getSchema)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (!txnType->HasInstance(info[0]->ToObject())){
        return Nan::ThrowError(Nan::New("Db.getSchema() - invalid arugment(s)").ToLocalChecked());
    }
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
    // ver 1 only txn
    if(info.Length() == 1){
        try {
            std::vector<nogdb::ClassDescriptor> classDs = nogdb::Db::getSchema(*txn->base);
            v8::Local<v8::Array> retval = Nan::New<v8::Array>(classDs.size());
            int i = 0;
            for(std::vector<nogdb::ClassDescriptor>::iterator it = classDs.begin(); it != classDs.end(); ++it)
            {
                Nan::Set(retval, i, v8ClassDescriptor(*it));
                i++;
            }
            info.GetReturnValue().Set(retval);
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    // ver 2 txn & className
    else if (info.Length() == 2 && info[1]->IsString()){
        std::string className = *Nan::Utf8String(info[1]->ToString());
        try {
            nogdb::ClassDescriptor classD = nogdb::Db::getSchema(*txn->base,className);
            info.GetReturnValue().Set(v8ClassDescriptor(classD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    } 
    // ver 3 txn & classId
    else if (info.Length() == 2 && info[1]->IsUint32()){
        unsigned int classId = info[1]->Uint32Value();
        try {
            nogdb::ClassDescriptor classD = nogdb::Db::getSchema(*txn->base,classId);
            info.GetReturnValue().Set(v8ClassDescriptor(classD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    } else {
        return Nan::ThrowError(Nan::New("Db.getSchema() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Db::getDbInfo)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 1 && txnType->HasInstance(info[0]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        try {
            nogdb::DBInfo dbInfo = nogdb::Db::getDbInfo(*txn->base);
            info.GetReturnValue().Set(v8DBInfo(dbInfo));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    } 
    else {
        return Nan::ThrowError(Nan::New("Db.getDbInfo() - invalid arugment(s)").ToLocalChecked());
    }
}