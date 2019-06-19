#include <memory>
#include <nan.h>
#include <nogdb/nogdb.h>

class Txn : public Nan::ObjectWrap {
public:
    std::shared_ptr<nogdb::Transaction> base;

    static NAN_MODULE_INIT(Init);
    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> ctx,v8::Local<v8::Value> mode);

private:
    explicit Txn(std::shared_ptr<nogdb::Transaction> txn_) : base(txn_) {} ;

    static NAN_METHOD(New);
    static NAN_METHOD(commit);
    static NAN_METHOD(rollback) ;
    static NAN_METHOD(getTxnMode) ;
    static NAN_METHOD(isCompleted);
    static NAN_METHOD(addClass);
    static NAN_METHOD(addSubClassOf);
    static NAN_METHOD(dropClass);
    static NAN_METHOD(renameClass);
    static NAN_METHOD(addProperty);
    static NAN_METHOD(renameProperty);
    static NAN_METHOD(dropProperty);
    static NAN_METHOD(addIndex);
    static NAN_METHOD(dropIndex);
    static NAN_METHOD(getDbInfo);
    static NAN_METHOD(getClasses);
    static NAN_METHOD(getPropertiesByClassName);
    static NAN_METHOD(getPropertiesByClassDescriptor);
    static NAN_METHOD(getClassByName);
    static NAN_METHOD(getClassById);
    static NAN_METHOD(getProperty);
    static NAN_METHOD(getIndex);
    static NAN_METHOD(fetchRecord);
    static NAN_METHOD(addVertex);
    static NAN_METHOD(addEdge);
    static NAN_METHOD(update);
    static NAN_METHOD(updateSrc);
    static NAN_METHOD(updateDst);
    static NAN_METHOD(remove);
    static NAN_METHOD(removeAll);
    static NAN_METHOD(find);
    static NAN_METHOD(findSubClassOf);
    static NAN_METHOD(findInEdge);
    static NAN_METHOD(findOutEdge);
    static NAN_METHOD(findEdge);
    static NAN_METHOD(fetchSrc);
    static NAN_METHOD(fetchDst);
    static NAN_METHOD(fetchSrcDst);
    static NAN_METHOD(traverseIn);
    static NAN_METHOD(traverseOut);
    static NAN_METHOD(traverse);
    static NAN_METHOD(shortestPath);

};