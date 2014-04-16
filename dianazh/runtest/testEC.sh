make EC
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql/catalog.sql > debugEC
./minirelEC testdb < sql/insert.sql >> debugEC
./minirelEC testdb < sql/datamation.sql >> debugEC
./minirelEC testdb < sql/select.sql >> debugEC
./minirelEC testdb < sql/join.sql >> debugEC
echo Y | ./dbdestroyEC testdb
