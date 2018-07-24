#include <nan.h>
#include <nogdb/nogdb.h>

#include "context.hpp"

Nan::Persistent<v8::FunctionTemplate> Context::constructor;

NAN_MODULE_INIT(Context::Init)
{
    v8::Local<v8::FunctionTemplate> constructTemplate =
        Nan::New<v8::FunctionTemplate>(Context::New);

    constructor.Reset(constructTemplate);
    constructTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    constructTemplate->SetClassName(Nan::New("Context").ToLocalChecked());

    Nan::SetPrototypeMethod(constructTemplate, "getMaxVersionId", Context::getMaxVersionId);
    Nan::SetPrototypeMethod(constructTemplate, "getMaxTxnId", Context::getMaxTxnId);
    Nan::SetPrototypeMethod(constructTemplate, "getMinActiveTxnId", Context::getMinActiveTxnId);

    target->Set(Nan::New("Context").ToLocalChecked(), constructTemplate->GetFunction());
}

NAN_METHOD(Context::New)
{
    if (!info.IsConstructCall())
    {
        return Nan::ThrowError(
            Nan::New("Context::New - called without new keyword").ToLocalChecked());
    }

    if (info.Length() == 1 && info[0]->IsString())
    {
        Context *ctx = new Context(*(Nan::Utf8String(info[0]->ToString())));
        ctx->Wrap(info.Holder());

        info.GetReturnValue().Set(info.Holder());
    }
    else
    {
        return Nan::ThrowError(Nan::New("new Context() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Context::getMaxVersionId) {
    Context *ctx = Nan::ObjectWrap::Unwrap<Context>(info.This());
    try {
        int maxVerId =  ctx->base.getMaxVersionId();
        info.GetReturnValue().Set(maxVerId);    
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Context::getMaxTxnId) {
    Context *ctx = Nan::ObjectWrap::Unwrap<Context>(info.This());
    try {
        int maxTxnId =  ctx->base.getMaxTxnId();
        info.GetReturnValue().Set(maxTxnId);    
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Context::getMinActiveTxnId) {
    Context *ctx = Nan::ObjectWrap::Unwrap<Context>(info.This());
    try {
        std::pair<int, int> minActiveTxnId = ctx->base.getMinActiveTxnId();
        v8::Local<v8::Object> retval = Nan::New<v8::Object>();
        Nan::Set(retval, Nan::New<v8::String>("first").ToLocalChecked(), Nan::New<v8::Number>(minActiveTxnId.first));
        Nan::Set(retval, Nan::New<v8::String>("second").ToLocalChecked(), Nan::New<v8::Number>(minActiveTxnId.second));
        info.GetReturnValue().Set(retval);    
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

v8::Local<v8::Object> Context::NewInstance(v8::Local<v8::Value> dbPath) {
    Nan::EscapableHandleScope scope;

    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = { dbPath };
    v8::Local<v8::FunctionTemplate> tp1 = Nan::New<v8::FunctionTemplate>(constructor);
    v8::Local<v8::Function> cons = Nan::GetFunction(tp1).ToLocalChecked();
    v8::Local<v8::Object> instance = Nan::NewInstance(cons,argc, argv).ToLocalChecked();

    return scope.Escape(instance);
}
