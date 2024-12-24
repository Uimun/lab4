#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ������ �Լ� ����
void* print_message_function(void* ptr) {
    char* message = (char*)ptr;
    for (int i = 0; i < 5; i++) {
        printf("%s (iteration %d)\n", message, i + 1);
        sleep(1); // 1�� ���
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2; // ������ �ĺ���
    const char* message1 = "Thread 1: Hello!";
    const char* message2 = "Thread 2: World!";

    // ù ��° ������ ����
    if (pthread_create(&thread1, NULL, print_message_function, (void*)message1)) {
        fprintf(stderr, "Error creating thread 1\n");
        return 1;
    }

    // �� ��° ������ ����
    if (pthread_create(&thread2, NULL, print_message_function, (void*)message2)) {
        fprintf(stderr, "Error creating thread 2\n");
        return 1;
    }

    // ������ ���� ���
    if (pthread_join(thread1, NULL)) {
        fprintf(stderr, "Error joining thread 1\n");
        return 1;
    }

    if (pthread_join(thread2, NULL)) {
        fprintf(stderr, "Error joining thread 2\n");
        return 1;
    }

    printf("Both threads finished execution.\n");
    return 0;
}
