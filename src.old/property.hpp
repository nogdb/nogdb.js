#include <nan.h>
#include <nogdb/nogdb.h>

class Property {
public:
    static NAN_MODULE_INIT(Init);
    static NAN_METHOD(add);
    static NAN_METHOD(alter);
    static NAN_METHOD(remove);
    static NAN_METHOD(createIndex);
    static NAN_METHOD(dropIndex);
};