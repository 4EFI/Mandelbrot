
#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

const int Window_Width  = 1000;
const int Window_Height = 1000;

//-----------------------------------------------------------------------------

void DrawMandelbrot( float min_x_coord,  float max_x_coord,
                     float min_y_coord,  float max_y_coord,
                     float max_r,        int   max_num_itrs );

//-----------------------------------------------------------------------------

int main()
{
    sf::RenderWindow window( sf::VideoMode( Window_Width, Window_Height ), "Mandelbrot" );

    const float max_r = 10;

    while( window.isOpen() )
    {
        sf::Event event;
        while( window.pollEvent(event) )
        {
            if( event.type == sf::Event::Closed ) window.close();
        }

        DrawMandelbrot( -2.0f, 2.0f, -2.0f, 2.0f, 10.0f, 256 );

        window.clear( sf::Color::White );
        window.display();
    }

    return 0;
}

//-----------------------------------------------------------------------------

void DrawMandelbrot( float min_x_coord,  float max_x_coord,
                     float min_y_coord,  float max_y_coord,
                     float max_r,        int   max_num_itrs )
{
    const float step_x = ( max_x_coord - min_x_coord ) / Window_Width;
    const float step_y = ( max_y_coord - min_y_coord ) / Window_Height;

    const float max_r2 = max_r * max_r;

    const int offset_x = Window_Width  / 2;
    const int offset_y = Window_Height / 2;

    for( float cur_y = min_y_coord; cur_y <= max_y_coord; cur_y += step_y )
    {
        for( float cur_x = min_x_coord; cur_x <= max_x_coord; cur_x += step_x )
        {
            for( int i = 0; i < max_num_itrs; i++ )
            {
                
            }
        }
    }
}

//-----------------------------------------------------------------------------
