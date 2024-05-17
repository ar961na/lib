#ifndef RADAR2_CONSTANTS_H
#define RADAR2_CONSTANTS_H

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

/**
 * @brief Установка позиции glm::vec3{0.0, 0.0, 0.0} на Земле, под радаром.
 */
//set glm::vec3{0.0, 0.0, 0.0} on Earth, under the radar

/**
 * @brief Число pi.
 */
const double pi = glm::pi<float>();

/**
 * @brief Начальная мощность сигнала.
 */
const double init_power = 150.0 * 1000.0;                       //150 kV - initial power of signal

/**
 * @brief Радиус Земли в модели, выраженный в метрах.
 */
const double e_radius = (4.0 / 3.0) * 6400.0 * 1000.0;          //(4 / 3) R - model in metres

/**
 * @brief Длина волны в метрах.
 */
const double lambda = 0.035;                                    //wavelength in metres

/**
 * @brief Положение центра Земли.
 */
glm::vec3 e_pos = glm::vec3{0.0, -e_radius, 0.0};      //Earth center

/**
 * @brief Вывод вектора.
 * @param a Вектор для вывода.
 */
glm::vec3 print(glm::vec3 a) {
    std::cout << a.x << " " << a.y << " " << a.z << std::endl;
}

#endif //RADAR2_CONSTANTS_H
