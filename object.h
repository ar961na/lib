#ifndef RADAR2_OBJECT_H
#define RADAR2_OBJECT_H

#include <glm/glm.hpp>

#include "constants.h"
#include "signal.h"

/**
 * @brief Базовый класс для объектов.
 */
class Base {
protected:
    glm::vec3 pos = {0.0, 0.0, 0.0}; /**< Положение объекта */
    double radius = 10.0; /**< Радиус объекта по умолчанию */
public:
    /**
     * @brief Конструктор по умолчанию.
     */
    Base() = default;

    /**
     * @brief Конструктор с параметром позиции.
     * @param p Позиция объекта.
     */
    Base(glm::vec3 p) : pos{p} {}

    /**
     * @brief Конструктор копирования.
     * @param b Объект для копирования.
     */
    Base(Base const &b) : pos{b.pos} {}

    /**
     * @brief Возвращает текущую позицию объекта.
     * @return Позиция объекта.
     */
    glm::vec3 get_pos() const { return pos; }

    /**
     * @brief Возвращает высоту объекта над поверхностью Земли.
     * @return Высота объекта.
     */
    double get_height() { return glm::distance(pos, e_pos) - static_cast<float>(e_radius); }

    /**
     * @brief Устанавливает новую высоту объекта над поверхностью Земли.
     * @param new_height Новая высота объекта.
     */
    void set_height(double new_height) {
        pos = static_cast<float>(e_radius + new_height) * glm::normalize(pos - e_pos) + e_pos;
    }

    /**
     * @brief Выводит информацию о позиции объекта.
     */
    void print() const {
        std::cout << "pos: { " << pos.x << ", " << pos.y << ", " << pos.z << " }" << std::endl;
    }
};

/**
 * @brief Класс, представляющий воздушное судно.
 */
class Aircraft : Base {
private:
public:
    float radius = 10.0; /**< Радиус воздушного судна по умолчанию */

    /**
     * @brief Конструктор копирования.
     * @param a Объект для копирования.
     */
    Aircraft(Aircraft const &a) : Base(a) {}

    using Base::Base;
    using Base::get_pos, Base::get_height, Base::set_height;
    using Base::print;

    /**
     * @brief Изменяет радиус воздушного судна.
     * @param new_rad Новый радиус.
     */
    void change_rad(float new_rad) {
        radius = new_rad;
    }
};

/**
 * @brief Класс, представляющий источник сигнала.
 */
class Source : Base {         //for Signal
private:
public:
    float radius = 10.0; /**< Радиус источника сигнала по умолчанию */

    using Base::Base;
    using Base::get_pos, Base::get_height, Base::set_height;
    using Base::print;
};

#endif //RADAR2_OBJECT_H
