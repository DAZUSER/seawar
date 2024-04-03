#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <ctime>

std::vector<int> ships = { 1, 2, 3, 4, 5 };

void print_ships(std::vector<int> ships)
{
    for (int i : ships)
    {
        std::cout << i << " ";
    }
}

struct Platform {
    static void Init()
    { 
        setlocale(LC_ALL, "rus");
    }
    static void CleanScreen() 
    {
        system("cls");
    }
    static void Sleep(int s)
    {
        ::Sleep(s);
    }
};

//возможные поля на поле
enum class Typesign
{
    Start,      // ' '
    BowLeft,    // Левое направление
    BowDown,    // Нижнее направление
    BowUp,      // Верхнее нарпавление
    BowRight,   // Правое направление
    Miss,       // Промах
    Hit,        // Попадания
    Ship,       // Часть корабля
    Opponent_see,   // поле для протвника
};

//вывод полей на поле
std::ostream& operator<<(std::ostream& os, const Typesign& c)
{
    switch (c) {
    case Typesign::Start: os << '-'; break;
    case Typesign::BowLeft: os << 'L'; break;
    case Typesign::BowDown: os << 'D'; break;
    case Typesign::BowUp: os << 'U'; break;
    case Typesign::BowRight: os << 'R'; break;
    case Typesign::Miss: os << 'M'; break;
    case Typesign::Hit: os << 'H'; break;
    case Typesign::Ship: os << '1'; break;
    case Typesign::Opponent_see: os << '*'; break;
    }
    return os;
}

//корабли от 1 до 5 палуб
//enum class Ships
//{
//    Ship1,
//    Ship2,
//    Ship3,
//    Ship4,
//    Ship5,
//};

//std::ostream& operator<<(std::ostream& os, const Ships& c)
//{
//    switch (c) {
//    case Ships::Ship1: os << '1'; break;
//    case Ships::Ship2: os << '2'; break;
//    case Ships::Ship3: os << '3'; break;
//    case Ships::Ship4: os << '4'; break;
//    case Ships::Ship5: os << '5';
//    }
//    return os;
//}
//структура точек

//struct Point {
//    int x;
//    int y;
//    void print_point(Point point)
//    {
//        std::cout << point.x << " " << point.y;
//    }
//};

//игровое поля для игроков
class Field {
    static const int height = 10;
    static const int weidht = 10;
    Typesign field[height][weidht];
public:
    Typesign get_sign(int x, int y)                         // смотреть конкретную ячейку(используется для проверки на попадание)
    {
        return field[x][y];
    }
    void get_field()                         // вывод поля
    {
        for (int x = 0; x < height; ++x)
        {
            std::cout << "\n";
            for (int y = 0; y < weidht; ++y)
                std::cout << field[x][y] << " ";
        }
    }
    void change_sign(int x, int y, Typesign z)              // изменять ячейку(используется для заполнения)
    {
        field[x][y] = z;
    }
    void fill(Typesign z)
    {
        for (int x = 0; x < height; ++x)
            for (int y = 0; y < weidht; ++y)
                field[x][y] = z;                           
    }
    void fill_ships(int l, int x, int y, char direction) 
    {
        switch (direction)
        {
        case 'U':
            for (int i = x; i < x + l; ++i)
                field[i][y] = Typesign::Ship;
            break;
        case 'D':
            for (int i = x; i > x - l; --i)
                field[i][y] = Typesign::Ship;
            break;
        case 'L':
            for (int i = y; i < y + l; ++i)
                field[x][i] = Typesign::Ship;
            break;
        case 'R':
            for (int i = y; i > y - l; --i)
                field[x][i] = Typesign::Ship;
            break;

        }
    }
    int get_height() 
    {
        int k = height;
        return k;
    }
    int get_weidht()
    {
        int k = weidht;
        return k;
    }
    int check_fields(int l, int x, int y, char direction) 
    {
        switch (direction)
        {
        case 'U':
            if (x + l > weidht) 
                return 0;
            if (x != 0 && field[x - 1][y] != Typesign::Start)
                return 0;
            for (int i = x; i <= x + l; ++i)
                if (field[i][y] != Typesign::Start)
                    return 0;
            break;
        case 'D':
            if (x - l + 1 < 0)
                return 0;
            if (x != 0 && field[x + 1][y] != Typesign::Start)
                return 0;
            for (int i = x; i >= x - l; --i)
                if (field[i][y] != Typesign::Start)
                    return 0;
            break;
        case 'L':
            if (y != 0 && field[x][y - 1] != Typesign::Start)
                return 0;
            if (y + l > height)
                return 0;
            for (int i = y; i <= y + l; ++i)
                if (field[x][i] != Typesign::Start)
                    return 0;
            break;
        case 'R':
            if (y != 0 && field[x][y + 1] != Typesign::Start)
                return 0;
            if (y - l + 1 < 0)
                return 0;
            for (int i = y; i >= y - l; --i)
                if (field[x][i] != Typesign::Start)
                    return 0;
            break;

        }
        return 1;
    }
    bool check_win()
    {
        for (int x = 0; x < height; ++x)
            for (int y = 0; y < weidht; ++y)
                if (field[x][y] == Typesign::Ship)
                    return false;
        return true;
    }
};

