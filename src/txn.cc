#include <memory>
#include <nan.h>
#include <nogdb/nogdb.h>

#include "context.hpp"
#include "txn.hpp"
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

        std::shared_ptr<nogdb::Transaction> baseTxn = std::make_shared<nogdb::Transaction>(ctx->base.beginTxn(txnMode));

        Txn *txn = new Txn(baseTxn);
        txn->Wrap(info.Holder());
        
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
    //TODO don't know how to test yet --Tae
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        bool isCompleted = txn->base->isCompleted();
        info.GetReturnValue().Set(Nan::New(isCompleted));
    } catch ( nogdb::Error& err ){
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::addClass) {
    //TODO don't know how to test yet --Tae
    if(info.Length() == 2 && info[0]->IsString() && info[1]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        //check if READ_WRITE
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to create class.");

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
        return Nan::ThrowError(Nan::New("Class.create() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::addSubClassOf) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::dropClass) {
    //TODO not tested yet --Tae

    if(info.Length() == 1 && info[0]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to drop class.");

        std::string className = *Nan::Utf8String(info[0]->ToString());

        try {
            txn->base->dropClass(className);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
}

NAN_METHOD(Txn::renameClass) {
    //TODO not tested yet --Tae

    if(info.Length() == 2 && info[0]->IsString() && info[1]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to drop class.");

        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string newName = *Nan::Utf8String(info[1]->ToString());

        try {
            txn->base->renameClass(className,newName);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
}

NAN_METHOD(Txn::addProperty) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::renameProperty) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::dropProperty) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::addIndex) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::dropIndex) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::getDbInfo) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::getClasses) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::getPropertiesByClassName) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::getPropertiesByClassDescriptor) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::getClassByName) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::getClassById) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::getProperty) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::getIndex) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::fetchRecord) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::addVertex) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::addEdge) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::update) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::updateSrc) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::updateDst) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::remove) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::removeAll) {
    //TODO
    info.GetReturnValue().SetUndefined();
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
