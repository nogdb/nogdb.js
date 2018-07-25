# nogdb.js Documentation
## Database Context
A database context must be initialized to create a connection to NogDB before calling any database operation. Noted that a particular database can be opened by only one process at a time and its database context needs to be treated as a singleton object in multi-thread programmings.

When the context is created:

- If a database directory doesn’t exist, it will make a new one with a default database schema.

- If a database directory has already existed, it will load the existing database schema and information into memory.

```javascript
// constructor
	const ctx = new nogdb.Context('mygraph.db');
// or	const ctx = newContext('mygraph.db');
```
> `ctx` is Context Object (Context{})

##### [NOTE]

- Each database must have its own individual database context object.
- It is allowed to have more than one database context when it uses more than one database instance simultaneously (one database per one context).

### Functions of Context Object (Transaction Stats)
#### getMaxVersionId()
  - -- getting the maximum number of transaction id
  - It will return maxVersionId in `Number` type
  - Example
```javascript
var maxVersionId = ctx.getMaxVersionId();
```
#### getMaxTxnId()
  - -- getting the maximum number of graph version id
  - It will return `maxTxnId` in `Number` type
  - Example
```javascript
var maxTxnId = ctx.getMaxTxnId();
```
#### getMinActiveTxnId()
  - -- the oldest active read-only transaction
  - It will return `minActiveTxnId` in `Object` type
  `minActiveTxnId` is a pair of particular txn id and latest graph version id related to it
> - `minActiveTxnId` look like `{ first: value_1 , second : value_2}`  
> - `first` and `second` are key of minActiveTxnId.
> - `value_1` and `value_2` are pair value of TxnId.
  
  - Example
```javascript
var minActiveTxnId = ctx.getMinActiveTxnId();
```

------------

## Transaction
Database operations can be performed and controlled via a transaction. According to a concept of NogDB transaction, MVCC is applied together with copy-on-write which allows multiple readers not to block a writer when using the same database context and vice versa. Generally, the database will take an effect after the transaction is committed and it will be untouched if the transaction is rolled back or not yet completed.

```javascript
// constructor , begin a transaction
	const txn = new nogdb.Txn(ctx,'READ_WRITE');	// to create a read-write txn
// or	const txn = newTxn(ctx,'READ_WRITE');

	const txn = new nogdb.Txn(ctx,'READ_ONLY');		// to create a read-only txn
// or	const txn = newTxn(ctx,'READ_ONLY');
```
> - `txn` is Txn Object (Txn{})
> - `ctx` - A  database context is `Context{}` type
> - `'READE_WRITE'` - A mode of txn refer to read-write txn
> - `'READE_ONLY'` - A mode of txn refer to read-only txn

##### [NOTE]

- A read-only transaction cannot be used for executing create/update/delete operations, only read operations are allowed, otherwise, the exceptions will be thrown.

- Multiple read-only transactions can be created and valid simultaneously without being blocked.

- Read-write transactions will block each other excluding read-only transactions. In other words, only one read-write transaction can be valid (accessing to the critical section) during the period of time until committed or aborted.

- If a transaction is not committed before its desctructor is called, the transaction will be aborted automatically.

- Transactions that have already been committed or aborted cannot be usable. Forcing to use a completed transaction will result in the exception thrown.

### Functions of Txn Object

#### commit()
  - -- commit
  - Example
```javascript
txn.commit();
```
#### rollback()
  - -- rollback or abort
  - Example
```javascript
txn.rollback();
```
#### getTxnId()
  - -- getting the number of transaction id
  - It will return txnId in `Number` type
  - Example
```javascript
var txnId = txn.getTxnId();
```
#### getVersionId()
  - -- getting the number of graph version id
  - It will return versionId in `Number` type
  - Example
```javascript
var versionId = txn.getVersionId();
```
#### getTxnMode()
  - -- getting the number of txn mode
  - It will return txnMode in `Number` type
  > txnMode = 1 meaning txn mode is READ_WRITE
  > txnMode = 0 meaning txn mode is READ_ONLY
  - Example
```javascript
var txnMode = txngetTxnMode();
```

------------

## Record
```javascript
// constructor, or create an empty record
	var record = new nogdb.Record();
// or	var record = nogdb.newRecord();
```
> `record` is Record Object (Record{})

### Functions of Record Object
#### set()
  - -- To set a value of a property in a record.
  - Example
```javascript
record.set(propName,value);
```
> - `propName` - A name of a property is `String `type
> - `value` A value of with an appropriate data type corresponding to the schema is `String` or `Number` type

#### getProperties()
  - -- To retrieve names from all existing properties in a record.
  - It will return `properties` in `Array` type
