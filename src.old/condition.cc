#include <memory>
#include <nan.h>
#include <nogdb/nogdb.h>

#include "condition.hpp"

Nan::Persistent<v8::FunctionTemplate> Condition::constructor;

NAN_MODULE_INIT(Condition::Init)
{
    v8::Local<v8::FunctionTemplate> constructTemplate =
        Nan::New<v8::FunctionTemplate>(Condition::New);

    constructor.Reset(constructTemplate);
    constructTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    constructTemplate->SetClassName(Nan::New("Condition").ToLocalChecked());

    Nan::SetPrototypeMethod(constructTemplate, "eq", Condition::eq);
    Nan::SetPrototypeMethod(constructTemplate, "gt", Condition::gt);
    Nan::SetPrototypeMethod(constructTemplate, "lt", Condition::lt);
    Nan::SetPrototypeMethod(constructTemplate, "ge", Condition::ge);
    Nan::SetPrototypeMethod(constructTemplate, "le", Condition::le);
    Nan::SetPrototypeMethod(constructTemplate, "contain", Condition::contain);
    Nan::SetPrototypeMethod(constructTemplate, "beginWith", Condition::beginWith);
    Nan::SetPrototypeMethod(constructTemplate, "endWith", Condition::endWith);
    Nan::SetPrototypeMethod(constructTemplate, "like", Condition::like);
    Nan::SetPrototypeMethod(constructTemplate, "regex", Condition::regex);
    Nan::SetPrototypeMethod(constructTemplate, "ignoreCase", Condition::ignoreCase);
    Nan::SetPrototypeMethod(constructTemplate, "null", Condition::null);
    Nan::SetPrototypeMethod(constructTemplate, "in", Condition::in);
    Nan::SetPrototypeMethod(constructTemplate, "between", Condition::between);

    target->Set(Nan::New("Condition").ToLocalChecked(), constructTemplate->GetFunction());
}

