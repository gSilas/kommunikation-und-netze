/* echoclient.c */

#include "webserver.h"
#include "include/cnaiapi.h"
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stropts.h>
#include <unistd.h>

appnum port;
connection conn;
char buffer[1000];
char inbuffer[1000];

char *requestedFile = "";
char *fileHeader = "htdocs";
const char *metaType =
    "<!DOCTYPE html>\n<head>\n<meta charset=\"utf-8\">\n</head>\n";
char finalFile[40];

const char s[3] = "\n\r";
const char s2[2] = " ";
char *token;
char *tok[10000];

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("usage: %s port \n", argv[0]);
    return 0;
  }

  else {
    port = atoi(argv[1]);

    if (port == -1)
      printf("-> port failed\n");
    else
      printf("-> port recognized\n");
  }

  size_t outsize;
  size_t insize;

  while (1) {

    conn = await_contact(port);

    if (conn != -1) {
      printf("-> client connection on socket!\n");
    }

    insize = read(conn, inbuffer, sizeof(inbuffer));

    if (insize > 0) {
      getParsedRequest();
      printf("-> parsed request\n");
      findFilePath();
      printf("-> requested file found\n");
      printf("-> requested file: %s\n", finalFile);
      outsize = fread(buffer, 1, sizeof(buffer), fopen(finalFile, "r"));
      write(conn, metaType, strlen(metaType));
      write(conn, buffer, outsize);
    }

    end_contact(conn);
    token = '\0';
    finalFile[0] = '\0';
    requestedFile = '\0';
  }
  return 0;
}

void getParsedRequest() {
  /* get the first token */
  int index = 0;
  token = strtok(inbuffer, s);

  /* walk through other tokens and save them*/
  while (token != NULL) {
    printf(" %s\n", token);
    tok[index] = token;
    token = strtok(NULL, s);
    index++;
  }
}

/*find GET and tok the file path */
void findFilePath() {
  for (int i = 0; i < (sizeof(tok) - 1); i++) {
    if ((requestedFile = strstr(tok[i], "GET")) != NULL) {
      strtok(requestedFile, s2);
      requestedFile = strtok(NULL, s2);
      strcat(finalFile, fileHeader);
      strcat(finalFile, requestedFile);
      break;
    }
  }
}
