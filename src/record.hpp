#include <nan.h>
#include <nogdb/nogdb.h>

class Record : public Nan::ObjectWrap {
public:
    nogdb::Record base;

    static NAN_MODULE_INIT(Init);
    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static v8::Local<v8::Object> NewInstance();

private:
    explicit Record() {} ;

    static NAN_METHOD(New);
    static NAN_METHOD(set) ;
    static NAN_METHOD(getProperties) ;
    static NAN_METHOD(unset) ;
    static NAN_METHOD(size) ;
    static NAN_METHOD(empty) ;
    static NAN_METHOD(clear) ;
};