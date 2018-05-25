pip 是一个Python包管理工具
=======================================
pip install -U pip	自我更新
pip install SomePackage             # latest version
pip uninstall SomePackage			卸载软件包
pip install --upgrade SomePackage	更新软件包
pip search "query"	搜素软件包

用python运行pip格式：
--------------------------------
python -mpip install --index https://pypi.mirrors.ustc.edu.cn/simple/ pandas	通过给定源地址安装pandas库；

在shell中输入import pip; print(pip.pep425tags.get_supported())可以获取到pip支持的文件名还有版本