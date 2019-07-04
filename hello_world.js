// hello_world.js
const nogdb = require("./build/Release/nogdb_js");

// Create database context pointing to file 'mygraph.db'
const ctx = new nogdb.Context("mygraph.db");

/* Write data */
// Create READ_WRITE transaction
console.log("test1");
var txn = new nogdb.Txn(ctx,"READ_WRITE");
// Create vertex 'Words'
console.log("test2");
txn.addClass("Words","VERTEX");
// Create vertex 'InitialWords'
console.log("test3");
txn.addSubClassOf("Words","InitialWords");
// Create edge 'WordLinks'
console.log("test4");
txn.addClass("WordLinks","EDGE");
// create property 'string' within vertex 'Words' (also applied to 'InitialWords')
console.log("test5");
txn.addProperty("Words", "messaged", "TEXT");
// Create prototypes of vertices
hello = new nogdb.Record();
world = new nogdb.Record();
hello.set("messaged", "Hello");
world.set("messaged", ", World.");
// Create vertices to db with created prototypes
console.log("test1");
vHello = txn.addVertex("InitialWords", hello);
vWorld = txn.addVertex("Words", world);
// Create link between both vertices
console.log("test2");
txn.addEdge("WordLinks", vHello, vWorld);
// Do a transaction commit
console.log("test3");
txn.commit();

// /* Read data */
// // Create READ_ONLY transaction
// txn = new nogdb.Txn(ctx,"READ_ONLY");
// // Get initial word from 'InitialWords'
// word1 = nogdb.Vertex.get(txn, "InitialWords");
// // Get property 'string' from vertex and print to screen
// str_vertex = word1[0].record.value.messaged.toText;
// // Get out edge from record
// edge = nogdb.Vertex.getOutEdge(txn,word1[0].recordDescriptor);
// // Get destination vertex from edge
// word2 = nogdb.Edge.getDst(txn,edge[0].recordDescriptor);
// // Get property 'string' from another vertex and print to screen
// str_another = word2.record.value.messaged.toText;
// // Print result
// console.log(str_vertex + str_another);