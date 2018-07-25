# nogdb.js
Node.js bindings to the [NogDB](https://nogdb.org/ "NogDB") library

------------

## Installation
We rely on [node-gyp](https://github.com/nodejs/node-gyp "node-gyp").

### How to install
- first step : Install [NogDB](https://github.com/nogdb/nogdb "NogDB") library.
- second step : Clone repo and npm install.
```
git clone https://github.com/nogdb/nogdb.js.git
cd nogdb.js
npm install
```
> [Note] : You need python2.7 for node-gyp

------------
## How to use
- **First step** In file javascript require nogdb in this form.

```javascript
const nogdb = require("./build/Debug/nogdb_js");
```
- **Second step** you must be create [Database Context](https://github.com/nogdb/nogdb.js/blob/master/doc.md#database-context "Database Context") before use another function like this.

```javascript
// "mygraph.db" is database path.
const ctx = new nogdb.Context("mygraph.db");
```

- **Third step** And create [Transaction](https://github.com/nogdb/nogdb.js/blob/master/doc.md#transaction "Transaction") for use [Database operations](Database Operations "Database operations") like this.

```javascript
// Transaction has 2 mode "READ_WRITE" for modifired database and "READ_ONLY" can't modified database
var txn = new nogdb.Txn(ctx,"READ_WRITE");
```
> In "READ_WRITE" mode you can call `txn.commit();` after complete the transaction for affect to database.

- **Fourth step** you can use [Database operations](Database Operations "Database operations").

 - `Class` Operations from [nogdb.Class](https://github.com/nogdb/nogdb.js/blob/master/doc.md#class-operations "nogdb.Class")

  - `Property` Operations from [nogdb.Property](https://github.com/nogdb/nogdb.js/blob/master/doc.md#property-operations "nogdb.Property")

  - `Database` Operations from [nogdb.Db](https://github.com/nogdb/nogdb.js/blob/master/doc.md#database-operations "nogdb.Db")

  - `Vertex` Operations from [nogdb.Vertex](https://github.com/nogdb/nogdb.js/blob/master/doc.md#vertex-operations "nogdb.Vertex")

  - `Edge` Operations from [nogdb.Edge](https://github.com/nogdb/nogdb.js/blob/master/doc.md#edge-operations "nogdb.Edge")

  - `Traverse` (Graph) Operations from [nogdb.Traverse](https://github.com/nogdb/nogdb.js/blob/master/doc.md#traverse-graph-operations "nogdb.Traverse")

#### See more [Document](https://github.com/nogdb/nogdb.js/blob/master/doc.md "Document")

------------


## Examples using nogdb.js

- Create classes VERTEX `Words` for words, VERTEX `InitialWords` for starting words and EDGE` WordLinks` for linking words

```javascript
	const nogdb = require("./build/Debug/nogdb_js");
// Create database context pointing to file 'mygraph.db'
	const ctx = new nogdb.Context("mygraph.db");
// Create READ_WRITE transaction
	var txn = new nogdb.Txn(ctx,"READ_WRITE");
// Create vertex 'Words'
	nogdb.Class.create(txn,"Words","VERTEX");
// Create vertex 'InitialWords'
	nogdb.Class.createExtend(txn,"InitialWords","Words");
// Create edge 'WordLinks'
	nogdb.Class.create(txn,"WordLinks","EDGE");
```
- Create Property `messaged` within VERTEX `Words`

```javascript
	const nogdb = require("./build/Debug/nogdb_js");
	const ctx = new nogdb.Context("mygraph.db");
	var txn = new nogdb.Txn(ctx,"READ_WRITE");

// create property 'string' within vertex 'Words' (also applied to 'InitialWords')
	nogdb.Property.add(txn, "Words", "messaged", "TEXT");
```
- Create vertices for `InitialWords` and `Words`. And create link between them.

```javascript
	const nogdb = require("./build/Debug/nogdb_js");
	const ctx = new nogdb.Context("mygraph.db");
	var txn = new nogdb.Txn(ctx,"READ_WRITE");

// Create prototypes of vertices
	hello = new nogdb.Record();
	world = new nogdb.Record();
	hello.set("messaged", "Hello");
	world.set("messaged", ", World.");
// Create vertices to db with created prototypes
	vHello = nogdb.Vertex.create(txn, "InitialWords", hello);
	vWorld = nogdb.Vertex.create(txn, "Words", world);
// Create link between both vertices
	nogdb.Edge.create(txn, "WordLinks", vHello, vWorld);
// Do a transaction commit
	txn.commit();
```

- Get Initial Word, go to another word through edge, print string for every word passed.

```javascript
	const nogdb = require("./build/Debug/nogdb_js");
	const ctx = new nogdb.Context("mygraph.db");

// Create READ_ONLY transaction
	var txn = new nogdb.Txn(ctx,"READ_ONLY");
// Get initial word from 'InitialWords'
	word1 = nogdb.Vertex.get(txn, "InitialWords");
// Get property 'string' from vertex and print to screen
	str_vertex = word1[0].record.value.messaged.toText;
// Get out edge from record
	edge = nogdb.Vertex.getOutEdge(txn,word1[0].recordDescriptor);
// Get destination vertex from edge
	word2 = nogdb.Edge.getDst(txn,edge[0].recordDescriptor);
// Get property 'string' from another vertex and print to screen
	str_another = word2.record.value.messaged.toText;
// Print show result
	console.log(str_vertex + str_another);
```

- This is what final `hello_world.js` looks like

```javascript
// hello_world.js
const nogdb = require("./build/Debug/nogdb_js");

// Create database context pointing to file 'mygraph.db'
	const ctx = new nogdb.Context("mygraph.db");

	 /* Write data */
// Create READ_WRITE transaction
	var txn = new nogdb.Txn(ctx,"READ_WRITE");
// Create vertex 'Words'
	nogdb.Class.create(txn,"Words","VERTEX");
// Create vertex 'InitialWords'
	nogdb.Class.createExtend(txn,"InitialWords","Words");
// Create edge 'WordLinks'
	nogdb.Class.create(txn,"WordLinks","EDGE");
// create property 'string' within vertex 'Words' (also applied to 'InitialWords')
	nogdb.Property.add(txn, "Words", "messaged", "TEXT");
// Create prototypes of vertices
	hello = new nogdb.Record();
	world = new nogdb.Record();
	hello.set("messaged", "Hello");
	world.set("messaged", ", World.");
// Create vertices to db with created prototypes
	vHello = nogdb.Vertex.create(txn, "InitialWords", hello);
	vWorld = nogdb.Vertex.create(txn, "Words", world);
// Create link between both vertices
	nogdb.Edge.create(txn, "WordLinks", vHello, vWorld);
// Do a transaction commit
	txn.commit();

	/* Read data */
// Create READ_ONLY transaction
	txn = new nogdb.Txn(ctx,"READ_ONLY");
// Get initial word from 'InitialWords'
	word1 = nogdb.Vertex.get(txn, "InitialWords");
// Get property 'string' from vertex and print to screen
	str_vertex = word1[0].record.value.messaged.toText;
// Get out edge from record
	edge = nogdb.Vertex.getOutEdge(txn,word1[0].recordDescriptor);
// Get destination vertex from edge
	word2 = nogdb.Edge.getDst(txn,edge[0].recordDescriptor);
// Get property 'string' from another vertex and print to screen
	str_another = word2.record.value.messaged.toText;
// Print result
	console.log(str_vertex + str_another);

```

- Run

`$	node hello_world.js`

Output : `Hello, World.`
