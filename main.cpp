#include <iostream>
#include "player.hpp" // ���������� ��� ����� Player
#include <locale.h>
#include <Windows.h>

int main() {
    // ��������� ������� Windows
    SetConsoleCP(1251);        // ��������� ������� �������� �����
    SetConsoleOutputCP(1251);  // ��������� ������� �������� ������
    setlocale(LC_ALL, "Russian"); // �����������

    // 1. ������� ������
    Player player;

    std::cout << "=== ������� ���: ������������ ===" << std::endl;

    // 2. ��������� ������ ����
    std::cout << "\n������ ���� (��� ������ ������ ���� 0):" << std::endl;
    player.displayGrid();

    // 3. ��������� �������
    std::cout << "\n��������� ������� � ����� (2,3), (5,5), (7,1):" << std::endl;
    player.addShip(2, 3);
    player.addShip(5, 5);
    player.addShip(7, 1);
    player.displayGrid();

    std::cout << "�������� �� ����: " << player.getShipsAlive() << " (������ �������� 3)" << std::endl;

    // 5. ��������� ��������
    std::cout << "\n�������� �� (2,3) - ������ ���� ���������:" << std::endl;
    player.fire(2, 3);

    std::cout << "\n�������� �� (0,0) - ������ ���� ������:" << std::endl;
    player.fire(0, 0);

    std::cout << "\n�������� �� (5,5) - ������ ���� ���������:" << std::endl;
    player.fire(5, 5);



    // 7. ��������� ��������� ����
    std::cout << "\n��������� ��������� ����:" << std::endl;
    player.displayGrid();
    std::cout << "�������� ��������: " << player.getShipsAlive() << std::endl;

    std::cout << "\n=== ������������ ��������� ===" << std::endl;

    return 0;
}