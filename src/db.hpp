#include <nan.h>
#include <nogdb/nogdb.h>

class Db {
public:
    static NAN_MODULE_INIT(Init);
    static NAN_METHOD(getRecord);
    static NAN_METHOD(getSchema);
    static NAN_METHOD(getDbInfo);
};