#include <nan.h>
#include <nogdb/nogdb.h>

class Edge {
public:
    static NAN_MODULE_INIT(Init);
    static NAN_METHOD(create);
    static NAN_METHOD(update);
    static NAN_METHOD(updateSrc);
    static NAN_METHOD(updateDst);
    static NAN_METHOD(destroy);
    static NAN_METHOD(get);
    static NAN_METHOD(getSrc);
    static NAN_METHOD(getDst);
    static NAN_METHOD(getSrcDst);
    static NAN_METHOD(getIndex);
};