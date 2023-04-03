# Множество Мандельброта

## Что это?

это множество таких c, для которых существует такое действительное R, что неравенство $| z_n | < R$ выполняется при всех натуральных n. Где $z_n$ - это рекуррентное соотношение: $z_{n+1} = z_n^2 + c$, $z_0 = 0$. 
Определение и название принадлежат французскому математику Адриену Дуади, в честь математика Бенуа Мандельброта.


<img src=img/image.png width="500px"/>


## Постановка задачи

Первостепенно мы должны программно получить множество Мандельброта и убедиться, что данный алгоритм требует много вычислений, и как следствие выдает низкий FPS. Далее мы попробуем различные методы оптимизации. 

## Компиляция 

Стоит отметить, что данный проект создавался на Linux и на других операционных системах процесс компиляции может завершиться ошибкой. Для того чтобы скомпилировать проект, вам потребуется библиотека SFML. Для Linux, пропишите команду:
~~~
sudo apt-get install libsfml-dev
~~~

После успешной установки библиотеки, можем перейти к компиляции, пропишите, находясь в главной директории:
~~~
make clear
make    
~~~

## Запуск

Для того чтобы запустить исполняемый файл, пропишите:
~~~
./main
~~~

Если вы хотите протестировать программу с SSE, используйте следующую директиву в файле ./src/main.cpp
~~~
#define SSE
~~~

И выполните стадию компиляции, указанную выше. 

Также вы можете поставить нужные вам флаги оптимизации для компиляции. Для этого перейдите в SOURCES и добавьте их в переменную FFLAGS. 

## Анализ 

Без какой-либо оптимизации, мы получаем следующую картинку в 1.5 FPS:

<img src=img/NO_OPT.png width="700px"/>

Если мы скопмилируем проект с флагом -O3, мы получаем 3 FPS, что уже в 2 раза быстрее, чем без какой-либо оптимизации:

<img src=img/O3.png width="700px"/>

С оптимизацией AVX2 мы получаем 5 FPS, что уже в 3.3 раза больше, чем в первоначальном варианте:

<img src=img/SSE.png width="700px"/>

Используя флаг -O3 и AVX2, мы получаем 15 FPS, а это уже ускорение в 10 раз:

<img src=img/SSE_O3.png width="700px"/>


