#include "integrator.h"
#include "scene.h"
#include <math.h>

class WhittedIntegrator : public Integrator {
public:
    int m_maxrec;
    WhittedIntegrator(const PropertyList &props) {
        m_maxrec = props.getInteger("maxRecursion",1);
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {

        Hit hit;
        scene->intersect(ray,hit);
        Color3f colorRet = Color3f(0.f);
        LightList list = scene->lightList();

        if(ray.recursionLevel > m_maxrec)
            return colorRet;

        if(hit.foundIntersection()){

            Color3f colorRet = Color3f(0.f);
            
            // ajouter la valeure de chaque source de lumiere.
            for(const Light* light : scene->lightList()){

                Point3f intersection = ray.at(hit.t());
                float dist;
                Vector3f lightDir = light->direction(intersection,&dist);
                
                // Ombre
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

            //faire la recursion de la reflexion

            // Rr = Ri - 2 N (Ri . N)
            Vector3f ray2dir = ray.direction - 2 * hit.normal() * ray.direction.dot(hit.normal());  // direction du rayon reflechi
            //Ray ray2(ray.at(hit.t()),ray2dir);
            Ray ray2(ray.at(hit.t())+hit.normal()*0.0001,ray2dir); // decalage pour eviter le buitage
            ray2.recursionLevel = ray.recursionLevel +1;
            float cos = fmax(ray2dir.dot(hit.normal()), 0.f);
            colorRet += Li(scene, ray2) * cos * hit.shape()->material()->reflectivity();

            return colorRet;
        }
        return scene->backgroundColor();
    }

    std::string toString() const {
        return "WhittedIntegrator[]";
    }
};

REGISTER_CLASS(WhittedIntegrator, "whitted")
