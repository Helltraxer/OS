#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

pthread_mutex_t mutex;

// Функция для записи данных в файл в формате JSON
void write_json(FILE* file, const char *data, long thread_id){
	fprintf(file, "{\"thread_id\": %ld, \"data\": \"%s\"}\n", thread_id, data);
}

// Функция для записи данных в файл в формате CSV
void write_csv(FILE* file, const char *data, long thread_id){
	fprintf(file, "%ld, %s\n", thread_id, data);
}

void* write_to_file(void*arg){
	char* filename = (char*)arg;
	FILE* file;

	pthread_mutex_lock(&mutex);
	file = fopen(filename, "a");
	if(file == NULL){
		fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", filename);
		pthread_mutex_unlock(&mutex);
		return NULL;
	}

	char data[100];
	printf("Введите данные для потока %ld: ", pthread_self());
	scanf("%99s", data); // Предполагаем, что пользователь вводит строку без пробелов
	
	char format[10];
	printf("Выберите формат (json/csv): ");
	scanf("%9s", format);

	if(strcmp(format, "json") == 0){
		write_json(file, data, pthread_self());
	} else if(strcmp(format, "csv") == 0){
		write_csv(file, data, pthread_self());
	} else {
		fprintf(stderr, "Ошибка: неправильный формат записи\n");
	}

	fclose(file);
	pthread_mutex_unlock(&mutex);

	return NULL;
}

int main(){
	int num_threads;
	printf("Введите количество потоков: ");
	scanf("%d", &num_threads);

	pthread_t threads[num_threads];
	char filename[100];

	printf("Введите название файла (с .txt): ");
	scanf("%99s", filename);

	pthread_mutex_init(&mutex, NULL);

	for(int i = 0; i < num_threads; i++){
		pthread_create(&threads[i], NULL, write_to_file, filename);
	}

	for(int i = 0; i < num_threads; i++){
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	printf("Данные были успешно записаны в %s\n", filename);

	return 0;
}
