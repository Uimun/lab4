#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5 // 버퍼의 크기

int buffer[BUFFER_SIZE]; // 제한 버퍼
int count = 0;           // 버퍼에 있는 항목 수
int in = 0;             // 버퍼에 항목을 추가할 위치
int out = 0;            // 버퍼에서 항목을 제거할 위치

pthread_mutex_t mutex; // 뮤텍스
pthread_cond_t not_empty; // 버퍼가 비어 있지 않음을 나타내는 조건 변수
pthread_cond_t not_full;  // 버퍼가 가득 차지 않음을 나타내는 조건 변수

void* producer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % 100; // 생성할 항목
        
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            // 버퍼가 가득 찼다면 기다림
            pthread_cond_wait(&not_full, &mutex);
        }

        // 항목 추가
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("Producer %d: Produced %d\n", id, item);

        pthread_cond_signal(&not_empty); // 소비자에게 신호
        pthread_mutex_unlock(&mutex);

        sleep(1); // 생산 간격
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            // 버퍼가 비어 있다면 기다림
            pthread_cond_wait(&not_empty, &mutex);
        }

        // 항목 제거
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("Consumer %d: Consumed %d\n", id, item);

        pthread_cond_signal(&not_full); // 생산자에게 신호
        pthread_mutex_unlock(&mutex);

        sleep(1); // 소비 간격
    }
    return NULL;
}

int main() {
    pthread_t producers[2], consumers[2];
    int producer_ids[2] = {1, 2};
    int consumer_ids[2] = {1, 2};

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    // 생산자 스레드 생성
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&producers[i], NULL, producer, &producer_ids[i])) {
            fprintf(stderr, "Error creating producer thread %d\n", i + 1);
            return 1;
        }
    }

    // 소비자 스레드 생성
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i])) {
            fprintf(stderr, "Error creating consumer thread %d\n", i + 1);
            return 1;
        }
    }

    // 스레드 종료 대기
    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return 0;
}
