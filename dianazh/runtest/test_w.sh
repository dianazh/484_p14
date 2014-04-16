make clean
make
echo Y | ./dbdestroy testdb
./dbcreate testdb
./minirel testdb < sql_w/insert-nonindex.sql > debug_w
./minirel testdb < sql_w/insert-index.sql >> debug_w
./minirel testdb < sql_w/select-scan.sql >> debug_w
./minirel testdb < sql_w/select-index.sql >> debug_w
./minirel testdb < sql_w/join-snl.sql >> debug_w




./minirel testdb < sql_w/join-index-both.sql >> debug_w
./minirel testdb < sql_w/join-index-left.sql >> debug_w
./minirel testdb < sql_w/join-index-right.sql >> debug_w
#./minirel testdb < sql_w/join-index-sqlite.sql >> debug_w
./minirel testdb < sql_w/join-smj-boundary.sql >> debug_w
./minirel testdb < sql_w/join-smj-difflen.sql >> debug_w
./minirel testdb < sql_w/join-smj-samelen.sql >> debug_w
echo Y | ./dbdestroy testdb
