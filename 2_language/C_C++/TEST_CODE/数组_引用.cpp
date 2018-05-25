int main(int argc,char*argv[])
{
        char sbuf[1024];
        char (&b)[100]=*(char(*)[100])(sbuf+200);
        cout<<sizeof(sbuf)<<'\t'<<&sbuf<<endl;
        cout<<sizeof(b)<<'\t'<<&b<<endl;
        return 0;

}