REGISTER /data/gdata/pig-0.15.0/thirdparty/mysql-connector-java-5.1.38-bin.jar;
REGISTER /data/gdata/pig-0.15.0/thirdparty/piggybank-0.15.0.jar;
REGISTER /data/gdata/pig-0.16.0/thirdparty/KVLoader-0.5.1.jar;

%declare tv 500

A = LOAD '/qos_kv/34/2/${MACRO_LOG_DAY}/${MACRO_LOG_HOUR}/*.data' USING com.tencent.gdata.pig.KVLoader('&', '=', 'business_name:lost_package_rate:timestamp');
A1 = FOREACH A GENERATE $0 as bn,$1 as lpr, $2 as timestamp;
B = FILTER A1 BY  bn is not null and lpr is not null and timestamp is not null and timestamp >= ${MACRO_LOG_START_TIMESTAMP} and timestamp <= ${MACRO_LOG_END_TIMESTAMP};
--dump C
C = foreach B generate bn as bn,(int)lpr as ilpr,(float)lpr as flpr ,timestamp as tt; 
--dump C;
C1 = foreach C generate bn as bn,((ilpr<1)?1:0) as r0,((ilpr>=1 and ilpr<2)?1:0) as r1,((ilpr>=2 and ilpr<4)?1:0) as r2,((ilpr>=4 and ilpr<8)?1:0) as r3,((ilpr>=8 and ilpr<16)?1:0) as r4,((ilpr>=16)?1:0) as r5,flpr as flpr,tt as tt;
--dump C1; 
D = group C1 by bn; 
D1 = foreach D { 
        generate group as bn,SUM(C1.r0) as r0,SUM(C1.r1) as r1,SUM(C1.r2) as r2,SUM(C1.r3) as r3,SUM(C1.r4) as r4,SUM(C1.r5) as r5,AVG(C1.flpr) as aflpr,MAX(C1.flpr) as mflpr,COUNT(C1.tt) as tc; 
}
--dump D1
D2 = foreach D1 generate ${MACRO_LOG_TIME}L as logtime, bn as business_name,(float)r0*100/tc as d0,(float)r1*100/tc as d1,(float)r2*100/tc as d2,(float)r3*100/tc as d3,(float)r4*100/tc as d4,(float)r5*100/tc as d5,aflpr as average_lost_rate,mflpr as max_lost_rate,tc as total_count;
--dump D2; 

STORE D2 INTO '/pigtest/test.result.7' USING org.apache.pig.piggybank.storage.DBStorage('com.mysql.jdbc.Driver', 
	'jdbc:mysql://${MACRO_DBHOST}:${MACRO_DBPORT}/${MACRO_DATABASE}', 
	'${MACRO_USERNAME}', '${MACRO_PASSWORD}', 
	'REPLACE INTO  ${MACRO_TABLENAME} (logtime, business_name, d0,d1,d2,d3,d4,d5,average_lost_rate,max_lost_rate,total_count) VALUES (?, ?, ?, ?,?,?,?,?,?,?,?)');