#include "clientside_for_frontend.h"

char* client_send_json_file_and_get_uuid(char* filename){
    int sfd =0, n=0, b;
    char rbuff[FILE_BUF_SIZE];
    char sendbuffer[BUF_SIZE];

    struct sockaddr_in serv_addr;

    memset(rbuff, '0', sizeof(rbuff));
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);

    b=connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (b==-1) {
        perror("Connect");
        return "1";
    }

    FILE *fp = fopen(filename, "rb");
    if(fp == NULL){
        perror("File");
        return "2";
    }

    while( (b = fread(sendbuffer, 1, sizeof(sendbuffer), fp))>0 ){
        send(sfd, sendbuffer, b, 0);
    }

    fclose(fp);
    char* temp = "Return";
    
    return temp;
}

int main(int argc, char** argv){
    printf("Starting client\n");
    char* filename;
    if(argc == 2){
        filename = argv[1];
        printf("Filename ->%s\n", filename);
    }
    else{
        printf("ERROR: Filename error\n");
    }

    client_send_json_file_and_get_uuid(filename);
}
