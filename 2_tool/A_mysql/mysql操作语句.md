mysql批量为某字段添加前缀或后缀
====================
update table set a=concat('prefix.',a) where ....;

update table set b=concat(a,'suffix') where ...;


JOIN
====================
left	左边的key将全部得到满足；右边有左边没有的忽略；
rigth	右边的Key全部得到满足；右边有，左边没有的忽略；
full	左右两边的key均得到满足；相当于并集；
innter	只取交集；
