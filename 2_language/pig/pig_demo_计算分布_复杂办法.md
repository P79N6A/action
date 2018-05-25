REGISTER /data/gdata/pig-0.15.0/thirdparty/mysql-connector-java-5.1.38-bin.jar;
REGISTER /data/gdata/pig-0.15.0/thirdparty/piggybank-0.15.0.jar;
REGISTER /data/gdata/pig-0.16.0/thirdparty/KVLoader-0.5.1.jar;

%declare tv 500

A = LOAD '/qos_kv/24/4/${MACRO_LOG_DAY}/*/*.data' USING com.tencent.gdata.pig.KVLoader('&', '=', 'type:business_name:request_type:delay_ms:timestamp');
B = FOREACH A GENERATE $0 as type, $1 as bn, $2 as rt, $3 as dm, $4 as timestamp;
C = FILTER B BY  type == 'rsp' and rt == 'uniq' and dm is not null and bn is not null and timestamp is not null and timestamp >= ${MACRO_LOG_START_TIMESTAMP} and timestamp <= ${MACRO_LOG_END_TIMESTAMP};
--dump C
D = FOREACH C GENERATE  bn as bn ,  (long)dm/${tv} as di, timestamp as timestamp;
D1 = FOREACH D GENERATE bn as bn, (di>=5?5:di) as di,timestamp as timestamp;
E = GROUP D1 BY (bn,di);
F = FOREACH E GENERATE group.bn as bn, group.di as di,(long)COUNT(D1.timestamp) as cnt;
--dump F;
d0 = FILTER F BY di == 0;
d1 = FILTER F BY di == 1;
d2 = FILTER F BY di == 2;
d3 = FILTER F BY di == 3;
d4 = FILTER F BY di == 4;
d5 = FILTER F BY di == 5;
G = COGROUP d0 BY bn,d1 BY bn,d2 BY bn,d3 BY bn,d4 BY bn,d5 BY bn;
--dump G;
--describe G;
H = FOREACH G GENERATE group as bn, FLATTEN(((IsEmpty(d0))?null:d0.cnt)) as d0, FLATTEN(((IsEmpty(d1))?null:d1.cnt)) as d1, FLATTEN(((IsEmpty(d2))?null:d2.cnt)) as d2, FLATTEN(((IsEmpty(d3))?null:d3.cnt)) as d3, FLATTEN(((IsEmpty(d4))?null:d4.cnt)) as d4, FLATTEN(((IsEmpty(d5))?null:d5.cnt)) as d5;
--dump H;
--describe H;
H1 = FOREACH H GENERATE  bn as bn, ((d0 is null)?0:d0) as d0, ((d1 is null)?0:d1) as d1, ((d2 is null)?0:d2) as d2, ((d3 is null)?0:d3) as d3, ((d4 is null)?0:d4) as d4, ((d5 is null)?0:d5) as d5;
--dump H1;
--describe H1;
T1 = group C by bn;
T2 = foreach T1 generate group as bn,(long)AVG(C.dm) as avg;
--dump T2;
--describe T2;
T3 = cogroup H1 by bn, T2 by bn;
T4 = foreach T3 generate group as bn, FLATTEN(H1.d0) as d0, FLATTEN(H1.d1) as d1, FLATTEN(H1.d2) as d2, FLATTEN(H1.d3) as d3, FLATTEN(H1.d4) as d4, FLATTEN(H1.d5) as d5,FLATTEN(T2.avg) as avg;
--dump T4;
--describe T4;
I = FOREACH T4{
    total = d0 + d1 + d2 + d3 + d4 +d5;
    GENERATE ${MACRO_LOG_TIME}L as logtime, bn as business_name, ${tv} as tcinterval, d0*100/total, d1*100/total, d2*100/total, d3*100/total, d4*100/total,d5*100/total,total as total_count,(float)avg/1000 as average;
}

STORE I INTO '/pigtest/test.result.7' USING org.apache.pig.piggybank.storage.DBStorage('com.mysql.jdbc.Driver', 
	'jdbc:mysql://${MACRO_DBHOST}:${MACRO_DBPORT}/${MACRO_DATABASE}', 
	'${MACRO_USERNAME}', '${MACRO_PASSWORD}', 
	'REPLACE INTO  ${MACRO_TABLENAME} (logtime, business_name, tcinterval, d0,d1,d2,d3,d4,d5,total_count,average) VALUES (?, ?, ?, ?,?,?,?,?,?,?,?)');