#include <SFML/Graphics.hpp>
#include <time.h>
#include <algorithm>

using namespace sf;

class Stav
{
    public:
        Sprite sprite[9];                                                                           //устанавливает позиции для каждого спрайта в массиве sprite в зависимости от их индексов (от 0 до 8)                                       
        bool tik[9];                                                                                //чтобы спрайты не были видны в начале игры
  
        Stav() {
            std::fill(tik, tik+9, false); 
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    sprite[i * 3 + j].setPosition(200 * j, 200 * i);
        };
        Stav(Texture& image)                                                                      //ссылка на текстуру, которая передается в класс при его создании
        { 
            std::fill(tik, tik+9, false);                                                         //заполняет массив tik значением false
            for (int i = 0; i < 9; i++)
            {
                sprite[i].setTexture(image);
                //tik[i] = false;
            }
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    sprite[i * 3 + j].setPosition(200 * j, 200 * i);
        }

        void setTexture(const Texture& image)                                                       //установика текстуры для всех спрайтов на игровом поле одновременно
        {
            for (int i = 0; i < 9; i++)
                sprite[i].setTexture(image);
        }

        void update(int& vid)                                                                     //обновляет текстурный прямоугольник каждого из девяти спрайтов в объекте Stav
        {
            // if (c.Choice == 0)
            // {
            //     vid -= 1;
            // }
            for (int i = 0; i < 9; i++)
                sprite[i].setTextureRect(IntRect(200 * (vid - 1), 0, 200, 200));
        }                                                                                     
};

class Game
{
    public:
        int Choice;                                                                                                 ////для того, чтобы выбирать крестик или нолик
        Texture net;                                                                                                //сетка
        Texture c;                                                                                                  //для выбора крестика или нолика
        Stav player, bot;
        Sprite choice[2];
        Sprite fon;

        Game(): Choice(0)
        {
            net.loadFromFile("fon.png");
            c.loadFromFile("crnol.png"); 
            player.setTexture(c);
            bot.setTexture(c);    

            fon.setTexture(net);
            for (int i = 0; i < 2; i++)
            {
                choice[i].setTexture(c);                                                                                //загрузка текстуры для получения спрайта
                choice[i].setPosition(50 + 300 * i, 180);                                                               //позиция спрайта(крестика/нолика)
            } 
        }
};


int main()
{
    srand(time(0));                                                                                             //для установки начального значения для генератора случайных чисел

    RenderWindow window(VideoMode(600, 600), "tic-tac-toe");
    Game game;

    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);                                                              //координаты курсора
        
        Event event;
        while (window.pollEvent(event))                                                                         //обработка событий окна
        {
            //printf("%s\n", "mouse is on the window");
            if (event.type == Event::Closed)
            {
                printf("%s\n", "window is closing");
                window.close();
            }

            if (event.type == Event::MouseButtonPressed)                                                        //сохраняет выбор (1-крестика/2-нолика) при нажатии в переменную Choice
            {
                printf("%s\n", "button was pressed");    
                if (event.mouseButton.button == Mouse::Left)
                {   
                    printf("%d %d\n", pos.x, pos.y);
                    if (game.Choice == 0)                                                                           //если элемент не выбран, будем выбирать
                    {
                        printf("%s\n", "no choice made");   
                        for (int i = 0; i < 2; i++)
                            if (game.choice[i].getGlobalBounds().contains(pos.x, pos.y))
                            {
                                game.Choice = i + 1;
                                printf("%s\n", "choice was made"); 
                            }
                    }
                    else
                    {     
                        printf("was a click after choice\n");              
                        for (int i = 0; i < 9; i++)
                        {
                            printf("%d\n", game.player.sprite[i].getTextureRect().left); 
                            printf("%d\n", game.player.sprite[i].getTextureRect().top); 
                            printf("%d\n", game.player.sprite[i].getTextureRect().width); 
                            printf("%d\n", game.player.sprite[i].getTextureRect().height); 
                            if (game.player.sprite[i].getGlobalBounds().contains(pos.x, pos.y))                 //если нажали на какую-то ячейку
                            {
                                printf("%s %d %s\n", "player clicked on the", i, "rectangle" ); 
                                game.player.tik[i] = true;                                                      //игрок сделал ход 
                                int botstav = rand() % 9;                                                       //бот делает случайный ход в ячейку от 0 до 8
                                game.bot.tik[botstav] = true;                                                   //бот сделал ход 
                            }
                            // printf("...\n" ); 
                            // int botstav = rand() % 9; 
                            // game.bot.tik[botstav] = true;   
                            // int botstav = rand() % 9;  
                            // if (game.player.tik[botstav] || game.bot.tik[botstav])                              //либо player, либо bot уже сделал ход в ячейку с номером botstav
                            //     while (game.bot.tik[botstav] == false)
                            //     {
                            //         if (game.player.tik[botstav] == game.bot.tik[botstav])                      //игрок и бот поставили знаки в одну и ту же ячейку, такой ход нам не нужен
                            //         {
                            //             int botstav = rand() % 9;  
                            //             game.bot.tik[botstav] = false;
                            //             continue;
                            //         }
                            //         else
                            //         {
                            //             int botstav = rand() % 9;  
                            //             game.bot.tik[botstav] = true;
                            //             break;
                            //         }
                            //     }                                
                        }   
                    }
                }
            }            
        }
       
        for (int i = 0; i < 2; i++)
        {
            if (game.choice[i].getGlobalBounds().contains(pos.x, pos.y))                                                 //если курсор попал в крестик/нолик
                game.choice[i].setTextureRect(IntRect(200*i, 200, 200, 200));                                            //обводим квадратом
            else
                game.choice[i].setTextureRect(IntRect(200*i, 0, 200, 200));
        }

        game.player.update(game.Choice);

        int ChoiceforBot = game.Choice + 1;                                                                      //чтобы игрок и бот не смогли выбрать одинаковый спрайт (чтобы бот играл крестиком, а игрок -- ноликом или наоборот)
        if (ChoiceforBot == 3)
            ChoiceforBot = 1;

        game.bot.update(ChoiceforBot);

        window.clear(Color::White);

        if (game.Choice == 0)                                                                                            //если выбор спрайта не сделан
            for (int i = 0; i < 2; i++)
                window.draw(game.choice[i]);                                                                             //на экран выводятся спрайты для выбора
        else
        {
            window.draw(game.fon);                                                                                       //на экран выводится сетка для игры
            for (int i = 0; i < 9; i++)
            {
                if (game.player.tik[i])                                     //
                    window.draw(game.player.sprite[i]);                     //

                if (game.bot.tik[i])                                        //  
                    window.draw(game.bot.sprite[i]);                        //
            }
        }
        window.display();
    }   
}