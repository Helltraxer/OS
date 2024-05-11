#include <stdio.h>
#include <stdbool.h>

#define P 3 // Количество процессов
#define R 2 // Количество типов ресурсов

// Функция для проверки возможности выделения ресурсов
bool isSafe(int avialable[], int allocation[][R], int need[][R]){
	bool finish[P] = {0};
	int work[R];
	for(int i = 0; i < R; i++){
		work[i] = avialable[i];
	}

	while(true){
		int found = false;
		for(int p = 0; p < P; p++){
			if(!finish[p]){
				int j;
				for(j = 0; j < R; j++)
					if(need[p][j] > work[j])
						break;
				if(j == R){
					for(int k = 0; k < R; k++)
						work[k] += allocation[p][k];
					finish[p] = true;
					found - true;
				}
			}
		}
		if(!found)
			break;
	}
	for(int i = 0; i < P; i++)
		if(!finish[i])
			return false;
	return true;
}

bool requestResources(int pid, int request[], int avialable[], int allocation[][R], int need[][R]){
	for(int i = 0; i < R; i++){
		if(request[i] > need[pid][i] || request[i] > avialable[i]){
			printf("Запрос не может быть удовлетворён.\n");
			return false;
		}
	}

	for(int i = 0; i < R; i++){
		avialable[i] -= request[i];
		allocation[pid][i] += request[i];
		need[pid][i] -= request[i];
	}

	if(!isSafe(avialable, allocation, need)){
		for(int i = 0; i < R; i++){
			avialable[i] += request[i];
			allocation[pid][i] -= request[i];
			need[pid][i] += request[i];
		}
		printf("Запрос приводит к небезопасному состоянию, откат изменений.\n");
		return false;
	}

	return true;
}

int main(){
	int avialable[R] = {10, 10}; // Доступные ресурсы
	int max[P][R] = { {7,5}, {3,2}, {9,0} }; // Максимальные требования каждого процесса
	int allocation[P][R] = { {1,1}, {2,0}, {3,0} }; // Текущее выделение ресурсов
	int need[P][R];

	// Расчёт требуемых ресурсов
	for(int i = 0; i < P; i++)
		for(int j = 0; j < R; j++)
			need[i][j] = max[i][j] - allocation[i][j];

	int requests[][R] = { {1,0}, {0,2}, {3,0}}; //Пример запросов
	for(int i = 0; i < 3; i++){
		printf("Обработка запроса %d: ", i + 1);
		if(requestResources(i % P, requests[i], avialable, allocation, need))
			printf("Запрос удовлетворен, система в безопасном состоянии.\n");
		else
			printf("Запрос не удовлетворён\n");
	}

	return 0;
}
