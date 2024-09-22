// making snake for real 8/29/2024

#include <SFML/Graphics.hpp>
#include <list>
#include <cstdlib>
#include <iostream>

#define GRID_X 30
#define GRID_Y 20
#define SQUARE_SIZE 25
#define WIN_X GRID_X * SQUARE_SIZE
#define WIN_Y GRID_Y * SQUARE_SIZE

void addSquare(char direction, std::list <sf::RectangleShape>* snake); // adds a square to the snake in the direction that it is headed
void move(char direction, std::list <sf::RectangleShape>* snake); // removes tail to put in front of head in the correct direction, makes head red
bool isDead(char direction, std::list <sf::RectangleShape>* snake); // checks to see if the snake if hitting the walls or itself
bool isEating(char direction, std::list <sf::RectangleShape>* snake, sf::RectangleShape* food); // checks to see if the snake head is going to eat
int randX(); // random value between 0 and WIN_X at multiples of SQUARE_SIZE
int randY(); // random value between 0 and WIN_Y at multiples of SQUARE_SIZE

int main()
{
    srand(time(0)); // ensures the food will be in a different position each playthrough 

    sf::RenderWindow window(sf::VideoMode(WIN_X, WIN_Y), "snek");
    
    char direction = 'e'; // will be: n s e w; used to keep track of whech way the snake is going
    char directionBuffer = direction; // to prevent turtling head and killing yourself

    std::list <sf::RectangleShape> snake; // list to keep track of the rectangles that make up the snake

    // make the initial snake that is 3 squares
    addSquare(direction, &snake);
    addSquare(direction, &snake);
    addSquare(direction, &snake);

    // initialize food
    sf::RectangleShape food(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    food.setFillColor(sf::Color::Yellow);
    food.setPosition(randX(), randY());

    sf::Clock moveTimer; // used to measure loop time for movement

    float speed = 0.2f; // how often in seconds the snake will move

    float sinceMove = 0.f; // counter to keep track of how long since last move

    // main loop
    while (window.isOpen())
    {
        // event loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // if wasd pressed, change direction
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && direction != 'e')
        {
            directionBuffer = 'w';
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction != 'w'))
        {
            directionBuffer = 'e';
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && direction != 'n')
        {
            directionBuffer = 's';
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && direction != 's')
        {
            directionBuffer = 'n';
        }

        // see if time to move and move
        sinceMove += moveTimer.restart().asSeconds();
        if (sinceMove >= speed)
        {
            sinceMove = 0;

            direction = directionBuffer;
            
            if (isDead(direction, &snake)) 
            {
                std::cout << "Dead! ";
                window.close();
            }
            else if (isEating(direction, &snake, &food))
            {
                std::cout << "Eat! "; 
                food.setPosition(randX(), randY());
                addSquare(direction, &snake);
            }
            else
            {
                std::cout << "Move! ";
                move(direction, &snake);
            }
        }

        window.clear();

        for (auto it = snake.begin(); it != snake.end(); ++it)
        {
            window.draw(*it);
        }

        window.draw(food);
        window.display();
    }

    return 0;
}


void addSquare(char direction, std::list <sf::RectangleShape>* snake)
{
    sf::RectangleShape sq(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    sq.setFillColor(sf::Color::Red);
    sq.setOutlineThickness(-1.5f);
    sq.setOutlineColor(sf::Color::Black);

    if (snake->empty())
    {
        snake->push_front(sq);
    }
    else if (direction == 'e')
    {
        sq.setPosition(snake->front().getPosition().x + SQUARE_SIZE, snake->front().getPosition().y);
        snake->begin()->setFillColor(sf::Color::Green);
        snake->push_front(sq);
    }
    else if (direction == 'w')
    {
        sq.setPosition(snake->front().getPosition().x - SQUARE_SIZE, snake->front().getPosition().y);
        snake->begin()->setFillColor(sf::Color::Green);
        snake->push_front(sq);
    }
    else if (direction == 'n')
    {
        sq.setPosition(snake->front().getPosition().x, snake->front().getPosition().y - SQUARE_SIZE);
        snake->begin()->setFillColor(sf::Color::Green);
        snake->push_front(sq);
    }
    else if (direction == 's')
    {
        sq.setPosition(snake->front().getPosition().x, snake->front().getPosition().y + SQUARE_SIZE);
        snake->begin()->setFillColor(sf::Color::Green);
        snake->push_front(sq);
    }
}


void move(char direction, std::list <sf::RectangleShape>* snake)
{
    snake->front().setFillColor(sf::Color::Green);

    if (direction == 'e')
    {
        snake->back().setPosition(snake->front().getPosition().x + SQUARE_SIZE, snake->front().getPosition().y);
    }
    else if (direction == 'w') {
        snake->back().setPosition(snake->front().getPosition().x - SQUARE_SIZE, snake->front().getPosition().y);
    }
    else if (direction == 'n')
    {
        snake->back().setPosition(snake->front().getPosition().x, snake->front().getPosition().y - SQUARE_SIZE);
    }
    else if (direction == 's')
    {
        snake->back().setPosition(snake->front().getPosition().x, snake->front().getPosition().y + SQUARE_SIZE);
    }

    snake->push_front(snake->back());
    snake->front().setFillColor(sf::Color::Red);
    snake->pop_back();

}


bool isDead(char direction, std::list <sf::RectangleShape>* snake)
{
    sf::Vector2f headPos = snake->front().getPosition(); // store the position of the head of the snake to make life easier

    // check if hit edge
    if (headPos.x == 0 && direction == 'w') { return true; }
    if (headPos.y == 0 && direction == 'n') { return true; }
    if (headPos.x == (GRID_X * SQUARE_SIZE) - SQUARE_SIZE && direction == 'e') { return true; }
    if (headPos.y == (GRID_Y * SQUARE_SIZE) - SQUARE_SIZE && direction == 's') { return true; }

    //check if hit self
    for (auto it = snake->begin(); it != snake->end(); ++it)
    {
        if (it != snake->begin() && headPos == it->getPosition()) { return true; }
    }

    return false;

}

bool isEating(char direction, std::list <sf::RectangleShape>* snake, sf::RectangleShape* food)
{
    sf::Vector2f headPos = snake->front().getPosition(); // store the position of the head of the snake to make life easier
    sf::Vector2f foodPos = food->getPosition(); // store the position of the head of the snake to make life easier

    if (direction == 'w' && foodPos.y == headPos.y && foodPos.x == headPos.x - SQUARE_SIZE) { return true; }
    if (direction == 'e' && foodPos.y == headPos.y && foodPos.x == headPos.x + SQUARE_SIZE) { return true; }
    if (direction == 'n' && foodPos.x == headPos.x && foodPos.y == headPos.y - SQUARE_SIZE) { return true; }
    if (direction == 's' && foodPos.x == headPos.x && foodPos.y == headPos.y + SQUARE_SIZE) { return true; }

    return false;
}


int randX()
{
    return (SQUARE_SIZE * (rand() % (GRID_X)));
}


int randY()
{
    return (SQUARE_SIZE * (rand() % (GRID_Y)));
}