
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

    float min_x = -2.0f, max_x = 2.0f, min_y = -2.0f, max_y = 2.0f, max_r = 10;
    int   max_num_itrs = 256;

    while( window.isOpen() )
    {
        sf::Event event;
        while( window.pollEvent(event) )
        {
            if( event.type == sf::Event::Closed ) window.close();
        }

        DrawMandelbrot( min_x, max_x, min_y, max_y, max_r, max_num_itrs );

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
            float x = cur_x;
            float y = cur_y;
            
            int n = 0;
            for( ; n < max_num_itrs; n++ )
            {
                float x2 = x * x;
                float y2 = y * y;
                float xy = x * y;  

                if( x2 + y2 > max_r2 ) break;

                x = x2 - y2 + cur_x;
                y = xy + xy + cur_y;
            }

            
        }
    }
}

//-----------------------------------------------------------------------------
