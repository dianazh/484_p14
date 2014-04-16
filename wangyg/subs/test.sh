make clean
make
echo Y | ./dbdestroy testdb
valgrind --leak-check=full ./dbcreate testdb
valgrind --leak-check=full ./minirel testdb sql/catalog.sql > debug
valgrind --leak-check=full ./minirel testdb sql/insert.sql >> debug
valgrind --leak-check=full ./minirel testdb sql/datamation.sql >> debug
valgrind --leak-check=full ./minirel testdb sql/select.sql >> debug
valgrind --leak-check=full ./minirel testdb sql/join.sql >> debug
echo Y | ./dbdestroy testdb

