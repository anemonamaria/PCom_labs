#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char* argv[])
{
    char* message;
    char* response;
    int sockfd;

    // Ex 1.1: GET dummy from main server
    char* host = "34.118.48.238";
    char* url = "/api/v1/dummy";

    sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(host, url, NULL, NULL, 0);
    printf("Message: %s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response: %s\n", response);



    // Ex 1.2: POST dummy and print response from main server
    char** bodyData = malloc(sizeof(char*) * 2);
    bodyData[0] = malloc(LINELEN);
    bodyData[1] = malloc(LINELEN);
    strcpy(bodyData[0], "key0=value0");
    strcpy(bodyData[1], "key1=value1");
    message = compute_post_request(host, url, "application/x-www-form-urlencoded", bodyData, 2, NULL, 0);

    // Ex 2: Login into main server
    memset(bodyData[0], 0, LINELEN);
    memset(bodyData[1], 0, LINELEN);
    strcpy(bodyData[0], "username=student");
    strcpy(bodyData[1], "password=student");
    message = compute_post_request(host, "/api/v1/auth/login", "application/x-www-form-urlencoded",
        bodyData, 2, NULL, 0);
    printf("Message %s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response: %s\n", response);


    char** cookies = malloc(sizeof(char*));
    cookies[0] = malloc(LINELEN);

    strcpy(cookies[0], "connect.sid=s%3AuGxYeVzQ0lfSvLTE1kk3QAMjuxsapy0v.oUqEYq9oth1xlYegIWo0eifGbTMe4KaGUg%2BDNBas%2F1k; Path=/; HttpOnly");



    // Ex 3: GET weather key from main server
    sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(host, "/api/v1/weather/key", NULL, cookies, 1);
    printf("Message: %s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response: %s\n", response);


    // Ex 5: POST weather data for verification to main server
   


    // Ex 6: Logout from main server
    sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(host, "/api/v1/auth/logout", NULL, cookies, 1);
    printf("Message: %s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response: %s\n", response);
   
    // // Ex 4: GET weather data from OpenWeather API
    // sockfd = open_connection("37.139.20.5", 80, AF_INET, SOCK_STREAM, 0);
    // message = compute_get_request("api.openweathermap.org", "/data/2.5/weather", "lat=10&lon=20&appid=", cookies, 1);
    // printf("Message: %s\n", message);
    // send_to_server(sockfd, message);
    // response = receive_from_server(sockfd);
    // printf("Response: %s\n", response);




    // BONUS: make the main server return "Already logged in!"

    // free the allocated data at the end!

    return 0;
}
