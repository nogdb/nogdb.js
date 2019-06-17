#include <memory>
#include <nan.h>
#include <nogdb/nogdb.h>

class Txn : public Nan::ObjectWrap {
public:
    std::shared_ptr<nogdb::Txn> base;

    static NAN_MODULE_INIT(Init);
    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> ctx,v8::Local<v8::Value> mode);

private:
    explicit Txn(std::shared_ptr<nogdb::Txn> txn_) : base(txn_) {} ;

    static NAN_METHOD(New);
    static NAN_METHOD(commit);
    static NAN_METHOD(rollback) ;
    static NAN_METHOD(getTxnId) ;
    static NAN_METHOD(getVersionId) ;
    static NAN_METHOD(getTxnMode) ;
};