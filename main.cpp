#include <SFML/Graphics.hpp>
#include <time.h>
#include <algorithm>

using namespace sf;

bool empty[9] = {false};

class Stav
{
public:
    Sprite sprite[9]; //устанавливает позиции для каждого спрайта в массиве sprite в зависимости от их индексов (от 0 до 8)
    bool tik[9];      //чтобы спрайты не были видны в начале игры

    Stav()
    {
        std::fill(tik, tik + 9, false);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                sprite[i * 3 + j].setPosition(200 * j, 200 * i);
    };
    Stav(Texture &image) //ссылка на текстуру, которая передается в класс при его создании
    {
        for (int i = 0; i < 9; i++)
            sprite[i].setTexture(image);
    }

    void setTexture(const Texture &image) //установика текстуры для всех спрайтов на игровом поле одновременно
    {
        for (int i = 0; i < 9; i++)
            sprite[i].setTexture(image);
    }

    void update(int &vid) //обновляет текстурный прямоугольник каждого из девяти спрайтов в объекте Stav
    {
        for (int i = 0; i < 9; i++)
            sprite[i].setTextureRect(IntRect(200 * (vid - 1), 0, 200, 200));
    }
};

int BotStav();

class Game
{
public:
    int Choice;  ////для того, чтобы выбирать крестик или нолик
    Texture net; //сетка
    Texture c;   //для выбора крестика или нолика
    Texture l;
    Stav player, bot;
    Sprite choice[2];
    Sprite fon;
    Sprite line;
    // bool win = false;
    Game();

    void handleMouseClick(Event &event, Vector2i &pos,RenderWindow &window, int &num_of_full_rect);

    void Run()
    {
        bool win = false;
        srand(time(0)); //для установки начального значения для генератора случайных чисел

        RenderWindow window(VideoMode(600, 600), "tic-tac-toe");
        //Game game;
        //game set window (что-то такое)
        int num_of_full_rect = 0; //будет считать кол-во заполненных ячеек

        while (window.isOpen())
        {
            Vector2i pos = Mouse::getPosition(window); //координаты курсора

            Event event;
            while (window.pollEvent(event)) //обработка событий окна
            {
                // printf("%s\n", "mouse is on the window");
                if (event.type == Event::Closed)
                {
                    printf("%s\n", "window is closing");
                    window.close();
                }
                handleMouseClick(event, pos, window, num_of_full_rect);
                
            }

            for (int i = 0; i < 2; i++)
            {
                if (choice[i].getGlobalBounds().contains(pos.x, pos.y))        //если курсор попал в крестик/нолик
                    choice[i].setTextureRect(IntRect(200 * i, 200, 200, 200)); //обводим квадратом
                else
                    choice[i].setTextureRect(IntRect(200 * i, 0, 200, 200));
            }

            bool winplay[8][2]; //события, при которых вощникает победа (то есть 3 по горизонтали/вертикали/диагонали всего таких комбинация 8), для игрока и бота (то есть 2)
            for (int i = 0; i < 8; i++)
            {
                if (i < 3)
                {
                    winplay[i][0] = player.tik[3 * i] && player.tik[1 + 3 * i] && player.tik[2 + 3 * i]; //являются ли три ячейки в горизонтальной линии i заняты символами игрока
                    winplay[i][1] = bot.tik[3 * i] && bot.tik[1 + 3 * i] && bot.tik[2 + 3 * i];
                }
                else if (i >= 3 && i < 6)
                {
                    winplay[i][0] = player.tik[i - 3] && player.tik[i] && player.tik[i + 3]; //вертикальная линия
                    winplay[i][1] = bot.tik[i - 3] && bot.tik[i] && bot.tik[i + 3];
                }
                else if (i >= 6)
                {
                    winplay[i][0] = player.tik[2 * (i - 6)] && player.tik[4] && player.tik[8 - 2 * (i - 6)]; //диагональная линия
                    winplay[i][1] = bot.tik[2 * (i - 6)] && bot.tik[4] && bot.tik[8 - 2 * (i - 6)];
                }

                for (int j = 0; j < 2; j++)
                    if (winplay[i][j])                                          //если какое-то из вышеперечисленных условий выполнилось
                    {
                        win = true;
                        if (i < 3)
                        {
                            line.setTextureRect(IntRect(0, 0, 600, 10)); //горизонталль
                            int ly = 95 + 200 * i;
                            printf("%d\n", ly);
                            line.setPosition(0, ly);
                        }
                        else if (i < 6)
                        {
                            line.setTextureRect(IntRect(0, 0, 600, 10)); //вертикаль
                            line.setRotation(90);
                            int lx = 105 + 200 * (i - 3);
                            printf("%d\n", lx);
                            line.setPosition(lx, 0);
                        }
                        else if (i == 6)
                        {
                            line.setTextureRect(IntRect(0, 0, 600, 10)); //диагональ
                            line.setRotation(45);
                            printf("45\n");
                        }
                        else if (i == 7)
                        {
                            line.setTextureRect(IntRect(0, 0, 600, 10)); //диагональ
                            line.setRotation(135);
                            line.setPosition(600, 0);
                            printf("135\n");
                            sf::Vertex line[] =//
                            {                                           //
                                sf::Vertex(sf::Vector2f(10, 10)),       //для реализации диагонали без текстуры как на сайте sfml
                                sf::Vertex(sf::Vector2f(150, 150))      //
                            };                                          //

                        window.draw(line, 2, sf::Lines);
                        }
                    }
            }

            player.update(Choice);

            int ChoiceforBot = Choice + 1; //чтобы игрок и бот не смогли выбрать одинаковый спрайт (чтобы бот играл крестиком, а игрок -- ноликом или наоборот)
            if (ChoiceforBot == 3)
                ChoiceforBot = 1;

            bot.update(ChoiceforBot);

            window.clear(Color::White);

            if (Choice == 0) //если выбор спрайта не сделан
                for (int i = 0; i < 2; i++)
                    window.draw(choice[i]); //на экран выводятся спрайты для выбора
            else
            {
                window.draw(fon); //на экран выводится сетка для игры
                for (int i = 0; i < 9; i++)
                {
                    if (player.tik[i])
                        window.draw(player.sprite[i]);

                    if (bot.tik[i])
                        window.draw(bot.sprite[i]);
                }
            }
            if (win)
            {
                window.draw(line);
                printf("line");
            }
            window.display();
        }
    }

};

