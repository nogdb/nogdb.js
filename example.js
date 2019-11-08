// example.js
const nogdb = require("./build/Release/nogdb_js");

// Create database context pointing to file 'mygraph.db'
const ctx = new nogdb.Context("mygraph.db");

/* Write data */
// Create READ_WRITE transaction
var txn = new nogdb.Txn(ctx,"READ_WRITE");
// Create vertex 'Words'
txn.addClass("Words","VERTEX");
// Create vertex 'InitialWords'
txn.addSubClassOf("Words","InitialWords");
// Create edge 'WordLinks'
txn.addClass("WordLinks","EDGE");
// create property 'string' within vertex 'Words' (also applied to 'InitialWords')
txn.addProperty("Words", "message", "TEXT");
// create indexes for vertices
txn.addIndex("InitialWords", "message", true);
txn.addIndex("Words", "message", false);
// Create prototypes of vertices
hello = new nogdb.Record();
world = new nogdb.Record();
hello.set("message", "Hello");
world.set("message", ", World.");
// Create vertices to db with created prototypes
vHello = txn.addVertex("InitialWords", hello);
vWorld = txn.addVertex("Words", world);
console.log(vHello);
console.log(vWorld);
// Create link between both vertices
eLink = txn.addEdge("WordLinks", vHello, vWorld);
console.log(eLink);
// Do a transaction commit
txn.commit();

/* Read data */
// Create READ_ONLY transaction
var txnRo = new nogdb.Txn(ctx,"READ_ONLY");
// find all records in InitialWords
res = txnRo.find("InitialWords");
console.log(res);
console.log(res[0].descriptor.rid);
console.log(res[0].record);
console.log(res[0].record.value.message.toText);
// find records in Words containing 'world' with non case-sensitive
res = txnRo.find("Words", new nogdb.Condition("message").contain("world").ignoreCase());
console.log(res);
console.log(res[0].descriptor.rid);
console.log(res[0].record);
console.log(res[0].record.value.message.toText);
// find unique indexed records in InitialWords
res = txnRo.find("InitialWords", { indexed: true });
console.log(res);
console.log(res[0].descriptor.rid);
console.log(res[0].record);
console.log(res[0].record.value.message.toText);
// find non indexed records in Words
res = txnRo.find("Words", new nogdb.Condition("message").eq(", World."), { indexed: true });
console.log(res);
console.log(res[0].descriptor.rid);
console.log(res[0].record);
console.log(res[0].record.value.message.toText);

// count all records in InitialWords
res = txnRo.count("InitialWords");
console.log(res);
// count records in Words containing 'world' with non case-sensitive
res = txnRo.count("Words", new nogdb.Condition("message").contain("world").ignoreCase());
console.log(res);
// count unique indexed records in InitialWords
res = txnRo.count("InitialWords", { indexed: true });
console.log(res);
// count non indexed records in Words
res = txnRo.count("Words", new nogdb.Condition("message").eq(", World."), { indexed: true });
console.log(res);