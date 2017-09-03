#include "clientside_for_frontend.h"

int init_sockfd(){
    int sfd =0, n=0, b;
    char rbuff[FILE_BUF_SIZE];

    struct sockaddr_in serv_addr;

    memset(rbuff, '0', sizeof(rbuff));
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);

    b=connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (b==-1) {
        perror("Connect");
        return -1;
    }

    return sfd;
}

char* client_send_json_file_and_get_uuid(char* filename){
    int sfd = init_sockfd();
    char sendbuffer[BUF_SIZE];
    int b; 
    char mode = 'f';
    write(sfd, &mode, sizeof(char));

    char* uuid_str = malloc(UUID_SIZE_FOR_STR);
    if((b = recv(sfd, uuid_str, UUID_SIZE_FOR_STR, 0)) < 0) {printf("EROR: Reciving UUID");}

    FILE *fp = fopen(filename, "rb");
    if(fp == NULL){
        perror("File");
        return "2";
    }

    while( (b = fread(sendbuffer, 1, sizeof(sendbuffer), fp))>0 ){
        send(sfd, sendbuffer, b, 0);
    }

    printf("UUID for the file->%s\n", uuid_str);
    printf("Use the UUID for requesting back the results\n");
    fclose(fp);
    sleep(1); 
    close(sfd);
    return uuid_str;
}

void client_get_result(char* uuid_str, char* output_filename){
    int sfd = init_sockfd();
    char file_exist_or_not;
    int b;
    char rbuff[BUF_SIZE];
    printf("Getting result from server\n");
    char mode = 'u';
    write(sfd, &mode, sizeof(char));

    //Sending UUID
    send(sfd, uuid_str, UUID_SIZE_FOR_STR, 0);
    printf("UUID sent\n");

    if((b = read(sfd, &file_exist_or_not, 1)) != 1)
        printf("ERROR:file exist or not flag not of lenght 1\n");

    if(file_exist_or_not == 'n'){
        printf("\n\nYour file is being processed. Check after some time....\n\n");
    }
    else if(file_exist_or_not == 't'){
        printf("Comensing file transfer\n");
        int tot=0, b;
        FILE* fp = fopen(output_filename, "wb");
        if(fp == NULL){
            perror("File");
        }   

        while( (b = recv(sfd, rbuff, sizeof(rbuff), 0))> 0 ) { 
            tot+=b;
            printf("Bytes recived -> %i\n", b); 
            fwrite(rbuff, 1, b, fp);
        }
        printf("Closing file\n");
        fclose(fp);

        sleep(1);
    }
    close(sfd);
}

int main(int argc, char** argv){
    printf("Starting client\n");
    char* mode;
    if(argc == 3){
        mode = argv[1];
        char* filename;
        filename = argv[2];
        printf("Filename ->%s\n", filename);
        if((strcmp(mode, "file")) == 0){
            client_send_json_file_and_get_uuid(filename);
        }
        else{
            printf("ERROR: Wrong Mode");
        }
    }
    else if(argc == 4){
        char* uuid_str;
        char* output_filename;
        mode = argv[1];
        uuid_str = argv[2];
        output_filename = argv[3];
        client_get_result(uuid_str, output_filename);
    }
    else{
        printf("ERROR: Use %s <mode> <input> <optional>\nWhere <mode> is either uuid or file, input is accordingly either json file or uuid\n", argv[0]);
        printf("If mode = uuid then <optional> is the txt filename in which result is to be stored\n");
    }

}
