#include <nogdb/nogdb.h>

#include "classFilter.hpp"
#include "txn.hpp"
#include "toV8type.hpp"

#include "traverse.hpp"

NAN_MODULE_INIT(Traverse::Init)
{
    v8::Handle<v8::Object> t = v8::Object::New(v8::Isolate::GetCurrent());

    Nan::SetMethod(t, "inEdgeBfs", inEdgeBfs);
    Nan::SetMethod(t, "outEdgeBfs", outEdgeBfs);
    Nan::SetMethod(t, "allEdgeBfs", allEdgeBfs);
    Nan::SetMethod(t, "inEdgeDfs", inEdgeDfs);
    Nan::SetMethod(t, "outEdgeDfs", outEdgeDfs);
    Nan::SetMethod(t, "allEdgeDfs", allEdgeDfs);
    Nan::SetMethod(t, "shortestPath", shortestPath);

    target->Set(Nan::New("Traverse").ToLocalChecked(), t);
}

NAN_METHOD(Traverse::inEdgeBfs)
{
    v8::Local<v8::FunctionTemplate> classFilterType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(ClassFilter::constructor);
    if (info.Length() == 5  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32()
                            && classFilterType->HasInstance(info[4]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info[4]->ToObject());
        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::inEdgeBfs(*txn->base,recordD,minDepth,maxDepth,classF->base);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else if (info.Length() == 4  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::inEdgeBfs(*txn->base,recordD,minDepth,maxDepth);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Traverse.inEdgeBfs() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Traverse::outEdgeBfs)
{
    v8::Local<v8::FunctionTemplate> classFilterType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(ClassFilter::constructor);
    if (info.Length() == 5  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32()
                            && classFilterType->HasInstance(info[4]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info[4]->ToObject());
        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::outEdgeBfs(*txn->base,recordD,minDepth,maxDepth,classF->base);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
        
    }
    else if (info.Length() == 4  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::outEdgeBfs(*txn->base,recordD,minDepth,maxDepth);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Traverse.outEdgeBfs() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Traverse::allEdgeBfs)
{
    v8::Local<v8::FunctionTemplate> classFilterType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(ClassFilter::constructor);
    if (info.Length() == 5  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32()
                            && classFilterType->HasInstance(info[4]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info[4]->ToObject());
        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::allEdgeBfs(*txn->base,recordD,minDepth,maxDepth,classF->base);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else if (info.Length() == 4  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::allEdgeBfs(*txn->base,recordD,minDepth,maxDepth);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Traverse.allEdgeBfs() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Traverse::inEdgeDfs)
{
    v8::Local<v8::FunctionTemplate> classFilterType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(ClassFilter::constructor);
    if (info.Length() == 5  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32()
                            && classFilterType->HasInstance(info[4]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info[4]->ToObject());
        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::inEdgeDfs(*txn->base,recordD,minDepth,maxDepth,classF->base);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else if (info.Length() == 4  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::inEdgeDfs(*txn->base,recordD,minDepth,maxDepth);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Traverse.inEdgeDfs() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Traverse::outEdgeDfs)
{
    v8::Local<v8::FunctionTemplate> classFilterType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(ClassFilter::constructor);
    if (info.Length() == 5  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32()
                            && classFilterType->HasInstance(info[4]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info[4]->ToObject());
        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::outEdgeDfs(*txn->base,recordD,minDepth,maxDepth,classF->base);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else if (info.Length() == 4  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::outEdgeDfs(*txn->base,recordD,minDepth,maxDepth);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Traverse.outEdgeDfs() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Traverse::allEdgeDfs)
{
    v8::Local<v8::FunctionTemplate> classFilterType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(ClassFilter::constructor);
    if (info.Length() == 5  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32()
                            && classFilterType->HasInstance(info[4]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info[4]->ToObject());
        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::allEdgeDfs(*txn->base,recordD,minDepth,maxDepth,classF->base);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else if (info.Length() == 4  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsUint32() && info[3]->IsUint32())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> recD = info[1]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        unsigned int minDepth = info[2]->Uint32Value();
        unsigned int maxDepth = info[3]->Uint32Value();

        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::allEdgeDfs(*txn->base,recordD,minDepth,maxDepth);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Traverse.allEdgeDfs() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Traverse::shortestPath)
{
    v8::Local<v8::FunctionTemplate> classFilterType = Nan::New<v8::FunctionTemplate>(Txn::constructor);
    v8::Local<v8::FunctionTemplate> txnType = Nan::New<v8::FunctionTemplate>(ClassFilter::constructor);
    if (info.Length() == 4  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsObject() && classFilterType->HasInstance(info[3]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> srcRecD = info[1]->ToObject();
        v8::Local<v8::Object> srcRid = Nan::Get(srcRecD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int srcClassId = Nan::Get(srcRid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int srcPositionId = Nan::Get(srcRid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor srcRecordD(srcClassId,srcPositionId);

        v8::Local<v8::Object> dstRecD = info[2]->ToObject();
        v8::Local<v8::Object> dstRid = Nan::Get(dstRecD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int dstClassId = Nan::Get(dstRid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int dstPositionId = Nan::Get(dstRid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor dstRecordD(dstClassId,dstPositionId);

        ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info[3]->ToObject());
        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::shortestPath(*txn->base,srcRecordD,dstRecordD,classF->base);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else if (info.Length() == 3  && txnType->HasInstance(info[0]->ToObject()) && info[1]->IsObject() 
                            && info[2]->IsObject())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info[0]->ToObject());

        v8::Local<v8::Object> srcRecD = info[1]->ToObject();
        v8::Local<v8::Object> srcRid = Nan::Get(srcRecD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int srcClassId = Nan::Get(srcRid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int srcPositionId = Nan::Get(srcRid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor srcRecordD(srcClassId,srcPositionId);

        v8::Local<v8::Object> dstRecD = info[2]->ToObject();
        v8::Local<v8::Object> dstRid = Nan::Get(dstRecD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int dstClassId = Nan::Get(dstRid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int dstPositionId = Nan::Get(dstRid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor dstRecordD(dstClassId,dstPositionId);

        try {
            nogdb::ResultSet resultSet = nogdb::Traverse::shortestPath(*txn->base,srcRecordD,dstRecordD);
            info.GetReturnValue().Set(v8ResultSet(resultSet));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Traverse.shortestPath() - invalid arugment(s)").ToLocalChecked());
    } 
}
