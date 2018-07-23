#include <nan.h>
#include <nogdb/nogdb.h>

#include "classFilter.hpp"

Nan::Persistent<v8::FunctionTemplate> ClassFilter::constructor;

NAN_MODULE_INIT(ClassFilter::Init)
{
    v8::Local<v8::FunctionTemplate> constructTemplate =
        Nan::New<v8::FunctionTemplate>(ClassFilter::New);

    constructor.Reset(constructTemplate);
    constructTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    constructTemplate->SetClassName(Nan::New("ClassFilter").ToLocalChecked());

    Nan::SetPrototypeMethod(constructTemplate, "add", ClassFilter::add);
    Nan::SetPrototypeMethod(constructTemplate, "remove", ClassFilter::remove);
    Nan::SetPrototypeMethod(constructTemplate, "size", ClassFilter::size);
    Nan::SetPrototypeMethod(constructTemplate, "empty", ClassFilter::empty);
    Nan::SetPrototypeMethod(constructTemplate, "getClassName", ClassFilter::getClassName);

    target->Set(Nan::New("ClassFilter").ToLocalChecked(), constructTemplate->GetFunction());
}

NAN_METHOD(ClassFilter::New)
{
    if (!info.IsConstructCall())
    {
        return Nan::ThrowError(
            Nan::New("new ClassFilter() - called without new keyword").ToLocalChecked());
    }

    if (info.Length() == 1 && info[0]->IsArray())
    {
        v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(info[0]);
        std::set<std::string> classNames;
        for (unsigned int i = 0; i < array->Length(); i++ ) {
            if (Nan::Has(array, i).FromJust()) {
                if(!Nan::Get(array, i).ToLocalChecked()->IsString()){
                    Nan::ThrowError("Array member must be string");
                    return;
                }
                classNames.insert(*Nan::Utf8String(Nan::Get(array, i).ToLocalChecked()->ToString()));
            }
        }
        ClassFilter *classF = new ClassFilter(classNames);;
        classF->Wrap(info.Holder());
        info.GetReturnValue().Set(info.Holder());
    }
    else
    {
        return Nan::ThrowError(Nan::New("new ClassFilter() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(ClassFilter::add) {
    if (info.Length() == 1 && info[0]->IsString())
    {
        ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info.This());
        classF->base.add(*Nan::Utf8String(info[0]->ToString()));
    }
    else
    {
        return Nan::ThrowError(Nan::New("ClassFilter.add() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(ClassFilter::remove) {
    if (info.Length() == 1 && info[0]->IsString())
    {
        ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info.This());
        classF->base.remove(*Nan::Utf8String(info[0]->ToString()));
    }
    else
    {
        return Nan::ThrowError(Nan::New("ClassFilter.remove() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(ClassFilter::size)
{
    ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Number>(classF->base.size()));
}

NAN_METHOD(ClassFilter::empty)
{
    ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(classF->base.empty()));
}
NAN_METHOD(ClassFilter::getClassName)
{
    ClassFilter *classF = Nan::ObjectWrap::Unwrap<ClassFilter>(info.This());
    std::set<std::string> classNames = classF->base.getClassName();
    v8::Local<v8::Array> retval = Nan::New<v8::Array>(classNames.size());
    int i = 0;
    for(std::string className: classNames){
        Nan::Set(retval, i, Nan::New<v8::String>(className).ToLocalChecked());
        i++;
    }
    info.GetReturnValue().Set(retval);
}

v8::Local<v8::Object> ClassFilter::NewInstance(v8::Local<v8::Value> classNames) {
    Nan::EscapableHandleScope scope;

    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = { classNames };
    v8::Local<v8::FunctionTemplate> tp1 = Nan::New<v8::FunctionTemplate>(constructor);
    v8::Local<v8::Function> cons = Nan::GetFunction(tp1).ToLocalChecked();
    v8::Local<v8::Object> instance = Nan::NewInstance(cons,argc, argv).ToLocalChecked();

    return scope.Escape(instance);
}