> properties - A array of existing property names.

  - Example
```javascript
var properties = record.getProperties();
```
#### unset()
  - -- To clear an individual property and its value in a record.
  - Example
```javascript
record.unset(propName);
```
> - `propName` - A name of a property to be deleted is `String `type

#### size()
 - -- getting the size of record
  - It will return `size` in `Number` type
  - Example
```javascript
var size = record.size();
```
#### empty()
 - -- To check if a record is empty.
  - It will return `isEmpty` in `Boolean` type
  - Example
```javascript
var isEmpty = record.empty();
```
#### clear()
  - -- To clear all properties and values in a record.
  - Example
```javascript
record.clear();
```

------------

## Condition
A comparable object in NogDB which is used to compare records with a defined condition.

```javascript
// constructor
	var condition = new nogdb.Condition(propName); // having NOT NULL operation by default
// or	var condition = nogdb.newCondition(propName);
```
> - `condition` is Condition Object (Condition{})
> - `propName` - A name of a property is `String `type

### Functions of Record Object
```javascript
// -- EQUAL: available for 'Number' and `String` types
condition.eq(propValue);

// -- GREATER: available for 'Number' and `String` types
condition.gt(propValue);

// -- GREATER EQUAL: available for 'Number' and `String` types
condition.ge(propValue);

// -- LESS: available for 'Number' and `String` types
condition.lt(propValue);

// -- LESS EQUAL: available for 'Number' and `String` types
condition.le(propValue);

// -- CONTAIN: available ONLY for `String` type
condition.contain(propSubstring);

// -- BEGIN WITH: available ONLY for `String` type
condition.beginWith(propSubstring);

// -- END WITH: available ONLY for `String` type
condition.endWith(propSubstring);

// -- LIKE: available ONLY for `String` type
// using '%' for representing zero, one, or multiple characters
// and using '_' for representing a single character
condition.like(propPattern);

// -- REGEX: available ONLY for `String` type
condition.regex(propPattern);

// -- IS NULL
condition.null()

//Note that comparing string in a condition can apply ignoreCase() to perform case insensitive matching. By default, it is case sensitive.
// -- CONTAIN -IgnoreCase
condition.contain(propSubstring)
condition.ignoreCase();
// -- BEGIN WITH -IgnoreCase
condition.beginWith(propSubstring)
condition.ignoreCase();
// -- END WITH -IgnoreCase
condition.endWith(propSubstring)
condition.ignoreCase();
// -- LIKE WITH -IgnoreCase
condition.like(propPattern)
condition.ignoreCase();
// -- REGEX -IgnoreCase
condition.regex(propPattern)
condition.ignoreCase();
```
> - `propValue` is `Number` or `String` types
> - `propSubstring` is `String` types
> - `propPattern` is `String` types


------------

## Class Filter
A class filtering is a set of class names which is internally represented as `std::set<std::string>` used to choose only some particular classes in the result set of edge functions like `*InEdge`, `*OutEdge`, `*AllEdge`, and `graph traversal` functions.

```javascript
// constructor
	var classFilter = new nogdb.ClassFilter(["className1","className2, ..."]);
// or	var classFilter = nogdb.newClassFilter(["className1","className2, ..."]);
```
> - `classFilter` is ClassFilter Object (ClassFilter{})
> - `["className1","className2, ..."]` - A array of a name of class is `Array` type
> -  `className` - A name of class is `String` type

### Functions of ClassFilter Object
#### add()
  - -- To add a name of a class in a classFilter.
  - Example
```javascript
classFilter.add(className);
```
> - `className` - A name of a class is `String `type
#### remove()
  - -- To remove an individual class in a classFilter.
  - Example
```javascript
classFilter.remove(className);
```
> - `className` - A name of a class is `String `type
#### size()
 - -- getting the size of classFilter
  - It will return `size` in `Number` type
  - Example
```javascript
var size = classFilter.size();
```
#### empty()
 - -- To check if a classFilter is empty.
  - It will return `isEmpty` in `Boolean` type
  - Example
```javascript
var isEmpty = classFilter.empty();
```
#### getClassName
  - -- To retrieve names from all existing class in a classFilter.
  - It will return `properties` in `Array` type
> properties - A array of existing class names.
  - Example
```javascript
var classNames = classFilter.getClassName();
```

------------

## Structure of return value
### ClassDescriptor {}

