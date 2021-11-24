extern int log_level;

void log_print(char* filename,const char* funcname, int line, char *fmt,...);

#define LOG_INFO(...) {if(log_level==1){log_print(__FILE__,__func__,__LINE__, __VA_ARGS__ );}}

#define LOG_DEBUG(...) {if(log_level==2){log_print(__FILE__,__func__,__LINE__, __VA_ARGS__ );}}

#define LOG_ERROR(...) {if(log_level==3){log_print(__FILE__,__func__,__LINE__, __VA_ARGS__ );}}

#define LOG_TRACE(...) {if(log_level==4){log_print(__FILE__,__func__,__LINE__, __VA_ARGS__ );}}