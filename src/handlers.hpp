#include "main.h"
template<class T>
void draw_collection(T begin, T end, glm::mat4 &VP) {
    for(;begin != end; ++begin)
        begin->draw(VP);
    return;
}

template <class It, class T>
It find_collision(It begin, It end, const T& obj, float position) {
    for(;begin != end; ++begin)
        if((abs(begin->box.x - obj.box.x) * 2 < (begin->box.width + obj.box.width)) && (abs(begin->box.y - obj.box.y) * 2 < (begin->box.height + obj.box.height)))
            break;
    return begin;
}