Game::Game() : Choice(0)
{
    net.loadFromFile("fon.png");
    c.loadFromFile("crnol.png");
    l.loadFromFile("line.png");
    line.setTexture(l);
    player.setTexture(c);
    bot.setTexture(c);

    fon.setTexture(net);
    for (int i = 0; i < 2; i++)
    {
        choice[i].setTexture(c);                  //загрузка текстуры для получения спрайта
        choice[i].setPosition(50 + 300 * i, 180); //позиция спрайта(крестика/нолика)
    }
}

void Game::handleMouseClick(Event &event, Vector2i &pos, RenderWindow &window, int &num_of_full_rect)
{
     if (event.type == Event::MouseButtonPressed) //сохраняет выбор (1-крестика/2-нолика) при нажатии в переменную Choice
    {
        if (event.mouseButton.button == Mouse::Left)
        {
            printf("%d %d\n", pos.x, pos.y);
            if (Choice == 0) //если элемент не выбран, будем выбирать
            {
                printf("%s\n", "no choice made");
                for (int i = 0; i < 2; i++)
                    if (choice[i].getGlobalBounds().contains(pos.x, pos.y))
                    {
                        Choice = i + 1;
                        printf("%s\n", "choice was made");
                    }
            }
            else
            {
                printf("was a click after choice\n");
                for (int i = 0; i < 9; i++)
                    if (player.sprite[i].getGlobalBounds().contains(pos.x, pos.y) && !empty[i]) //если нажали на какую-то ячейку
                    {
                        printf("%s %d %s\n", "player clicked on the", i, "rectangle");
                        player.tik[i] = true;
                        empty[i] = true;
                        num_of_full_rect += 1; //кол-во заполненных ячеек увеличивается на 1 после выполнения хода

                        if (num_of_full_rect < 9)
                        {
                            num_of_full_rect++;
                            int botstav = BotStav();
                            bot.tik[botstav] = true;
                            empty[botstav] = true;
                        }
                    }
            }
        }
    }
}





int BotStav() //функция, отвечающая за поиск свободной ячейки и возвращающая ее номер
{
    int num_of_empty_rect = 0;

    bool IsEmpty = false; //мы нашли какую-нибудь свободную ячейку или нет?
    while (!IsEmpty)
    {
        num_of_empty_rect = rand() % 9;
        if (!empty[num_of_empty_rect])
            IsEmpty = true;
        else
            IsEmpty = false;
    }
    return num_of_empty_rect;
}

int main()
{
    Game game;
    game.Game::Run();    
}