#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

int read_numbers_from_file(const char *filename, int **arr, int *n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Ошибка: не удалось открыть файл '%s'\n", filename);
        return -1;
    }

    int capacity = 10;
    *arr = (int*)malloc(capacity * sizeof(int));
    if (!*arr) {
        fclose(fp);
        fprintf(stderr, "Ошибка: недостаточно памяти\n");
        return -1;
    }

    int count = 0;
    int value;
    while (fscanf(fp, "%d", &value) == 1) {
        if (count >= capacity) {
            capacity *= 2;
            int *tmp = (int*)realloc(*arr, capacity * sizeof(int));
            if (!tmp) {
                free(*arr);
                fclose(fp);
                fprintf(stderr, "Ошибка: недостаточно памяти при чтении\n");
                return -1;
            }
            *arr = tmp;
        }
        (*arr)[count++] = value;
    }

    if (fclose(fp) != 0) {
        fprintf(stderr, "Ошибка: не удалось закрыть файл\n");
    }

    if (count == 0) {
        free(*arr);
        *arr = 0;
        fprintf(stderr, "Ошибка: файл пуст или не содержит чисел\n");
        return -1;
    }

    *n = count;
    return 0;
}

int read_numbers_from_stdin(int **arr, int *n) {
    int count;
    char buffer[256];

    printf("Введите количество чисел: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        fprintf(stderr, "Ошибка: не удалось прочитать ввод\n");
        return -1;
    }

    if (sscanf(buffer, "%d", &count) != 1 || count <= 0) {
        fprintf(stderr, "Ошибка: введите положительное целое число\n");
        return -1;
    }

    if (count > 1000000) {
        fprintf(stderr, "Ошибка: слишком много элементов (максимум 1,000,000)\n");
        return -1;
    }

    *arr = (int*)malloc(count * sizeof(int));
    if (!*arr) {
        fprintf(stderr, "Ошибка: недостаточно памяти\n");
        return -1;
    }

    printf("Введите %d целых чисел (через пробел или Enter):\n", count);
    int read_count = 0;
    while (read_count < count) {
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            free(*arr);
            fprintf(stderr, "Ошибка: не удалось прочитать ввод\n");
            return -1;
        }

        char *ptr = buffer;
        int num;
        int parsed;
        while (sscanf(ptr, "%d%n", &num, &parsed) == 1) {
            (*arr)[read_count++] = num;
            ptr += parsed;
            if (read_count >= count) break;
        }

        if (read_count < count) {
            printf("Введите ещё %d чисел: ", count - read_count);
        }
    }

    *n = count;
    return 0;
}

int write_numbers_to_file(const char *filename, int *arr, int n) {
    if (n == 0) {
        fprintf(stderr, "Ошибка: массив пуст, запись не выполнена\n");
        return -1;
    }

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Ошибка: не удалось создать файл '%s'\n", filename);
        return -1;
    }

    for (int i = 0; i < n; i++) {
        if (fprintf(fp, "%d\n", arr[i]) < 0) {
            fclose(fp);
            fprintf(stderr, "Ошибка: не удалось записать данные в файл\n");
            return -1;
        }
    }

    if (fclose(fp) != 0) {
        fprintf(stderr, "Ошибка: не удалось закрыть файл\n");
        return -1;
    }

    return 0;
}

void print_numbers(int *arr, int n) {
    if (n == 0) {
        printf("Массив пуст\n");
        return;
    }

    printf("Отсортированный массив:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
        if ((i + 1) % 20 == 0 && i + 1 < n) {
            printf("\n");
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int *arr = 0;
    int n = 0;

    if (argc == 1) {
        printf("=== Интерактивный режим ===\n");
        if (read_numbers_from_stdin(&arr, &n) != 0) {
            return 1;
        }
        binary_tree_sort(arr, n);
        print_numbers(arr, n);
    }
    else if (argc == 2) {
        printf("=== Чтение из файла '%s' ===\n", argv[1]);
        if (read_numbers_from_file(argv[1], &arr, &n) != 0) {
            return 1;
        }
        binary_tree_sort(arr, n);
        print_numbers(arr, n);
    }
    else if (argc == 3) {
        printf("=== Чтение из '%s', запись в '%s' ===\n", argv[1], argv[2]);
        if (read_numbers_from_file(argv[1], &arr, &n) != 0) {
            return 1;
        }
        binary_tree_sort(arr, n);
        if (write_numbers_to_file(argv[2], arr, n) != 0) {
            free(arr);
            return 1;
        }
        printf("Результат записан в файл '%s'\n", argv[2]);
        print_numbers(arr, n);
    }
    else {
        fprintf(stderr, "Использование:\n"
                        "  %s                   - интерактивный ввод с клавиатуры\n"
                        "  %s <входной_файл>    - чтение из файла, вывод на экран\n"
                        "  %s <входной> <выходной> - чтение из файла, запись в файл\n",
                argv[0], argv[0], argv[0]);
        return 1;
    }

    if (arr) free(arr);
    return 0;
}
