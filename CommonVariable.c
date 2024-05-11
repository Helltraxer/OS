#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define NUM_THREADS 5 // Количество потоков

//Общая переменная и мьютекс
int shared_variable = 0;
pthread_mutex_t mutex;

// Функция, выполняемая потоками
void* increment_variable(void* arg){
	int increment = *(int*) arg;

	// Захват мьютекса перед изменением переменной
	int ret;
	ret = pthread_mutex_lock(&mutex);
	if(ret != 0){
		fprintf(stderr, "Не удалось захватить мьютекс: %s\n", strerror(ret));
		return NULL;
	}
	shared_variable += increment;
	ret = pthread_mutex_unlock(&mutex);
	if(ret != 0){
		fprintf(stderr, "Не удалось освободить мьютекс: %s\n", strerror(ret));
	}

	return NULL;
}

int main(){
	pthread_t threads[NUM_THREADS];
	int increments[NUM_THREADS] = {10, 20, 30 , 40, 50};

	// Инициализация мьютекса
	int ret = pthread_mutex_init(&mutex, NULL);
	if(ret != 0){
		fprintf(stderr, "Не удалось инициализировать мьютекс: %s\n", strerror(ret));
		return EXIT_FAILURE;
	}

	// Создание потоков
	for(int i = 0; i < NUM_THREADS; i++){
		ret = pthread_create(&threads[i], NULL, increment_variable, &increments[i]);
		if(ret != 0){
			fprintf(stderr, "Не удалось создать поток %d: %s\n", i, strerror(ret));
			// Освобождение ресурсов в случае ошибки
			for(int j = 0; j < i; j++){
				pthread_join(threads[i], NULL);
			}
			pthread_mutex_destroy(&mutex);
			return EXIT_FAILURE;
		}
	}

	// Ожидание завершения потоков
	for(int i = 0; i < NUM_THREADS; i++){
		ret = pthread_join(threads[i], NULL);
		if(ret !=0){
			fprintf(stderr, "Не удалось соединиться с потоком %d: %s\n", i, strerror(ret));
		}
	}

	//Вывод результата
	printf("Окончательное значение общей переменной: %d\n", shared_variable);

	return EXIT_SUCCESS;
}
