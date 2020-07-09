# ccmysql
封装动态库的工程文件
# test_ccmysql
测试封装库的工程文件
以上两个工程用VS2019打开，在同一个解决方案下，
设置解决方案的属性->启动项目->单启项目 选test_mysql；项目依赖-> 项目选test_mysql 依赖ccmysql 选上。可直接编译运行。
Linux环境下，打开ccmysql文件，直接make即可，会生成test_mysql执行文件
# 其他test文件
其他test文件都是对MySQL官方API的测试程序，只有test_mysqllib文件里有makefile文件，其他文件都一样，可以仿写一下就行。