NAN_METHOD(Condition::New)
{
    if (!info.IsConstructCall())
    {
        return Nan::ThrowError(
            Nan::New("new Condition() - called without new keyword").ToLocalChecked());
    }

    if (info.Length() == 1 && info[0]->IsString())
    {
        Condition *condition = new Condition(*(Nan::Utf8String(info[0]->ToString())));
        condition->Wrap(info.Holder());

        info.GetReturnValue().Set(info.Holder());
    }
    else
    {
        return Nan::ThrowError(Nan::New("new Condition() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::eq) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsString())
    {
        std::string value = *Nan::Utf8String(info[0]->ToString());
        try {
            cond->base = cond->base.eq(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    } 
    else if (info.Length() == 1 && info[0]->IsNumber())
    {
        double value = info[0]->NumberValue();
        try {
            cond->base = cond->base.eq(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Condition.eq() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::gt) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsString())
    {
        std::string value = *Nan::Utf8String(info[0]->ToString());
        try {
            cond->base = cond->base.gt(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    } 
    else if (info.Length() == 1 && info[0]->IsNumber())
    {
        double value = info[0]->NumberValue();
        try {
            cond->base = cond->base.gt(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Condition.gt() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::lt) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsString())
    {
        std::string value = *Nan::Utf8String(info[0]->ToString());
        try {
            cond->base = cond->base.lt(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }  
    } 
    else if (info.Length() == 1 && info[0]->IsNumber())
    {
        double value = info[0]->NumberValue();
        try {
            cond->base = cond->base.lt(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    }
    else
    {
        return Nan::ThrowError(Nan::New("Condition.lt() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::ge) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsString())
    {
        std::string value = *Nan::Utf8String(info[0]->ToString());
        try {
            cond->base = cond->base.ge(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    } 
    else if (info.Length() == 1 && info[0]->IsNumber())
    {
        double value = info[0]->NumberValue();
        try {
            cond->base = cond->base.ge(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Condition.ge() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::le) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsString())
    {
        std::string value = *Nan::Utf8String(info[0]->ToString());
        try {
            cond->base = cond->base.le(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    } 
    else if (info.Length() == 1 && info[0]->IsNumber())
    {
        double value = info[0]->NumberValue();
        try {
            cond->base = cond->base.le(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Condition.le() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::contain) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsString())
    {
        std::string value = *Nan::Utf8String(info[0]->ToString());
        try {
            cond->base = cond->base.contain(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    } 
    else
    {
        return Nan::ThrowError(Nan::New("Condition.contain() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::beginWith) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsString())
    {
        std::string value = *Nan::Utf8String(info[0]->ToString());
        try {
            cond->base = cond->base.beginWith(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    } 
    else
    {
        return Nan::ThrowError(Nan::New("Condition.beginWith() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::endWith) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsString())
    {
        std::string value = *Nan::Utf8String(info[0]->ToString());
        try {
            cond->base = cond->base.endWith(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }
    } 
    else
    {
        return Nan::ThrowError(Nan::New("Condition.endWith() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::like) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsString())
    {
        std::string value = *Nan::Utf8String(info[0]->ToString());
        try {
            cond->base = cond->base.like(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        }     
    } 
    else
    {
        return Nan::ThrowError(Nan::New("Condition.like() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::regex) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsString())
    {
        std::string value = *Nan::Utf8String(info[0]->ToString());
        try {
            cond->base = cond->base.regex(value);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    } 
    else
    {
        return Nan::ThrowError(Nan::New("Condition.regex() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::ignoreCase) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    try {
        cond->base = cond->base.ignoreCase();
        info.GetReturnValue().SetUndefined();        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Condition::in) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 1 && info[0]->IsArray())
    {
        v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(info[0]);
        if(Nan::Get(array, 0).ToLocalChecked()->IsString()){
            std::set<std::string> propValues;
            for (unsigned int i = 0; i < array->Length(); i++ ) {
                if (Nan::Has(array, i).FromJust()) {
                    if(Nan::Get(array, i).ToLocalChecked()->IsString()){
                        propValues.insert(*Nan::Utf8String(Nan::Get(array, i).ToLocalChecked()->ToString()));
                    } 
                    else 
                    {
                        Nan::ThrowError("value type error");
                        return;
                    }
                }
            }
            try {
                cond->base = cond->base.in(propValues);
                info.GetReturnValue().SetUndefined();        
            } catch ( nogdb::Error& err ) {
                Nan::ThrowError(err.what());
            }
        } 
        else if(Nan::Get(array, 0).ToLocalChecked()->IsNumber()){
            std::set<double> propValues;
            for (unsigned int i = 0; i < array->Length(); i++ ) {
                if (Nan::Has(array, i).FromJust()) {
                    if(Nan::Get(array, i).ToLocalChecked()->IsNumber()){
                        propValues.insert(Nan::Get(array, i).ToLocalChecked()->NumberValue());
                    } else
                    {
                        Nan::ThrowError("value type error");
                        return;
                    }
                }
            }
            try {
                cond->base = cond->base.in(propValues);
                info.GetReturnValue().SetUndefined();        
            } catch ( nogdb::Error& err ) {
                Nan::ThrowError(err.what());
            }
        }
        else
        {
            Nan::ThrowError("value type error");
            return;
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Condition.in() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::between) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    if (info.Length() == 2 && info[0]->IsString() && info[1]->IsString())
    {
        std::string lower = *Nan::Utf8String(info[0]->ToString());
        std::string upper = *Nan::Utf8String(info[1]->ToString());
        try {
            cond->base = cond->base.between(lower,upper);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    } 
    else if (info.Length() == 2 && info[0]->IsNumber() && info[1]->IsNumber())
    {
        double lower = info[0]->NumberValue();
        double upper = info[1]->NumberValue();
        try {
            cond->base = cond->base.between(lower,upper);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    } 
    else if (info.Length() == 4 && info[0]->IsString() && info[1]->IsString() && info[2]->IsBoolean() && info[3]->IsBoolean())
    {
        std::string lower = *Nan::Utf8String(info[0]->ToString());
        std::string upper = *Nan::Utf8String(info[1]->ToString());
        std::pair<bool, bool> isIncludeBound = {info[2]->BooleanValue(),info[3]->BooleanValue()};
        try {
            cond->base = cond->base.between(lower,upper,isIncludeBound);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    } 
    else if (info.Length() == 4 && info[0]->IsNumber() && info[1]->IsNumber() && info[2]->IsBoolean() && info[3]->IsBoolean())
    {
        double lower = info[0]->NumberValue();
        double upper = info[1]->NumberValue();
        std::pair<bool, bool> isIncludeBound = {info[2]->BooleanValue(),info[3]->BooleanValue()};
        try {
            cond->base = cond->base.between(lower,upper,isIncludeBound);
            info.GetReturnValue().SetUndefined();    
        } catch ( nogdb::Error& err ) {
            Nan::ThrowError(err.what());
        } 
    } 
    else
    {
        return Nan::ThrowError(Nan::New("Condition.between() - invalid arugment(s)").ToLocalChecked());
    }
}

NAN_METHOD(Condition::null) {
    Condition *cond = Nan::ObjectWrap::Unwrap<Condition>(info.This());
    try {
        cond->base = cond->base.null();
        info.GetReturnValue().SetUndefined();        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

v8::Local<v8::Object> Condition::NewInstance(v8::Local<v8::Value> propName) {
    Nan::EscapableHandleScope scope;

    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = { propName };
    v8::Local<v8::FunctionTemplate> tp1 = Nan::New<v8::FunctionTemplate>(constructor);
    v8::Local<v8::Function> cons = Nan::GetFunction(tp1).ToLocalChecked();
    v8::Local<v8::Object> instance = Nan::NewInstance(cons,argc, argv).ToLocalChecked();

    return scope.Escape(instance);
}



