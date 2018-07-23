#include <nan.h>
#include <nogdb/nogdb.h>

class Vertex {
public:
    static NAN_MODULE_INIT(Init);
    static NAN_METHOD(create);
    static NAN_METHOD(update);
    static NAN_METHOD(destroy);
    static NAN_METHOD(get);
    static NAN_METHOD(getInEdge);
    static NAN_METHOD(getOutEdge);
    static NAN_METHOD(getAllEdge);
    static NAN_METHOD(getIndex);
};