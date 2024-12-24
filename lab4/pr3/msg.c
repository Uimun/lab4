#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CLIENTS 5       // 최대 클라이언트 수
#define MESSAGE_SIZE 256    // 메시지 크기

// 공유 자원
char message[MESSAGE_SIZE];
int message_ready = 0; // 메시지가 준비되었는지 여부

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 뮤텍스
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   // 조건 변수

// 서버(부모) 쓰레드
void *server_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // 메시지가 준비될 때까지 대기
        while (!message_ready) {
            pthread_cond_wait(&cond, &mutex);
        }

        // 메시지 방송
        printf("Server broadcasting message: %s\n", message);
        message_ready = 0; // 메시지 상태 초기화

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// 클라이언트(자식) 쓰레드
void *client_thread(void *arg) {
    int client_id = *(int *)arg;
    char client_message[MESSAGE_SIZE];

    while (1) {
        // 클라이언트 메시지 생성
        snprintf(client_message, MESSAGE_SIZE, "Hello from client %d!", client_id);
        sleep(rand() % 3 + 1); // 랜덤 대기 시간

        pthread_mutex_lock(&mutex);

        // 서버에 메시지 전달
        strncpy(message, client_message, MESSAGE_SIZE);
        message_ready = 1;
        printf("Client %d sent message: %s\n", client_id, client_message);

        pthread_cond_signal(&cond); // 서버 쓰레드에 신호
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t server;
    pthread_t clients[MAX_CLIENTS];
    int client_ids[MAX_CLIENTS];

    // 서버 쓰레드 생성
    pthread_create(&server, NULL, server_thread, NULL);

    // 클라이언트 쓰레드 생성
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        client_ids[i] = i + 1;
        pthread_create(&clients[i], NULL, client_thread, &client_ids[i]);
    }

    // 메인 쓰레드는 클라이언트 종료를 기다리지 않음(무한 실행)
    pthread_join(server, NULL);

    return 0;
}
