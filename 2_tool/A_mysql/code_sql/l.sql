SELECT CONCAT('select \'',a.table_name,'\';select * from ',a.table_name,' where field_name in (57,69,0,109,68,113);') AS query
FROM information_schema.tables a
WHERE a.table_schema = 'db_name'
and a.table_name like 'table_name%'
