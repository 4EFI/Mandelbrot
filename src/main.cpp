
#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

int Window_Width  = 1600;
int Window_Height = 1000;

//-----------------------------------------------------------------------------

int main()
{
    sf::RenderWindow window( sf::VideoMode( Window_Width, Window_Height ), "CrackPook!" );

    while( window.isOpen() )
    {
        sf::Event event;
        while( window.pollEvent(event) )
        {
            if( event.type == sf::Event::Closed ) window.close();
        }
        
        window.clear( sf::Color::White );
        window.display();
    }

    return 0;
}

//-----------------------------------------------------------------------------
