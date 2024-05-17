#ifndef RADAR2_RADAR_H
#define RADAR2_RADAR_H

#include <glm/glm.hpp>
#include <vector>
#include "constants.h"
#include "object.h"
#include "signal.h"


/**
 * @brief Класс, представляющий радар.
 */
class Radar : public Base {
private:
    int fineness_of_coating = 3; /**< Точность покрытия радара */
public:
    const float radius = 10.0f; /**< Радиус радара */
    glm::vec3 dir = glm::normalize(glm::vec3{1.0f, 1.0f, 0.0f}); /**< Направление радара */

    using Base::Base;
    using Base::get_pos;
    using Base::get_height;
    using Base::set_height;
    using Base::print;

    /**
     * @brief Изменяет направление радара.
     * @param new_dir Новое направление радара.
     */
    void change_dir(const glm::vec3 &new_dir) {
        dir = glm::normalize(new_dir);
    }

    /**
     * @brief Отправляет сигналы из радара.
     * @param phase Фаза сигнала.
     * @param power Мощность сигнала.
     * @return Возвращает вектор сигналов, отправленных из радара.
     */
    std::vector<Signal> send_signals(double phase, double power) const {
        std::vector<Signal> signals;

        double total_power_factor = 0;
        for (int j = 1; j <= fineness_of_coating; ++j) {
            total_power_factor += 2 * j * j;
        }

        double new_power = power / total_power_factor;

        for (int j = 1; j <= fineness_of_coating; ++j) {
            double rad = sin(j * pi / (2 * fineness_of_coating));
            double y = cos(j * pi / (2 * fineness_of_coating));
            int count = 2 * j;

            for (int i = 0; i < count; ++i) {
                double angle = 2 * pi / count * i;
                double x = cos(angle) * rad * sqrt(1 - y * y);
                double z = sin(angle) * rad * sqrt(1 - y * y);
                glm::vec3 signal_direction = glm::normalize(dir + glm::vec3(x, y, z));
                signals.push_back(Signal(phase, new_power, get_pos() + glm::vec3(x, y, z), signal_direction));
            }
        }
        return signals;
    }
};


#endif // RADAR2_RADAR_H
