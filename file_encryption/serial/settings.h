
#ifndef SSC_SETTINGSHF
#define SSC_SETTINGSHF

/* some macros for compatibility */
#define cmalloc(x) calloc(1,x)
#define cfree(x) free(x)
#define mitbase64_decode(x,y,z) base64_decode(x,y,z)
#define mitbase64_encode(x,y,z) base64_encode(x,y,z)

#if defined(DEBUG) && defined(RELEASE_IMAGE) 
	#error You cannot have debug enabled in a release build. 
#endif /* DEBUG && RELEASE_IMAGE */

#if defined(DEBUG) && defined(SSC_FUNCTION_LOG)
	#define debuginfo() cfunction_info()
	#define debugprint() cfunction_info()
#else
	#define debuginfo() cempty_function()
	#define debugprint() cempty_function()
#endif /* DEBUG && SSC_FUNCTION_LOG */

#endif
