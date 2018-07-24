#include <nan.h>
#include <nogdb/nogdb.h>

#include "toV8type.hpp"

v8::Local<v8::Value> v8ClassDescriptor(nogdb::ClassDescriptor classD){
    v8::Local<v8::Object> retval = Nan::New<v8::Object>();
    Nan::Set(retval, Nan::New<v8::String>("id").ToLocalChecked(), Nan::New<v8::Uint32>(classD.id));
    Nan::Set(retval, Nan::New<v8::String>("name").ToLocalChecked(), Nan::New<v8::String>(classD.name).ToLocalChecked());
    v8::Local<v8::String> v8type;
    switch(classD.type)
    {
        case nogdb::ClassType::EDGE :
            v8type = Nan::New<v8::String>("EDGE").ToLocalChecked();         break;
        case nogdb::ClassType::VERTEX : 
            v8type = Nan::New<v8::String>("VERTEX").ToLocalChecked();       break;
        case nogdb::ClassType::UNDEFINED :
            v8type = Nan::New<v8::String>("UNDEFINED").ToLocalChecked();    break;
        default : std::cout << "fail" << std::endl;
    }
    Nan::Set(retval, Nan::New<v8::String>("type").ToLocalChecked(), v8type);
    v8::Local<v8::Object> classProperty = Nan::New<v8::Object>();
    for (std::map<std::string, nogdb::PropertyDescriptor>::iterator it=classD.properties.begin(); it!=classD.properties.end(); ++it)
    {
        Nan::Set(classProperty, Nan::New<v8::String>(it->first).ToLocalChecked(), v8PropertyDescriptor(it->second));
    }
    Nan::Set(retval, Nan::New<v8::String>("properties").ToLocalChecked(), classProperty);
    Nan::Set(retval, Nan::New<v8::String>("super").ToLocalChecked(), Nan::New<v8::String>(classD.super).ToLocalChecked());
    v8::Local<v8::Array> sub = Nan::New<v8::Array>(classD.sub.size());
    int i = 0;
    for(std::vector<std::string>::iterator it = classD.sub.begin(); it != classD.sub.end(); ++it){
        Nan::Set(sub, i, Nan::New<v8::String>(*it).ToLocalChecked());
        i++;
    }
    Nan::Set(retval, Nan::New<v8::String>("sub").ToLocalChecked(), sub);
    return retval;
}

v8::Local<v8::Value> v8PropertyDescriptor(nogdb::PropertyDescriptor propD){
    v8::Local<v8::Object> retval = Nan::New<v8::Object>();
    Nan::Set(retval, Nan::New<v8::String>("id").ToLocalChecked(), Nan::New<v8::Uint32>(propD.id));
    v8::Local<v8::String> v8type;
    switch(propD.type)
    {
        case nogdb::PropertyType::TINYINT :
            v8type = Nan::New<v8::String>("TINYINT").ToLocalChecked();              break;
        case nogdb::PropertyType::UNSIGNED_TINYINT : 
            v8type = Nan::New<v8::String>("UNSIGNED_TINYINT").ToLocalChecked();     break;
        case nogdb::PropertyType::SMALLINT :
            v8type = Nan::New<v8::String>("SMALLINT").ToLocalChecked();             break;
        case nogdb::PropertyType::UNSIGNED_SMALLINT :
            v8type = Nan::New<v8::String>("UNSIGNED_SMALLINT").ToLocalChecked();    break;
        case nogdb::PropertyType::INTEGER :
            v8type = Nan::New<v8::String>("INTEGER").ToLocalChecked();              break;
        case nogdb::PropertyType::UNSIGNED_INTEGER :
            v8type = Nan::New<v8::String>("UNSIGNED_INTEGER").ToLocalChecked();     break;
        case nogdb::PropertyType::BIGINT :
            v8type = Nan::New<v8::String>("BIGINT").ToLocalChecked();               break;
        case nogdb::PropertyType::UNSIGNED_BIGINT :
            v8type = Nan::New<v8::String>("UNSIGNED_BIGINT").ToLocalChecked();      break;
        case nogdb::PropertyType::TEXT :
            v8type = Nan::New<v8::String>("TEXT").ToLocalChecked();                 break;
        case nogdb::PropertyType::REAL :
            v8type = Nan::New<v8::String>("REAL").ToLocalChecked();                 break;
        case nogdb::PropertyType::BLOB :
            v8type = Nan::New<v8::String>("BLOB").ToLocalChecked();                 break;
        default : std::cout << "fail" << std::endl;
    }
    Nan::Set(retval, Nan::New<v8::String>("type").ToLocalChecked(), v8type);
    v8::Local<v8::Object> indexInfo = Nan::New<v8::Object>();
    
    for (std::map<nogdb::IndexId, std::pair<nogdb::ClassId, bool>>::iterator it=propD.indexInfo.begin(); it!=propD.indexInfo.end(); ++it)
    {
        v8::Local<v8::Object> value = Nan::New<v8::Object>();
        Nan::Set(value, Nan::New<v8::String>("ClassId").ToLocalChecked(), Nan::New<v8::Uint32>(it->second.first));
        Nan::Set(value, Nan::New<v8::String>("isUnique").ToLocalChecked(), Nan::New<v8::Boolean>(it->second.second));
        Nan::Set(indexInfo, Nan::New<v8::Uint32>(it->first), value);
    }
    Nan::Set(retval, Nan::New<v8::String>("indexInfo").ToLocalChecked(), indexInfo);
    return retval;
}

