
websocket
=======================================
<https://tools.ietf.org/html/rfc6455>


websocket从客户端发往服务器时需要带mask标识，而返回时不能带mask，看协议中这样可以防攻击；所以不能把从客户端收到的包，直接返回；

websocket在http升级前,服务器最好对origin做一个判断，防止跨站点 WebSocket 劫持（Hijacking）。[websocket最好严格通过挑战鉴权或动态口令等来识别用户身份]