| Key  | Value  | Meaning |
| :------------ | :------------ | :------------ |
| id | `Number` | -- An id of class |
| name | `String` | -- A name of class |
| type | `String` | -- A type of class ('VERTEX' or 'EDGE') |
| properties | `Object` `Properties {}` | -- An object of properties |
| super | `String` | -- A name of super class |
| sub | `Array` | -- An array of a name of sub class |

   > - `Properties {}`
   >   - `key` is a name of property
   >   - `value` is `PropertyDescriptor {}`
   

### PropertyDescriptor {}

| Key  | Value  | Meaning |
| :------------ | :------------ | :------------ |
| type | `String` | -- A type of property |
| indexInfo | `Object` `IndexInfo {}` | -- An object of IndexInfo|
> - `IndexInfo {}`
>   - `key` is a number of indexId
>   - `value` is `Object` it have 2 keys( `classId` and `isUinque`) refer to infomation of index

### RecordDescriptor {}

| Key  | Value  | Meaning |
| :------------ | :------------ | :------------ |
| rid| `RecordId {}`| An object of record id|

### Record {}

| Key  | Value  | Meaning |
| :------------ | :------------ | :------------ |
| properties|`Array`| -- An array of a name of property|
| value| `Objects` `Value {}` | -- An object of value |
| className | `String`| -- A name of class |
| rid | `RecordId` | -- An object of record id |
| depth | `Number` | A number refer to distance of a vertex node from the source node if the record is a result of graph traversal operations (@depth = 0 by default if other operations are applied).|
| version | `Number`| A number refer to version number of a record. Any updates and changes in properties or relations would result in the version increment by one.|
| size | `Number`| A size of class|
| empty | `Boolean` | To check if a record is empty. |

> - `Value {}`
>   - It have 2 keys
>     - `toText`  : value is `String` type
>     - `toNumber`  : value is `Number` type
>   - The type of property is `'TEXT'` it send the invalid value for `toNumber`
>   - And Another type send invalid value for `toText`
> - This `Record {}` could not to be the parameter in `Vertex.create()` , `Vertex.update()` , `Edge.create()` and `Edge.update()` 

### RecordId {}

| Key  | Value  | Meaning |
| :------------ | :------------ | :------------ |
| classId | `Number` | An id of class|
| positionId | `Number` | An id of position |

### ResultSet
> It is an `Array` of `Result {}`

### Result {}

| Key  | Value  | Meaning |
| :------------ | :------------ | :------------ |
| recordDescriptor|`RecordDescriptor {}`| An object of recordDescriptor|
| record| `Record {}`| An object of record|

### DbInfo {}

| Key  | Value  | Meaning |
| :------------ | :------------ | :------------ |
|dbPath| `String`|A path to the database folder.|
|maxDB| `Number`|A maximum number of databases that can be handled.|
|maxDBSize| `Number`|the largest size of a database.|
|maxPropertyId| `Number`|The largest property number(id) in the entire database.|
|numProperty| `Number`|A number of properties in the database.|
|maxClassId| `Number`|The largest class number(id) in the entire database.|
|numClass| `Number` |A number of classes in the database.|
|maxIndexId| `Number`|The largest index number(id) in the entire database.|
|numIndex| `Number`|A number of index in the database.|

------------


## nogdb.js Operations
### Class Operations
#### create()

```javascript
var classDescriptor = nogdb.Class.create(txn,className,type);
```
- Description:
   - To create a new class.

- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A name of a class that will be created.
   
   - `String` , type - A type of a class. Note that there are two class types available, `'VERTEX'` and `'EDGE'`
   
- Return:
   - `ClassDescriptor {}` , classDescriptor - A class descriptor of a created class.
   
- Exceptions:
   - `CTX_INVALID_CLASSNAME` - A length of class name is zero.
   
   - `CTX_INVALID_CLASSTYPE` - A type of class is not valid.
   
   - `CTX_DUPLICATE_CLASS` - A specified class name has already existed.
   
   - `CTX_LIMIT_DBSCHEMA` - A limitation of a database schema has been reached.
   
#### createExtend()
```javascript
var classDescriptor = nogdb.Class.createExtend(txn,className,superClassName);
```

- Description:
   - To create a sub-class (aka. a derived class) of a super class (aka. a base class).
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A name of a sub-class that will be created.

   - `String` , superClassName - A name of a super class that will be derived from.
   
- Return:
   - `ClassDescriptor {}` , classDescriptor - A class descriptor of a created sub-class.
   
- Exceptions:

   - `CTX_INVALID_CLASSNAME` - A length of class name is zero.

   - `CTX_DUPLICATE_CLASS` - A specified class name has already existed.

   - `CTX_LIMIT_DBSCHEMA` - A limitation of a database schema has been reached.
   
