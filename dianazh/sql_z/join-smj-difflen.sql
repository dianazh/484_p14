-- Test sort merge join with two relations having the different length

-- Create tables
CREATE TABLE DDA (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE, unq INTEGER);
CREATE TABLE DDB (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE, unq INTEGER);

-- Insert records
INSERT INTO DDA (serial, ikey, filler, dkey, unq) VALUES (1, 10, '00000 string record',  0.0, 0);
INSERT INTO DDA (serial, ikey, filler, dkey, unq) VALUES (1, 11, '00000 string record',  0.0, 1);
INSERT INTO DDA (serial, ikey, filler, dkey, unq) VALUES (1, 12, '00000 string record',  0.0, 2);

INSERT INTO DDA (serial, ikey, filler, dkey, unq) VALUES (2, 10, '00000 string record',  0.0, 4);

INSERT INTO DDA (serial, ikey, filler, dkey, unq) VALUES (3, 10, '00000 string record',  0.0, 8);
INSERT INTO DDA (serial, ikey, filler, dkey, unq) VALUES (3, 11, '00000 string record',  0.0, 9);
INSERT INTO DDA (serial, ikey, filler, dkey, unq) VALUES (3, 13, '00000 string record',  0.0, 11);

INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (1, 10, '00000 string record',  0.0, 0);
INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (1, 11, '00000 string record',  0.0, 1);
INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (1, 12, '00000 string record',  0.0, 2);
INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (1, 13, '00000 string record',  0.0, 3);

INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (2, 10, '00000 string record',  0.0, 4);
INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (2, 11, '00000 string record',  0.0, 5);
INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (2, 12, '00000 string record',  0.0, 6);
INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (2, 13, '00000 string record',  0.0, 7);

INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (3, 10, '00000 string record',  0.0, 8);
INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (3, 11, '00000 string record',  0.0, 9);
INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (3, 12, '00000 string record',  0.0, 10);
INSERT INTO DDB (serial, ikey, filler, dkey, unq) VALUES (3, 13, '00000 string record',  0.0, 11);


--Join on duplicate heavy fields
-- Join on int
SELECT DDA.serial, DDB.serial, DDA.ikey, DDB.ikey, DDA.dkey, DDB.filler FROM DDA, DDB WHERE DDA.serial = DDB.serial;
SELECT DDA.serial, DDB.serial, DDA.ikey, DDB.ikey, DDA.dkey, DDB.filler FROM DDA, DDB WHERE DDB.serial = DDA.serial;
SELECT DDA.serial, DDB.serial, DDA.ikey, DDB.ikey, DDA.dkey, DDA.filler, DDB.filler FROM DDA, DDB WHERE DDA.ikey = DDB.ikey;
SELECT DDA.serial, DDB.serial, DDA.ikey, DDB.ikey, DDA.dkey, DDA.filler, DDB.filler FROM DDA, DDB WHERE DDB.ikey = DDA.ikey;

-- Join on double
SELECT DDA.ikey, DDA.dkey, DDB.dkey, DDA.filler FROM DDA, DDB WHERE DDA.dkey = DDB.dkey;
SELECT DDA.ikey, DDA.dkey, DDB.dkey, DDA.filler FROM DDA, DDB WHERE DDB.dkey = DDA.dkey;

-- Join on string
SELECT * FROM DDA, DDB WHERE DDA.filler = DDB.filler;
SELECT * FROM DDA, DDB WHERE DDB.filler = DDA.filler;

--Join on no duplicate fields

SELECT DDA.serial, DDA.dkey, DDB.dkey, DDA.filler, DDB.filler, DDA.ikey, DDA.unq, DDB.unq from DDA,DDB WHERE DDA.unq = DDB.unq;
SELECT DDA.serial, DDA.dkey, DDB.dkey, DDA.filler, DDB.filler, DDA.ikey, DDA.unq, DDB.unq from DDA,DDB WHERE DDB.unq = DDA.unq;
select * from dda, ddb where dda.unq = ddb.unq;
select * from dda, ddb where ddb.unq = dda.unq;

DROP TABLE DDA;
DROP TABLE DDB;
