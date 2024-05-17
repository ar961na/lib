#ifndef RADAR2_MODULE_REFLECTION_H
#define RADAR2_MODULE_REFLECTION_H

#include <glm/gtx/intersect.hpp>
#include <stdexcept>

#include "signal.h"
#include "object.h"

#include "constants.h"

/**
 * @brief Класс для модуля отражений.
 *
 * Данный класс выполняет отражение сигнала от объектов и изменение его характеристик.
 */
class moduleReflection {
private:
    Signal sig; /**< Сигнал */
    Aircraft air; /**< Воздушное судно */

    const float phase_0 = pi; /**< Сдвиг фазы для базового отражения */

    /**
     * @brief Проверяет, является ли вектор корректным (не содержит NaN или бесконечности).
     *
     * @param vector Вектор для проверки.
     * @return true, если вектор корректный, в противном случае - false.
     */
    bool isValidVector(const glm::vec3& vec) {
        if (glm::isnan(vec.x) || glm::isnan(vec.y) || glm::isnan(vec.z)) {
            throw std::invalid_argument("Vector contains NaN value.");
        }
        if (glm::isinf(vec.x) || glm::isinf(vec.y) || glm::isinf(vec.z)) {
            throw std::invalid_argument("Vector contains infinity value.");
        }
        return true;
    }

    /**
     * @brief Получение высоты источника сигнала.
     * @param s Сигнал.
     * @return Высота источника сигнала.
     */
    static float h_rad(Signal s) { return s.get_height(); }

    /**
     * @brief Получение высоты воздушного судна.
     * @param a Воздушное судно.
     * @return Высота воздушного судна.
     */
    static float h_air(Aircraft a) { return a.get_height(); }

    /**
     * @brief Получение коэффициента для изменения фазы.
     * @param s Сигнал.
     * @return Коэффициент для изменения фазы.
     */
    static float k(Signal s) { return sin(s.get_phase()); }

    /**
     * @brief Расчет расстояния между точкой отражения и источником сигнала.
     * @param s Сигнал.
     * @return Расстояние между точкой отражения и источником сигнала.
     */
    static float l_rad(Signal s) {
        return -e_radius * k(s) + std::sqrt(e_radius * e_radius * k(s) * k(s) + 2 * e_radius * h_rad(s)
                                            + h_rad(s) * h_rad(s));
    }

    /**
     * @brief Расчет расстояния между точкой отражения и воздушным судном.
     * @param s Сигнал.
     * @param a Воздушное судно.
     * @return Расстояние между точкой отражения и воздушным судном.
     */
    static float l_air(Signal s, Aircraft a) {
        return -e_radius * k(s) + std::sqrt(e_radius * e_radius * k(s) * k(s) + 2 * e_radius * h_air(a)
                                            + h_air(a) * h_air(a));
    }

    /**
     * @brief Расчет расстояния между источником сигнала и воздушным судном.
     * @param s Сигнал.
     * @param a Воздушное судно.
     * @return Расстояние между источником сигнала и воздушным судном.
     */
    static float r(Signal s, Aircraft a) { return glm::distance(s.get_pos(), a.get_pos()); }

    /**
     * @brief Расчет угла места для воздушного судна.
     * @param s Сигнал.
     * @param a Воздушное судно.
     * @return Угол места для воздушного судна.
     */
    static float eps_air(Signal s, Aircraft a) {
        float l = -pi / 2 + acos(
                (pow(e_radius + h_rad(s), 2) + pow(r(s, a), 2)
                 - pow(e_radius + h_air(a), 2)) / (2 * r(s, a) * (e_radius + h_rad(s))));
        return l;
    }

    /**
     * @brief Расчет угла между воздушным судном, радаром и точкой отражения.
     * @param s Сигнал.
     * @param a Воздушное судно.
     * @return Угол между воздушным судном, радаром и точкой отражения.
     */
    static float delta(Signal s, Aircraft a) {
        return acos((l_rad(s) * l_rad(s) + r(s, a) * r(s, a) - l_air(s, a) * l_air(s, a)) / (2 * l_rad(s) * r(s, a)));
    }

    /**
     * @brief Расчет угла места для точки отражения.
     * @param s Сигнал.
     * @param a Воздушное судно.
     * @return Угол места для точки отражения.
     */
    float eps_pointReflection(Signal s, Aircraft a) { return eps_air(s, a) - delta(s, a); }

    /**
     * @brief Расчет изменения фазы.
     * @param s Сигнал.
     * @param a Воздушное судно.
     * @return Изменение фазы.
     */
    float getDPhase(Signal s, Aircraft a) {
        float d_phase = phase_0 + (2 * pi / lambda) * (l_rad(s) + l_air(s, a) - r(s, a));  //phase shift
        return d_phase;
    }

    /**
     * @brief Расчет точки отражения.
     * @param s Сигнал.
     * @param a Воздушное судно.
     * @return Точка отражения.
     */
    glm::vec3 getPointReflection(Signal s, Aircraft a) {
        glm::vec3 l_dop = glm::vec3{(air.get_pos() - sig.get_pos()).x,
                                    (air.get_pos() - sig.get_pos()).y
                                    - r(s, a) * sin(eps_air(s, a))
                                    - l_rad(s) * sin(eps_pointReflection(s, a)),
                                    (air.get_pos() - sig.get_pos()).z};
        glm::vec3 l = sig.get_pos()
                      - (float) l_rad(s) * glm::normalize(l_dop);
        //print(l);
        //std::cout << e_radius << std::endl;
        return l;
    }

public:
    /**
     * @brief Конструктор по умолчанию.
     *
     * Удаляет конструктор по умолчанию.
     */
    moduleReflection() = delete;

    /**
     * @brief Конструктор инициализации.
     *
     * Инициализирует модуль отражений сигнала и воздушного судна.
     *
     * @param s Сигнал.
     * @param a Воздушное судно.
     */
    moduleReflection(Signal s, Aircraft a) : sig(s), air(a) {}

    /**
     * @brief Отражение сигнала.
     *
     * Отражает сигнал от воздушного судна и изменяет его характеристики.
     *
     * @return Отраженный сигнал.
     */
    Signal reflect() {
        try {
            Signal new_sig(sig);
            new_sig.shift_phase(getDPhase(sig, air));
            new_sig.change_src(getPointReflection(sig, air));
            new_sig.change_dir(
                    glm::normalize(glm::reflect(sig.get_dir(), glm::normalize(getPointReflection(sig, air) - e_pos))));
            new_sig.change_power(r(sig, air));
            new_sig.change_reflect();
            //new_sig.print();
            isValidVector(new_sig.get_dir());
            isValidVector(new_sig.get_pos());
            glm::vec3 new_pos = new_sig.get_pos();
            if(abs(new_pos.x) > 10000.0 ||
                    abs(new_pos.y) > 10000.0 ||
                    abs(new_pos.z) > 10000.0)
                throw std::invalid_argument("Vector contains too big value.");
            return new_sig;
        } catch (std::runtime_error &e) {
            // Обработка исключения, если вычисления не удалось выполнить
            //std::cerr << "Error during reflection: " << e.what() << std::endl;
            // Возвращаем исходный сигнал
            return sig;
        } catch (const std::invalid_argument& e) {
            //std::cerr << "Error: " << e.what() << std::endl;
            // Обработка ошибки
            return sig;
        }
    }
};

#endif //RADAR2_MODULE_REFLECTION_H
