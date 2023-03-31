
#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

const int Window_Width  = 1000;
const int Window_Height = 1000;

//-----------------------------------------------------------------------------

void DrawMandelbrot( sf::Image* img,
                     float min_x_coord, float max_x_coord,
                     float min_y_coord, float max_y_coord,
                     float max_r,       int   max_num_itrs );

void MoveCoords( sf::Event& event, 
                 float* min_x_coord, float* max_x_coord, float step_x,
                 float* min_y_coord, float* max_y_coord, float step_y,
                 float  scale );

//-----------------------------------------------------------------------------

int main()
{
    sf::RenderWindow window( sf::VideoMode( Window_Width, Window_Height ), "Mandelbrot" );

    float min_x = -2.375f, max_x = 1.625f, min_y = -2.0f, max_y = 2.0f, max_r = 10;
    int   max_num_itrs = 256;

    sf::Image img;
    img.create( Window_Width, Window_Height, sf::Color::White );


    sf::Texture texture;
    texture.loadFromImage( img );

    sf::Sprite sprite;
    sprite.setTexture( texture );

    while( window.isOpen() )
    {
        sf::Event event;
        while( window.pollEvent(event) )
        {
            if( event.type == sf::Event::Closed ) window.close();

            MoveCoords( event, &min_x, &max_x, 0.1f, &min_y, &max_y, 0.1f, 1.1f );
        }
        
        DrawMandelbrot( &img, min_x, max_x, min_y, max_y, max_r, max_num_itrs );
        texture.update( img );

        window.clear();
        window.draw ( sprite );
        window.display();
    }

    return 0;
}

//-----------------------------------------------------------------------------

void DrawMandelbrot( sf::Image* img,  
                     float min_x_coord,  float max_x_coord,
                     float min_y_coord,  float max_y_coord,
                     float max_r,        int   max_num_itrs )
{
    const float step_x = ( max_x_coord - min_x_coord ) / float( img->getSize().x );
    const float step_y = ( max_y_coord - min_y_coord ) / float( img->getSize().y );

    const float max_r2 = max_r * max_r;

    int cur_img_y = 0;
    for( float cur_y = min_y_coord; cur_y <= max_y_coord; cur_y += step_y )
    {
        int cur_img_x = 0;
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

            img->setPixel( cur_img_x, cur_img_y, sf::Color( n, n, n ) );

            cur_img_x++;
        }

        cur_img_y++;
    }
}

//-----------------------------------------------------------------------------

void MoveCoords( sf::Event& event, 
                 float* min_x_coord,  float* max_x_coord, float step_x,
                 float* min_y_coord,  float* max_y_coord, float step_y,
                 float  scale )
{
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
            case sf::Keyboard::Key::S:     
                (*min_y_coord) += step_y;
                (*max_y_coord) += step_y;                                             
                break;

            case sf::Keyboard::Key::W: 
                (*min_y_coord) -= step_y;
                (*max_y_coord) -= step_y;                                             
                break;

            case sf::Keyboard::Key::D: 
                (*min_x_coord) += step_x;
                (*max_x_coord) += step_x;                                             
                break;

            case sf::Keyboard::Key::A: 
                (*min_x_coord) -= step_x;
                (*max_x_coord) -= step_x;                                             
                break;

            case sf::Keyboard::Key::M: 
                (*min_x_coord) *= scale;
                (*max_x_coord) *= scale;
                (*min_y_coord) *= scale;
                (*max_y_coord) *= scale;                                             
                break;

            case sf::Keyboard::Key::P: 
                (*min_x_coord) /= scale;
                (*max_x_coord) /= scale;
                (*min_y_coord) /= scale;
                (*max_y_coord) /= scale;                                             
                break;
            
            default: 
                break;
        }

    }
}

//-----------------------------------------------------------------------------
