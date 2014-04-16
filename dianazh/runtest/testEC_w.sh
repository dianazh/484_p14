make EC
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_w/insert-index.sql > debugEC_w
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_w/insert-nonindex.sql > debugEC_w
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_w/join-index-both.sql >> debugEC_w
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_w/join-index-left.sql >> debugEC_w
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_w/join-index-right.sql >> debugEC_w
#./minirelEC testdb < sql_w/join-index-sqlite.sql >> debugEC_w
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_w/join-smj-boundary.sql >> debugEC_w
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_w/join-smj-difflen.sql >> debugEC_w
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_w/join-smj-samelen.sql >> debugEC_w
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_w/select-index.sql >> debugEC_w
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_w/select-scan.sql >> debugEC_w
echo Y | ./dbdestroyEC testdb