#### drop()
```javascript
nogdb.Class.drop(txn,className);
```
- Description:
   - To drop a class.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A name of a class that will be dropped.
   
- Exceptions:
   - `CTX_INVALID_CLASSTYPE` - A type of class is not valid.

   - `CTX_NOEXST_CLASS` - A class does not exist.
   
#### alter()
```javascript
nogdb.Class.alter(txn,oldClassName,newClassName);
```

- Description:
   - To modify a class name.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , oldClassName - An old name of a class that will be changed from.

   - `String` , newClassName - A new name of a class that will be changed to.
   
- Exceptions:
   - `CTX_INVALID_CLASSNAME` - A length of a new class name is zero.

   - `CTX_NOEXST_CLASS` - An old class does not exist.

   - `CTX_DUPLICATE_CLASS` - A new class name has already existed.
   
### Property Operations
#### add()
```javascript
var propertyDescriptor = nogdb.Property.add(txn,className,propertyName,type); 
```

- Description:
   - To add a property to a class.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A name of a class that a property will be added into.

   - `String` , propertyName - A name of a property that will be added.

   - `String` , type - A type of a property. (Please refer to "Types of Properties").
   > The value of type
   > - `'TINYINT'`
   >   - Range of Values = -128 to 127
   
   > - `'UNSIGNED_TINYINT'`
   >   - Range of Values = 0 to 255
   
   > - `'SMALLINT'`
   >   - Range of Values = –32,768 to 32,767
   
   > - `'UNSIGNED_SMALLINT'`
   >   - Range of Values = 0 to 65,535
   
   > - `'INTEGER'`
   >   - Range of Values = –2,147,483,648 to 2,147,483,647
   
   > - `'UNSIGNED_INTEGER'`
   >   - Range of Values = 0 to 4,294,967,295
   
   > - `'BIGINT'`
   >   - Range of Values = –9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
   
   > - `'UNSIGNED_BIGINT'`
   >   - Range of Values = 0 to 18,446,744,073,709,551,615
   
   > - `'REAL'`
   >   - Range of Values = 1.7E +/- 308 (15 digits)
   
   > - `'TEXT'`
   >   - Range of Values = no boundary

- Return:
   - `PropertyDescriptor {}` , propertyDescriptor - A property descriptor of a created property.
   
- Exceptions:
   - `CTX_INVALID_PROPERTYNAME` - A length of property name is zero.

   - `CTX_INVALID_PROPTYPE` - A type of class is not valid.

   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_DUPLICATE_PROPERTY` - A specified property name has already existed.

   - `CTX_OVERRIDE_PROPERTY` - A speficied property name can be overriden the others among its sub-class.

   - `CTX_LIMIT_DBSCHEMA` - A limitation of a database schema has been reached.
   
#### alter()
```javascript
nogdb.Property.alter(txn,className,oldPropertyName,newPropertyName);
```
- Description:
   - To modify a property’s name.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A name of a class to which a property currently belongs.

   - `String` , oldPropertyName - An old name of a property that will be changed from.

   - `String` , newName - A new name of a property that will be changed to.
   
- Exceptions:
   - `CTX_INVALID_PROPERTYNAME` - A length of a new property name is zero.

   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_NOEXST_PROPERTY` - An old property name does not exist.

   - `CTX_DUPLICATE_PROPERTY` - A new property name has already existed.

   - `CTX_OVERRIDE_PROPERTY` - A speficied property name can be overriden the others among its sub-class.
   
#### remove()
```javascript
nogdb.Property.remove(txn,className,propertyName);
```
- Description:
   - To delete a property.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A name of a class to which a property currently belongs.

   - `String` , propertyName - A name of an existing property that will be deleted.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.
   
#### createIndex()
```javascript
nogdb.Property.createIndex(txn,className,propertyName,isUnique);
nogdb.Property.createIndex(txn,className,propertyName);					// isUnique = false
```
- Description:
   - To create an index on a specified property.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A name of a class to which a property currently belongs.

   - `String` , propertyName - A name of an existing property that will be indexed.
   
   - `Boolean` , isUnique
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_LIMIT_DBSCHEMA` - A limitation of a database schema has been reached.

   - `CTX_INVALID_PROPTYPE_INDEX` - A property type does not support database indexing.

   - `CTX_DUPLICATE_INDEX` - An index has already existed.

   - `CTX_INVALID_INDEX_CONSTRAINT` - An index could not be created with a unique constraint due to some duplicated values in existing records.
   
#### dropIndex()
```javascript
nogdb.Property.dropIndex(txn,className,propertyName);
```

- Description:
   - To drop an index on a specified property.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A name of a class to which a property currently belongs.

   - `String` , propertyName - A name of an existing property with an index that will be removed.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_NOEXST_INDEX` - An index does not exist on a specified class and property.
   
