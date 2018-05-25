set -e;
cp ../index.html_tpl ../index.html
urls=`ls -l ./*.md | sed 's|^.*[ /]\([^ /]\{1,\}\)\.md$|<a href="NotesOfTools/\1.html">\1</a></br>|g' | tr '\n' ' '`;	
sed -i '58i'"${urls}" ../index.html 	
