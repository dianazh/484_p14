-- Test insert with index

-- Create relation
CREATE TABLE table2(ikey INTEGER, dkey DOUBLE, skey CHAR(20));

-- Create index
CREATE INDEX table2(ikey);
CREATE INDEX table2(dkey);

-- Insert with attributes in order
INSERT INTO table2(ikey, dkey, skey) VALUES (1, 1.1, 'abcd1');
INSERT INTO table2(ikey, dkey, skey) VALUES (2, 2.2, 'abcd2');
INSERT INTO table2(ikey, dkey, skey) VALUES (3, 3.3, 'abcd3');

SELECT * FROM table2;

-- Insert with attributes out of order
INSERT INTO table2(skey, dkey, ikey) VALUES ('abcd4', 4.4, 4);
INSERT INTO table2(skey, ikey, dkey) VALUES ('abcd4', 4, 4.4);
INSERT INTO table2(dkey, skey, ikey) VALUES (5.5, 'abcd5', 5);
INSERT INTO table2(dkey, ikey, skey) VALUES (5.5, 5, 'abcd5');
INSERT INTO table2(ikey, skey, dkey) VALUES (6, 'abcd6', 6.6);
INSERT INTO table2(ikey, dkey, skey) VALUES (6, 6.6, 'abcd6');

SELECT * FROM table2;
SELECT * FROM relcat;
SELECT * FROM attrcat;
SELECT * FROM table2 WHERE table2.skey >= 'abcd3';
SELECT * FROM table2 WHERE table1.ikey = 5;

DROP TABLE table2;

