/************************FILE INFOMATION***********************************
**
** Project Project       : C++Mysql8.0数据库跨平台编程实战
** Contact               : 913902738@qq.com
** Author                : 朱文泉
** Function              ：测试封装库
***************************************************************************/

#include <iostream>
#include"ccmysql.h"
#include <thread> 
using namespace std;
using namespace cc;
int main()
{
    CCMysql my;
    //1 mysql 初始化
    cout << "my.Init() = " << my.Init() << endl;
    my.SetConnectTionout(3);
    my.SetReconnect(true);
    //2 连接MySQL 如果没有调用Init 内部会自动调用
    if (my.Connect("127.0.0.1", "root", "123456", "zwq")) //vs运行连接的是Windows的MySQL Linux运行连接的是Linux的MySQL
    {
        cout << "my.Connect success!" << endl;
    }
    //3 执行sql语句创建表
    string sql = "";
    sql = "CREATE TABLE IF NOT EXISTS `t_ccmysql`\
        (`id` INT AUTO_INCREMENT PRIMARY KEY,\
         `name` VARCHAR(1024),\
         `data` BLOB,\
         `size` INT)";
    cout << my.Query(sql.c_str()) << endl;
    //4 测试自动重连
    /* for (;;)
     {
         cout << my.Query(sql.c_str()) << flush;
     }*/
     //5 插入一条数据
     //清空数据，包括自增id
    sql = "TRUNCATE t_ccmysql";
    cout << my.Query(sql.c_str()) << endl;
    SQLDATA data1;
    //data1.insert(make_pair("name", CCData("insertname001")));
    data1["name"] = "insertname000";
    data1["size"] = "1024";
    //cout << my.GetInsertSql(data1,"t_ccmysql");
    my.Insert(data1, "t_ccmysql");
    data1["name"] = "insertname001";
    my.Insert(data1, "t_ccmysql");

    //6 插入图片 二进制数据
    CCData file1;
    file1.LoadFile("me.jpg");
    data1["name"] = "me_new1.JPG";
    data1["data"] = file1;
    data1["size"] = &file1.size;
    my.InsertBin(data1, "t_ccmysql");
    //换个名称再插入一次
    data1["name"] = "me_new2.JPG";
    my.InsertBin(data1, "t_ccmysql");
    file1.Drop();

    //7 修改id=1的数据 从修改普通数据
    SQLDATA updata;
    updata["name"] = "updename001";
    cout << "my.Update" << my.Update(updata, "t_ccmysql", "where id=1") << endl;
    // 修改id=3的图片 修改二进制数据
    SQLDATA updatabin;
    CCData file2;
    file2.LoadFile("thz.jpg");
    updatabin["data"] = file2;
    cout << "my.UpdateBin" << my.UpdateBin(updatabin, "t_ccmysql", "where id=3") << endl;
    file2.Drop();

    //8使用事务插入多条数据
    my.StartTransaction();
    SQLDATA data3;
    data3["name"] = "transname000";
    data3["size"] = "1024";
    my.Insert(data3, "t_ccmysql");
    data3["name"] = "transname001";
    my.Insert(data3, "t_ccmysql");

    my.RollBack();//回滚  MySQL引擎必须时InnoDB

    data3["name"] = "transname001";
    my.Insert(data3, "t_ccmysql");
    data3["name"] = "transname002";
    my.Insert(data3, "t_ccmysql");

    CCData file3;
    file3.LoadFile("thz.jpg");
    data3["name"] = "thz_new.JPG";
    data3["data"] = file3;
    data3["size"] = &file3.size;
    my.InsertBin(data3, "t_ccmysql");
    data3["name"] = "thz_new2.JPG";
    my.InsertBin(data3, "t_ccmysql");
    file3.Drop();
    my.Commit();
    my.StopTransaction();

    //9  获取结果集
    //获取结果集测试
    sql = "select * from t_ccmysql";
    cout << my.Query(sql.c_str()) << endl;
    my.StoreResult(); //结果集全部获取
    for (;;)
    {
        //获取一行数据
        std::vector<CCData> row = my.FetchRow();
        if (row.size() == 0)break;
        row[2].SaveFile(row[1].data);
        for (int i = 0; i < row.size(); i++)
        {
            if (row[i].data)
                cout << row[i].data << " ";
        }
        cout << endl;
    }
    my.FreeResult();
    cout << my.Query(sql.c_str()) << endl;
    my.UseResult();  //开始接收结果集
    my.FreeResult();


    //10 开始测试字符集 问题， 插入，读取 GBK utf-8
    cout << "开始测试字符集" << endl;
    //测试utf8 指定字段name的 utf 字符集
    sql = "CREATE TABLE IF NOT EXISTS `t_utf8` \
		(`id` INT AUTO_INCREMENT,	\
		`name` VARCHAR(1024) CHARACTER SET utf8 COLLATE utf8_bin,\
		PRIMARY KEY(`id`))";

    my.Query(sql.c_str());
    //清空数据
    my.Query("TRUNCATE t_utf8");
    //指定与mysql处理的字符集
    my.Query("set names utf8");
    {
        SQLDATA data;
        data["name"] = u8"测试的UTF中文";
        my.Insert(data, "t_utf8");
    }


    //10.1 插入gbk数据
    sql = "CREATE TABLE IF NOT EXISTS `t_gbk` \
		(`id` INT AUTO_INCREMENT,	\
		`name` VARCHAR(1024) CHARACTER SET gbk COLLATE gbk_bin,\
		PRIMARY KEY(`id`))";

    my.Query(sql.c_str());
    //清空数据
    my.Query("TRUNCATE t_gbk");
    //指定与mysql处理的字符集
    my.Query("set names gbk");
    {
        SQLDATA data;
        CCData tmp = u8"测试的GBK中文";
        string gbk = tmp.UTF8ToGBK();
        data["name"] = gbk.c_str();
        my.Insert(data, "t_gbk");
    }

    //读取gbk格式数据
    cout << "====Print gbk string" << endl;
    my.Query("set names gbk");
    my.Query("select * from t_gbk");
    my.StoreResult(); //结果集全部获取
    for (;;)
    {
        //获取一行数据
        std::vector<CCData> row = my.FetchRow();
        if (row.size() == 0)break;
#ifdef _WIN32
        cout << "id: " << row[0].data << " name:" << row[1].data << endl;
#else
        cout << "id: " << row[0].data << "  name:" << row[1].data << endl;
        cout << "转换UTF8后 id: " << row[0].data << "  name:" << row[1].GBKToUTF8() << endl;
#endif
    }
    my.FreeResult();

    //读取utf8格式数据

    cout << "====Print utf-8 string" << endl;
    my.Query("set names utf8");
    my.Query("select * from t_utf8");

    my.StoreResult(); //结果集全部获取
    for (;;)
    {
        //获取一行数据
        std::vector<CCData> row = my.FetchRow();
        if (row.size() == 0)break;
#ifdef _WIN32
        cout << "id: " << row[0].data << "  name:" << row[1].data << endl;
        cout << "转换GBK后 id: " << row[0].data << "  name:" << row[1].UTF8ToGBK() << endl;
#else
        cout << "id:" << row[0].data << "name:" << row[1].data << endl;
#endif
    }
    my.FreeResult();

    XROWS rows = my.GetResult("select * from t_ccmysql");
    for (int i = 0; i < rows.size(); i++)
    {
        auto row = rows[i];
        for (int j = 0; j < row.size(); j++)
        {

            if (!row[j].data)
            {
                cout << "[NULL],";
                continue;
            }
            switch (row[j].type)
            {
            case CC_MYSQL_TYPE_BLOB:
                cout << "[BLOB]";
                break;
            case CC_MYSQL_TYPE_LONG:
                cout << (long)row[j].data;
                break;
            case CC_MYSQL_TYPE_STRING:

                break;
            default:
                cout << row[j].data;
                break;
            }
            cout << ",";
        }
        cout << endl;
    }


    //订票模拟（事务）t_tickets(id int,sold int)
    //
    sql = "CREATE TABLE IF NOT EXISTS `t_tickets` \
		(`id` INT AUTO_INCREMENT,	\
		`sold` int,\
		PRIMARY KEY(`id`))";
    my.Query(sql.c_str());
    {
        SQLDATA data1;
        //data1.insert(make_pair("name", CCData("insertname001")));
        data1["sold"] = "0";
        //cout << my.GetInsertSql(data1,"t_ccmysql");
        my.Insert(data1, "t_tickets");
        //事务开始
        my.StartTransaction();
        string id = rows[0][0].data;
        XROWS rows = my.GetResult("select * from t_tickets where sold=0 order by id for update");

        cout << "Buy ticket id is " << id << endl;

        //模拟冲突
        this_thread::sleep_for(10s);
        data1["sold"] = "1";
        string where = "where id=";
        where += id;
        cout << my.Update(data1, "t_tickets", where) << endl;


        //my.GetResult("select * from t_tickets where sold=0 for update");
        my.Commit();
        my.StopTransaction();
        //事务结束
    }
    //清除内存
    my.Close();
    std::cout << "test_CCMysql!\n";
    system("pause");
}