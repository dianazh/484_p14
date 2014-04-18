CREATE TABLE DDA (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);
CREATE TABLE DDB (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);

INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (1, 10, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (1, 11, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (1, 12, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (1, 13, '00000 string record',  0.0);

INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (2, 10, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (2, 11, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (2, 12, '00000 string record',  0.0);
INSERT INTO DDA (serial, ikey, filler, dkey) VALUES (2, 13, '00000 string record',  0.0);

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

SELECT * FROM DDA, DDB WHERE DDA.serial = DDB.serial;

DROP TABLE DDA;
DROP TABLE DDB;
