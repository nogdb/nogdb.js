#include <nan.h>
#include <nogdb/nogdb.h>

nogdb::ClassDescriptor toClassDescriptor(v8::Local<v8::Object> ClassDescIn);

nogdb::RecordDescriptor toRecordDescriptor(v8::Local<v8::Object> RecordDescIn);