SELECT GROUP_CONCAT(t.query SEPARATOR ';')
FROM
(
    SELECT CONCAT('select * from ',a.table_name,' where field_name in (57,69,0,109,68,113)') AS query, '1' AS id
    FROM information_schema.tables a
    WHERE a.table_schema = 'db_name'
    and a.table_name like 'table_name%'
) t
GROUP BY t.id;
