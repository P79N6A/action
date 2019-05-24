<?php
class Jwt {
    private static $header=array(
        'alg'=>'HS256', 
        'typ'=>'JWT'   
    );
    private static $algo='sha256';

    public static function getToken(array $payload,$key)
    {
        if(is_array($payload))
        {
            $base64header=self::base64UrlEncode(json_encode(self::$header,JSON_UNESCAPED_UNICODE));
            $base64payload=self::base64UrlEncode(json_encode($payload,JSON_UNESCAPED_UNICODE));
            $token=$base64header.'.'.$base64payload.'.'.self::signature($base64header.'.'.$base64payload,$key);
            return $token;
        }else{
            return false;
        }
    }

    public static function verifyToken($Token,array $mk)
    {
        $tokens = explode('.', $Token);
        if (count($tokens) != 3)
            return false;

        list($base64header, $base64payload, $sign) = $tokens;


	$payload = json_decode(self::base64UrlDecode($base64payload),true);

	if (!isset($payload['iat']) || $payload['iat'] < time()-86400)
	{
		error_log('timestamp too old');
		return false;
	}
	if(!isset($payload['appid']))
	{
		error_log('appid not set');
		return false;
	}

	$ss=self::signature($base64header . '.' . $base64payload, $mk[$payload['appid']]);
	if ( $ss!== $sign)
	{
		error_log('token not equal {'.$ss.'!=='.$sign.'}');
		return false;
	}
	return $payload;
    }




    private static function base64UrlEncode($input)
    {
        return str_replace('=', '', strtr(base64_encode($input), '+/', '-_'));
    }

    private static function base64UrlDecode($input)
    {
        $remainder = strlen($input) % 4;
        if ($remainder) {
            $addlen = 4 - $remainder;
            $input .= str_repeat('=', $addlen);
        }
        return base64_decode(strtr($input, '-_', '+/'));
    }

    private static function signature($input, $key)
    {
        return self::base64UrlEncode(hash_hmac('sha256', $input, $key,true));
    }
}

//    $payload=array('sub'=>'1234567890','name'=>'John Doe','iat'=>1516239022);
//    $jwt=new Jwt;
//    $token=$jwt->getToken($payload);
//    echo "<pre>";
//    echo $token;
//    
//    $getPayload=$jwt->verifyToken($token);
//    echo "<br><br>";
//    var_dump($getPayload);
//    echo "<br><br>";
//    
//    
//    $payload_test=array('iss'=>'admin','iat'=>time(),'exp'=>time()+7200,'nbf'=>time(),'sub'=>'www.admin.com','jti'=>md5(uniqid('JWT').time()));;
//    $token_test=Jwt::getToken($payload_test);
//    echo "<pre>";
//    echo $token_test;
//    
//    $getPayload_test=Jwt::verifyToken($token_test);
//    echo "<br><br>";
//    var_dump($getPayload_test);
//    echo "<br><br>";


?>
