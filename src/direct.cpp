#include "integrator.h"
#include "scene.h"
#include <math.h>

class DirectIntegrator : public Integrator {
public:
    DirectIntegrator(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {

        Hit hit;
        scene->intersect(ray,hit);
        Color3f colorRet = Color3f(0.f);
        LightList list = scene->lightList();

        
        if(hit.foundIntersection()){

            Color3f colorRet = Color3f(0.f);
            
            // ajouter la valeure de chaque source de lumiere.
            for(const Light* light : scene->lightList()){

                Point3f intersection = ray.at(hit.t());
                float dist;
                Vector3f lightDir = light->direction(intersection,&dist);
                
                // // Ombre
                Hit obstruction;
                Ray shadowRay = Ray(intersection+hit.normal()*0.0001,lightDir);
                scene->intersect(shadowRay, obstruction);

                // si obstruction on passe à la lumiere suivante
                if (obstruction.foundIntersection() && obstruction.t() < dist)
                    continue;

                Color3f PHONG = hit.shape()->material()->brdf(-ray.direction,lightDir,hit.normal(),hit.uv());
                float cos = fmax(lightDir.dot(hit.normal()), 0.f);
                colorRet += PHONG*cos*light->intensity(ray.at(hit.t()));
                
            }
            return colorRet;
        }
        return scene->backgroundColor();
    }

    std::string toString() const {
        return "DirectIntegrator[]";
    }
};

REGISTER_CLASS(DirectIntegrator, "direct")
