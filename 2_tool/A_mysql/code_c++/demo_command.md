SET NAMES UTF8

... FROM table1 INNER|LEFT|RIGHT|CROSS JOIN table2 ON conditiona

select * from tb1 union select * from tb2; //查询的出来的表，以tb1的表的查询结果的字段为准，且需要列数相同。

select a.* from (select * from tb1 where 过滤条件) as a where 过滤条件;

select a.column1,a.column2,b.column1,b.column2... from (select * from tb1 where 过滤条件) as a , (select * from tb2 where 过滤条件) as b where 外键匹配 and 过滤条件;



https://stackoverflow.com/questions/37313983/how-to-loop-through-all-the-tables-on-a-database-to-update-columns
对一个数据库YourDBNameHere中所有包含"tenzhen"表进行操作(生成的字符串再copy进mysql回车)
---------------------------------------------------------------------------------
SELECT GROUP_CONCAT(t.query SEPARATOR '; ')
FROM
(
    SELECT CONCAT('UPDATE ', a.table_name,
                  ' SET date = "2016-04-20" WHERE name = "Example";') AS query,
        '1' AS id
    FROM information_schema.tables a
    WHERE a.table_schema = 'YourDBNameHere'
    and a.talbes like '%tenzhen%'
) t
GROUP BY t.id


SELECT GROUP_CONCAT(t.query SEPARATOR '; ')
FROM
(
    SELECT CONCAT('alter table ', a.table_name,' add is_cycle  int(10) default 0 after play_endtime;') AS query, '1' AS id
    FROM information_schema.tables a
    WHERE a.table_schema = 'db_gfm'
    and a.table_name like 'fm_seg_info%'
) t
GROUP BY t.id;



select FROM_UNIXTIME(1156219870);
Select UNIX_TIMESTAMP(’2006-11-04 12:23:00′);