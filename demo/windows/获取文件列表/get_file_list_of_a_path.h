//
//
	void get_filelist_of_path(const char*path,vector<string>&full_file_list,vector<string>&file_list)
	{
		string path_nofilter(path);
		while(path_nofilter.back()!='\\'&&path_nofilter.back()!='/')
		{
			path_nofilter.pop_back();
		}

		intptr_t lf;_finddata_t file;
		if((lf=_findfirst(path,&file))==-1) return;
		do 
		{
			if(!(strcmp(file.name,".")==0)
				&&!(strcmp(file.name,"..")==0))
			{
				full_file_list.push_back(path_nofilter+string(file.name));
				file_list.push_back(file.name);
			}
		} while (_findnext(lf,&file)==0);
		_findclose(lf);
	}
//
// 1 void getAllFiles( string path, vector<string>& files)  
// 2 {  
// 3     //文件句柄  
// 4     long   hFile   =   0;  
// 5     //文件信息  
// 6     struct _finddata_t fileinfo;  
// 7     string p;  
// 8     if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
// 9     {  
//10         do  
//11         {   
//12             if((fileinfo.attrib &  _A_SUBDIR))  
//13             {  
//14                 if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
//15                 {
//16                     files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
//17                     getFilesall( p.assign(path).append("\\").append(fileinfo.name), files ); 
//18                 }
//19             }  
//20             else  
//21             {  
//22                 files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
//23             }  
//24         }while(_findnext(hFile, &fileinfo)  == 0);  
//25         _findclose(hFile); 
//26     } 
//27 }