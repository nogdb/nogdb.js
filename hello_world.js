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