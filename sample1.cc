// basic hfsm sample

#include <iostream>
#include "fsm.hpp"

// common triggers (verbs). you must provide these for all FSMs
fsm::state begin("begin"), end("end"), pause("pause"), resume("resume");

// custom states (gerunds) and triggers (verbs)
fsm::state walking("walking"), attacking("attacking");
fsm::state update("udpate");

class ant {
public:
        fsm::stack fsm;
        int health, distance, flow;

        ant() : health(0), distance(0), flow(1) {
            // set initial fsm state
            fsm = walking;

            // define fsm transitions: on(state,trigger) -> do lambda
            fsm.on(walking,resume) = [&]( const fsm::args &args ) {
                std::cout << "pre-walk! distance:" << distance << std::endl;
            };
            fsm.on(walking,update) = [&]( const fsm::args &args ) {
                std::cout << "\r" << "\\|/-"[ distance % 4 ] << " walking " << (flow > 0 ? "-->" : "<--") << " ";
                distance += flow;
                if( 1000 == distance ) {
                    std::cout << "at food!" << std::endl;
                    flow = -flow;
                }
                if( -1000 == distance ) {
                    std::cout << "at home!" << std::endl;
                    flow = -flow;
                }
            };
            fsm.on(attacking,begin) = [&]( const fsm::args &args ) {
                std::cout << "pre-attack!" << std::endl;
                health = 1000;
            };
            fsm.on(attacking,update) = [&]( const fsm::args &args ) {
                std::cout << "\r" << "\\|/-$"[ distance % 4 ] << " fighting !! hp:(" << health << ")   ";
                --health;
                if( health < 0 ) {
                    std::cout << std::endl;
                    fsm.pop();
                }
            };
        }
};

int main( int argc, const char **argv ) {
    // hfsm ant sample
    ant mini;
    for(;;) {
        if( 0 == rand() % 10000 ) mini.fsm.push(attacking);
        mini.fsm(update());
    }
}
