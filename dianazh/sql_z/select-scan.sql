-- Test scan select

-- Create relation
CREATE TABLE table3(ikey INTEGER, dkey DOUBLE, skey CHAR(20));
-- Insert with attributes in order
INSERT INTO table3(ikey, dkey, skey) VALUES (1, 1.1, 'abcd1');
INSERT INTO table3(ikey, dkey, skey) VALUES (2, 2.2, 'abcd2');
INSERT INTO table3(ikey, dkey, skey) VALUES (3, 3.3, 'abcd3');

-- Insert with attributes out of order
INSERT INTO table3(skey, dkey, ikey) VALUES ('abcd4', 4.4, 4);
INSERT INTO table3(skey, dkey, ikey) VALUES ('abcd5', 5.5, 5);
INSERT INTO table3(skey, dkey, ikey) VALUES ('abcd6', 6.6, 6);

--Test select all 3 attrs in different order
-- Operate LT
-- Predicate on int
SELECT table3.ikey, table3.dkey, table3.skey FROM table3 WHERE table3.ikey < 3;
-- Predicate on double
SELECT table3.ikey, table3.skey, table3.dkey FROM table3 WHERE table3.dkey < 3;
-- Predicate on string
SELECT table3.dkey, table3.ikey, table3.skey FROM table3 WHERE table3.skey < 'abcd3';

-- Operate LTE
-- Predicate on int
SELECT table3.dkey, table3.skey, table3.ikey FROM table3 WHERE table3.ikey <= 3;
-- Predicate on double
SELECT table3.skey, table3.ikey, table3.dkey FROM table3 WHERE table3.dkey <= 3;
-- Predicate on string
SELECT table3.skey, table3.dkey, table3.ikey FROM table3 WHERE table3.skey <= 'abcd3';

--Test select 2 out of 3 attrs in different order
-- Operate EQ
-- Predicate on int
SELECT table3.ikey, table3.dkey FROM table3 WHERE table3.ikey = 3;
-- Predicate on double
SELECT table3.ikey, table3.skey FROM table3 WHERE table3.dkey = 3;
-- Predicate on string
SELECT table3.dkey, table3.ikey FROM table3 WHERE table3.skey = 'abcd3';

-- Operate GTE
-- Predicate on int
SELECT table3.dkey, table3.skey FROM table3 WHERE table3.ikey >= 3;
-- Predicate on double
SELECT table3.skey, table3.ikey FROM table3 WHERE table3.dkey >= 3;
-- Predicate on string
SELECT table3.skey, table3.dkey FROM table3 WHERE table3.skey >= 'abcd3';

--Test select 1 out of 3 attrs in different order
-- Operate GT
-- Predicate on int
SELECT table3.ikey FROM table3 WHERE table3.ikey > 3;
-- Predicate on double
SELECT table3.ikey FROM table3 WHERE table3.dkey > 3;
-- Predicate on string
SELECT table3.dkey FROM table3 WHERE table3.skey > 'abcd3';

-- Operate NE
-- Predicate on int
SELECT table3.dkey FROM table3 WHERE table3.ikey <> 3;
-- Predicate on double
SELECT table3.skey FROM table3 WHERE table3.dkey <> 3;
-- Predicate on string
SELECT table3.skey FROM table3 WHERE table3.skey <> 'abcd3';

DROP TABLE table3;

