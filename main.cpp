#include <iostream>
#include <vector>

#include "constants.h"
#include "signal.h"
#include "object.h"
#include "radar.h"
#include "collision_detector.h"

int main() {
    // Create radar and aircraft objects
    Radar rad { glm::vec3{0.0, 20.0, 0.0} };
    rad.change_dir(glm::normalize(glm::vec3{ 1.0, 0.0, 0.0 }));

    Aircraft air {glm::vec3{11.0, 20.0, 0}};
    air.change_rad(1.0);

    // Create collision detector object
    collisionDetector col;

    // Prepare objects vector with the aircraft object
    std::vector<Aircraft> objects(1, air);

    // Send signals from the radar
    std::vector<Signal> signals = rad.send_signals(0, init_power);

    // Print initial signals
    std::cout << "Initial signals:" << std::endl;
    //std::cout << signals.size() << std::endl;
    for (const auto& sig : signals)
        sig.print();

    // Check for collisions and print collided signals
    std::cout << "\nSignals after initial collision check:" << std::endl;
    std::vector<Signal> collided_signals;
    for (const auto& sig : signals) {
        for(const auto& new_sig : col.scanObjects(sig, objects)) {
            collided_signals.push_back(new_sig);
            new_sig.print();
        }
    }

    //std::cout << collided_signals.size() << std::endl;

    // Check for collisions again if needed
    std::cout << "\nSignals after secondary collision check:" << std::endl;
    std::vector<Signal> secondary_collided_signals;
    for (const auto& sig : collided_signals) {
        for(const auto& new_sig : col.scanObjects(sig, objects)) {
            secondary_collided_signals.push_back(new_sig);
            //new_sig.print();
        }
    }
    //std::cout << secondary_collided_signals.size() << std::endl;

    return 0;
}
