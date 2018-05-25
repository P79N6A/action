pids=`ps ax | grep elf_name | grep -v grep | grep -v stop | awk '{print $1}'`
curpath=`pwd `;
echo "curpath:${curpath}"
for pid in ${pids}
do
path=`ls -l /proc/${pid} | grep exe | awk '{print $(11)}'`
path=`dirname ${path}`
echo "pid:${pid}  path:${path}"
if [ ${curpath} = ${path} ] && [ $# = 0 ];then
kill -9 ${pid}
echo "[${pid}] has been stopped"
exit 0
fi
done
echo 'nothing stops'
