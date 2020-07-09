# MySqlAPIforCPP
mysql提供了官方的CAPI，使用起来相对复杂。本项目对MySQL的官方api进行了MySQL的隐藏式封装，可跨平台使用，使用者无须了解更多的MySQL语句和官方的CAPI。由于能力范围有限，本项目旨在通过C++对MySQL数据库进行简单操作，并未考虑线程安全和数据读取性能,仅供学习使用。文件中除了test_ccmysql文件是对C++接口的测试，其余test文件皆是对MySQL官方接口的测试。ccmysql是封装动态库的源码。
# 环境配置
# windows 环境：
#MySQL Version：MySQL8.0.13
#Visual Stdio 2019          
#doc文件里提供MySQL Workbench的免安装版
# Linux环境：
#Ubuntu18.04       
#MySQL Version:MySQL8.0.13   
#gcc version: 7.05   
#make version: GNU Make 4.1   
