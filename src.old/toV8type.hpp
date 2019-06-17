#include <nan.h>
#include <nogdb/nogdb.h>

v8::Local<v8::Value> v8ClassDescriptor(nogdb::ClassDescriptor classD);

v8::Local<v8::Value> v8PropertyDescriptor(nogdb::PropertyDescriptor propD);

v8::Local<v8::Value> v8RecordDescriptor(nogdb::RecordDescriptor recD);

v8::Local<v8::Value> v8Record(nogdb::Record rec);

v8::Local<v8::Value> v8RecordId(nogdb::RecordId rid);

v8::Local<v8::Value> v8ResultSet(nogdb::ResultSet resultSet);

v8::Local<v8::Value> v8Result(nogdb::Result result);

v8::Local<v8::Value> v8DBInfo(nogdb::DBInfo dbInfo);