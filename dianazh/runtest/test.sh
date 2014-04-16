make
echo Y | ./dbdestroy testdb
./dbcreate testdb
./minirel testdb < sql/catalog.sql > debug
./minirel testdb < sql/insert.sql >> debug
./minirel testdb < sql/datamation.sql >> debug
./minirel testdb < sql/select.sql >> debug
./minirel testdb < sql/join.sql >> debug
echo Y | ./dbdestroy testdb

