-- Test index select

-- Create relation
CREATE TABLE table4(ikey INTEGER, dkey DOUBLE, skey CHAR(20));

--Create index
CREATE INDEX table4(ikey);
CREATE INDEX table4(dkey);

-- Insert with attributes in order
INSERT INTO table4(ikey, dkey, skey) VALUES (1, 1.1, 'abcd1');
INSERT INTO table4(ikey, dkey, skey) VALUES (2, 2.2, 'abcd2');
INSERT INTO table4(ikey, dkey, skey) VALUES (3, 3.3, 'abcd3');

-- Insert with attributes out of order
INSERT INTO table4(skey, dkey, ikey) VALUES ('abcd4', 4.4, 4);
INSERT INTO table4(skey, dkey, ikey) VALUES ('abcd5', 5.5, 5);
INSERT INTO table4(skey, dkey, ikey) VALUES ('abcd6', 6.6, 6);

-- Operate EQ
-- Predicate on int
SELECT * FROM table4 WHERE table4.ikey = 4;
-- Predicate on double
SELECT * FROM table4 WHERE table4.dkey = 4;

--other selections that should not be index select
SELECT table4.ikey, table4.dkey, table4.skey FROM table4 WHERE table4.ikey < 3;
SELECT table4.skey, table4.ikey, table4.dkey FROM table4 WHERE table4.dkey <= 3;
SELECT table4.skey, table4.dkey, table4.ikey FROM table4 WHERE table4.skey <= 'abcd3';
SELECT table4.dkey, table4.skey FROM table4 WHERE table4.ikey >= 3;
SELECT table4.ikey FROM table4 WHERE table4.dkey > 3;
SELECT table4.skey FROM table4 WHERE table4.skey <> 'abcd3';

--Test if index select is chosen
--project 3 items in different order
SELECT table4.ikey, table4.dkey, table4.skey FROM table4 WHERE table4.ikey = 3;
SELECT table4.ikey, table4.skey, table4.dkey FROM table4 WHERE table4.dkey = 3.3;
SELECT table4.dkey, table4.ikey, table4.skey FROM table4 WHERE table4.dkey = 3.3;
SELECT table4.dkey, table4.skey, table4.ikey FROM table4 WHERE table4.ikey = 3;
SELECT table4.skey, table4.ikey, table4.dkey FROM table4 WHERE table4.dkey = 3.3;
SELECT table4.skey, table4.dkey, table4.ikey FROM table4 WHERE table4.ikey = 3;
--project 2 items in different order
SELECT table4.ikey, table4.skey FROM table4 WHERE table4.dkey = 3.3;
SELECT table4.ikey, table4.dkey FROM table4 WHERE table4.dkey = 3.3;
SELECT table4.dkey, table4.skey FROM table4 WHERE table4.dkey = 3.3;
SELECT table4.dkey, table4.ikey FROM table4 WHERE table4.ikey = 6;
SELECT table4.skey, table4.ikey FROM table4 WHERE table4.ikey = 6;
SELECT table4.skey, table4.dkey FROM table4 WHERE table4.ikey = 6;
--project 1 item
SELECT table4.ikey FROM table4 WHERE table4.dkey = 3.3;
SELECT table4.dkey FROM table4 WHERE table4.ikey = 4;
SELECT table4.skey FROM table4 WHERE table4.ikey = 4;


DROP TABLE table4;