v8::Local<v8::Value> v8RecordDescriptor(nogdb::RecordDescriptor recD){
    v8::Local<v8::Object> retval = Nan::New<v8::Object>();
    Nan::Set(retval, Nan::New<v8::String>("rid").ToLocalChecked(), v8RecordId(recD.rid));
    return retval;
}

v8::Local<v8::Value> v8Record(nogdb::Record rec){
    v8::Local<v8::Object> retval = Nan::New<v8::Object>();

    std::vector<std::string> properties =  rec.getProperties();
    v8::Local<v8::Array> prop = Nan::New<v8::Array>(properties.size());
    v8::Local<v8::Object> values = Nan::New<v8::Object>();
    int i = 0;
    for(std::vector<std::string>::iterator it = properties.begin(); it != properties.end(); ++it)
    {
        v8::Local<v8::Object> value = Nan::New<v8::Object>();
        Nan::Set(value, Nan::New<v8::String>("toText").ToLocalChecked(),Nan::New<v8::String>(rec.getText(*it)).ToLocalChecked());
        Nan::Set(value, Nan::New<v8::String>("toNumber").ToLocalChecked(),Nan::New<v8::Number>(rec.getReal(*it)));
        Nan::Set(values, Nan::New<v8::String>(*it).ToLocalChecked(),value);
        Nan::Set(prop, i, Nan::New<v8::String>(*it).ToLocalChecked());
        i++;
    }

    Nan::Set(retval, Nan::New<v8::String>("properties").ToLocalChecked(), prop);
    Nan::Set(retval, Nan::New<v8::String>("value").ToLocalChecked(), values);


    Nan::Set(retval, Nan::New<v8::String>("className").ToLocalChecked(), Nan::New<v8::String>(rec.getClassName()).ToLocalChecked());
    Nan::Set(retval, Nan::New<v8::String>("rid").ToLocalChecked(), v8RecordId(rec.getRecordId()));
    Nan::Set(retval, Nan::New<v8::String>("depth").ToLocalChecked(), Nan::New<v8::Number>(rec.getDepth()));
    Nan::Set(retval, Nan::New<v8::String>("version").ToLocalChecked(), Nan::New<v8::Number>(rec.getVersion()));
    Nan::Set(retval, Nan::New<v8::String>("size").ToLocalChecked(), Nan::New<v8::Number>(rec.size()));
    Nan::Set(retval, Nan::New<v8::String>("empty").ToLocalChecked(), Nan::New<v8::Boolean>(rec.empty()));
    return retval;
}

v8::Local<v8::Value> v8RecordId(nogdb::RecordId rid){
    v8::Local<v8::Object> retval = Nan::New<v8::Object>();
    Nan::Set(retval, Nan::New<v8::String>("classId").ToLocalChecked(), Nan::New<v8::Uint32>(rid.first));
    Nan::Set(retval, Nan::New<v8::String>("positionId").ToLocalChecked(), Nan::New<v8::Uint32>(rid.second));
    return retval;
}

v8::Local<v8::Value> v8ResultSet(nogdb::ResultSet resultSet){
    v8::Local<v8::Array> retval = Nan::New<v8::Array>(resultSet.size());
    int i = 0;
    for(std::vector<nogdb::Result>::iterator it = resultSet.begin(); it != resultSet.end(); ++it)
    {
        Nan::Set(retval, i, v8Result(*it));
        i++;
    }
    return retval;
}

v8::Local<v8::Value> v8Result(nogdb::Result result){
    v8::Local<v8::Object> retval = Nan::New<v8::Object>();
    Nan::Set(retval, Nan::New<v8::String>("recordDescriptor").ToLocalChecked(), v8RecordDescriptor(result.descriptor));
    Nan::Set(retval, Nan::New<v8::String>("record").ToLocalChecked(), v8Record(result.record));
    return retval;
}

v8::Local<v8::Value> v8DBInfo(nogdb::DBInfo dbInfo){
    v8::Local<v8::Object> retval = Nan::New<v8::Object>();
    Nan::Set(retval, Nan::New<v8::String>("dbPath").ToLocalChecked(), Nan::New<v8::String>(dbInfo.dbPath).ToLocalChecked());
    Nan::Set(retval, Nan::New<v8::String>("maxDB").ToLocalChecked(), Nan::New<v8::Number>(dbInfo.maxDB));
    Nan::Set(retval, Nan::New<v8::String>("maxDBSize").ToLocalChecked(), Nan::New<v8::Number>(dbInfo.maxDBSize));
    Nan::Set(retval, Nan::New<v8::String>("maxPropertyId").ToLocalChecked(), Nan::New<v8::Number>(dbInfo.maxPropertyId));
    Nan::Set(retval, Nan::New<v8::String>("numProperty").ToLocalChecked(), Nan::New<v8::Number>(dbInfo.numProperty));
    Nan::Set(retval, Nan::New<v8::String>("maxClassId").ToLocalChecked(), Nan::New<v8::Number>(dbInfo.maxClassId));
    Nan::Set(retval, Nan::New<v8::String>("numClass").ToLocalChecked(), Nan::New<v8::Number>(dbInfo.numClass));
    Nan::Set(retval, Nan::New<v8::String>("maxIndexId").ToLocalChecked(), Nan::New<v8::Number>(dbInfo.maxIndexId));
    Nan::Set(retval, Nan::New<v8::String>("numIndex").ToLocalChecked(), Nan::New<v8::Number>(dbInfo.numIndex));
    return retval;
}