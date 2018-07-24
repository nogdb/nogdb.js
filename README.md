# nogdb.js
Node.js bindings to the [NogDB](https://nogdb.org/ "NogDB") library

------------


## Installation
We rely on [node-gyp](https://github.com/nodejs/node-gyp "node-gyp").
#### Prerequisites
##### On Unix
- `python` (`v2.7` re- commended, `v3.x.x` is not supported)
- `make`
- A proper C/C++ compiler toolchain, like [GCC](https://gcc.gnu.org/ "GCC")

##### On macOS
- `python` (`v2.7` recommended, `v3.x.x` is not supported) (already installed on macOS)

- [Xcode](https://developer.apple.com/xcode/download/ "Xcode")
 - You also need to install the `Command Line Tools` via Xcode. You can find this under the menu `Xcode -> Preferences -> Downloads` (or by running `xcode-select --install` in your Terminal)
   - This step will install `gcc` and the related toolchain containing `make`
   
##### On Windows
**Option 1**

Install all the required tools and configurations using Microsoft's [windows-build-tools](https://github.com/felixrieseberg/windows-build-tools "windows-build-tools") using npm `install --global --production windows-build-tools` from an elevated PowerShell or CMD.exe (run as Administrator).

**Option 2**

Install tools and configuration manually:

- Visual C++ Build Environment:

 - Option 1: Install Visual C++ Build Tools using the **Default Install** option.

 - Option 2: Install [Visual Studio 2015](https://www.visualstudio.com/products/visual-studio-community-vs "Visual Studio 2015") and select Common Tools for Visual C++ during setup. This also works with the free Community and Express for Desktop editions.

 - Option 3: if you already have Visual Studio 2015 installed and did not install the Common Tools for Visual C++ during setup, you can `File -> New -> Project`, pick any of the options under `Templates -> Other Languages -> Visual C++` then `Ok` and Visual Studio will offer to install the Common Tools for Visual C++ with a "Install Missing Features" / "You need the Universal Windows App Development Tools to develop Windows app projects." dialog.

> 💡 [Windows Vista / 7 only] requires [.NET Framework 4.5.1](http://www.microsoft.com/en-us/download/details.aspx?id=40773 ".NET Framework 4.5.1")

- Install [Python 2.7](https://www.python.org/downloads/ "Python 2.7") (`v3.x.x` is not supported), and run` npm config set python python2.7` (or see below for further instructions on specifying the proper Python version and path.)

- Launch cmd, `npm config set msvs_version 2015`
If the above steps didn't work for you, please visit [Microsoft's Node.js Guidelines for Windows](https://github.com/Microsoft/nodejs-guidelines/blob/master/windows-environment.md#compiling-native-addon-modules "Microsoft's Node.js Guidelines for Windows") for additional tips.

If you have multiple Python versions installed, you can identify which Python version `node-gyp` uses by setting the '--python' variable:

`$ node-gyp --python /path/to/python2.7`

If `node-gyp` is called by way of `npm`, and you have multiple versions of Python installed, then you can set `npm`'s 'python' config key to the appropriate value:

`$ npm config set python /path/to/executable/python2.7`

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
- Create Property `string` within VERTEX `Words`

```javascript
	const nogdb = require("./build/Debug/nogdb_js");
	const ctx = new nogdb.Context("mygraph.db");
	var txn = new nogdb.Txn(ctx,"READ_WRITE");

// create property 'string' within vertex 'Words' (also applied to 'InitialWords')
	nogdb.Property.add(txn, "Words", "string", "TEXT");
```
- Create vertices for `InitialWords` and `Words`. And create link between them.

```javascript
	const nogdb = require("./build/Debug/nogdb_js");
	const ctx = new nogdb.Context("mygraph.db");
	var txn = new nogdb.Txn(ctx,"READ_WRITE");
// Create prototypes of vertices
	hello = new nogdb.Record();
	world = new nogdb.Record();
	hello.set("string", "Hello");
	world.set("string", ", World.");
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
	str_vertex = word1[0].record.value.string.toText;
// Get out edge from record
	edge = nogdb.Vertex.getOutEdge(txn,word1[0].recordDescriptor);
// Get destination vertex from edge
	word2 = nogdb.Edge.getDst(txn,edge[0].recordDescriptor);
// Get property 'string' from another vertex and print to screen
	str_another = word2.record.value.string.toText;
// Print show result
	console.log(str_vertex + str_another);
```

- This is what final 'hello_world.js' looks like

```javascript
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
	nogdb.Property.add(txn, "Words", "string", "TEXT");
// Create prototypes of vertices
	hello = new nogdb.Record();
	world = new nogdb.Record();
	hello.set("string", "Hello");
	world.set("string", ", World.");
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
	str_vertex = word1[0].record.value.string.toText;
// Get out edge from record
	edge = nogdb.Vertex.getOutEdge(txn,word1[0].recordDescriptor);
// Get destination vertex from edge
	word2 = nogdb.Edge.getDst(txn,edge[0].recordDescriptor);
// Get property 'string' from another vertex and print to screen
	str_another = word2.record.value.string.toText;
// Print show result
	console.log(str_vertex + str_another);

```

- Run

`$	node hello_world.js`

Output : `Hello, World.`
