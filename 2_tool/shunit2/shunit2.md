
shunit2
================================

<https://github.com/kward/shunit2>

<http://ssb.stsci.edu/testing/shunit2/shunit2.html>

shunit2是运行在类unix机器上的自动化测式框架，主要用于Bourne based shell scripts；

每个用例写在test打头的函数里面，如果testcase1,testxxx等；

注意尽量不要使用set -e，包括用source运行的其它文件中也不要出现，否则会导致shunit2出现一些未知问题

#### 主要需要重写的函数有：
	oneTimeSetUp	所有case运行前运行，主要用于构造总运行环境
	oneTearDown		所有case运行结果后运行，主要用于清理运行环境
	setUp		每个case运行前运行；
	tearDown	每个case运行结构后运行
	
#### 相关的断言有：
	assertEquals [message] expected actual
	assertNotEquals [message] expected actual
	assertSame [message] expected actual
	assertNotSame [message] unexpected actual
	assertNull [message] value
	assertNotNull [message] value
	assertTrue [message] condition	如：assertTrue 'test failed' "[ -r /some/non-existant/file' ]"
	assertFalse [message] condition

#### 代码示例：
~~~
#! /bin/sh
oneTimeSetUp(){
echo '=====================SetUp';
}

oneTimeTearDown(){
echo '=====================TearDown';
}
setUp(){
echo '-----------------setUp';
}

tearDown(){
echo '-----------------tearDown';
}

testEquality() {
  assertEquals 1 1 
  assertEquals 1 0 
}

test1234() {
  assertEquals 1 1 
}
# Load shUnit2.
. ./shunit2-master/shunit2
~~~
demo中最后一行需要运行库中shunit2库，由此开始运行所有case;