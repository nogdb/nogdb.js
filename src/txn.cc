#include <memory>
#include <nan.h>
#include <nogdb/nogdb.h>

#include "context.hpp"
#include "txn.hpp"
#include "toV8type.hpp"

Nan::Persistent<v8::FunctionTemplate> Txn::constructor;

NAN_MODULE_INIT(Txn::Init)
{
    v8::Local<v8::FunctionTemplate> constructTemplate =
        Nan::New<v8::FunctionTemplate>(Txn::New);

    constructor.Reset(constructTemplate);
    constructTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    constructTemplate->SetClassName(Nan::New("Txn").ToLocalChecked());

    Nan::SetPrototypeMethod(constructTemplate, "commit", Txn::commit);
    Nan::SetPrototypeMethod(constructTemplate, "rollback", Txn::rollback);
    Nan::SetPrototypeMethod(constructTemplate, "getTxnMode", Txn::getTxnMode);
    Nan::SetPrototypeMethod(constructTemplate, "isCompleted", Txn::isCompleted);
    Nan::SetPrototypeMethod(constructTemplate, "addClass", Txn::addClass);
    Nan::SetPrototypeMethod(constructTemplate, "addSubClassOf", Txn::addSubClassOf);
    Nan::SetPrototypeMethod(constructTemplate, "dropClass", Txn::dropClass);
    Nan::SetPrototypeMethod(constructTemplate, "renameClass", Txn::renameClass);
    Nan::SetPrototypeMethod(constructTemplate, "addProperty", Txn::addProperty);
    Nan::SetPrototypeMethod(constructTemplate, "renameProperty", Txn::renameProperty);
    Nan::SetPrototypeMethod(constructTemplate, "dropProperty", Txn::dropProperty);
    Nan::SetPrototypeMethod(constructTemplate, "addIndex", Txn::addIndex);
    Nan::SetPrototypeMethod(constructTemplate, "dropIndex", Txn::dropIndex);
    Nan::SetPrototypeMethod(constructTemplate, "getDbInfo", Txn::getDbInfo);
    Nan::SetPrototypeMethod(constructTemplate, "getClasses", Txn::getClasses);
    Nan::SetPrototypeMethod(constructTemplate, "getPropertiesByClassName", Txn::getPropertiesByClassName);
    Nan::SetPrototypeMethod(constructTemplate, "getPropertiesByClassDescriptor", Txn::getPropertiesByClassDescriptor);
    Nan::SetPrototypeMethod(constructTemplate, "getClassByName", Txn::getClassByName);
    Nan::SetPrototypeMethod(constructTemplate, "getClassById", Txn::getClassById);
    Nan::SetPrototypeMethod(constructTemplate, "getProperty", Txn::getProperty);
    Nan::SetPrototypeMethod(constructTemplate, "getIndex", Txn::getIndex);
    Nan::SetPrototypeMethod(constructTemplate, "fetchRecord", Txn::fetchRecord);
    Nan::SetPrototypeMethod(constructTemplate, "addVertex", Txn::addVertex);
    Nan::SetPrototypeMethod(constructTemplate, "addEdge", Txn::addEdge);
    Nan::SetPrototypeMethod(constructTemplate, "update", Txn::update);
    Nan::SetPrototypeMethod(constructTemplate, "updateSrc", Txn::updateSrc);
    Nan::SetPrototypeMethod(constructTemplate, "updateDst", Txn::updateDst);
    Nan::SetPrototypeMethod(constructTemplate, "remove", Txn::remove);
    Nan::SetPrototypeMethod(constructTemplate, "removeAll", Txn::removeAll);
    Nan::SetPrototypeMethod(constructTemplate, "find", Txn::find);
    Nan::SetPrototypeMethod(constructTemplate, "findSubClassOf", Txn::findSubClassOf);
    Nan::SetPrototypeMethod(constructTemplate, "findInEdge", Txn::findInEdge);
    Nan::SetPrototypeMethod(constructTemplate, "findOutEdge", Txn::findOutEdge);
    Nan::SetPrototypeMethod(constructTemplate, "findEdge", Txn::findEdge);
    Nan::SetPrototypeMethod(constructTemplate, "fetchSrc", Txn::fetchSrc);
    Nan::SetPrototypeMethod(constructTemplate, "fetchDst", Txn::fetchDst);
    Nan::SetPrototypeMethod(constructTemplate, "fetchSrcDst", Txn::fetchSrcDst);
    Nan::SetPrototypeMethod(constructTemplate, "traverseIn", Txn::traverseIn);
    Nan::SetPrototypeMethod(constructTemplate, "traverseOut", Txn::traverseOut);
    Nan::SetPrototypeMethod(constructTemplate, "traverse", Txn::traverse);
    Nan::SetPrototypeMethod(constructTemplate, "shortestPath", Txn::shortestPath);

    target->Set(Nan::New("Txn").ToLocalChecked(), constructTemplate->GetFunction());
}

