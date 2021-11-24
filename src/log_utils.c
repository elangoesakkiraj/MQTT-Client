#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "log_utils.h"
FILE *fp ;

char* print_time()
{
    int size = 0;
    time_t t;
    char *buf;
    
    t=time(NULL); /* get current calendar time */
    
    char *timestr = asctime( localtime(&t) );
    timestr[strlen(timestr) - 1] = 0;  //Getting rid of \n
    
    size = strlen(timestr)+ 1 + 2; //Additional +2 for square braces
    buf = (char*)malloc(size);
    
    memset(buf, 0x0, size);
    snprintf(buf,size,"[%s]", timestr);
   
    return buf;
}
void log_print(char* filename,const char* funcname, int line, char *fmt,...)
{
    va_list         list;
    char            *p, *r;
    int             e;

    fp = fopen ("../log/client.log","a+");
    
    fprintf(fp,"%s ",print_time());
    fprintf(fp,"[%s][func: %s][line: %d] ",filename,funcname,line);
    va_start( list, fmt );

    for ( p = fmt ; *p ; ++p )
    {
        if ( *p != '%' )//If simple string
        {
            fputc( *p,fp );
        }
        else
        {
            switch ( *++p )
            {
                /* string */
            case 's':
            {
                r = va_arg( list, char * );

                fprintf(fp,"%s", r);
                continue;
            }

            /* integer */
            case 'd':
            {
                e = va_arg( list, int );

                fprintf(fp,"%d", e);
                continue;
            }

            default:
                fputc( *p, fp );
            }
        }
    }
    va_end( list );
    fputc( '\n', fp );
    fclose(fp);
}
