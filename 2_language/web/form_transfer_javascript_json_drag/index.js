var arc=new Array();
for(i=0;i<7;i++)
{
	arc[i]={"date":i.toString(),"thing":i.toString()};
}
function allowDrop(ev)
{
	ev.preventDefault();
}

function drag(ev)
{
	ev.dataTransfer.setData("Text",ev.target.id);
	console.log("drag");
	// alert("drag");
}

function drop(ev)
{
	ev.preventDefault();
	var data=ev.dataTransfer.getData("Text");
	// alert(data);
	// ev.target.parentNode.insertBefore(document.getElementById(data),ev.target.id);
	var tar=ev.target;
	while(tar.parentNode.id!="list")
		tar=tar.parentNode;
	
	tar.parentNode.insertBefore(document.getElementById(data),tar);
	
	var fr=tar.parentNode;
	var all_items=fr.getElementsByTagName("div");
	// alert(all_items.length);
	arc=new Array();
	for(i=0;i<all_items.length;i++)
	{
		var a=all_items[i].childNodes[0].value;
		var b=all_items[i].childNodes[1].value;
		arc[i]={"date":a,"thing":b};
	}
	// alert(ev.target.id);
	// var s=document.getElementById(data).si;
	// var d=tar.si;
	
	// if(s==d+1)
		// return true;
	// if(s>d)
	// {
		// for(i=d;i<s;i++)
			// arc[i].si++;
		// arc[s].si=d;
	// }
	// else{
		// for(i=s+1;i<d;i++)
			// arc[i].si--;
		// arc[s].si=d;
	// }
	// for(i=0;i<arc.length;i++)
	// {
		// if(arc[i].si!=i)
		// {
			// for(j=i+1;j<arc.length;j++)
			// {
				// if(arc[j].si==i)
				// {
					// t=arc[i];
					// arc[i]=arc[j];
					// arc[j]=t;
					// break;
				// }
			// }
		// }
	// }
	
	// alert(window.JSON.stringify(arc));
	debug=document.getElementById("debug");
	debug.innerHTML=window.JSON.stringify(arc);
}
window.onload=function()
{
	for(i=0;i<arc.length;i++)
	{
		
		var tt=document.createTextNode(arc[i].date);
		var tx=document.createElement("input");
		tx.type="text";
		tx.value=arc[i].thing;
		
		var ut=document.createElement("div");
		ut.appendChild(tt);
		ut.appendChild(tx);
		ut.id=arc[i].date;
		ut.draggable="true";
		ut.addEventListener("dragstart",drag);
		ut.si=i;
		
		
		// ut.ondragover="allowDrop(event)";
		// ut.ondrop="drop(event)";
		
		// var fr=document.getElementsByTagName("body");
		// fr=fr[0];
		var fr=document.getElementById("list");
		fr.appendChild(ut);
		fr.addEventListener("dragover",allowDrop);
		fr.addEventListener("drop",drop);
	}
		var fr2=document.getElementById("list2");
		fr2.addEventListener("dragover",allowDrop);
		fr2.addEventListener("drop",drop);
		
	var ext=document.getElementById("ext");
	ext.ondragover="allowDrop(event)";
	ext.ondrop="drop(event)";
}