make clean
make
echo Y | ./dbdestroy testdb
./dbcreate testdb
./minirel testdb < sql_z/insert-nonindex.sql > debug_z
./minirel testdb < sql_z/insert-index.sql >> debug_z
./minirel testdb < sql_z/select-scan.sql >> debug_z
./minirel testdb < sql_z/select-index.sql >> debug_z
./minirel testdb < sql_z/join-snl.sql >> debug_z
./minirel testdb < sql_z/join-snl-dup.sql >> debug_z
./minirel testdb < sql_z/join-index.sql >> debug_z
./minirel testdb < sql_z/join-index-dup.sql >> debug_z
./minirel testdb < sql_z/join-smj-difflen.sql >> debug_z
./minirel testdb < sql_z/join-smj-samelen.sql >> debug_z
echo Y | ./dbdestroy testdb
