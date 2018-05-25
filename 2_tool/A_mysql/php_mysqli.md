//连接数据库
$dbc=new mysqli('localhost','username','passwd','db_name');
//判断是否连接错误
if(mysqli_connect_errno())
{
		echo 'connection error'.mysqli_connect_error();
		exit();
}
//escape字符串，防止db注入
$field=$dbc->escape_string($req->field);

//查询
$result=$dbc->query($query);

//得到一个Key value结构
while ($row = $result->fetch_assoc()) {
	//do something...
}

//如果返回给前前端的json串里面本身某个value本身就是一个json串，则需要先把这个要作为value的json先json_encode一次（相当于把其中的引号等均转义）；