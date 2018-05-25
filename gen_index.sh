cp index.html_tpl index.html
urls=`sh gen_content.sh`
echo ${urls}
#urls="hahah"
sed -i '58i\'"${urls}"'' index.html 	
