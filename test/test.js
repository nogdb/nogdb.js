var assert = require('assert');
const nogdb = require("../build/Debug/nogdb_js");
const ctx = new nogdb.Context("mygraph.db");
describe('Object', function() {
  describe('Txn', function() {
    describe('Constructor' , function() {
        it('should be pass when valid arugment', function() {
            let txn1 = new nogdb.Txn(ctx,"READ_ONLY"); 
            let txn2 = new nogdb.Txn(ctx,"READ_WRITE");
            let txn3 = nogdb.newTxn(ctx,"READ_ONLY");
            txn2.commit();
        });
    });
  });
  describe('Record', function() {
    describe('Constructor' , function() {
        it('should be pass when valid arugment', function() {
            let record = new nogdb.Record(); 
        });
    });
    describe('set()' , function() {
        it('should be pass when valid arugment', function() {
            let record = new nogdb.Record(); 
            record.set("name","A");
            record.set("age","B");
            assert.equal(2,record.size());
        });
    });
    describe('getProperties()' , function() {
        it('should be pass when valid arugment', function() {
            let record = new nogdb.Record(); 
            record.set("name","A");
            record.set("age","8");
            assert.deepEqual(['age','name'],record.getProperties());
        });
    });
    describe('unset()' , function() {
        it('should be pass when valid arugment', function() {
            let record = new nogdb.Record(); 
            record.set("name","A");
            record.set("age","9");
            assert.equal(2,record.size());
            record.unset("age");
            assert.equal(1,record.size());
        });
    });
    describe('size()' , function() {
        it('should be pass when valid arugment', function() {
            let record = new nogdb.Record(); 
            record.set("name","A");
            record.set("age","8");
            assert.equal(2,record.size());
        });
    });
    describe('size()' , function() {
        it('should be pass when valid arugment', function() {
            let record = new nogdb.Record(); 
            record.set("name","A");
            record.set("age","8");
            assert.equal(2,record.size());
        });
    });
    describe('empty()' , function() {
        it('should be pass when valid arugment', function() {
            let record = new nogdb.Record(); 
            record.set("name","A");
            record.set("age","8");
            assert.equal(false,record.empty());
        });
    });
    describe('clear()' , function() {
        it('should be pass when valid arugment', function() {
            let record = new nogdb.Record(); 
            record.set("name","A");
            record.set("age","8");
            record.clear();
            assert.equal(true,record.empty());
        });
    });
  });
});