### Database Operations
#### getRecord()
```javascript
var record = nogdb.Db.getRecord(txn,recordDescriptor);
```
- Description:
   - To get a record from a record descriptor.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor` , recordDescriptor - A record descriptor.
   
- Return:
   - `Record {}` , record - A record of a specified record descriptor.

- Exceptions:
   - `CTX_NOEXST_RECORD` - A record with the given descriptor does not exist.

   - `CTX_NOEXST_CLASS` - A class does not exi
   
#### getSchema()
```javascript
// Version 1
var classDescriptors = nogdb.Db.getSchema(txn);

// Version 2
var classDescriptor = nogdb.Db.getSchema(txn,className);
var classDescriptor = nogdb.Db.getSchema(txn,classId);
```
- Description:
   - To retrieve a schema information.

   - **[Version 1] **return a read-only a array of schema `ClassDescriptor {}` for the whole classes in the database schema.

   - **[Version 2]** return a read-only `ClassDescriptor {}`
   
- Parameters:
   - `Txn{}` ,txn - A database transaction.

   - `String` , className - A specified name of a class to be retrieved (only for version 2).

   - `Number` ,classId - An id of a class to be retrieved (only for version 2).
   
- Return:
   - `[ClassDescriptor {},...]` , classDescriptors - An array of classes in the whole database schema.

   - or `ClassDescriptor {}` , classDescriptor - A schema of a specified class.
   
#### getDbInfo()
```javascript
var dbInfo = nogdb.Db.getDbInfo(txn);
```
- Description:
   - To retrieve a database (metadata) information.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.
   
- Return:
   - `DBInfo {}` , dbInfo - A database information
   
### Vertex Operations
#### create()
```javascript
var recordDescriptor = nogdb.Vertex.create(txn,className,record);
var recordDescriptor = nogdb.Vertex.create(txn,className);					// record is empty
```
- Description:
   - To create a vertex.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A name of a class.

   - `Record{}` , record - A record object.
   
- Return:
   - `RecordDescriptor {}` , recordDescriptor - A record descriptor of a created vertex.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_EMPTY_RECORD` - A class has no properties (required at least one).

> Some possible LMDB errors: MDB_PANIC, MDB_MAP_RESIZED, MDB_READERS_FULL, MDB_NOTFOUND, MDB_DBS_FULL, MDB_MAP_FULL, MDB_TXN_FULL

#### update()
```javascript
nogdb.Vertex.upadte(txn,recordDescriptor,record);
```
- Description:
   - To update a vertex.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor.

   - `Record{}` , record - A new record object with modified properties and values.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_EMPTY_RECORD` - A class has no properties (required at least one).

   - `GRAPH_NOEXST_VERTEX` - A vertex doesn't exist.
   
#### destroy()
```javascript
nogdb.Vertex.destroy(txn,recordDescriptor);
nogdb.Vertex.destroy(txn,className);
```
- Description:
   - To delete a vertex or all vertices in the same class. All associated edges will be deleted as well.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDesciptor - A record descriptor.

   - `String` , className - A class name of vertices that will be entirely deleted.

- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.
   
#### get()
```javascript
// To get all vertices in specific classes.
var resultSet = nogdb.Vertex.get(txn,className);
// To find vertices with a given condition.
var resultSet = nogdb.Vertex.get(txn,className,condition);
```
- Description:
   - To get all vertices in specific classes.
   
   - or To find vertices with a given condition.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A class name of a vertex.

   - `Condition{}` , condition - A condition that consists of a name of a property, a value, and a comparator used in searching for records.
   
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_INVALID_COMPARATOR` - A comparator is not defined.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `CTX_CONFLICT_PROPTYPE` - A type of some properties in a condition used in finding vertices conflicts.
   
