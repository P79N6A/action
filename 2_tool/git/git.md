
参考链接：
---------------------------------

<https://git-scm.com/doc>

<http://rogerdudler.github.io/git-guide/index.zh.html>

查看帮助
---------------------------------
git -h

git --help

创建代码目录
---------------------------------
git clone https://user:password@url [local_dir_name]

更新代码
---------------------------------
git pull

git pull origin master:master

推送代码
---------------------------------
git push origin master:master

git push origin branch1:branch2


分支处理
---------------------------------
git branch -a	查看所有分支

git checkout -b branch1	创建branch1分支

git push origin branch1	把branch1分支推送到远端

git checkout branch1	切到分支branch1

git branch -d brranch1	删除本地branch1分支

git push --delete origin branch1	删除远程branch1分支

git push origin :branch1	删除远程branch1分支

git标签
---------------------------------
git tag -l 列出所有tag
git tag -a tagname	为当前代码添加标签
git push origin tagname	把tag推送到远程
git tag --delete		删除本地标签
git push origin --delete tagname	删除远程标签



代码回滚或放弃更改
---------------------------------
git reset --hard origin/master把当前branch reset成与远程origin的master一样

git reset --hard commit-id	回滚到commit-id，即放弃commit-id之后的提交

git reset --hard HEAD~3		将最近3次提交回滚

git-revert - Revert some existing commits，只放弃commitid的提交；

git checkout master file.txt 放弃文件file.txt的本地更改，使用当前master中相对应的文件

git checkout HEAD -- file.txt 放弃文件file.txt的本地更改，使用HEAD中相对应的文件；（-- basically means: treat every argument after this point as a file name）

git checkout source_branch -- <paths>...		Checkout the path(s) from the branch you want to merge,PATH是从根目录开始的文件夹名。如git checkout branch1 apps/folder

git checkout -p source_branch -- <paths>...		selectively merge hunks

Alternatively, use reset and then add with the option -p,
$ git reset <paths>...
$ git add -p <paths>...

注：直接git reset 到一个地方往往不能提交远程使远程回滚，可以这样操作，先git checkout 到一个临时的分支tmp，再切回master，然后git checkout tmp path..file用分支目录覆盖当前目录，然后再次提交就可以达到回退的目的。

查看版本差异
---------------------------------

git diff 查看当前编辑未提交的文件

git diff filename 查看特定文件更新

git diff –cached  查看已经暂存起来的文件和上次提交的版本之间的差异

git diff –cached filename 查看已经暂存起来的某个文件和上次提交的版本之间的差异

git diff version1 version2 查看某两个版本之间的差异

git diff tag1:filename tag2:filename 查看某两个版本的某个文件之间的差异

git diff tagA:./dir1/dir2 tagB:./dir1/dir2 查看某两个版本从当前目录开始某个文件夹的差异

git diff orgin/master:dir1/file dir1/file	对比远程仓库中文件file与本地文件file之间的差异



