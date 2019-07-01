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

    Nan::SetPrototypeMethod(constructTemplate, "getMaxDb", Context::getMaxDb);
    Nan::SetPrototypeMethod(constructTemplate, "getMaxDbSize", Context::getMaxDbSize);
    Nan::SetPrototypeMethod(constructTemplate, "getDbPath", Context::getDbPath);
    Nan::SetPrototypeMethod(constructTemplate, "isVersionEnabled", Context::isVersionEnabled);

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
        try {
            Context *ctx = new Context(*(Nan::Utf8String(info[0]->ToString())));
            ctx->Wrap(info.Holder());
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
        info.GetReturnValue().Set(info.Holder());
    }
    else if (info.Length() == 2 && info[0]->IsString() && info[1]->IsObject())
    {
        try {
            v8::Local<v8::Object> setting = info[1]->ToObject();
            unsigned int maxDb = Nan::Get(setting, Nan::New<v8::String>("maxDb").ToLocalChecked()).ToLocalChecked()->Uint32Value();
            unsigned int maxDbSize = Nan::Get(setting, Nan::New<v8::String>("maxDbSize").ToLocalChecked()).ToLocalChecked()->Uint32Value();
            bool versionEnabled = Nan::Get(setting, Nan::New<v8::String>("versionEnabled").ToLocalChecked()).ToLocalChecked()->BooleanValue();
            nogdb::ContextSetting ctxSetting = nogdb::ContextSetting();
            ctxSetting._maxDb = maxDb;
            ctxSetting._maxDbSize = maxDbSize;
            ctxSetting._versionEnabled = versionEnabled;
            Context *ctx = new Context(*(Nan::Utf8String(info[0]->ToString())), ctxSetting);
            ctx->Wrap(info.Holder());
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("new Context() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Context::getMaxDb) {
    Context *ctx = Nan::ObjectWrap::Unwrap<Context>(info.This());
    try {
        int maxDb = ctx->base.getMaxDb();
        info.GetReturnValue().Set(maxDb);    
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Context::getMaxDbSize) {
    Context *ctx = Nan::ObjectWrap::Unwrap<Context>(info.This());
    try {
        int maxDbSize =  ctx->base.getMaxDbSize();
        info.GetReturnValue().Set(maxDbSize);    
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Context::getDbPath) {
    Context *ctx = Nan::ObjectWrap::Unwrap<Context>(info.This());
    try {
        std::string dbPath =  ctx->base.getDbPath();
        info.GetReturnValue().Set(Nan::New<v8::String>(dbPath).ToLocalChecked());    
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Context::isVersionEnabled) {
    Context *ctx = Nan::ObjectWrap::Unwrap<Context>(info.This());
    try {
        bool enabled =  ctx->base.isVersionEnabled();
        info.GetReturnValue().Set(enabled);    
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

v8::Local<v8::Object> Context::NewInstance(v8::Local<v8::Value> dbPath, v8::Local<v8::Value> setting) {
    Nan::EscapableHandleScope scope;

    const unsigned argc = 2;
    v8::Local<v8::Value> argv[argc] = { dbPath, setting };
    v8::Local<v8::FunctionTemplate> tp1 = Nan::New<v8::FunctionTemplate>(constructor);
    v8::Local<v8::Function> cons = Nan::GetFunction(tp1).ToLocalChecked();
    v8::Local<v8::Object> instance = Nan::NewInstance(cons, argc, argv).ToLocalChecked();

    return scope.Escape(instance);
}
