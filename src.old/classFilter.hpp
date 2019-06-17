#include <nan.h>
#include <nogdb/nogdb.h>

class ClassFilter : public Nan::ObjectWrap {
public:
    nogdb::ClassFilter base;

    static NAN_MODULE_INIT(Init);
    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> classNames);

private:
    explicit ClassFilter(const std::set<std::string> &classNames_) : base(classNames_) {} ;

    static NAN_METHOD(New) ;
    static NAN_METHOD(add) ;
    static NAN_METHOD(remove) ;
    static NAN_METHOD(size) ;
    static NAN_METHOD(empty) ;
    static NAN_METHOD(getClassName);
};