-- Test index join when right attribute has index

-- Create relation
CREATE TABLE table5(ikey INTEGER, dkey DOUBLE, skey CHAR(20));
CREATE TABLE table6(ikey INTEGER, dkey DOUBLE, skey CHAR(20));

--Create index
CREATE INDEX table5(ikey);
CREATE INDEX table5(dkey);

-- Insert with attributes in order
INSERT INTO table5(ikey, dkey, skey) VALUES (1, 1.1, "abcd1");
INSERT INTO table5(ikey, dkey, skey) VALUES (2, 2.2, "abcd2");
INSERT INTO table5(ikey, dkey, skey) VALUES (3, 3.3, "abcd3");

-- Insert with attributes out of order
INSERT INTO table6(skey, dkey, ikey) VALUES ("abcd1", 1.1, 4);
INSERT INTO table6(skey, dkey, ikey) VALUES ("abcd2", 5.5, 2);
INSERT INTO table6(skey, dkey, ikey) VALUES ("abcd6", 3.3, 3);

-- Operate EQ
-- Predicate on int
SELECT * FROM table6, table5 WHERE table6.ikey = table5.ikey;
-- Predicate on double
SELECT * FROM table5, table5 WHERE table6.dkey = table5.dkey;

DROP TABLE table5;
DROP TABLE table6;
