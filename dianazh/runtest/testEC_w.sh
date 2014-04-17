make clean
make EC
echo Y | ./dbdestroyEC testdb
./dbcreateEC testdb
./minirelEC testdb < sql_z/insert-nonindex.sql > debugEC_z
./minirelEC testdb < sql_z/insert-index.sql >> debugEC_z
./minirelEC testdb < sql_z/select-scan.sql >> debugEC_z
./minirelEC testdb < sql_z/select-index.sql >> debugEC_z
./minirelEC testdb < sql_z/join-snl.sql >> debugEC_z
./minirelEC testdb < sql_z/join-snl-dup.sql >> debugEC_z
./minirelEC testdb < sql_z/join-index.sql >> debugEC_z
./minirelEC testdb < sql_z/join-index-dup.sql >> debugEC_z
./minirelEC testdb < sql_z/join-smj-difflen.sql >> debugEC_z
./minirelEC testdb < sql_z/join-smj-samelen.sql >> debugEC_z
echo Y | ./dbdestroyEC testdb
