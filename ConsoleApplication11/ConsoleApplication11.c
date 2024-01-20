#include <stdio.h>
#include <stdlib.h>
#include <locale.h>


typedef struct {
    float cementCto;
    float sandCto;
    float gravelCto;
} MaterialCosts;

typedef struct {
    float cementPrice;
    float sandPrice;
    float gravelPrice;
} MaterialPrices;

void getInput(float* length, float* width, float* height);
MaterialCosts selectMaterialQuality();
MaterialCosts readMaterialCosts(const char* filename);
MaterialPrices readPricesFromFile(const char* filename);
void displayPricesTable(const char* type, MaterialPrices prices);
void calculateMaterials(float length, float width, float height, MaterialCosts costs, float* cement, float* sand, float* gravel, float* totalCost);
void displayResults(float length, float width, float height, float cement, float sand, float gravel, float totalCost);
void writeToFile(float length, float width, float height, float cement, float sand, float gravel, float totalCost);





int main() {
    setlocale(LC_CTYPE, "RUS");

    printf("Студент: Козяр Кирилл Максимович Группа:бИСТ-231 Строительный калькулятор\n");
    printf("\n====================================\n");
    printf("  Добро пожаловать в калькулятор!  \n");
    printf("====================================\n\n");

    // Отображение таблицы цен
    MaterialPrices standardPrices = readPricesFromFile("standard_costs.txt");
    MaterialPrices normalPrices = readPricesFromFile("normal_costs.txt");
    MaterialPrices premiumPrices = readPricesFromFile("premium_costs.txt");

    printf("+------------------+------------------+\n");
    printf("|       Вид        |      Цены        |\n");
    displayPricesTable("Стандарт", standardPrices);
    displayPricesTable("Нормальное", normalPrices);
    displayPricesTable("Премиум", premiumPrices);
    printf("+------------------+------------------+\n");

    char choice;
    do {
        float length, width, height;
        getInput(&length, &width, &height);

        MaterialCosts costs = selectMaterialQuality();
        float cement, sand, gravel, totalCost;
        calculateMaterials(length, width, height, costs, &cement, &sand, &gravel, &totalCost);
        displayResults(length, width, height, cement, sand, gravel, totalCost);
        writeToFile(length, width, height, cement, sand, gravel, totalCost);

        printf("\nХотите выполнить еще один расчет? (y/n): \nДля того чтобы продолжить введите: y\nДля того чтобы завершить введите: n\n");
        scanf_s(" %c", &choice);
        while (getchar() != '\n'); // Очистка буфера ввода
    } while (choice == 'y' || choice == 'Y');

    printf("Программа завершена.\n");
    return 0;
}
void getInput(float* length, float* width, float* height) {
    int inputStatus;

    do {
        printf("Введите длину площадки (в метрах): ");
        inputStatus = scanf("%f", length);
        while (getchar() != '\n');
        if (inputStatus != 1)
            printf("Неверный ввод. Пожалуйста, введите число.\n");
    } while (inputStatus != 1);

    do {
        printf("Введите ширину площадки (в метрах): ");
        inputStatus = scanf("%f", width);
        while (getchar() != '\n');
        if (inputStatus != 1)
            printf("Неверный ввод. Пожалуйста, введите число.\n");
    } while (inputStatus != 1);

    do {
        printf("Введите высоту площадки (в метрах): ");
        inputStatus = scanf("%f", height);
        while (getchar() != '\n');
        if (inputStatus != 1)
            printf("Неверный ввод. Пожалуйста, введите число.\n");
    } while (inputStatus != 1);
}

MaterialCosts selectMaterialQuality() {
    MaterialCosts costs;
    int choice;

    printf("Выберите качество материалов:\n");
    printf("1 - Стандартное\n");
    printf("2 - Нормальное\n");
    printf("3 - Премиум\n");
    printf("Введите ваш выбор (1-3): ");
    scanf_s("%d", &choice);
    while (getchar() != '\n');

    switch (choice) {
    case 1:
        costs = readMaterialCosts("standard_costs.txt");
        break;
    case 2:
        costs = readMaterialCosts("normal_costs.txt");
        break;
    case 3:
        costs = readMaterialCosts("premium_costs.txt");
        break;
    default:
        printf("Неверный выбор. Используем стандартное качество.\n");
        costs = readMaterialCosts("standard_costs.txt");
    }

    return costs;
}

MaterialCosts readMaterialCosts(const char* filename) {
    FILE* file = fopen(filename, "r");
    MaterialCosts costs;

    if (!file) {
        fprintf(stderr, "Ошибка при открытии файла %s.\n", filename);
        exit(1);
    }

    if (fscanf_s(file, "%f %f %f", &costs.cementCto, &costs.sandCto, &costs.gravelCto) != 3) {
        fprintf(stderr, "Ошибка чтения из файла %s.\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return costs;
}

MaterialPrices readPricesFromFile(const char* filename) {
    MaterialPrices prices;
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Ошибка при открытии файла %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    if (fscanf_s(file, "%f %f %f", &prices.cementPrice, &prices.sandPrice, &prices.gravelPrice) != 3) {
        fprintf(stderr, "Ошибка чтения из файла %s.\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return prices;
}

void displayPricesTable(const char* type, MaterialPrices prices) {
    printf("| %-16s | Цемент = %-7.2f |\n", type, prices.cementPrice);
    printf("|                  | Песок  = %-7.2f |\n", prices.sandPrice);
    printf("|                  | Щебень = %-7.2f |\n", prices.gravelPrice);
}

void calculateMaterials(float length, float width, float height, MaterialCosts costs, float* cement, float* sand, float* gravel, float* totalCost) {
    const float cementRatio = 0.02; // Доля цемента
    const float sandRatio = 0.04;   // Доля песка
    const float gravelRatio = 0.08; // Доля щебня

    float area = length * width;
    float totalVolume = area * height;

    *cement = totalVolume * cementRatio;
    *sand = totalVolume * sandRatio;
    *gravel = totalVolume * gravelRatio;

    float CC = *cement * costs.cementCto; // Стоимость цемента
    float SC = *sand * costs.sandCto;     // Стоимость песка
    float GC = *gravel * costs.gravelCto; // Стоимость щебня

    *totalCost = CC + SC + GC;
}

void displayResults(float length, float width, float height, float cement, float sand, float gravel, float totalCost) {
    printf("\n====================================\n");
    printf("Для площадки размерами %.2f м x %.2f м x %.2f м:\n", length, width, height);
    printf("Необходимо цемента: %.2f кг\n", cement);
    printf("Необходимо песка: %.2f кг\n", sand);
    printf("Необходимо щебня: %.2f кг\n", gravel);
    printf("Общая стоимость всех материалов: %.2f рублей\n", totalCost);
    printf("====================================\n");
}

void writeToFile(float length, float width, float height, float cement, float sand, float gravel, float totalCost) {
    FILE* file = fopen("calculator_results.txt", "a");
    if (file == NULL) {
        printf("Ошибка при открытии файла для записи.\n");
        return;
    }

    fprintf(file, "Размеры площадки: %.2f м x %.2f м x %.2f м\n", length, width, height);
    fprintf(file, "Необходимо цемента: %.2f кг\n", cement);
    fprintf(file, "Необходимо песка: %.2f кг\n", sand);
    fprintf(file, "Необходимо щебня: %.2f кг\n", gravel);
    fprintf(file, "Общая стоимость всех материалов: %.2f рублей\n", totalCost);
    fprintf(file, "----------------------------------------\n");

    fclose(file);
}
