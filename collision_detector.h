#ifndef RADAR2_COLLISION_DETECTOR_H
#define RADAR2_COLLISION_DETECTOR_H

#include <vector>
#include <glm/gtx/intersect.hpp>

#include "constants.h"
#include "signal.h"
#include "object.h"
#include "module_reflection.h"

/**
 * @brief Класс для обнаружения коллизий сигналов.
 */
class collisionDetector {
private:
public:
    /**
 * @brief Проверяет, произошло ли столкновение с объектом.
 *
 * @param sig Сигнал.
 * @param object Объект.
 * @return true, если произошло столкновение, в противном случае - false.
 */
    bool checkCollision(const Signal &sig, const Aircraft &object) {
        float t;
        return glm::intersectRaySphere(sig.get_pos(), sig.get_dir(), object.get_pos(), object.radius * object.radius, t);
    }

/**
 * @brief Находит ближайший объект, с которым произошло столкновение.
 *
 * @param sig Сигнал.
 * @param objects Список объектов.
 * @return Индекс ближайшего объекта, с которым произошло столкновение, или -1, если столкновений не было.
 */
    int findClosestCollision(const Signal &sig, const std::vector<Aircraft> &objects) {
        int closestObjectIndex = -1;
        float minCollisionDistance = std::numeric_limits<float>::max();

        for (size_t i = 0; i < objects.size(); ++i) {
            if (checkCollision(sig, objects[i])) {
                float t;
                glm::intersectRaySphere(sig.get_pos(), sig.get_dir(), objects[i].get_pos(),
                                        objects[i].radius * objects[i].radius, t);
                if (t > 0 && t < minCollisionDistance) {
                    closestObjectIndex = i;
                    minCollisionDistance = t;
                }
            }
        }

        return closestObjectIndex;
    }

/**
 * @brief Обрабатывает столкновение с объектом, создавая отраженные сигналы.
 *
 * @param sig Сигнал.
 * @param object Объект, с которым произошло столкновение.
 * @return Список отраженных сигналов.
 */
    std::vector<Signal> handleCollision(const Signal &sig, const Aircraft &object) {
        moduleReflection mod(sig, object);
        Signal newSig = mod.reflect();

        Signal reflectedSig(newSig);
        reflectedSig.change_src(object.get_pos());
        reflectedSig.change_dir(glm::normalize(-1.f * reflectedSig.get_dir()));

        std::vector<Signal> reflectedSignals = newSig.generateSignals();
        reflectedSignals.insert(reflectedSignals.end(), reflectedSig.generateSignals().begin(),
                                reflectedSig.generateSignals().end());

        return reflectedSignals;
    }

/**
 * @brief Сканирует объекты для обнаружения столкновений с сигналом.
 *
 * @param sig Сигнал.
 * @param objects Список объектов.
 * @return Список новых сигналов после отражения.
 */
    std::vector<Signal> scanObjects(const Signal &sig, const std::vector<Aircraft> &objects) {
        std::vector<Signal> reflectedSignals;

        int closestObjectIndex = findClosestCollision(sig, objects);
        if (closestObjectIndex != -1) {
            reflectedSignals = handleCollision(sig, objects[closestObjectIndex]);
        }

        return reflectedSignals;
    }

};

#endif // RADAR2_COLLISION_DETECTOR_H
