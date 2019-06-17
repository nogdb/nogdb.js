#include <nan.h>
#include <nogdb/nogdb.h>

class Condition : public Nan::ObjectWrap {
public:
    nogdb::Condition base;

    static NAN_MODULE_INIT(Init);
    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> propName);

private:
    explicit Condition(const std::string &propName) : base(propName) {} ;

    static NAN_METHOD(New);
    static NAN_METHOD(eq) ;
    static NAN_METHOD(gt) ;
    static NAN_METHOD(lt) ;
    static NAN_METHOD(ge) ;
    static NAN_METHOD(le) ;
    static NAN_METHOD(contain) ;
    static NAN_METHOD(beginWith) ;
    static NAN_METHOD(endWith) ;
    static NAN_METHOD(like) ;
    static NAN_METHOD(regex) ;
    static NAN_METHOD(ignoreCase) ;
    static NAN_METHOD(null) ;
    static NAN_METHOD(in) ;
    static NAN_METHOD(between) ;
};