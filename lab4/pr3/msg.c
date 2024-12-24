#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CLIENTS 5       // �ִ� Ŭ���̾�Ʈ ��
#define MESSAGE_SIZE 256    // �޽��� ũ��

// ���� �ڿ�
char message[MESSAGE_SIZE];
int message_ready = 0; // �޽����� �غ�Ǿ����� ����

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // ���ؽ�
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   // ���� ����

// ����(�θ�) ������
void *server_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // �޽����� �غ�� ������ ���
        while (!message_ready) {
            pthread_cond_wait(&cond, &mutex);
        }

        // �޽��� ���
        printf("Server broadcasting message: %s\n", message);
        message_ready = 0; // �޽��� ���� �ʱ�ȭ

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Ŭ���̾�Ʈ(�ڽ�) ������
void *client_thread(void *arg) {
    int client_id = *(int *)arg;
    char client_message[MESSAGE_SIZE];

    while (1) {
        // Ŭ���̾�Ʈ �޽��� ����
        snprintf(client_message, MESSAGE_SIZE, "Hello from client %d!", client_id);
        sleep(rand() % 3 + 1); // ���� ��� �ð�

        pthread_mutex_lock(&mutex);

        // ������ �޽��� ����
        strncpy(message, client_message, MESSAGE_SIZE);
        message_ready = 1;
        printf("Client %d sent message: %s\n", client_id, client_message);

        pthread_cond_signal(&cond); // ���� �����忡 ��ȣ
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t server;
    pthread_t clients[MAX_CLIENTS];
    int client_ids[MAX_CLIENTS];

    // ���� ������ ����
    pthread_create(&server, NULL, server_thread, NULL);

    // Ŭ���̾�Ʈ ������ ����
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        client_ids[i] = i + 1;
        pthread_create(&clients[i], NULL, client_thread, &client_ids[i]);
    }

    // ���� ������� Ŭ���̾�Ʈ ���Ḧ ��ٸ��� ����(���� ����)
    pthread_join(server, NULL);

    return 0;
}
