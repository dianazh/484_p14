-- Test insert with index

-- Create relation
CREATE TABLE table1(ikey INTEGER, dkey DOUBLE, skey CHAR(20));
-- Insert with attributes in order
INSERT INTO table1(ikey, dkey, skey) VALUES (1, 1.1, "abcd1");
INSERT INTO table1(ikey, dkey, skey) VALUES (2, 2.2, "abcd2");
INSERT INTO table1(ikey, dkey, skey) VALUES (3, 3.3, "abcd3");

SELECT * FROM table1;

-- Insert with attributes out of order
INSERT INTO table1(skey, dkey, ikey) VALUES ("abcd4", 4.4, 4);
INSERT INTO table1(skey, dkey, ikey) VALUES ("abcd5", 5.5, 5);
INSERT INTO table1(skey, dkey, ikey) VALUES ("abcd6", 6.6, 6);

SELECT * FROM table1;

DROP TABLE table1;

