
#ifndef COMMON_PP_COMMON_PP_FUN_H_
#define COMMON_PP_COMMON_PP_FUN_H_
#include<string.h>
static void* memncpy(void*dest,size_t dest_size,const void*src,size_t n)
{
	if(dest_size<n)
	{
		return memcpy(dest,src,dest_size);
	}
	else
	{
		return memcpy(dest,src,n);
	}
}

#endif /* COMMON_PP_COMMON_PP_FUN_H_ */
