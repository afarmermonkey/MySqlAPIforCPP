all:libccmysql.so test_ccmysql install
libccmysql.so:CCData.cpp ccmysql.cpp ccmysql.h CCData.h
	g++ -fPIC -shared $^ -o $@ -I/usr/include/mysql -lmysqlclient
test_ccmysql:../test_ccmysql/test_ccmysql.cpp
	g++ $^ -o $@ -I./ -lccmysql -L./
install:
	cp libccmysql.so /usr/lib/
	@echo "install libccmysql.so success!"
clean:
	rm -rf *.so
	rm -rf /usr/lib/libccmysql.so
	rm -rf test_ccmysql
