
perf
================================
<https://www.ibm.com/developerworks/cn/linux/l-cn-perf1/>

perf stat ./a.out	可以得到一个直观的判断：当前程序是CPU消耗型还是IO消耗型

perf top -p pid

perf record -e cpu-clock -g ./a.out	运行并把待分析数据写在perf.data里面


perf report			生成报告