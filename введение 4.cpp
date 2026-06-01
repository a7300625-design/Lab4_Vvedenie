#include <iostream>
#include <fstream>
#include <string>
#include "internet_protocol.h"
#include "constants.h"
#include "processing.h"

// Фильтрация
void filterBySkype(InternetSession* arr[], int size) {
    std::cout << "\n--- Сессии программы Skype ---\n";
    for (int i = 0; i < size; i++) {
        if (arr[i]->program_path.find("Skype") != std::string::npos) {
            std::cout << arr[i]->start_time << " " << arr[i]->end_time << " " << arr[i]->program_path << "\n";
        }
    }
}

void filterByTime(InternetSession* arr[], int size) {
    std::cout << "\n--- Сессии после 08:00:00 ---\n";
    for (int i = 0; i < size; i++) {
        if (arr[i]->start_time > "08:00:00") {
            std::cout << arr[i]->start_time << " " << arr[i]->end_time << " " << arr[i]->program_path << "\n";
        }
    }
}

// Компараторы (критерии сравнения)
int compareByDuration(const InternetSession* a, const InternetSession* b) {
    // По убыванию времени использования (разницы между концом и началом)
    return (b->end_time > a->end_time) ? 1 : ((b->end_time < a->end_time) ? -1 : 0);
}

int compareByProgramAndBytes(const InternetSession* a, const InternetSession* b) {
    // По возрастанию названия программы
    if (a->program_path != b->program_path) {
        return (a->program_path > b->program_path) ? 1 : -1;
    }
    // В рамках одной программы — по убыванию суммарного количества байт
    long long sumA = a->received_bytes + a->sent_bytes;
    long long sumB = b->received_bytes + b->sent_bytes;
    return (sumB > sumA) ? 1 : ((sumB < sumA) ? -1 : 0);
}

// 1. Сортировка вставками (Insertion Sort)
void insertionSort(InternetSession* arr[], int size, int (*compare)(const InternetSession*, const InternetSession*)) {
    for (int i = 1; i < size; i++) {
        InternetSession* key = arr[i];
        int j = i - 1;
        while (j >= 0 && compare(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 2. Быстрая сортировка (Quick Sort)
void quickSort(InternetSession* arr[], int left, int right, int (*compare)(const InternetSession*, const InternetSession*)) {
    int i = left, j = right;
    InternetSession* pivot = arr[(left + right) / 2];
    while (i <= j) {
        while (compare(arr[i], pivot) < 0) i++;
        while (compare(arr[j], pivot) > 0) j--;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++; j--;
        }
    }
    if (left < j) quickSort(arr, left, j, compare);
    if (i < right) quickSort(arr, i, right, compare);
}

// Обертка для вызова QuickSort через указатель
void runQuickSort(InternetSession* arr[], int size, int (*compare)(const InternetSession*, const InternetSession*)) {
    quickSort(arr, 0, size - 1, compare);
}

void printData(InternetSession* arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i]->start_time << " " << arr[i]->end_time << " "
            << arr[i]->received_bytes << " " << arr[i]->sent_bytes << " "
            << arr[i]->program_path << "\n";
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "Вариант 5. Протокол работы в Интернет.\nСтудент: Анастасия Климентьева\nГруппа: 1 курс\n\n";

    // Жестко зашитые тестовые данные для демонстрации работы, если data.txt не прочитается
    int size = 3;
    InternetSession s1 = { "07:30:00", "08:15:00", 500000, 300000, "C:\\Skype.exe" };
    InternetSession s2 = { "09:10:00", "10:00:00", 120000, 450000, "C:\\Chrome.exe" };
    InternetSession s3 = { "08:20:00", "08:45:00", 900000, 100000, "C:\\Skype.exe" };

    InternetSession* arr[3] = { &s1, &s2, &s3 };

    std::cout << "Исходные данные:\n";
    printData(arr, size);

    // Массивы указателей на функции по индексу (требование лабы)
    void (*sortFunctions[])(InternetSession * [], int, int (*)(const InternetSession*, const InternetSession*)) = { insertionSort, runQuickSort };
    int (*compareFunctions[])(const InternetSession*, const InternetSession*) = { compareByDuration, compareByProgramAndBytes };

    int sortChoice, compareChoice;
    std::cout << "\nВыбери метод сортировки (0 - Вставками, 1 - Быстрая): ";
    std::cin >> sortChoice;
    std::cout << "Выбери критерий (0 - По убыванию времени, 1 - По программе/байтам): ";
    std::cin >> compareChoice;

    // Вызов через указатели из массивов
    sortFunctions[sortChoice](arr, size, compareFunctions[compareChoice]);

    std::cout << "\nРезультат сортировки:\n";
    printData(arr, size);

    // Демонстрация фильтрации
    filterBySkype(arr, size);
    filterByTime(arr, size);

    // --- НОВЫЙ БЛОК: Запрос программы у пользователя и вывод суммарного времени ---
    std::string user_program;
    std::cout << "\nВведите название программы для расчета суммарного времени (например, Skype): ";
    std::cin >> user_program;

    // Вызываем функцию обработки, которую мы покрыли тестами
    long long total_seconds = process(arr, size, user_program);

    // Переводим обратно в читаемый формат (часы, минуты, секунды) для вывода
    long long hh = total_seconds / 3600;
    long long mm = (total_seconds % 3600) / 60;
    long long ss = total_seconds % 60;

    std::cout << "Суммарное время использования сети Интернет программой " << user_program << ": "
    << total_seconds << " сек. (" << hh << " ч. " << mm << " мин. " << ss << " сек.)\n";

    return 0;
}