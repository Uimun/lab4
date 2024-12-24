#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5 // ������ ũ��

int buffer[BUFFER_SIZE]; // ���� ����
int count = 0;           // ���ۿ� �ִ� �׸� ��
int in = 0;             // ���ۿ� �׸��� �߰��� ��ġ
int out = 0;            // ���ۿ��� �׸��� ������ ��ġ

pthread_mutex_t mutex; // ���ؽ�
pthread_cond_t not_empty; // ���۰� ��� ���� ������ ��Ÿ���� ���� ����
pthread_cond_t not_full;  // ���۰� ���� ���� ������ ��Ÿ���� ���� ����

void* producer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % 100; // ������ �׸�
        
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            // ���۰� ���� á�ٸ� ��ٸ�
            pthread_cond_wait(&not_full, &mutex);
        }

        // �׸� �߰�
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("Producer %d: Produced %d\n", id, item);

        pthread_cond_signal(&not_empty); // �Һ��ڿ��� ��ȣ
        pthread_mutex_unlock(&mutex);

        sleep(1); // ���� ����
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            // ���۰� ��� �ִٸ� ��ٸ�
            pthread_cond_wait(&not_empty, &mutex);
        }

        // �׸� ����
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("Consumer %d: Consumed %d\n", id, item);

        pthread_cond_signal(&not_full); // �����ڿ��� ��ȣ
        pthread_mutex_unlock(&mutex);

        sleep(1); // �Һ� ����
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

    // ������ ������ ����
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&producers[i], NULL, producer, &producer_ids[i])) {
            fprintf(stderr, "Error creating producer thread %d\n", i + 1);
            return 1;
        }
    }

    // �Һ��� ������ ����
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i])) {
            fprintf(stderr, "Error creating consumer thread %d\n", i + 1);
            return 1;
        }
    }

    // ������ ���� ���
    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return 0;
}
