#include <memory>
#include <nan.h>
#include <nogdb/nogdb.h>

#include "context.hpp"

#include "txn.hpp"

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
    Nan::SetPrototypeMethod(constructTemplate, "getTxnId", Txn::getTxnId);
    Nan::SetPrototypeMethod(constructTemplate, "getVersionId", Txn::getVersionId);
    Nan::SetPrototypeMethod(constructTemplate, "getTxnMode", Txn::getTxnMode);

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

        nogdb::Txn::Mode txnMode;
        if(mode=="READ_WRITE")      txnMode = nogdb::Txn::Mode::READ_WRITE;
        else if(mode=="READ_ONLY")  txnMode = nogdb::Txn::Mode::READ_ONLY;
        else Nan::ThrowError("TxnMode Invalid");

        std::shared_ptr<nogdb::Txn> baseTxn = std::make_shared<nogdb::Txn>(ctx->base, txnMode);

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

NAN_METHOD(Txn::getTxnId) {
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        int txnId =  txn->base->getTxnId();
        info.GetReturnValue().Set(txnId);        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::getVersionId) {
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        int verId =  txn->base->getVersionId();
        info.GetReturnValue().Set(verId);        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
    
}
  NAN_METHOD(Txn::getTxnMode) {
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        int txnMode =  txn->base->getTxnMode();
        info.GetReturnValue().Set(txnMode);        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
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
