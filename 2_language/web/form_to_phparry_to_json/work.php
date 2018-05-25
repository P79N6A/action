<?php
	//echo "<html>"
	$raw_post_data = file_get_contents('php://input', 'r');
	echo $raw_post_data;
	parse_str($raw_post_data,$a);
	var_dump($a);
	//echo "<br/>"
	//echo "--------------------"
	echo json_encode($a);
	//echo "<br/>"
	$r=json_decode($raw_post_data);
	if(!$r) echo "json_decode error";
	var_dump($r); 
	//echo "</html>"
?>

