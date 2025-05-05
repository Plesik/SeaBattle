#include <iostream>
#include "player.hpp" // Подключаем наш класс Player
#include <locale.h>
#include <Windows.h>

int main() {
    // Настройка консоли Windows
    SetConsoleCP(1251);        // Установка кодовой страницы ввода
    SetConsoleOutputCP(1251);  // Установка кодовой страницы вывода
    setlocale(LC_ALL, "Russian"); // Локализация

    // 1. Создаем игрока
    Player player;

    std::cout << "=== Морской бой: тестирование ===" << std::endl;

    // 2. Тестируем пустое поле
    std::cout << "\nПустое поле (все клетки должны быть 0):" << std::endl;
    player.displayGrid();

    // 3. Добавляем корабли
    std::cout << "\nДобавляем корабли в точки (2,3), (5,5), (7,1):" << std::endl;
    player.addShip(2, 3);
    player.addShip(5, 5);
    player.addShip(7, 1);
    player.displayGrid();

    std::cout << "Кораблей на поле: " << player.getShipsAlive() << " (должно остаться 3)" << std::endl;

    // 5. Тестируем выстрелы
    std::cout << "\nСтреляем по (2,3) - должно быть попадание:" << std::endl;
    player.fire(2, 3);

    std::cout << "\nСтреляем по (0,0) - должно быть промах:" << std::endl;
    player.fire(0, 0);

    std::cout << "\nСтреляем по (5,5) - должно быть попадание:" << std::endl;
    player.fire(5, 5);



    // 7. Финальное состояние поля
    std::cout << "\nФинальное состояние поля:" << std::endl;
    player.displayGrid();
    std::cout << "Кораблей осталось: " << player.getShipsAlive() << std::endl;

    std::cout << "\n=== Тестирование завершено ===" << std::endl;

    return 0;
}