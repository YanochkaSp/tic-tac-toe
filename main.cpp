#include <SFML/Graphics.hpp>

using namespace sf;

class Stav
{
    public:
        Sprite sprite[9];                                                                                       //поскольку всего 9 ячеек
        bool tik[9];                                                                                            //для того, чтобы спрайты в ячейках были (не)видны

};

int main()
{
    RenderWindow window(VideoMode(600, 600), "tic-tac-toe");

    Texture net;                                                                                                //сетка
    net.loadFromFile("/Users/yanasp/tic-tac-toe/paint/fon.png");
    Sprite fon(net);

    Texture c;                                                                                                  //для выбора крестика или нолика
    c.loadFromFile("/Users/yanasp/tic-tac-toe/paint/crnol.png");
    Sprite choice[2];
    for (int i = 0; i < 2; i++)
    {
        choice[i].setTexture(c);                                                                                //загрузка текстуры для получения спрайта
        choice[i].setPosition(50 + 300 * i, 180);                                                               //позиция спрайта(крестика/нолика)
    }

    int Choice = 0;                                                                                             //для того, чтобы выбирать крестик или нолик

    while (window.isOpen())
    {

        Vector2i pos = Mouse::getPosition(window);                                                              //координаты курсора
        
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)                                                        //сохраняет выбор (1-крестика/2-нолика) при нажатии в переменную Choice
            {
                if (event.key.code == Mouse::Left)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (choice[i].getGlobalBounds().contains(pos.x, pos.y))
                        {
                            Choice = i + 1;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < 2; i++)
        {
            if (choice[i].getGlobalBounds().contains(pos.x, pos.y))                                                 //если курсор попал в крестик/нолик
                choice[i].setTextureRect(IntRect(200*i, 200, 200, 200));                                            //обводим квадратом
            else
                choice[i].setTextureRect(IntRect(200*i, 0, 200, 200));
        }

        window.clear(Color::White);

        if (Choice == 0)                                                                                            //если выбор спрайта не сделан
        {
            for (int i = 0; i < 2; i++)
                window.draw(choice[i]);                                                                             //на экран выводятся спрайты для выбора
        }
        else
        {
            window.draw(fon);                                                                                       //на экран выводится сетка для игры
        }
        window.display();
    }   

}
