#ifndef RADAR2_SCENE_H
#define RADAR2_SCENE_H

#include <vector>
#include "signal.h"
#include "object.h"

class Scene {
private:
    Signal sig;
    std::vector<Aircraft> objects {};
    Radar radar;
public:
    Scene() = default;
    Scene(Signal s, std::vector<Aircraft> o, Radar r) : sig(s), objects(o), radar(r) {}

    std::vector<Aircraft>* get_objects() { return &objects; }
};

#endif //RADAR2_SCENE_H
