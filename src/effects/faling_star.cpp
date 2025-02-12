#include "cube.h"
#include <vector>
#include <cstdlib>
#include <ctime>

//
// В классе EffectFallingStar (определённом в заголовочном файле) объявлены:
// - std::vector<Star> stars;
// - long columnTimers[8];
// - unsigned long minDelay, maxDelay;
// - float fallSpeed;
// - uint32_t color, tailColor;
// - uint8_t tailLength;

float getRandomFloatInRange(float min, float max)
{
    return min + (static_cast<float>(rand()) / RAND_MAX) * (max - min);
}

//

EffectFallingStar::EffectFallingStar()
{
    srand(static_cast<unsigned int>(time(0)));
    // Инициализируем вектор из 8 элементов (по одному для каждого столбца)
    stars.resize(8);
    // Инициализация для эффекта: для каждого столбца задаём задержку и запускаем первую звезду
    for (int i = 0; i < 8; ++i)
    {
        initializeColumnDelay(i);
        addStar(i, false);
    }
}

void EffectFallingStar::render(Cube &cube, unsigned long deltaTime)
{
  
 std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (int i = 0; i < 8; ++i)
    {
        // Если звезда в столбце НЕ активна, идёт отсчёт задержки
        if (!stars[i].active)
        {
            if (columnTimers[i] > static_cast<long>(deltaTime))
            {
                columnTimers[i] -= deltaTime;
            }
            else
            {
                // Задержка истекла – добавляем звезду и переинициализируем задержку
                addStar(i, true);
                initializeColumnDelay(i);
            }
        }
        else
        { // Если звезда активна, обновляем её положение
            stars[i].lastUpdate += deltaTime;
            // Если накопилось достаточно времени для следующего шага (звезда двигается с периодом speed * 1000 мс)
            if (stars[i].lastUpdate >= stars[i].speed * 1000)
            {
                for (int f = 0; f < faces.size(); f++) {
                    faces[f].get().clearColumn(i);
                }

                // Рисуем саму звезду (если она в пределах видимой области)
                if (stars[i].y < 8)
                {
                    for (int f = 0; f < faces.size(); f++) {
                        faces[f].get().setPixel(i, stars[i].y, colors[f], 1.0f);
                    }
                }

                // Рисуем хвост с эффектом затухания
                for (int j = 1; j <= tailLength; ++j)
                {                               // Начинаем с 1, чтобы хвост был позади звезды
                    int tailY = stars[i].y - j; // Сдвигаем каждый следующий элемент хвоста ниже
                    if (tailY >= 0 && tailY < 8)
                    { // Убедимся, что хвост не выходит за пределы матрицы
                        // Чем дальше элемент хвоста, тем меньше яркость
                        float brightness = 0.8f - (float(j - 1) / float(tailLength - 1)) * (0.8f - 0.25f);
                        for (int f = 0; f < faces.size(); f++) {
                            faces[f].get().setPixel(i, tailY, colors[f], brightness);
                        }
                    }
                }
                // Сбрасываем накопленное время для звезды
                stars[i].lastUpdate = 0;
                // Двигаем звезду вниз (до 8 + tailLength + 1 (сама звезда), чтобы хвост успевал прорисоваться)
                if (stars[i].y < 8 + tailLength + 1)
                {
                    stars[i].y++;
                }
            }

            // Если звезда (с хвостом) полностью прошла матрицу, помечаем её неактивной,
            // чтобы начать отсчёт задержки для следующей звезды в этом столбце
            if (stars[i].y >= 8 + tailLength + 1)
            {
                stars[i].active = false;
                // Обнуляем позицию и хвост для будущего появления
                stars[i].y = 0;
            }
        }
    }








    // Рендерим обновления на кубе
    cube.render();
}

void EffectFallingStar::initializeColumnDelay(int column)
{
    // Генерируем случайную задержку для столбца (между minDelay и maxDelay)
    // Используем тип long для возможности отрицательных значений при вычитании
    columnTimers[column] = static_cast<long>(rand() % (maxDelay - minDelay + 1) + minDelay);
}

void EffectFallingStar::addStar(int column, bool active)
{
    // Создаём новую звезду для столбца
    Star newStar;
    newStar.y = 0; // Звезда появляется в верхней строке
    float randomValue = getRandomFloatInRange(0.0f, 0.05f);
    newStar.speed = fallSpeed + randomValue; // Скорость падения (например, 0.5 сек на шаг)
    newStar.lastUpdate = 0;
    newStar.active = true; // Помечаем звезду как активную

    // Сохраняем звезду в массиве для данного столбца
    stars[column] = newStar;
}