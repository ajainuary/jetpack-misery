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

template <class A, class B>
bool objects_collide(const A& obj1, const B& obj2) {
    if(detect_collision(obj1.box, obj2.box)) {
        if(obj1.advanced_collision_detection == true && obj2.advanced_collision_detection == true)
        {
            if(poly_intersect(obj1.mesh, obj1.num_vertices, obj1.position.x, obj1.position.y, obj2.mesh, obj2.num_vertices, obj2.position.x, obj2.position.y))
                return true;
            else {
                return false;
            }
        }
        else {
            return true;
        }
    }
    return false;
}

template <class A, class B>
bool collides(const A& x, const B& y) {
    for(auto &p : x.objects)
        for(auto &q : y.objects)
            if(objects_collide(p.first, q.first))
                return true;
    return false;
}
