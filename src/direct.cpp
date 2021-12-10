#include "integrator.h"
#include "scene.h"

class DirectIntegrator : public Integrator {
public:
    DirectIntegrator(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {
        /** TODO : Find the surface that is visible in the requested direction
                   Return its ambiant color */
        Hit* hit = new Hit();
        scene->intersect(ray,*hit);

        if(hit->shape() != NULL){
            
            float r = abs(hit->normal()[0]);
            float g = abs(hit->normal()[1]);
            float b = abs(hit->normal()[2]);
            Color3f color = Color3f(r,g,b);
            delete(hit);
            return color;

        }
        delete(hit);
        return Color3f(0.f);
    }

    std::string toString() const {
        return "DirectIntegrator[]";
    }
};

REGISTER_CLASS(DirectIntegrator, "direct")
