#include <nan.h>
#include <nogdb/nogdb.h>

class Context : public Nan::ObjectWrap {
public:
    nogdb::Context base;

    static NAN_MODULE_INIT(Init);
    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> dbPath);

private:
    explicit Context(const std::string &path) : base(path) {} ;

    static NAN_METHOD(New);
    static NAN_METHOD(getMaxVersionId) ;
    static NAN_METHOD(getMaxTxnId) ;
    static NAN_METHOD(getMinActiveTxnId) ;
};