#### getInEdge()
```javascript
// To find edges which are incoming edges of a vertex.
var resultSet = nogdb.Vertex.getInEdge(txn,recordDescriptor,classFilter);
var resultSet = nogdb.Vertex.getInEdge(txn,recordDescriptor);
// To find edges with a given condition which are incoming edges of a vertex.
var resultSet = nogdb.Vertex.getInEdge(txn,recordDescriptor,condition,classFilter);
var resultSet = nogdb.Vertex.getInEdge(txn,recordDescriptor,condition);
```
- Description:
   - To find edges which are incoming edges of a vertex.
   
   - or To find edges with a given condition which are incoming edges of a vertex.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - a record descriptor of a vertex.

   - `Condition{}` , condition - A condition that consists of a name of a property, a value, and a comparator used in searching for records.

   - `ClassFilter` , classFilter - A class filtering. If this parameter is specified, only edges with a given class name (or class names) will be returned (optional).
   
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_INVALID_COMPARATOR` - A comparator is not defined.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `CTX_CONFLICT_PROPTYPE` - A type of some properties in a condition used in finding vertices conflicts.

   - `GRAPH_NOEXST_VERTEX` - A vertex doesn’t exist.
   
#### getOutEdge()
```javascript
// To find edges which are outgoing edges of a vertex.
var resultSet = nogdb.Vertex.getOutEdge(txn,recordDescriptor,classFilter);
var resultSet = nogdb.Vertex.getOutEdge(txn,recordDescriptor);
// To find edges with a given condition which are outgoing edges of a vertex.
var resultSet = nogdb.Vertex.getOutEdge(txn,recordDescriptor,condition,classFilter);
var resultSet = nogdb.Vertex.getOutEdge(txn,recordDescriptor,condition);
```
- Description:
   - To find edges which are outgoing edges of a vertex.
   
   - or To find edges with a given condition which are outgoing edges of a vertex.

- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - a record descriptor of a vertex.

   - `Condition{}` , condition - A condition that consists of a name of a property, a value, and a comparator used in searching for records.

   - `ClassFilter{}` , classFilter - A class filtering. If this parameter is specified, only edges with a given class name (or class names) will be returned (optional).

- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.

- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_INVALID_COMPARATOR` - A comparator is not defined.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `CTX_CONFLICT_PROPTYPE` - A type of some properties in a condition used in finding vertices conflicts.

   - `GRAPH_NOEXST_VERTEX` - A vertex doesn’t exist.
   
#### getAllEdge()
```javascript
// To find all edges (both direction) which are associated with a vertex.
var resultSet = nogdb.Vertex.getAllEdge(txn,recordDescriptor,classFilter);
var resultSet = nogdb.Vertex.getAllEdge(txn,recordDescriptor);
// To find edges (both direction) that are associated with a vertex, and with a given condition.
var resultSet = nogdb.Vertex.getAllEdge(txn,recordDescriptor,condition,classFilter);
var resultSet = nogdb.Vertex.getAllEdge(txn,recordDescriptor,condition);
```
- Description:
   - To find all edges (both direction) which are associated with a vertex.
   
   - or To find edges (both direction) that are associated with a vertex, and with a given condition.

- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - a record descriptor of a vertex.

   - `Condition{}` , condition - A condition that consists of a name of a property, a value, and a comparator used in searching for records.

   - `ClassFilter{}` , classFilter - A class filtering. If this parameter is specified, only edges with a given class name (or class names) will be returned (optional).

- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.

- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_INVALID_COMPARATOR` - A comparator is not defined.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `CTX_CONFLICT_PROPTYPE` - A type of some properties in a condition used in finding vertices conflicts.

   - `GRAPH_NOEXST_VERTEX` - A vertex doesn’t exist.
   
#### getIndex()
```javascript
var resultSet = nogdb.Vertex.getIndex(txn,className,condition);
```
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A class name as std::string of an edge.

   - `Condition{}` , condition - A condition that consists of a name of a property, a value, and a comparator used in searching for records.
   
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.

### Edge Operations
#### create()
```javascript
var recordDescriptor = nogdb.Edge.create(txn,className,srcVertexRecordDescriptor,dstVertexRecordDescriptor,record);
var recordDescriptor = nogdb.Edge.create(txn,className,srcVertexRecordDescriptor,dstVertexRecordDescriptor);				// record = empty
```
- Description:
   - To create an edge.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.
   
   - `String` , className - A name of a class.

   - `RecordDescriptor{}` , srcVertexRecordDescriptor - A record descriptor of a source vertex.

   - `RecordDescriptor{}` , dstVertexRecordDescriptor - A record descriptor of a destinaton vertex.

   - `Record{}` , record - A record object as nogdb::Record (can be empty if not specified).
   
- Return:
   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of a created edge.

- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_EMPTY_RECORD` - A class has no properties (required at least one).

   - `GRAPH_NOEXST_SRC` - A source vertex doesn’t exist.

   - `GRAPH_NOEXST_DST` - A destination vertex doesn’t exist.
   
#### update()
```javascript
nogdb.Edge.update(txn,recordDescriptor,record);
```
- Description:
   - To update an edge.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor` , recordDescriptor - A record descriptor.

   - `Record{}` , record - A new record object with modified properties and values.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_EMPTY_RECORD` - A class has no properties (required at least one).

   - `GRAPH_NOEXST_EDGE` - An edge doesn’t exist.
   
