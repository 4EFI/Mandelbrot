
#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SSE

//-----------------------------------------------------------------------------

const int Window_Width  = 1024;
const int Window_Height = 1024;

//-----------------------------------------------------------------------------

void DrawMandelbrot( sf::Image* img,
                     float min_x_coord, float max_x_coord,
                     float min_y_coord, float max_y_coord,
                     float max_r,       int   max_num_itrs );

void DrawMandelbrotSSE( sf::Image* img,  
                        float min_x_coord,  float max_x_coord,
                        float min_y_coord,  float max_y_coord,
                        float max_r,        int   max_num_itrs );

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

    sf::Clock clock;

    while( window.isOpen() )
    {
        sf::Time elapsed = clock.restart();

        sf::Event event;
        while( window.pollEvent(event) )
        {
            if( event.type == sf::Event::Closed ) window.close();

            MoveCoords( event, &min_x, &max_x, 0.1f, &min_y, &max_y, 0.1f, 1.1f );
        }
        
#ifdef SSE
        DrawMandelbrotSSE( &img, min_x, max_x, min_y, max_y, max_r, max_num_itrs );
#else  
        DrawMandelbrot   ( &img, min_x, max_x, min_y, max_y, max_r, max_num_itrs );
#endif

        texture.update( img );

        printf ( "FPS %.3f\n", 1 / elapsed.asSeconds() );

        window.clear( sf::Color::Red );
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
    for( float cur_y = min_y_coord; cur_y < max_y_coord; cur_y += step_y )
    {
        int cur_img_x = 0;
        for( float cur_x = min_x_coord; cur_x < max_x_coord; cur_x += step_x )
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

            
            if( cur_img_x >= 1024 || cur_img_y >= 1024 ) continue;

            img->setPixel( cur_img_x, cur_img_y, sf::Color( sf::Uint8(n), sf::Uint8(n), sf::Uint8(n) ) );

            cur_img_x++;
        }

        cur_img_y++;
    }
}

//-----------------------------------------------------------------------------

void DrawMandelbrotSSE( sf::Image* img,  
                        float min_x_coord,  float max_x_coord,
                        float min_y_coord,  float max_y_coord,
                        float max_r,        int   max_num_itrs )
{
    float step_x = ( max_x_coord - min_x_coord ) / float( img->getSize().x );
    float step_y = ( max_y_coord - min_y_coord ) / float( img->getSize().y );

    __m256 max_r2  = _mm256_set1_ps( max_r * max_r );

    __m256 mul_num = _mm256_set_ps( 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f );

    int cur_img_y = 0;
    for( float cur_y = min_y_coord; cur_y < max_y_coord; cur_y += step_y )
    {
        int cur_img_x = 0;
        for( float cur_x = min_x_coord; cur_x < max_x_coord; cur_x += step_x * 8.0f )
        {
            __m256 x0 = _mm256_add_ps ( _mm256_mul_ps( mul_num, _mm256_set1_ps( step_x ) ), 
                                        _mm256_set1_ps( cur_x ) );
            __m256 y0 = _mm256_set1_ps( cur_y );
            
            __m256 x = x0;
            __m256 y = y0;
            
            __m256i N = _mm256_set1_epi32( 0 );
            for( int i = 0; i < max_num_itrs; i++ )
            {
                __m256 x2 = _mm256_mul_ps( x, x );
                __m256 y2 = _mm256_mul_ps( y, y );
                __m256 xy = _mm256_mul_ps( x, y );

                __m256 r2  = _mm256_add_ps( x2, y2 );
                __m256 cmp = _mm256_cmp_ps( r2, max_r2, _CMP_LE_OQ );

                if( !_mm256_movemask_ps( cmp ) ) break;

                N = _mm256_sub_epi32( N, _mm256_castps_si256( cmp ) );

                x = _mm256_add_ps( _mm256_sub_ps( x2, y2 ), x0 ); 
                y = _mm256_add_ps( _mm256_add_ps( xy, xy ), y0 );
            }

            int* colors = ( int* )&N;
            for( int i = 0; i < 8; i++ )
            {
                sf::Uint8 n( colors[i] );
                if( cur_img_x + i >= 1024 || cur_img_y >= 1024 ) break;

                img->setPixel( cur_img_x + i, cur_img_y, sf::Color( n, n, n ) );
            }

            cur_img_x += 8;
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
