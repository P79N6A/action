declare -A m;
declare -A m=（）
declare -A m=(["100"]="1" ["200"]="2")  初始化时赋值
m[$key]=$value;	定义后赋值
echo ${!map[@]} 输出所有key；
echo ${map[@]}  输出所有value

遍历
----------------------------------
for key in ${!map[@]}  
do  
    echo ${map[$key]}  
done  