#### updateSrc()
```javascript
nogdb.Edge.updateSrc(txn,recordDescriptor,srcVertexRecordDescriptor);
```
- Description:
   - To update a source vertex of an edge.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of an edge itself.

   - `RecordDescriptor{}` , newSrcVertexRecordDescriptor - A record descriptor of a new source vertex.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_EDGE` - An edge doesn’t exist.

   - `GRAPH_NOEXST_SRC` - A source vertex doesn’t exist.
   
#### updateDst()
```javascript
nogdb.Edge.updateDst(txn,recordDescriptor,dstVertexRecordDescriptor);
```
- Description:
   - To update a destination vertex of an edge.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDesriptor{}` , recordDescriptor - A record descriptor of an edge itself.

   - `RecordDescriptor{}` , newDstVertexRecordDescriptor - A record descriptor of a new destination vertex.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_EDGE` - An edge doesn’t exist.

   - `GRAPH_NOEXST_DST` - A destination vertex doesn’t exist.
   
#### destroy()
```javascript
nogdb.Edge.destroy(txn,recordDescriptor);
nogdb.Edge.destroy(txn,className);
```
- Description:
   - To delete an edge or all edges in the same class.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor.

   - `String` , className - A class name of edges that will be entirely deleted.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.
   
#### get()
```javascript
// To get all edges in a specific class.
var resultSet = nogdb.Edge.get(txn,className);
// To find edges with a given condition.
var resultSet = nogdb.Edge.get(txn,className,condition);
```
- Description:
   - To get all edges in a specific class.
   
   - or To find edges with a given condition.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A class name as std::string of an edge.

   - `Condition{}` , condition - A condition that consists of a name of a property, a value, and a comparator used in searching for records.
   
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_NOEXST_PROPERTY` - A property does not exist.

   - `CTX_INVALID_COMPARATOR` - A comparator is not defined.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `CTX_CONFLICT_PROPTYPE` - A type of some properties in a condition used in finding edges conflicts.
   
#### getSrc()
```javascript
var result = nogdb.Edge.getSrc(txn,recordDescriptor);
```
- Description:
   - To find a vertex which is a source node of an out-edge.
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of an out-edge.

- Return:
   - `Result{}` , result - A result of record descriptor and record.

- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_EDGE` - An edge doesn’t exist.
#### getDst()
```javascript
var result = nogdb.Edge.getDst(txn,recordDescriptor);
```
- Description:
   - To find a vertex which is a destination node of an in-edge.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of an out-edge.
- Return:
   - `Result{}` , result - A result of record descriptor and record.

- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_EDGE` - An edge doesn’t exist.
   
#### getSrcDst()
```javascript
var resultSet = nogdb.Edge.getSrcDst(txn,recordDescriptor);
```
- Description:
   - To find both source and destication vertices which is associated with an interesting edge.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of an interesting edge.
   
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results including source and destination vertices.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_EDGE` - An edge doesn’t exist.
   
#### getIndex()
```javascript
var resultSet = nogdb.Edge.getIndex(txn,className,condition);
```
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `String` , className - A class name as std::string of an edge.

   - `Condition{}` , condition - A condition that consists of a name of a property, a value, and a comparator used in searching for records.
   
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.

### Traverse (Graph) Operations
#### inEdgeBfs()
```javascript
var resultSet = nogdb.Traverse.inEdgeBfs(txn,recordDescriptor,minDepth,maxDepth,classFilter);
var resultSet = nogdb.Traverse.inEdgeBfs(txn,recordDescriptor,minDepth,maxDepth);
```
- Description:
   - To find vertices as a result of in-edges graph traversal (BFS) starting from a specific vertex to other vertices.
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of a vertex.

   - `Number` , minDepth - A minimum level of graph traversal.

   - `Number` , maxDepth - A maximum level of graph traversal.

   - `String` , classFilter - A class filtering. If this parameter is specified, only edges with a given class name (or class names) will be returned (optional).
	
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_VERTEX` - A vertex doesn’t exist.
   