NAN_METHOD(Txn::New)
{
    if (!info.IsConstructCall())
    {
        return Nan::ThrowError(
            Nan::New("Txn::New - called without new keyword").ToLocalChecked());
    }
    v8::Local<v8::FunctionTemplate> contextType = Nan::New<v8::FunctionTemplate>(Context::constructor);
    if (info.Length() == 2 && contextType->HasInstance(info[0]->ToObject()) && info[1]->IsString())
    {
        Context *ctx = Nan::ObjectWrap::Unwrap<Context>(info[0]->ToObject());
        std::string mode = *Nan::Utf8String(info[1]->ToString());

        nogdb::TxnMode txnMode;
        if(mode=="READ_WRITE")      txnMode = nogdb::TxnMode::READ_WRITE;
        else if(mode=="READ_ONLY")  txnMode = nogdb::TxnMode::READ_ONLY;
        else Nan::ThrowError("Invalid txn mode");

        std::shared_ptr<nogdb::Transaction> baseTxn = std::make_shared<nogdb::Transaction>(ctx->base.beginTxn(txnMode));

        Txn *txn = new Txn(baseTxn);
        txn->Wrap(info.Holder());
        
        info.GetReturnValue().Set(info.Holder());
    }
    else
    {
        return Nan::ThrowError(Nan::New("new Txn() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::commit)
{
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        txn->base->commit();
        info.GetReturnValue().SetUndefined();        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::rollback) {
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        txn->base->rollback();
        info.GetReturnValue().SetUndefined();        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::getTxnMode) {
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        nogdb::TxnMode mode = txn->base->getTxnMode();
        std::string txnMode = "";
        if(mode==nogdb::TxnMode::READ_WRITE)      txnMode = "READ_WRITE";
        else if(mode==nogdb::TxnMode::READ_ONLY)  txnMode = "READ_ONLY";
        info.GetReturnValue().Set(Nan::New(txnMode).ToLocalChecked());        
    } catch ( nogdb::Error& err ) {
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::isCompleted) {
    //TODO don't know how to test yet --Tae
    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
    try {
        bool isCompleted = txn->base->isCompleted();
        info.GetReturnValue().Set(Nan::New(isCompleted));
    } catch ( nogdb::Error& err ){
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::addClass) {
    //TODO don't know how to test yet --Tae
    if(info.Length() == 2 && info[0]->IsString() && info[1]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        //check if READ_WRITE
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to create class.");

        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string type = *Nan::Utf8String(info[1]->ToString());

        nogdb::ClassType classType = nogdb::ClassType::VERTEX;
        if(type=="VERTEX")      classType = nogdb::ClassType::VERTEX;
        else if(type=="EDGE")   classType = nogdb::ClassType::EDGE;
        else Nan::ThrowError("ClassType Invalid");
        try {
            nogdb::ClassDescriptor classD = txn->base->addClass(className, classType);
            info.GetReturnValue().Set(v8ClassDescriptor(classD));
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.addClass() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::addSubClassOf) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::dropClass) {
    //TODO not tested yet --Tae

    if(info.Length() == 1 && info[0]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to drop class.");

        std::string className = *Nan::Utf8String(info[0]->ToString());

        try {
            txn->base->dropClass(className);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.dropClass() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::renameClass) {
    //TODO not tested yet --Tae

    if(info.Length() == 2 && info[0]->IsString() && info[1]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to drop class.");

        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string newName = *Nan::Utf8String(info[1]->ToString());

        try {
            txn->base->renameClass(className,newName);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.renameClass() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::addProperty) {
    //TODO not tested yet --Tae
    
    if(info.Length() == 3 && info[0]->IsString() && info[1]->IsString() && info[2]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to add class property.");

        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propName = *Nan::Utf8String(info[1]->ToString());
        std::string type = *Nan::Utf8String(info[2]->ToString());

        nogdb::PropertyType propertyType = nogdb::PropertyType::TEXT;
        if(type=="TINYINT")                 propertyType = nogdb::PropertyType::TINYINT;
        else if(type=="UNSIGNED_TINYINT")   propertyType = nogdb::PropertyType::UNSIGNED_TINYINT;
        else if(type=="SMALLINT")           propertyType = nogdb::PropertyType::SMALLINT;
        else if(type=="UNSIGNED_SMALLINT")  propertyType = nogdb::PropertyType::UNSIGNED_SMALLINT;
        else if(type=="INTEGER")            propertyType = nogdb::PropertyType::INTEGER;
        else if(type=="UNSIGNED_INTEGER")   propertyType = nogdb::PropertyType::UNSIGNED_INTEGER;
        else if(type=="BIGINT")             propertyType = nogdb::PropertyType::BIGINT;
        else if(type=="UNSIGNED_BIGINT")    propertyType = nogdb::PropertyType::UNSIGNED_BIGINT;
        else if(type=="TEXT")               propertyType = nogdb::PropertyType::TEXT;
        else if(type=="REAL")               propertyType = nogdb::PropertyType::REAL;
        else if(type=="BLOB")               propertyType = nogdb::PropertyType::BLOB;
        else Nan::ThrowError("propertyType Invalid");

        try {
            nogdb::PropertyDescriptor pdesc = txn->base->addProperty(className,propName,propertyType);
            info.GetReturnValue().Set(v8PropertyDescriptor(pdesc));
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.addProperty() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::renameProperty) {
    //TODO not tested yet --Tae
    
    if(info.Length() == 3 && info[0]->IsString() && info[1]->IsString() && info[2]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to rename class property.");

        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string oldPropName = *Nan::Utf8String(info[1]->ToString());
        std::string newPropName = *Nan::Utf8String(info[2]->ToString());

        try {
            txn->base->renameProperty(className,oldPropName,newPropName);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.renameProperty() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::dropProperty) {
    //TODO not tested yet --Tae
    
    if(info.Length() == 2 && info[0]->IsString() && info[1]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to drop class property.");

        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propName = *Nan::Utf8String(info[1]->ToString());

        try {
            txn->base->dropProperty(className,propName);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.dropProperty() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::addIndex) {
    //TODO not tested yet --Tae

    if(info.Length() == 3 && info[0]->IsString() && info[1]->IsString() && info[2]->IsBoolean()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to add index.");

        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propName = *Nan::Utf8String(info[1]->ToString());
        bool isUnique = info[2]->BooleanValue();

        try {
            nogdb::IndexDescriptor idesc = txn->base->addIndex(className,propName,isUnique);
            info.GetReturnValue().Set(v8IndexDescriptor(idesc));
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.addIndex() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::dropIndex) {
    //TODO not tested yet --Tae

    if(info.Length() == 2 && info[0]->IsString() && info[1]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());
        nogdb::TxnMode mode = txn->base->getTxnMode();
        if(mode!=nogdb::TxnMode::READ_WRITE)
            Nan::ThrowError("Must be READ_WRITE mode to drop index.");

        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propName = *Nan::Utf8String(info[1]->ToString());

        try {
            txn->base->dropIndex(className,propName);
            info.GetReturnValue().SetUndefined();
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.dropIndex() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::getDbInfo) {
    //TODO not tested yet --Tae

    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());

    try {
        nogdb::DbInfo dbInfo = txn->base->getDbInfo();
        info.GetReturnValue().Set(v8DBInfo(dbInfo));
    } catch ( nogdb::Error& err ){
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::getClasses) {
    //TODO not tested yet --Tae

    Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());

    try {
        std::vector<nogdb::ClassDescriptor> classes = txn->base->getClasses();
        v8::Local<v8::Array> retval = Nan::New<v8::Array>(classes.size());
        int i = 0;
        for(std::vector<nogdb::ClassDescriptor>::iterator it = classes.begin(); it != classes.end(); ++it)
        {
            Nan::Set(retval, i, v8ClassDescriptor(*it));
            i++;
        }
        info.GetReturnValue().Set(retval);
    } catch ( nogdb::Error& err ){
        Nan::ThrowError(err.what());
    }
}

NAN_METHOD(Txn::getPropertiesByClassName) {
    //TODO not tested yet --Tae

    if(info.Length() == 1 && info[0]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());

        std::string className = *Nan::Utf8String(info[0]->ToString());

        try {
            std::vector<nogdb::PropertyDescriptor> props = txn->base->getProperties(className);
            v8::Local<v8::Array> retval = Nan::New<v8::Array>(props.size());
            int i = 0;
            for(std::vector<nogdb::PropertyDescriptor>::iterator it = props.begin(); it != props.end(); ++it)
            {
                Nan::Set(retval, i, v8PropertyDescriptor(*it));
                i++;
            }
            info.GetReturnValue().Set(retval);
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getPropertiesByClassName() - invalid argument(s)").ToLocalChecked());
    }
    
}

NAN_METHOD(Txn::getPropertiesByClassDescriptor) {
    //TODO not tested yet --Tae
    if (info.Length() == 1 && info[0]->IsObject()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());

        v8::Local<v8::Object> ClassDescIn = info[0]->ToObject();
        
        v8::Local<v8::String> idProp = Nan::New("id").ToLocalChecked();
        v8::Local<v8::String> nameProp = Nan::New("name").ToLocalChecked();
        v8::Local<v8::String> baseProp = Nan::New("base").ToLocalChecked();
        v8::Local<v8::String> typeProp = Nan::New("type").ToLocalChecked();

        nogdb::ClassId *id = new nogdb::ClassId();
        std::string name = "";
        nogdb::ClassId *baseClass = new nogdb::ClassId();
        nogdb::ClassType *classType = new nogdb::ClassType();

        if (Nan::HasOwnProperty(ClassDescIn, idProp).FromJust()) {
            v8::Local<v8::Value> idValue = Nan::Get(ClassDescIn, idProp).ToLocalChecked();
            id = Nan::ObjectWrap::Unwrap<nogdb::ClassId>(idValue);
        }
        if (Nan::HasOwnProperty(ClassDescIn, nameProp).FromJust()) {
            v8::Local<v8::Value> nameValue = Nan::Get(ClassDescIn, nameProp).ToLocalChecked();
            name = *Nan::Utf8String(nameValue->ToString());
        }
        if (Nan::HasOwnProperty(ClassDescIn, baseProp).FromJust()) {
            v8::Local<v8::Value> baseValue = Nan::Get(ClassDescIn, baseProp).ToLocalChecked();
            baseClass = Nan::ObjectWrap::Unwrap<nogdb::ClassId>(baseValue);
        }
        if (Nan::HasOwnProperty(ClassDescIn, typeProp).FromJust()) {
            v8::Local<v8::Value> typeValue = Nan::Get(ClassDescIn, typeProp).ToLocalChecked();
            classType = Nan::ObjectWrap::Unwrap<nogdb::ClassType>(typeValue);
        }
        nogdb::ClassDescriptor *classDesc = new nogdb::ClassDescriptor(*id,name,*baseClass,*classType);

        std::vector<nogdb::PropertyDescriptor> props = txn->base->getProperties(*classDesc);

        v8::Local<v8::Array> retval = Nan::New<v8::Array>(props.size());
        int i = 0;
        for(std::vector<nogdb::PropertyDescriptor>::iterator it = props.begin(); it != props.end(); ++it)
        {
            Nan::Set(retval, i, v8PropertyDescriptor(*it));
            i++;
        }
        info.GetReturnValue().Set(retval);
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getPropertiesByClassDescriptor() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::getClassByName) {
    //TODO not tested yet --Tae

    if(info.Length() == 1 && info[0]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());

        std::string className = *Nan::Utf8String(info[0]->ToString());

        try {
            nogdb::ClassDescriptor classDesc = txn->base->getClass(className);
            info.GetReturnValue().Set(v8ClassDescriptor(classDesc));
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getClassByName() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::getClassById) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::getProperty) {
    //TODO not tested yet --Tae
    
    if(info.Length() == 2 && info[0]->IsString() && info[1]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());

        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propName = *Nan::Utf8String(info[1]->ToString());

        try {
            nogdb::PropertyDescriptor propDesc = txn->base->getProperty(className,propName);
            info.GetReturnValue().Set(v8PropertyDescriptor(propDesc));
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getProperty() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::getIndex) {
    //TODO not tested yet --Tae
    
    if(info.Length() == 2 && info[0]->IsString() && info[1]->IsString()){
        Txn *txn = Nan::ObjectWrap::Unwrap<Txn>(info.This());

        std::string className = *Nan::Utf8String(info[0]->ToString());
        std::string propName = *Nan::Utf8String(info[1]->ToString());

        try {
            nogdb::IndexDescriptor indexDesc = txn->base->getIndex(className,propName);
            info.GetReturnValue().Set(v8IndexDescriptor(indexDesc));
        } catch ( nogdb::Error& err ){
            Nan::ThrowError(err.what());
        }
    }
    else
    {
        return Nan::ThrowError(Nan::New("Txn.getIndex() - invalid argument(s)").ToLocalChecked());
    }
}

NAN_METHOD(Txn::fetchRecord) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::addVertex) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::addEdge) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::update) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::updateSrc) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::updateDst) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::remove) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::removeAll) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::find) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::findSubClassOf) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::findInEdge) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::findOutEdge) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::findEdge) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::fetchSrc) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::fetchDst) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::fetchSrcDst) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::traverseIn) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::traverseOut) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::traverse) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Txn::shortestPath) {
    //TODO
    info.GetReturnValue().SetUndefined();
}

v8::Local<v8::Object> Txn::NewInstance(v8::Local<v8::Value> ctx,v8::Local<v8::Value> mode) {
    Nan::EscapableHandleScope scope;

    const unsigned argc = 2;
    v8::Local<v8::Value> argv[argc] = { ctx,mode };
    v8::Local<v8::FunctionTemplate> tp1 = Nan::New<v8::FunctionTemplate>(constructor);
    v8::Local<v8::Function> cons = Nan::GetFunction(tp1).ToLocalChecked();
    v8::Local<v8::Object> instance = Nan::NewInstance(cons,argc, argv).ToLocalChecked();

    return scope.Escape(instance);
}
