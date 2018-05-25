
MD = markdown2  

all:z_tool/git/git.html z_tool/gcov_lcov/gcov_lcov.html
z_tool/git/git.html:z_tool/git/git.md
	$(MD) --encoding=UTF-8 $< > $@; 
	sed -i '1i\<meta http-equiv="content-type" content="text/html; charset=UTF-8">' $@;
z_tool/gcov_lcov/gcov_lcov.html:z_tool/gcov_lcov/gcov_lcov.md
	$(MD) --encoding=UTF-8 $< > $@; 
	sed -i '1i\<meta http-equiv="content-type" content="text/html; charset=UTF-8">' $@;
