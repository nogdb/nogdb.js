#include <memory>
#include <nan.h>
#include <nogdb/nogdb.h>

#include "context.hpp"
#include "txn.hpp"
#include "record.hpp"
#include "toV8type.hpp"

Nan::Persistent<v8::FunctionTemplate> Txn::constructor;

NAN_MODULE_INIT(Txn::Init)
{
    v8::Local<v8::FunctionTemplate> constructTemplate =
        Nan::New<v8::FunctionTemplate>(Txn::New);

    constructor.Reset(constructTemplate);
    constructTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    constructTemplate->SetClassName(Nan::New("Txn").ToLocalChecked());

    Nan::SetPrototypeMethod(constructTemplate, "commit", Txn::commit);
    Nan::SetPrototypeMethod(constructTemplate, "rollback", Txn::rollback);
    Nan::SetPrototypeMethod(constructTemplate, "getTxnMode", Txn::getTxnMode);
    Nan::SetPrototypeMethod(constructTemplate, "isCompleted", Txn::isCompleted);
    Nan::SetPrototypeMethod(constructTemplate, "addClass", Txn::addClass);
    Nan::SetPrototypeMethod(constructTemplate, "addSubClassOf", Txn::addSubClassOf);
    Nan::SetPrototypeMethod(constructTemplate, "dropClass", Txn::dropClass);
    Nan::SetPrototypeMethod(constructTemplate, "renameClass", Txn::renameClass);
    Nan::SetPrototypeMethod(constructTemplate, "addProperty", Txn::addProperty);
    Nan::SetPrototypeMethod(constructTemplate, "renameProperty", Txn::renameProperty);
    Nan::SetPrototypeMethod(constructTemplate, "dropProperty", Txn::dropProperty);
    Nan::SetPrototypeMethod(constructTemplate, "addIndex", Txn::addIndex);
    Nan::SetPrototypeMethod(constructTemplate, "dropIndex", Txn::dropIndex);
    Nan::SetPrototypeMethod(constructTemplate, "getDbInfo", Txn::getDbInfo);
    Nan::SetPrototypeMethod(constructTemplate, "getClasses", Txn::getClasses);
    Nan::SetPrototypeMethod(constructTemplate, "getPropertiesByClassName", Txn::getPropertiesByClassName);
    Nan::SetPrototypeMethod(constructTemplate, "getPropertiesByClassDescriptor", Txn::getPropertiesByClassDescriptor);
    Nan::SetPrototypeMethod(constructTemplate, "getClassByName", Txn::getClassByName);
    Nan::SetPrototypeMethod(constructTemplate, "getClassById", Txn::getClassById);
    Nan::SetPrototypeMethod(constructTemplate, "getProperty", Txn::getProperty);
    Nan::SetPrototypeMethod(constructTemplate, "getIndex", Txn::getIndex);
    Nan::SetPrototypeMethod(constructTemplate, "fetchRecord", Txn::fetchRecord);
    Nan::SetPrototypeMethod(constructTemplate, "addVertex", Txn::addVertex);
    Nan::SetPrototypeMethod(constructTemplate, "addEdge", Txn::addEdge);
    Nan::SetPrototypeMethod(constructTemplate, "update", Txn::update);
    Nan::SetPrototypeMethod(constructTemplate, "updateSrc", Txn::updateSrc);
    Nan::SetPrototypeMethod(constructTemplate, "updateDst", Txn::updateDst);
    Nan::SetPrototypeMethod(constructTemplate, "remove", Txn::remove);
    Nan::SetPrototypeMethod(constructTemplate, "removeAll", Txn::removeAll);
    Nan::SetPrototypeMethod(constructTemplate, "find", Txn::find);
    Nan::SetPrototypeMethod(constructTemplate, "findSubClassOf", Txn::findSubClassOf);
    Nan::SetPrototypeMethod(constructTemplate, "findInEdge", Txn::findInEdge);
    Nan::SetPrototypeMethod(constructTemplate, "findOutEdge", Txn::findOutEdge);
    Nan::SetPrototypeMethod(constructTemplate, "findEdge", Txn::findEdge);
    Nan::SetPrototypeMethod(constructTemplate, "fetchSrc", Txn::fetchSrc);
    Nan::SetPrototypeMethod(constructTemplate, "fetchDst", Txn::fetchDst);
    Nan::SetPrototypeMethod(constructTemplate, "fetchSrcDst", Txn::fetchSrcDst);
    Nan::SetPrototypeMethod(constructTemplate, "traverseIn", Txn::traverseIn);
    Nan::SetPrototypeMethod(constructTemplate, "traverseOut", Txn::traverseOut);
    Nan::SetPrototypeMethod(constructTemplate, "traverse", Txn::traverse);
    Nan::SetPrototypeMethod(constructTemplate, "shortestPath", Txn::shortestPath);

    target->Set(Nan::New("Txn").ToLocalChecked(), constructTemplate->GetFunction());
}

