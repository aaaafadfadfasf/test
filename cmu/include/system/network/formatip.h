#ifndef _FORMAT_IP_H_
#define _FORMAT_IP_H_

//usage: printf( "%d.%d.%d.%d",FORMAT_IP(dwIpAddr));

#define FORMAT_IP(addr) \
    (int)(((unsigned char *)&addr)[0]), \
    (int)(((unsigned char *)&addr)[1]), \
    (int)(((unsigned char *)&addr)[2]), \
    (int)(((unsigned char *)&addr)[3])
    
#endif

////end of file


