#ifndef RADAR2_SIGNAL_H
#define RADAR2_SIGNAL_H

#include <iostream>
#include <glm/glm.hpp>

#include "constants.h"
#include "object.h"

/**
 * @brief Класс, представляющий сигнал радара.
 */
class Signal {
private:
    double phase = 20.0;                 ///< Фаза сигнала.
    double power = init_power * 1.0; ///< Мощность сигнала.

    Source src; ///< Источник сигнала.

    glm::vec3 dir = glm::normalize(glm::vec3{1.0, 1.0, 1.0}); ///< Направление сигнала.

    int reflectionDepth = 1; ///< Глубина отражения.
    int reflection = 0; ///< Количество отражений.

    int fineness_of_coating = 3; ///< Степень покрытия.
public:
/**
* @brief Конструктор по умолчанию.
*/
    Signal() = default;

/**
* @brief Конструктор с параметрами.
* @param p Позиция сигнала.
*/
    explicit Signal(glm::vec3 p) : src{Source{p}} {}

/**
* @brief Конструктор с параметрами.
* @param ph Фаза сигнала.
* @param pw Мощность сигнала.
* @param p Позиция сигнала.
* @param d Направление сигнала.
*/
    Signal(double ph, double pw, glm::vec3 p, glm::vec3 d) :
            phase{ph}, power{pw}, src{Source{p}}, dir{d} {}

    /**
* @brief Конструктор копирования.
* @param sig Сигнал для копирования.
*/
    Signal(Signal const &sig) :
            phase{sig.phase}, power{sig.power}, src{sig.src}, dir{sig.dir} {}

    /**
 * @brief Получить фазу сигнала.
 * @return Фаза сигнала.
 */
    double get_phase() { return phase; }

/**
 * @brief Получить мощность сигнала.
 * @return Мощность сигнала.
 */
    double get_power() { return power; }

/**
 * @brief Получить высоту источника сигнала.
 * @return Высота источника сигнала.
 */
    double get_height() { return src.get_height(); }

/**
 * @brief Получить позицию сигнала.
 * @return Позиция сигнала.
 */
    glm::vec3 get_pos() const { return src.get_pos(); }

/**
 * @brief Получить направление сигнала.
 * @return Направление сигнала.
 */
    glm::vec3 get_dir() const { return dir; }

/**
 * @brief Получить количество отражений сигнала.
 * @return Количество отражений.
 */
    int get_reflected() { return reflection; }

/**
 * @brief Проверить, достигнута ли максимальная глубина отражений.
 * @return true, если достигнута максимальная глубина отражений, иначе false.
 */
    bool isMaxReflection() {
        return reflection == reflectionDepth;
    }

/**
 * @brief Сдвиг фазы сигнала.
 * @param d_phase Сдвиг фазы.
 */
    void shift_phase(double d_phase) {
        phase = std::fmod((phase + d_phase), 2 * pi);
    }

/**
 * @brief Изменить мощность сигнала.
 * @param r Коэффициент изменения мощности.
 */
    void change_power(double r) {
        power /= r * r;
    }

/**
 * @brief Изменить источник сигнала.
 * @param new_src Новый источник сигнала.
 */
    void change_src(glm::vec3 new_src) {
        src = Source{new_src};
    }

/**
 * @brief Изменить направление сигнала.
 * @param new_dir Новое направление сигнала.
 */
    void change_dir(glm::vec3 new_dir) {
        dir = glm::normalize(new_dir);
    }

/**
 * @brief Изменить количество отражений.
 */
    void change_reflect() {
        if (isMaxReflection()) {
            throw std::runtime_error("Signal: maximum reflection depth already has been reached");
        } else {
            reflection++;
            return;
        }
    }

/**
 * @brief Вывести информацию о сигнале.
 */
    void print() const {
        std::cout //<< "position: "
                << "( " << get_pos().x
                << ", " << get_pos().y
                << ", " << get_pos().z
                << "), ";
        //<< "direction: "
        //<< "( " << get_dir().x
        //<< ", " << get_dir().y
        //<< ", " << get_dir().z
        //<< ") power: "
        //<< get_power() << std::endl;
    }

/**
 * @brief Сгенерировать новые сигналы на основе текущего.
 * @return Вектор новых сигналов.
 */
    std::vector<Signal> generateSignals() {
        std::vector<Signal> signals(0);

        double new_power = 0;
        for (int j = 1; j < fineness_of_coating + 1; j++) {
            new_power += 2 * j * j;
        }

        for (int j = 1; j < fineness_of_coating + 1; j++) {
            double rad = sin(j * pi / (2 * fineness_of_coating));
            double y = cos(j * pi / (2 * fineness_of_coating));
            int count = 2 * j; // Varying number of signals per iteration based on 'j'

            for (int i = 0; i < count; ++i) {
                double angle = 2 * pi / count * i;
                double x = cos(angle) * rad * sqrt(1 - y * y); // Adjusted x coordinate for half of the sphere
                double z = sin(angle) * rad * sqrt(1 - y * y); // Adjusted y coordinate for half of the sphere
                glm::vec3 signal_direction = glm::normalize(dir + glm::vec3(x, y, z));
                signals.push_back(Signal(0.0, new_power, src.get_pos() + glm::vec3(x, y, z), signal_direction));
            }
        }

        return signals;
    }

};

#endif //RADAR2_SIGNAL_H
