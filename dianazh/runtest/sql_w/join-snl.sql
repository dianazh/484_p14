-- Test simple nested loop join

-- Copy data from datamation.sql

CREATE TABLE DA (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);
CREATE TABLE DB (serial INTEGER, ikey INTEGER, filler CHAR(80), dkey DOUBLE);

-- load 20 tuples into the DA table
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (0, 11618, '00000 string record',  0.0);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (1, 15378, '00001 string record',  1.1);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (2, 17233, '00002 string record',  2.2);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (3, 19052, '00003 string record',  3.3);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (4, 24069, '00004 string record',  4.4);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (5, 17188, '00005 string record',  5.5);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (6, 28769, '00006 string record',  6.6);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (7, 15168, '00007 string record',  7.7);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (8, 578, '00008 string record',  8.8);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (9, 21055, '00009 string record',  9.9);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (10, 7648, '00010 string record', 11.0);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (11, 10153, '00011 string record', 12.1);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (12, 31851, '00012 string record', 13.2);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (13, 25573, '00013 string record', 14.3);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (14, 23765, '00014 string record', 15.4);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (15, 8758, '00015 string record', 16.5);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (16, 18243, '00016 string record', 17.6);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (17, 26285, '00017 string record', 18.7);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (18, 32157, '00018 string record', 19.8);
INSERT INTO DA (serial, ikey, filler, dkey) VALUES (19, 21879, '00019 string record', 20.9);


-- load 10 tuples into the DB table
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (0, 23798, '00000 string record',  0.0);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (1, 24529, '00001 string record',  1.1);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (2, 1440, '00002 string record',  2.2);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (3, 20441, '00003 string record',  3.3);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (4, 8210, '00004 string record',  4.4);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (5, 31830, '00005 string record',  5.5);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (6, 18816, '00006 string record',  6.6);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (7, 22849, '00007 string record',  7.7);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (8, 28448, '00008 string record',  8.8);
INSERT INTO DB (serial, ikey, filler, dkey) VALUES (9, 27189, '00009 string record',  9.9);


-- Operator LT
-- Join on int
SELECT DA.serial, DA.ikey, DB.ikey, DA.dkey, DB.dkey, DB.filler FROM DA, DB WHERE DA.ikey < DB.ikey;
-- Join on double
SELECT DA.serial, DA.ikey, DB.ikey, DA.dkey, DB.dkey, DB.filler FROM DA, DB WHERE DA.dkey < DB.dkey;
-- Join on string
SELECT DA.serial, DB.ikey, DA.dkey, DB.dkey, DB.filler FROM DA, DB WHERE DA.filler < DB.filler;

-- Operator LTE
-- Join on int
SELECT DA.ikey, DB.ikey FROM DA, DB WHERE DA.ikey <= DB.ikey;
-- Join on double
SELECT DA.dkey, DB.dkey FROM DA, DB WHERE DA.dkey <= DB.dkey;
-- Join on string
SELECT DA.filler, DB.filler FROM DA, DB WHERE DA.filler <= DB.filler;

-- Operator GTE
-- Join on int
SELECT DA.ikey, DB.ikey FROM DA, DB WHERE DA.ikey >= DB.ikey;
-- Join on double
SELECT DA.dkey, DB.dkey FROM DA, DB WHERE DA.dkey >= DB.dkey;
-- Join on string
SELECT DA.filler, DB.filler FROM DA, DB WHERE DA.filler >= DB.filler;

-- Operator GT
-- Join on int
SELECT DA.ikey, DB.ikey FROM DA, DB WHERE DA.ikey > DB.ikey;
-- Join on double
SELECT DA.dkey, DB.dkey FROM DA, DB WHERE DA.dkey > DB.dkey;
-- Join on string
SELECT DA.filler, DB.filler FROM DA, DB WHERE DA.filler > DB.filler;

-- Operator NE
-- Join on int
SELECT DA.ikey, DB.ikey FROM DA, DB WHERE DA.ikey <> DB.ikey;
-- Join on double
SELECT DA.dkey, DB.dkey FROM DA, DB WHERE DA.dkey <> DB.dkey;
-- Join on string
SELECT DA.filler, DB.filler FROM DA, DB WHERE DA.filler <> DB.filler;

DROP TABLE DA;
DROP TABLE DB;
