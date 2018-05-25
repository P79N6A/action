#ifundef INT_IP_TO_STRING
#define INT_IP_TO_STRING
        const char* ipstr(unsigned int ip,char *buf=0)
        {
                static char sbuf[128];
                if(buf==0)
                        buf=sbuf;
                unsigned char*b=(unsigned char*)&ip;
                sprintf(buf,"%d.%d.%d.%d",b[0],b[1],b[2],b[3]);
                return buf;
        }
#endif