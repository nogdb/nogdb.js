var assert = require('assert');
const nogdb = require("../build/Release/nogdb_js");
const ctx = new nogdb.Context("test.db");
describe('nogdb.js', function() {
    describe('Class', function() {
        describe('Create class...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Class.create(txn,"Person","VERTEX");
                txn.commit(); 
            });
        });
        describe('Create extend class...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Class.createExtend(txn,"Student","Person");
                txn.commit(); 
            });
        });
        describe('drop class...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Class.drop(txn,"Student");
                txn.commit();
            });
        });
        describe('modified name class...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Class.alter(txn,"Person","Room");
                txn.commit();
            });
        });
    });
    describe('Property', function() {
        describe('add property...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Property.add(txn,"Room","name","TEXT"); 
                nogdb.Property.add(txn,"Room","area","UNSIGNED_INTEGER"); 
                txn.commit(); 
            });
        });
        describe('modified name property...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Property.alter(txn,"Room","area","size"); 
                txn.commit(); 
            });
        });
        describe('remove property...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Property.remove(txn,"Room","size"); 
                txn.commit(); 
            });
        });
        describe('create index property...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Property.add(txn,"Room","area","UNSIGNED_INTEGER"); 
                nogdb.Property.createIndex(txn,"Room","name",true);
                nogdb.Property.createIndex(txn,"Room","area");
                txn.commit(); 
            });
        });
        describe('drop index property...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Property.dropIndex(txn,"Room","area");
                txn.commit(); 
            });
        });
    });
    describe('Database', function() {
        describe('get record...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                let record = new nogdb.Record();
                record.set("name","Classroom");
                let recordDescriptor = nogdb.Vertex.create(txn,"Room",record);
                txn.commit(); 

                txn = new nogdb.Txn(ctx,"READ_ONLY"); 
                let r = nogdb.Db.getRecord(txn,recordDescriptor); 
                assert.deepEqual( ['name'],r.properties);
                assert.equal( 'Classroom',r.value.name.toText);
                
            });
        });
        describe('get Schema...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY"); 
                let cds = nogdb.Db.getSchema(txn);
                assert.equal(1,cds.length);
                assert.equal('Room',cds[0].name);
                let cd = nogdb.Db.getSchema(txn,"Room");
                assert.equal('Room',cd.name);
            });
        });
        describe('get DbInfo...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY"); 
                let dbInfo = nogdb.Db.getDbInfo(txn);
                assert.equal('test.db',dbInfo.dbPath);
            });
        });
    });
    describe('Vertex', function() {
        describe('create vertex...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                let record = new nogdb.Record();
                let resultSet = nogdb.Vertex.get(txn,'Room'); 
                assert.equal(1,resultSet.length);
                record.set("name","Hall");
                nogdb.Vertex.create(txn,"Room",record);
                record.clear();
                record.set("name","Kitchen");
                nogdb.Vertex.create(txn,"Room",record);
                resultSet = nogdb.Vertex.get(txn,'Room'); 
                assert.equal(3,resultSet.length);
                txn.commit(); 
            });
        });
        describe('update vertex...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                let record1 = new nogdb.Record();
                record1.set("name","Library");
                let recordDescriptor = nogdb.Vertex.create(txn,"Room",record1);
                let r1 = nogdb.Db.getRecord(txn,recordDescriptor); 
                assert.deepEqual( ['name'],r1.properties);
                assert.equal( 'Library',r1.value.name.toText);
                resultSet = nogdb.Vertex.get(txn,'Room'); 
                assert.equal(4,resultSet.length);
                txn.commit(); 

                txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                let record2 = new nogdb.Record();
                record2.set("name","Toilet");
                nogdb.Vertex.update(txn,recordDescriptor,record2);
                let r2 = nogdb.Db.getRecord(txn,recordDescriptor); 
                assert.deepEqual( ['name'],r2.properties);
                assert.equal( 'Toilet',r2.value.name.toText);
                resultSet = nogdb.Vertex.get(txn,'Room'); 
                assert.equal(4,resultSet.length);
                txn.commit(); 
            });
        });
        describe('destroy vertex...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                let record1 = new nogdb.Record();
                record1.set("name","Bedroom");
                let recordDescriptor = nogdb.Vertex.create(txn,"Room",record1);
                resultSet = nogdb.Vertex.get(txn,'Room'); 
                assert.equal(5,resultSet.length);
                txn.commit(); 

                txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Vertex.destroy(txn,recordDescriptor);
                resultSet = nogdb.Vertex.get(txn,'Room'); 
                assert.equal(4,resultSet.length);
                txn.commit(); 
            });
        });
        describe('get vertex...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY");
                let resultSet = nogdb.Vertex.get(txn,'Room'); 
                assert.equal(4,resultSet.length);
                assert.equal('Classroom',resultSet[0].record.value.name.toText);
                assert.equal('Hall',resultSet[1].record.value.name.toText);
                assert.equal('Kitchen',resultSet[2].record.value.name.toText);
                assert.equal('Toilet',resultSet[3].record.value.name.toText);

                let condition = new nogdb.Condition('name');
                condition.eq('Hall');
                resultSet = nogdb.Vertex.get(txn,'Room',condition); 
                assert.equal(1,resultSet.length);
                assert.equal('Hall',resultSet[0].record.value.name.toText);
            });
        });
        describe('getInEdge vertex...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                nogdb.Class.create(txn,"Person","VERTEX");
                nogdb.Class.create(txn,"Building","VERTEX");
                nogdb.Class.create(txn,"visit","EDGE");
                nogdb.Class.create(txn,"at","EDGE");
                nogdb.Property.add(txn,"Person","name","TEXT");
                nogdb.Property.add(txn,"Building","name","TEXT");
                let record = new nogdb.Record();
                record.set("name","A");
                let recordDescriptorP = nogdb.Vertex.create(txn,"Person",record);
                record.clear();
                record.set("name","School");
                let recordDescriptorB = nogdb.Vertex.create(txn,"Building",record);
                let condition = new nogdb.Condition('name');
                condition.eq('Hall');
                let recordDescriptorR = nogdb.Vertex.get(txn,'Room',condition)[0].recordDescriptor; 
                nogdb.Edge.create(txn,"visit",recordDescriptorP,recordDescriptorR);
                nogdb.Edge.create(txn,"at",recordDescriptorR,recordDescriptorB);
                txn.commit(); 

                txn = new nogdb.Txn(ctx,"READ_ONLY"); 
                condition2 = new nogdb.Condition('name');
                condition2.eq('Hall');
                let recordDescriptor = nogdb.Vertex.get(txn,'Room',condition2)[0].recordDescriptor; 
                let resultSet = nogdb.Vertex.getInEdge(txn,recordDescriptor);
                assert.equal('visit',resultSet[0].record.className);
            });
        });
        describe('getOutEdge vertex...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY"); 
                let condition = new nogdb.Condition('name');
                condition.eq('Hall');
                let recordDescriptor = nogdb.Vertex.get(txn,'Room',condition)[0].recordDescriptor; 
                let resultSet = nogdb.Vertex.getOutEdge(txn,recordDescriptor);
                assert.equal('at',resultSet[0].record.className);
            });
        });
        describe('getAllEdge vertex...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY"); 
                let condition = new nogdb.Condition('name');
                condition.eq('Hall');
                let recordDescriptor = nogdb.Vertex.get(txn,'Room',condition)[0].recordDescriptor; 
                let resultSet = nogdb.Vertex.getAllEdge(txn,recordDescriptor);
                assert.equal(2,resultSet.length);
            });
        });
        describe('getIndex vertex...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY"); 
                let condition = new nogdb.Condition('name');
                let resultSet = nogdb.Vertex.getIndex(txn,"Room",condition);
                assert.equal(0,resultSet.length);
            });
        });
    });
    describe('Edge', function() {
        describe('create edge...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                let record = new nogdb.Record();
                record.set("name","B");
                let recordDescriptorP = nogdb.Vertex.create(txn,"Person",record);
                record.clear();
                record.set("name","School");
                let recordDescriptorB = nogdb.Vertex.create(txn,"Building",record);
                let condition = new nogdb.Condition('name');
                condition.eq('Classroom');
                let recordDescriptorR = nogdb.Vertex.get(txn,'Room',condition)[0].recordDescriptor; 
                nogdb.Edge.create(txn,"visit",recordDescriptorP,recordDescriptorR);
                nogdb.Edge.create(txn,"at",recordDescriptorR,recordDescriptorB);
                txn.commit(); 
            });
        });
        describe('update edge...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                let record = new nogdb.Record();
                record.set("name","A");
                let recordDescriptorP = nogdb.Vertex.create(txn,"Person",record);
                let condition = new nogdb.Condition('name');
                condition.eq('Classroom');
                let recordDescriptorR = nogdb.Vertex.get(txn,'Room',condition)[0].recordDescriptor; 
                nogdb.Property.add(txn,"visit","times","TINYINT");
                record.clear();
                record.set("times",9);
                let recordDescriptor = nogdb.Edge.create(txn,"visit",recordDescriptorP,recordDescriptorR,record);
                record.clear();
                record.set("times",10);
                nogdb.Edge.update(txn,recordDescriptor,record);
                txn.commit(); 
            });
        });
        describe('updateSrc edge...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY"); 
                let condition = new nogdb.Condition('name');
                condition.eq('Hall');
                let recordDescriptorHall = nogdb.Vertex.get(txn,'Room',condition)[0].recordDescriptor; 
                condition.eq('Toilet');
                let recordDescriptorToilet = nogdb.Vertex.get(txn,'Room',condition)[0].recordDescriptor;
                let resultSetHall = nogdb.Vertex.getOutEdge(txn,recordDescriptorHall);
                let resultSetToilet = nogdb.Vertex.getOutEdge(txn,recordDescriptorToilet);
                assert.equal(1,resultSetHall.length);
                assert.equal(0,resultSetToilet.length);

                txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                let recordDescriptorE = resultSetHall[0].recordDescriptor;
                nogdb.Edge.updateSrc(txn,recordDescriptorE,recordDescriptorToilet);
                resultSetHall = nogdb.Vertex.getOutEdge(txn,recordDescriptorHall);
                resultSetToilet = nogdb.Vertex.getOutEdge(txn,recordDescriptorToilet);
                assert.equal(0,resultSetHall.length);
                assert.equal(1,resultSetToilet.length);
                txn.commit();
            });
        });
        describe('updateDst edge...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY"); 
                let condition = new nogdb.Condition('name');
                condition.eq('Hall');
                let recordDescriptorHall = nogdb.Vertex.get(txn,'Room',condition)[0].recordDescriptor; 
                condition.eq('Toilet');
                let recordDescriptorToilet = nogdb.Vertex.get(txn,'Room',condition)[0].recordDescriptor;
                let resultSetHall = nogdb.Vertex.getInEdge(txn,recordDescriptorHall);
                let resultSetToilet = nogdb.Vertex.getInEdge(txn,recordDescriptorToilet);
                assert.equal(1,resultSetHall.length);
                assert.equal(0,resultSetToilet.length);

                txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                let recordDescriptorE = resultSetHall[0].recordDescriptor;
                nogdb.Edge.updateDst(txn,recordDescriptorE,recordDescriptorToilet);
                resultSetHall = nogdb.Vertex.getInEdge(txn,recordDescriptorHall);
                resultSetToilet = nogdb.Vertex.getInEdge(txn,recordDescriptorToilet);
                assert.equal(0,resultSetHall.length);
                assert.equal(1,resultSetToilet.length);
                txn.commit();
            });
        });
        describe('destroy edge...' , function() {
            it('should be pass when valid arugment', function() {
                txn = new nogdb.Txn(ctx,"READ_WRITE"); 
                let condition = new nogdb.Condition('name');
                condition.eq('Toilet');
                let recordDescriptorToilet = nogdb.Vertex.get(txn,'Room',condition)[0].recordDescriptor;
                let resultSetToilet = nogdb.Vertex.getInEdge(txn,recordDescriptorToilet);
                let recordDescriptorE = resultSetToilet[0].recordDescriptor;
                assert.equal(1,resultSetToilet.length);
                nogdb.Edge.destroy(txn,recordDescriptorE);
                resultSetToilet = nogdb.Vertex.getInEdge(txn,recordDescriptorToilet);
                assert.equal(0,resultSetToilet.length);
                txn.commit();
            });
        });
        describe('get edge...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY");
                let resultSet = nogdb.Edge.get(txn,'visit'); 
                assert.equal(2,resultSet.length);
            });
        });
        describe('getSrc edge...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY");
                let recordDescriptorE = nogdb.Edge.get(txn,'visit')[0].recordDescriptor;
                let result = nogdb.Edge.getSrc(txn,recordDescriptorE);
                assert.equal('Person',result.record.className);
            });
        });
        describe('getDst edge...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY");
                let recordDescriptorE = nogdb.Edge.get(txn,'visit')[0].recordDescriptor;
                let result = nogdb.Edge.getDst(txn,recordDescriptorE);
                assert.equal('Room',result.record.className);
            });
        });
        describe('getSrcDst edge...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY");
                let recordDescriptorE = nogdb.Edge.get(txn,'visit')[0].recordDescriptor;
                let resultSet = nogdb.Edge.getSrcDst(txn,recordDescriptorE);
                assert.equal(2,resultSet.length);
            });
        });
        describe('getIndex edge...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY"); 
                let condition = new nogdb.Condition('times');
                let resultSet = nogdb.Edge.getIndex(txn,"visit",condition);
                assert.equal(0,resultSet.length);
            });
        });
    });
    describe('Traverse', function() {
        describe('prepare data...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_WRITE");
                nogdb.Class.create(txn,"folders","VERTEX");
                nogdb.Class.create(txn,"files","VERTEX");
                nogdb.Class.create(txn,"link","EDGE");
                nogdb.Class.create(txn,"symbolic","EDGE");
                nogdb.Property.add(txn,"folders","name","TEXT");
                nogdb.Property.add(txn,"files","name","TEXT"); 
                nogdb.Property.add(txn,"link","name","TEXT"); 
                nogdb.Property.add(txn,"symbolic","name","TEXT");  
                
                let rf = new nogdb.Record();
                rf.set("name","A");
                let A = nogdb.Vertex.create(txn,"folders",rf);
                rf.set("name","B");
                let B = nogdb.Vertex.create(txn,"folders",rf);
                rf.set("name","C");
                let C = nogdb.Vertex.create(txn,"folders",rf);
                rf.set("name","D");
                let D = nogdb.Vertex.create(txn,"folders",rf);
                rf.set("name","E");
                let E = nogdb.Vertex.create(txn,"folders",rf);
                rf.set("name","F");
                let F = nogdb.Vertex.create(txn,"folders",rf);
                rf.set("name","G");
                let G = nogdb.Vertex.create(txn,"folders",rf);
                rf.set("name","H");
                let H = nogdb.Vertex.create(txn,"folders",rf);
                rf.set("name","Z");
                let Z = nogdb.Vertex.create(txn,"folders",rf);

                let rl = new nogdb.Record();
                rl.set("name","a");
                let a = nogdb.Vertex.create(txn,"files",rl);
                rl.set("name","b");
                let b = nogdb.Vertex.create(txn,"files",rl);
                rl.set("name","c");
                let c = nogdb.Vertex.create(txn,"files",rl);
                rl.set("name","d");
                let d = nogdb.Vertex.create(txn,"files",rl);
                rl.set("name","e");
                let e = nogdb.Vertex.create(txn,"files",rl);
                rl.set("name","f");
                let f = nogdb.Vertex.create(txn,"files",rl);

                nogdb.Edge.create(txn,"link",A,B);
                nogdb.Edge.create(txn,"link",A,a);
                nogdb.Edge.create(txn,"link",A,C);
                nogdb.Edge.create(txn,"link",B,D);
                nogdb.Edge.create(txn,"link",B,E);
                nogdb.Edge.create(txn,"link",B,b);
                nogdb.Edge.create(txn,"link",C,c);
                nogdb.Edge.create(txn,"link",C,f);
                nogdb.Edge.create(txn,"link",E,G);
                nogdb.Edge.create(txn,"link",F,d);
                nogdb.Edge.create(txn,"link",F,H);
                nogdb.Edge.create(txn,"link",F,e);
                nogdb.Edge.create(txn,"link",G,f);
                nogdb.Edge.create(txn,"symbolic",B,b);
                nogdb.Edge.create(txn,"symbolic",C,e);
                nogdb.Edge.create(txn,"symbolic",D,A);
                nogdb.Edge.create(txn,"symbolic",D,f);
                nogdb.Edge.create(txn,"symbolic",E,F);
                nogdb.Edge.create(txn,"symbolic",H,C);
                nogdb.Edge.create(txn,"symbolic",a,a);

                txn.commit();
            });
        });
        describe('InEdge Bfs...' , function() {
            it('should be pass when valid arugment', function() {
                let txn = new nogdb.Txn(ctx,"READ_ONLY");
                let condition = new nogdb.Condition('name');
                condition.eq('H');
                let H = nogdb.Vertex.get(txn,'folders',condition)[0].recordDescriptor;
                let classF = new nogdb.ClassFilter(["link"]);
                let res = nogdb.Traverse.inEdgeBfs(txn,H,1,10,classF);
                console.log(res.length);
                
            });
        });
        describe('OutEdge Bfs...' , function() {
            it('should be pass when valid arugment', function() {

            });
        });
        describe('AllEdge Bfs...' , function() {
            it('should be pass when valid arugment', function() {

            });
        });
        describe('InEdge Dfs...' , function() {
            it('should be pass when valid arugment', function() {

            });
        });
        describe('OutEdge Dfs...' , function() {
            it('should be pass when valid arugment', function() {

            });
        });
        describe('AllEdge Dfs...' , function() {
            it('should be pass when valid arugment', function() {

            });
        });
        describe('Shortest path...' , function() {
            it('should be pass when valid arugment', function() {

            });
        });
    });
});