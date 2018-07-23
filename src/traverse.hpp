#include <nan.h>
#include <nogdb/nogdb.h>

class Traverse {
public:
    static NAN_MODULE_INIT(Init);
    static NAN_METHOD(inEdgeBfs);
    static NAN_METHOD(outEdgeBfs);
    static NAN_METHOD(allEdgeBfs);
    static NAN_METHOD(inEdgeDfs);
    static NAN_METHOD(outEdgeDfs);
    static NAN_METHOD(allEdgeDfs);
    static NAN_METHOD(shortestPath);
};