NAN_METHOD(Txn::New)
{
    if (!info.IsConstructCall())
    {
        return Nan::ThrowError(
            Nan::New("Txn::New - called without new keyword").ToLocalChecked());
    }
    v8::Local<v8::FunctionTemplate> contextType = Nan::New<v8::FunctionTemplate>(Context::constructor);
    if (info.Length() == 2 && contextType->HasInstance(info[0]->ToObject()) && info[1]->IsString())
    {
        Context *ctx = Nan::ObjectWrap::Unwrap<Context>(info[0]->ToObject());
        std::string mode = *Nan::Utf8String(info[1]->ToString());

        nogdb::TxnMode txnMode;
        if(mode=="READ_WRITE")      txnMode = nogdb::TxnMode::READ_WRITE;
        else if(mode=="READ_ONLY")  txnMode = nogdb::TxnMode::READ_ONLY;
        else Nan::ThrowError("Invalid txn mode");

        try {
            Txn *txn = new Txn(ctx->base, txnMode);
            txn->Wrap(info.Holder());
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
        
        info.GetReturnValue().Set(info.Holder());
    }
    else
    {
        return Nan::ThrowError(Nan::New("new Txn() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::commit)
{
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        txn->base->commit();
        info.GetReturnValue().SetUndefined();        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::rollback) {
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        txn->base->rollback();
        info.GetReturnValue().SetUndefined();        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::getTxnMode) {
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        nogdb::TxnMode mode = txn->base->getTxnMode();
        std::string txnMode = "";
        if(mode==nogdb::TxnMode::READ_WRITE)      txnMode = "READ_WRITE";
        else if(mode==nogdb::TxnMode::READ_ONLY)  txnMode = "READ_ONLY";
        info.GetReturnValue().Set(Nan::New(txnMode).ToLocalChecked());        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::isCompleted) {
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        bool isCompleted = txn->base->isCompleted();
        info.GetReturnValue().Set(Nan::New(isCompleted));
    } catch ( nogdb::Error& err ){
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::addClass) {
    if(info.Length() == 2 && info[0]->IsString() && info[1]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string type = *Nan::Utf8String(info[1]->ToString());

        nogdb::ClassType classType = nogdb::ClassType::VERTEX;
        if(type=="VERTEX")      classType = nogdb::ClassType::VERTEX;
        else if(type=="EDGE")   classType = nogdb::ClassType::EDGE;
        else Nan::ThrowError("ClassType Invalid");
        try {
            nogdb::ClassDescriptor classD = txn->base->addClass(className, classType);
            info.GetReturnValue().Set(v8ClassDescriptor(classD));
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.addClass() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::addSubClassOf) {
    if(info.Length() == 2 && info[0]->IsString() && info[1]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string superClass = *Nan::Utf8String(info[0]->ToString());
        std::string className = *Nan::Utf8String(info[1]->ToString());
        try {
            nogdb::ClassDescriptor classD = txn->base->addSubClassOf(superClass, className);
            info.GetReturnValue().Set(v8ClassDescriptor(classD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.addSubClassOf() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::dropClass) {
    if (info.Length() == 1 && info[0]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        try {
            txn->base->dropClass(className);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.dropClass() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::renameClass) {
    if (info.Length() == 2 && info[0]->IsString() && info[1]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string oldClassName = *Nan::Utf8String(info[0]->ToString());
        std::string newClassName = *Nan::Utf8String(info[1]->ToString());
        try {
            txn->base->renameClass(oldClassName, newClassName);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.renameClass() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::addProperty) {
    if (info.Length() == 3 && info[0]->IsString() && info[1]->IsString() && info[2]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propertyName = *Nan::Utf8String(info[1]->ToString());
        std::string type = *Nan::Utf8String(info[2]->ToString());

        nogdb::PropertyType propertyType = nogdb::PropertyType::TEXT;
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
        try {
            nogdb::PropertyDescriptor propD = txn->base->addProperty(className,propertyName,propertyType);
            info.GetReturnValue().Set(v8PropertyDescriptor(propD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.addProperty() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::renameProperty) {
    if (info.Length() == 3 && info[0]->IsString() && info[1]->IsString() && info[2]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string oldPropertyName = *Nan::Utf8String(info[1]->ToString());
        std::string newPropertyName = *Nan::Utf8String(info[2]->ToString());
        try {
            txn->base->renameProperty(className,oldPropertyName,newPropertyName);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.renameProperty() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::dropProperty) {
    if (info.Length() == 2 && info[0]->IsString() && info[1]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propertyName = *Nan::Utf8String(info[1]->ToString());
        try {
            txn->base->dropProperty(className,propertyName);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.dropProperty() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::addIndex) {
    if(info.Length() == 3 && info[0]->IsString() && info[1]->IsString() && info[2]->IsBoolean()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propertyName = *Nan::Utf8String(info[1]->ToString());
        bool isUnique = info[2]->BooleanValue();

        try {
            nogdb::IndexDescriptor indexD = txn->base->addIndex(className, propertyName, isUnique);
            info.GetReturnValue().Set(v8IndexDescriptor(indexD));
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.addIndex() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::dropIndex) {
    if (info.Length() == 2 && info[0]->IsString() && info[1]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propertyName = *Nan::Utf8String(info[1]->ToString());
        try {
            txn->base->dropIndex(className,propertyName);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.dropIndex() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::getDbInfo) {
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        nogdb::DbInfo dbInfo = txn->base->getDbInfo();
        info.GetReturnValue().Set(v8DBInfo(dbInfo));
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    } 
}

NAN_METHOD(Txn::getClasses) {
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        std::vector<nogdb::ClassDescriptor> classDs = txn->base->getClasses();
        v8::Local<v8::Array> retval = Nan::New<v8::Array>(classDs.size());
        int i = 0;
        for(const auto& classD: classDs)
        {
            Nan::Set(retval, i++, v8ClassDescriptor(classD));
        }
        info.GetReturnValue().Set(retval);
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::getPropertiesByClassName) {
    if (info.Length() == 1 && info[0]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        try {
            std::vector<nogdb::PropertyDescriptor> propDs = txn->base->getProperties(className);
            v8::Local<v8::Array> retval = Nan::New<v8::Array>(propDs.size());
            int i = 0;
            for(const auto& propD: propDs)
            {
                Nan::Set(retval, i++, v8PropertyDescriptor(propD));
            }
            info.GetReturnValue().Set(retval);
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getPropertiesByClassName() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::getPropertiesByClassDescriptor) {
    if (info.Length() == 1 && info[0]->IsObject())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        v8::Local<v8::Object> clsD = info[0]->ToObject();
        unsigned int id = Nan::Get(clsD, Nan::New<v8::String>("id").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        std::string name = *Nan::Utf8String(Nan::Get(clsD, Nan::New<v8::String>("name").ToLocalChecked()).ToLocalChecked()->ToString());
        unsigned int base = Nan::Get(clsD, Nan::New<v8::String>("base").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        std::string type = *Nan::Utf8String(Nan::Get(clsD, Nan::New<v8::String>("type").ToLocalChecked()).ToLocalChecked()->ToString());

        nogdb::ClassType classType = nogdb::ClassType::VERTEX;
        if(type=="VERTEX")      classType = nogdb::ClassType::VERTEX;
        else if(type=="EDGE")   classType = nogdb::ClassType::EDGE;
        else Nan::ThrowError("ClassType Invalid");
        try {
            nogdb::ClassDescriptor classD(id, name, base, classType);
            std::vector<nogdb::PropertyDescriptor> propDs = txn->base->getProperties(classD);
            v8::Local<v8::Array> retval = Nan::New<v8::Array>(propDs.size());
            int i = 0;
            for(const auto& propD: propDs)
            {
                Nan::Set(retval, i++, v8PropertyDescriptor(propD));
            }
            info.GetReturnValue().Set(retval);
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getPropertiesByClassDescriptor() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::getClassByName) {
    if (info.Length() == 1 && info[0]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        try {
            nogdb::ClassDescriptor classD = txn->base->getClass(className);
            info.GetReturnValue().Set(v8ClassDescriptor(classD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getClassByName() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::getClassById) {
    if (info.Length() == 1 && info[0]->IsUint32())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        unsigned int id = info[0]->Uint32Value();
        try {
            nogdb::ClassDescriptor classD = txn->base->getClass(id);
            info.GetReturnValue().Set(v8ClassDescriptor(classD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getClassById() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::getProperty) {
    if (info.Length() == 1 && info[0]->IsString() && info[1]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propertyName = *Nan::Utf8String(info[1]->ToString());
        try {
            nogdb::PropertyDescriptor propD = txn->base->getProperty(className, propertyName);
            info.GetReturnValue().Set(v8PropertyDescriptor(propD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getProperty() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::getIndex) {
    if (info.Length() == 1 && info[0]->IsString() && info[1]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propertyName = *Nan::Utf8String(info[1]->ToString());
        try {
            nogdb::IndexDescriptor indexD = txn->base->getIndex(className, propertyName);
            info.GetReturnValue().Set(v8IndexDescriptor(indexD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getIndex() - invalid arugment(s)").ToLocalChecked());
    } 
}

NAN_METHOD(Txn::fetchRecord) {
    if (info.Length() == 1 && info[0]->IsObject())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        v8::Local<v8::Object> recD = info[0]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);
        try {
            nogdb::Record record = txn->base->fetchRecord(recordD);
            info.GetReturnValue().Set(v8Record(record));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.fetchRecord() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::addVertex) {
    v8::Local<v8::FunctionTemplate> recordType = Nan::New<v8::FunctionTemplate>(Record::constructor);
    if (info.Length() == 2 && info[0]->IsString() && recordType->HasInstance(info[1]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        Record *record = Nan::ObjectWrap::Unwrap<Record>(info[1]->ToObject());
        try {
            nogdb::RecordDescriptor recD = txn->base->addVertex(className, record->base);
            info.GetReturnValue().Set(v8RecordDescriptor(recD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else if (info.Length() == 1  && info[0]->IsString())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        try {
            nogdb::RecordDescriptor recD = txn->base->addVertex(className);
            info.GetReturnValue().Set(v8RecordDescriptor(recD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.addVertex() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::addEdge) {
    v8::Local<v8::FunctionTemplate> recordType = Nan::New<v8::FunctionTemplate>(Record::constructor);
    if (info.Length() == 4  && info[0]->IsString() && info[1]->IsObject()
                            && info[2]->IsObject() && recordType->HasInstance(info[3]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
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

        Record *record = Nan::ObjectWrap::Unwrap<Record>(info[3]->ToObject());
        try {
            nogdb::RecordDescriptor recD = txn->base->addEdge(className, srcRecordD, dstRecordD, record->base);
            info.GetReturnValue().Set(v8RecordDescriptor(recD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else if (info.Length() == 3 && info[0]->IsString() && info[1]->IsObject() && info[2]->IsObject())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
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
            nogdb::RecordDescriptor recD = txn->base->addEdge(className, srcRecordD, dstRecordD);
            info.GetReturnValue().Set(v8RecordDescriptor(recD));
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.addEdge() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::update) {
    v8::Local<v8::FunctionTemplate> recordType = Nan::New<v8::FunctionTemplate>(Record::constructor);
    if (info.Length() == 2 && info[0]->IsObject() && recordType->HasInstance(info[1]->ToObject()))
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        v8::Local<v8::Object> recD = info[0]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        Record *record = Nan::ObjectWrap::Unwrap<Record>(info[2]->ToObject());

        try {
            txn->base->update(recordD, record->base);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }         
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.update() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::updateSrc) {
    if (info.Length() == 2 && info[0]->IsObject() && info[1]->IsObject())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        v8::Local<v8::Object> recD = info[0]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        v8::Local<v8::Object> srcRecD = info[1]->ToObject();
        v8::Local<v8::Object> srcRid = Nan::Get(srcRecD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int srcClassId = Nan::Get(srcRid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int srcPositionId = Nan::Get(srcRid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor srcRecordD(srcClassId,srcPositionId);
        try {
            txn->base->updateSrc(recordD, srcRecordD);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.updateSrc() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::updateDst) {
    if (info.Length() == 2 && info[0]->IsObject() && info[1]->IsObject())
    {
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        v8::Local<v8::Object> recD = info[0]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);

        v8::Local<v8::Object> dstRecD = info[1]->ToObject();
        v8::Local<v8::Object> dstRid = Nan::Get(dstRecD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int dstClassId = Nan::Get(dstRid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int dstPositionId = Nan::Get(dstRid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor dstRecordD(dstClassId,dstPositionId);
        try {
            txn->base->updateSrc(recordD, dstRecordD);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.updateDst() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::remove) {
    if(info.Length() == 1 && info[0]->IsObject()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        v8::Local<v8::Object> recD = info[0]->ToObject();
        v8::Local<v8::Object> rid = Nan::Get(recD, Nan::New<v8::String>("rid").ToLocalChecked()).ToLocalChecked()->ToObject();
        unsigned int classId = Nan::Get(rid, Nan::New<v8::String>("classId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        unsigned int positionId = Nan::Get(rid, Nan::New<v8::String>("positionId").ToLocalChecked()).ToLocalChecked()->Uint32Value();
        nogdb::RecordDescriptor recordD(classId,positionId);
        try {
            txn->base->remove(recordD);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }  
    }
    else {
        return Nan::ThrowError(Nan::New("Txn.remove() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::removeAll) {
    if(info.Length() == 1 && info[0]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        std::string className = *Nan::Utf8String(info[0]->ToString());
        try {
            txn->base->removeAll(className);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    } else {
        return Nan::ThrowError(Nan::New("Txn.removeAll() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::find) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::findSubClassOf) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::findInEdge) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::findOutEdge) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::findEdge) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::fetchSrc) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::fetchDst) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::fetchSrcDst) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::traverseIn) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::traverseOut) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::traverse) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::shortestPath) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

v8::Local<v8::Object> Txn::NewInstance(v8::Local<v8::Value> ctx,v8::Local<v8::Value> mode) {
    Nan::EscapableHandleScope scope;

    const unsigned argc = 2;
    v8::Local<v8::Value> argv[argc] = { ctx,mode };
    v8::Local<v8::FunctionTemplate> tp1 = Nan::New<v8::FunctionTemplate>(constructor);
    v8::Local<v8::Function> cons = Nan::GetFunction(tp1).ToLocalChecked();
    v8::Local<v8::Object> instance = Nan::NewInstance(cons,argc, argv).ToLocalChecked();

    return scope.Escape(instance);
}