#### outEdgeBfs()
```javascript
var resultSet = nogdb.Traverse.outEdgeBfs(txn,recordDescriptor,minDepth,maxDepth,classFilter);
var resultSet = nogdb.Traverse.outEdgeBfs(txn,recordDescriptor,minDepth,maxDepth);
```
- Description:
   - To find vertices as a result of out-edges graph traversal (BFS) starting from a specific vertex to other vertices.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of a vertex.

   - `Number` , minDepth - A minimum level of graph traversal.

   - `Number` , maxDepth - A maximum level of graph traversal.

   - `String` , classFilter - A class filtering. If this parameter is specified, only edges with a given class name (or class names) will be returned (optional).
	
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_VERTEX` - A vertex doesn’t exist.
   
#### allEdgeBfs()
```javascript
var resultSet = nogdb.Traverse.allEdgeBfs(txn,recordDescriptor,minDepth,maxDepth,classFilter);
var resultSet = nogdb.Traverse.allEdgeBfs(txn,recordDescriptor,minDepth,maxDepth);
```
- Description:
   - To find vertices as a result of in- and out-edges graph traversal (BFS) starting from a specific vertex to other vertices.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of a vertex.

   - `Number` , minDepth - A minimum level of graph traversal.

   - `Number` , maxDepth - A maximum level of graph traversal.

   - `String` , classFilter - A class filtering. If this parameter is specified, only edges with a given class name (or class names) will be returned (optional).
	
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_VERTEX` - A vertex doesn’t exist.
   
#### inEdgeDfs()
```javascript
var resultSet = nogdb.Traverse.inEdgeDfs(txn,recordDescriptor,minDepth,maxDepth,classFilter);
var resultSet = nogdb.Traverse.inEdgeDfs(txn,recordDescriptor,minDepth,maxDepth);
```
- Description:
   - To find vertices as a result of in-edges graph traversal (DFS) starting from a specific vertex to other vertices.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of a vertex.

   - `Number` , minDepth - A minimum level of graph traversal.

   - `Number` , maxDepth - A maximum level of graph traversal.

   - `String` , classFilter - A class filtering. If this parameter is specified, only edges with a given class name (or class names) will be returned (optional).
	
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_VERTEX` - A vertex doesn’t exist.
   
#### outEdgeDfs()
```javascript
var resultSet = nogdb.Traverse.outEdgeDfs(txn,recordDescriptor,minDepth,maxDepth,classFilter);
var resultSet = nogdb.Traverse.outEdgeDfs(txn,recordDescriptor,minDepth,maxDepth);
```
- Description:
   - To find vertices as a result of out-edges graph traversal (DFS) starting from a specific vertex to other vertices.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of a vertex.

   - `Number` , minDepth - A minimum level of graph traversal.

   - `Number` , maxDepth - A maximum level of graph traversal.

   - `String` , classFilter - A class filtering. If this parameter is specified, only edges with a given class name (or class names) will be returned (optional).
	
- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_VERTEX` - A vertex doesn’t exist.
   
#### allEdgeDfs()
```javascript
var resultSet = nogdb.Traverse.allEdgeDfs(txn,recordDescriptor,minDepth,maxDepth,classFilter);
var resultSet = nogdb.Traverse.allEdgeDfs(txn,recordDescriptor,minDepth,maxDepth);
```
- Description:
   - To find vertices as a result of in- and out-edges graph traversal (DFS) starting from a specific vertex to other vertices.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   - `RecordDescriptor{}` , recordDescriptor - A record descriptor of a vertex.

   - `Number` , minDepth - A minimum level of graph traversal.

   - `Number` , maxDepth - A maximum level of graph traversal.

   - `String` , classFilter - A class filtering. If this parameter is specified, only edges with a given class name (or class names) will be returned (optional).

- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_VERTEX` - A vertex doesn’t exist.
   
#### shortestPath()
```javascript
var resultSet = nogdb.Traverse.shortestPath(txn,srcVertexRecordDescriptor,dstVertexRecordDescriptor,classFilter);
var resultSet = nogdb.Traverse.shortestPath(txn,srcVertexRecordDescriptor,dstVertexRecordDescriptor);
```
- Description:
   - To find all vertices in the shortest path (BFS) from a source vertex to a destination vertex.
   
- Parameters:
   - `Txn{}` , txn - A database transaction.

   -`RecordDescriptor{}` , srcVertexRecordDescriptor - A record descriptor of a source vertex.

   - `RecordDescriptor{}` , dstVertexRecordDescriptor - A record descriptor of a destination vertex.

   - `ClassFilter{}` , classFilter - A class filtering. If this parameter is specified, only edges with a given class name (or class names) will be returned (optional).

- Return:
   - `ResultSet` or `[Result{},..]` , resultSet - An array of results.
   
- Exceptions:
   - `CTX_NOEXST_CLASS` - A class does not exist.

   - `CTX_MISMATCH_CLASSTYPE` - A type of a class does not match as expected.

   - `GRAPH_NOEXST_SRC` - A source vertex doesn’t exist.

   - `GRAPH_NOEXST_DST` - A destination vertex doesn’t exist.