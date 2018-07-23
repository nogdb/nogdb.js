#include <memory>
#include <nan.h>
#include <nogdb/nogdb.h>

#include "condition.hpp"
#include "record.hpp"
#include "txn.hpp"
#include "toV8type.hpp"

#include "edge.hpp"

NAN_MODULE_INIT(Edge::Init)
{
    v8::Handle<v8::Object> e = v8::Object::New(v8::Isolate::GetCurrent());

    Nan::SetMethod(e, "create", create);
    Nan::SetMethod(e, "update", update);
    Nan::SetMethod(e, "updateSrc", updateSrc);
    Nan::SetMethod(e, "updateDst", updateDst);
    Nan::SetMethod(e, "destroy", destroy);
    Nan::SetMethod(e, "get", get);
    Nan::SetMethod(e, "getSrc", getSrc);
    Nan::SetMethod(e, "getDst", getDst);
    Nan::SetMethod(e, "getSrcDst", getSrcDst);
    Nan::SetMethod(e, "getIndex", getIndex);

    target->Set(Nan::New("Edge").ToLocalChecked(), e);
}

NAN_METHOD(Edge::create)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> recordType = Nan::New<v8::FunctionTemplate>(Record::constructor);
    if (info.Length() == 5  && txnType->HasInstance(info[0]->ToObject()) 
                            && info[1]->IsString() && info[2]->IsObject()
                            && info[3]->IsObject() && recordType->HasInstance(info[4]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        std::string className = *Nan::Utf8String(info[1]->ToString());

        v8::Local<v8::Object> srcRecD = info[2]->ToObject();
        v8::Local<v8::Object> srcRid = Nan::Get(srcRecD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int srcClassId = Nan::Get(srcRid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int srcPositionId = Nan::Get(srcRid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor srcRecordD(srcClassId,srcPositionId);

        v8::Local<v8::Object> dstRecD = info[3]->ToObject();
        v8::Local<v8::Object> dstRid = Nan::Get(dstRecD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int dstClassId = Nan::Get(dstRid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int dstPositionId = Nan::Get(dstRid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor dstRecordD(dstClassId,dstPositionId);

        Record *record = Nan::ObjectWrap::Unwrap<Record>(info[4]->ToObject());

        nogdb::RecordDescriptor recD = nogdb::Edge::create(*txn->base, className,srcRecordD,dstRecordD, record->base);
        info.GetReturnValue().Set(v8RecordDescriptor(recD));
    }
    else
    {
        return Nan::ThrowError(Nan::New("Edge.create() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Edge::update)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> recordType = Nan::New<v8::FunctionTemplate>(Record::constructor);
    if (info.Length() == 3  && txnType->HasInstance(info[0]->ToObject()) 
                            && info[1]->IsObject() && recordType->HasInstance(info[2]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        Record *record = Nan::ObjectWrap::Unwrap<Record>(info[2]->ToObject());
        nogdb::Edge::update(*txn->base, recordD, record->base);
        info.GetReturnValue().SetUndefined();
    }
    else
    {
        return Nan::ThrowError(Nan::New("Edge.update() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Edge::updateSrc)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 3  && txnType->HasInstance(info[0]->ToObject()) 
                            && info[1]->IsObject() && info[2]->IsObject())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        v8::Local<v8::Object> srcRecD = info[2]->ToObject();
        v8::Local<v8::Object> srcRid = Nan::Get(srcRecD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int srcClassId = Nan::Get(srcRid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int srcPositionId = Nan::Get(srcRid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor srcRecordD(srcClassId,srcPositionId);

        nogdb::Edge::updateSrc(*txn->base, recordD, srcRecordD);
        info.GetReturnValue().SetUndefined();
    }
    else
    {
        return Nan::ThrowError(Nan::New("Edge.updateSrc() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Edge::updateDst)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (info.Length() == 3  && txnType->HasInstance(info[0]->ToObject()) 
                            && info[1]->IsObject() && info[2]->IsObject())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        v8::Local<v8::Object> dstRecD = info[2]->ToObject();
        v8::Local<v8::Object> dstRid = Nan::Get(dstRecD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int dstClassId = Nan::Get(dstRid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int dstPositionId = Nan::Get(dstRid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor dstRecordD(dstClassId,dstPositionId);

        nogdb::Edge::updateDst(*txn->base, recordD, dstRecordD);
        info.GetReturnValue().SetUndefined();
    }
    else
    {
        return Nan::ThrowError(Nan::New("Edge.updateDst() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Edge::destroy)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    if (!txnType->HasInstance(info[0]->ToObject())){
        return Nan::ThrowError(Nan::New("Edge::destroy - invalid arugment(s)").ToLocalChecked());
    }
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
    // ver 1 recordD
    if(info[1]->IsObject()){
        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        nogdb::Edge::destroy(*txn->base, recordD);
    }
    // ver 2 className
    else if(info[1]->IsString()){
        std::string className = *Nan::Utf8String(info[1]->ToString());
        nogdb::Edge::destroy(*txn->base, className);
    } else {
        return Nan::ThrowError(Nan::New("Edge.destroy() - invalid arugment(s)").ToLocalChecked());
    }
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Edge::get)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> conditionType = Nan::New<v8::FunctionTemplate>(Condition::constructor);
    if (!txnType->HasInstance(info[0]->ToObject()) && !info[1]->IsString()){
        return Nan::ThrowError(Nan::New("Edge::get - invalid arugment(s)").ToLocalChecked());
    }
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
    std::string className = *Nan::Utf8String(info[1]->ToString());
    // get
    if(info.Length() == 2){
        nogdb::ResultSet resultSet = nogdb::Edge::get(*txn->base,className);
        info.GetReturnValue().Set(v8ResultSet(resultSet));
    }
    // get with condition
    else if(info.Length() == 3 && conditionType->HasInstance(info[2]->ToObject())){
        Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info[2]->ToObject());
        nogdb::ResultSet resultSet = nogdb::Edge::get(*txn->base,className,cond->base);
        info.GetReturnValue().Set(v8ResultSet(resultSet));
    }else {
        return Nan::ThrowError(Nan::New("Edge.get() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Edge::getSrc)
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

        nogdb::Result result = nogdb::Edge::getSrc(*txn->base,recordD);
        info.GetReturnValue().Set(v8Result(result));
    }
    else
    {
        return Nan::ThrowError(Nan::New("Edge.getSrc() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Edge::getDst)
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

        nogdb::Result result = nogdb::Edge::getDst(*txn->base,recordD);
        info.GetReturnValue().Set(v8Result(result));
    }
    else
    {
        return Nan::ThrowError(Nan::New("Edge.getDst() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Edge::getSrcDst)
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

        nogdb::ResultSet resultSet = nogdb::Edge::getSrcDst(*txn->base,recordD);
        info.GetReturnValue().Set(v8ResultSet(resultSet));
    }
    else
    {
        return Nan::ThrowError(Nan::New("Edge.getSrcDst() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Edge::getIndex)
{
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> conditionType = Nan::New<v8::FunctionTemplate>(Condition::constructor);
    if (info.Length() == 3  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsString()
                            && conditionType->HasInstance(info[2]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());
        std::string className = *Nan::Utf8String(info[1]->ToString());
        Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info[2]->ToObject());

        nogdb::ResultSet resultSet = nogdb::Edge::getIndex(*txn->base,className,cond->base);
        info.GetReturnValue().Set(v8ResultSet(resultSet));
    }
    else
    {
        return Nan::ThrowError(Nan::New("Edge.getIndex() - invalid arugment(s)").ToLocalChecked());
    } 
}