//игрок заполняет поле
void player_fill(Field& player_field, std::vector<int> ships)
{
    std::vector<int> player_ships = ships;
    int length_ship = 0;

    while (player_ships.size() > 0)
    {
        std::cout << "заполните поле\n" << "ваши корабли:\n";
        print_ships(player_ships);
        while (true) {
            std::cout << "\n" << "введите какой длины будет корабль:\n";
            std::cin >> length_ship;
            if (player_ships[player_ships.size()-1] != length_ship && *find(player_ships.begin(), player_ships.end(), length_ship) == player_ships.size())
                std::cout << "у вас нет такого корабля\n введите снова\n";
            else
                break;
        }
        char direction{};
        if (length_ship != 1) {
            std::cout << "\n" << "введите направление корабля\n чтобы корабль был носом вниз введите " << Typesign::BowDown <<
                "\n чтобы корабль был носом вверх введите " << Typesign::BowUp <<
                "\n чтобы корабль был носом вправо введите " << Typesign::BowRight <<
                "\n чтобы корабль был носом влево введите " << Typesign::BowLeft << "\n";
            
            std::cin >> direction;
        }
        else
            direction = 'U';
        while (true) {
            std::cout << "\n" << "введите координаты носа корабля\n";
            int x, y;
            std::cin >> y >> x;
            if (player_field.check_fields(length_ship, x, y, direction) == 1) {
                player_field.fill_ships(length_ship, x, y, direction);
                player_ships.erase(find(player_ships.begin(), player_ships.end(), length_ship));
                break;
            }
            else
                std::cout << "корабль не помещается выберите другие координаты\n";
        }
        player_field.get_field();
        std::cout << "\n";
        
    }
    std::cout << "Вы заполнили поле. Вы хотите поменять свое решение?\n Если да введите 1, если нет, то 0\n";
    int change;
    std::cin >> change;
    if (change == 1) {
        player_field.fill(Typesign::Start);
        player_fill(player_field, ships);
    }
}

void bot_fill(Field& bot_field, std::vector<int> ships)
{
    srand(time(0));
    int x = rand() % 10, y = rand() % 10;
    char direction[] = { 'U', 'D', 'R','L' };
    for (int i = 0; i < ships.size(); ++i)
    {
        while (!bot_field.check_fields(ships[i], x, y, direction[rand() % 4]))
        {
            x = rand() % 10, y = rand() % 10;
        }
        bot_field.fill_ships(ships[i], x, y, direction[rand() % 4]);

    }
}

//сделать код рабочим под линукс и мак, начать делать минимальный интерфейс
int game(Field player_field, Field bot_field)
{
    player_fill(player_field, ships);
    bot_fill(bot_field, ships);
    Field player_see_field, bot_see_field;
    player_see_field.fill(Typesign::Opponent_see);
    bot_see_field.fill(Typesign::Opponent_see);
    while (true)
    {
        std::cout << "Ваш ход, выберите координаты по которым хотите стрелять\n";
        player_see_field.get_field();
        std::cout << "\n";
        while (true) {
            int x, y;
            std::cin >> y >> x;
            if (player_see_field.get_sign(x % 10, y % 10) == Typesign::Opponent_see) {
                if (bot_field.get_sign(x % 10, y % 10) == Typesign::Ship) {
                    player_see_field.change_sign(x % 10, y % 10, Typesign::Hit);
                    bot_field.change_sign(x % 10, y % 10, Typesign::Hit);
                }
                else 
                {
                    player_see_field.change_sign(x % 10, y % 10, Typesign::Miss);
                    break;
                }
                player_see_field.get_field();
                if (bot_field.check_win())
                {
                    std::cout << "Вы победили!!!\nВыше поле:";
                    
                    player_field.get_field();
                    std::cout << "Поле противника:";
                    bot_field.get_field();
                    return 0;
                }
            }
            else
                std::cout << "Вы уже били по это клетке, выберите другую";
        }
        while (true) {
            int x = rand() % 10, y = rand() % 10;
            if (bot_see_field.get_sign(x, y) == Typesign::Opponent_see) {
                if (player_field.get_sign(x, y) == Typesign::Ship) {
                    bot_see_field.change_sign(x, y, Typesign::Hit);
                    player_field.change_sign(x, y, Typesign::Hit);
                }
                else {
                    bot_see_field.change_sign(x, y, Typesign::Miss);
                    break;
                }
                if (player_field.check_win())
                {
                    std::cout << "Бот победил(((\nВыше поле:";
                    player_field.get_field();
                    std::cout << "Поле противника:";
                    bot_field.get_field();
                    return 0;
                }
            }
        }
    }
    return 0;
}





int main()
{
    Platform::Init();
    Field player_field{}, bot_field{};
    game(player_field, bot_field);
    return 0;
}

