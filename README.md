# JCJ--Production--versionControl
# JCJ175驱动程序文档
# 这是什么？
这是JCJ175的C语言驱动程序，这个驱动程序是JCJ175能够正常运行的必要条件；<br>
当前没有发现版本号，但是已经进入正常应用阶段，版本号至少不低于1.0.0；<br>
这份程序有两个时间标志，最晚的为20160220.
## 它有些什么？
这份程序包含有以下文件：（以功能做分类）<br>
主程序（main.c，main.h）；<br>
宏处理（define.h）；<br>
初始化程序（int.c，int.h）；<br>
补偿计算（ad.c，ad.h）；<br>
报警控制（alarm.c，alarm.h）；<br>
输出控制（bs.c，bs.h）；<br>
菜单控制（menu.c，menu.h）；<br>
菜单文件（code.c，code.h）；<br>
程序读写（i2_rw.c，i2_rw.h）；<br>
按键操作（key.c，key.h）；<br>
液晶屏操作（ht1621.c，ht1621.h）；<br>
液晶屏显示（led.c，led.h）；<br>
异步通信（uart_0.c，uart_0.h）；<br>
探头操作（sht10.c，sht10.h）；<br>
以及其他单片机运行所需的程序文件。
## 它能用来干什么？
这段程序支持JCJ175进行485modbus输出。


# 我怎么用它？
## 编译及下载
使用MBLAB IDE及HI-TECH编译器PICC 9.80进行编译，获得的.hex文件下载进单片机测试。
## 修改及调试
由于缺少注释及其他详细的文档，这里无法提供一份可用的教程或实例。


# 版权
## 版权声明
无版权声明
## 版本更迭
当前版本至少不低于1.0.0<br>
无版本更迭信息。
