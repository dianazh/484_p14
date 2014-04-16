-- Test sort merge join with two relations having the different length

-- Create tables
CREATE TABLE DDA (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);
CREATE TABLE DDB (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);

-- Insert records
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (1, 10, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (1, 11, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (1, 12, '00000 string record',  0.0);

INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (2, 10, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (2, 11, '00000 string record',  0.0);

INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (3, 10, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (3, 11, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (3, 12, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (3, 13, '00000 string record',  0.0);

INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (1, 10, '00000 string record',  0.0);
INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (1, 11, '00000 string record',  0.0);
INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (1, 12, '00000 string record',  0.0);
INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (1, 13, '00000 string record',  0.0);

INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (2, 10, '00000 string record',  0.0);
INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (2, 11, '00000 string record',  0.0);
INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (2, 12, '00000 string record',  0.0);
INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (2, 13, '00000 string record',  0.0);

INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (3, 10, '00000 string record',  0.0);
INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (3, 11, '00000 string record',  0.0);
INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (3, 12, '00000 string record',  0.0);
INSERT INTO DDB (serial, ikey, filler, dkey) VALUES (3, 13, '00000 string record',  0.0);

-- Join on int
SELECT * FROM DDA, DDB WHERE DDA.serial = DDB.serial;

-- Join on double
SELECT * FROM DDA, DDB WHERE DDA.dkey = DDB.dkey;

-- Join on string
SELECT * FROM DDA, DDB WHERE DDA.filler = DDB.filler;

DROP TABLE DDA;
DROP TABLE DDB;
