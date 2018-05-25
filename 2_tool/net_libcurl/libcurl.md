libcurl
================================================


curl_multi_socket_action(g.multi, CURL_SOCKET_TIMEOUT, 0, &running_count) 该调用如果在启动运行，它会回调callback以使由curl内部创建的socket，通过用户自定义的callbak_fucntion被设置到全局epoll中，然后开始启动传输。gdb调试发现，断点在写数据回调中，在整个上面这个函数还没有完全运行完，就可以完成较小任务的全部传输，但如果不gdb同样的进程直接运行，则需要运行curl_multi_socket_action(g.multi,events[i].data.fd, CURL_CSELECT_IN.... , &running_count)等来驱动进程。也就说libcurl内部在上面这个参数的调用中开始存在一个自驱动。另外如果检测发生了超时，也需要调用如上函数，以完成对相应资源的释放。



---------------------------------------------------------------------------

libcurl对大量请求连接提供了管理socket的方法，用户可使用select/poll/epoll事件管理器监控socket事件，可读写时通知libcurl读写数据，libcurl读写完成后再通知用户程序改变监听socket状态。
两个重要的设置：
curl_multi_setopt(g.multi, CURLMOPT_SOCKETFUNCTION, sock_cb);
curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, multi_timer_cb);
1.当使用curl_multi_add_handle(g->multi, conn->easy)添加请求时会回调multi_timer_cb，然后调用curl_multi_socket_action(g->multi, CURL_SOCKET_TIMEOUT, 0, &g->still_running)初始化请求并得到一个socket(fd)。
2.调用sock_cb回调函数，传入新建的sockfd，根据传入的what状态添加到相应的事件管理器，如封装epoll的libev或libevent。
3.当事件管理器发现socket状态改变时通过curl_multi_socket_action(g->multi, fd, action, &g->still_running)通知libcurl读写数据，然后再调用sock_cb通知事件管理器，如此反复。
libcurl官网提供的基于libev和libevent事件管理示例：
http://curl.haxx.se/libcurl/c/hiperfifo.html
http://curl.haxx.se/libcurl/c/evhiperfifo.html