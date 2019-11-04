#include <nan.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <nogdb/nogdb.h>
#include <iostream>

class Context : public Nan::ObjectWrap {
public:
    nogdb::Context base;

    static NAN_MODULE_INIT(Init);
    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> dbPath);
    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> dbPath, v8::Local<v8::Value> setting);

private:
    explicit Context(const std::string &path) {
        struct stat fileStat;
        if (stat((char*)path.c_str(), &fileStat) == 0) {
            base = nogdb::Context(path);
        } else {
            nogdb::ContextInitializer ctxi = nogdb::ContextInitializer(path);
            base = ctxi.init();
        }
    } ;

    Context(const std::string &path, unsigned int maxDB, unsigned int maxDBSize, bool enableVersion) {
        struct stat fileStat;
        if (stat((char*)path.c_str(), &fileStat) == 0) {
            base = nogdb::Context(path);
        } else {
            nogdb::ContextInitializer ctxi = nogdb::ContextInitializer(path);
            if (maxDB > 0) ctxi.setMaxDB(maxDB);
            if (maxDBSize > 0) ctxi.setMaxDBSize(maxDBSize);
            if (enableVersion) ctxi.enableVersion();
            base = ctxi.init();
        }
    } ;

    static NAN_METHOD(New);
    static NAN_METHOD(getDbPath);
    static NAN_METHOD(getMaxDb);
    static NAN_METHOD(getMaxDbSize);
    static NAN_METHOD(isVersionEnabled);
};