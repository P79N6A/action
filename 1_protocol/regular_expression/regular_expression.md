
前言
=========================================
正则表达式分基础正则表达式（BRE）和扩展正则表达式（ERE）。在linux sed命令的info手册中看到下面一句话，

	`-r'
	`--regexp-extended'
		Use extended regular expressions rather than basic regular
		 expressions.  Extended regexps are those that `egrep' accepts;
		 they can be clearer because they usually have less backslashes,
		 but are a GNU extension and hence scripts that use them are not
		 portable.  *Note Extended regular expressions: Extended regexps.
		 
> 扩展正则表达式相比于基础正则表达式需要使用的'\'要更少.

> linux中sed和grep均支持正则表达式和扩展正则表达式，如果使用扩展正则表达式，分别通过-r和-e来指定。





正则表达式（BRE）
=========================================
如果文件file.txt的内容是

	just do it
	

### . 匹配任意单个字符
> grep 'ju...do it' file.txt	//匹配到第一行

### * 匹配其前面的字符任意次
> grep 'j* do it' file.txt 

### ^ 锚定行首，此字符后面的任意内容必须出现在行首
> grep '^j.*t' file.txt  

### $ 锚定行尾，此字符前面的任意内容必须出现在行尾
> grep '.*t$' file.txt 

### ^...$ 整个字符串
> grep '^j.*t$' file.txt

### ^$   空白行
> grep  -n '^$' file.txt //查看空白行
> cat file.txt | grep -v  '^$' > file2.txt //去除空白行

### \\{n,m\\} 匹配其前单个字符重现的次数区间，至少n次，至多m次
	\{n\}:重现n次
	\(n,\}:至少重现n次
	\{0,m\} :至多m次

### .* 匹配任意长度任意字符
### ？ 匹配其前面的字符1次或0次

### \\<或\\b 锚定词首 
	grep '\<do' file.txt</pre>

### \>> 锚定词尾
	grep 'do\>' file.txt

### 反向引用，在表达式里面用（）包裹起来字符串，如果包裹了多个，则会被依次编号为1...99，然后在表达式后面的部分引用它。

	对于字符串aaacbbbaaabc abcdea abcdeb
	grep '\(aaa\).*\1' file.txt //前面aaa被包裹，则后面\1就表示aaa，如果在字符串后面再次出现aaa则被匹配到
	grep '\([ac]\).*\1' file.txt	//如果a被匹配到，则\1表示a，如果c被匹配到\1表示c，注意\1不能同时表示ac，程序在运行时，会开辟内存块把不同实际匹配到的字符串存起来，以待解析字符串后面的时候引用

### [] 匹配被它括起来的任意单个字符
### [^]	匹配不被它括起来的任意单个字符


## 基础正则表达式优先级
<table border='1px'>
<tr><th> 运算符 					</th><th>描述 </th></tr>
<tr><td> \ 							</td><td>转义符 </td></tr>
<tr><td> (), (?:), (?=), [] 		</td><td>圆括号和方括号 </td></tr>
<tr><td> *, +, ?, {n}, {n,}, {n,m}	</td><td>限定符</td></tr>
<tr><td>^, $, \任何元字符、任何字符	</td><td>定位点和序列（即：位置和顺序）</td></tr>
<tr><td>|							</td><td>替换，"或"操作。字符具有高于替换运算符的优先级，使得"m|food"匹配"m"或"food"。若要匹配"mood"或"food"，请使用括号创建子表达式，从而产生"(m|f)ood"。</td></tr>
</table>





