#include "sphere.h"
#include <iostream>

Sphere::Sphere(float radius)
    : m_radius(radius)
{
}

Sphere::Sphere(const PropertyList &propList)
{
    m_radius = propList.getFloat("radius",1.f);
    m_center = propList.getPoint("center",Point3f(0,0,0));
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray& ray, Hit& hit) const
{
    /// TODO: compute ray-sphere intersection
    Point3f     o = ray.origin;
    Point3f     c = m_center;
    float       r = m_radius;
    Vector3f    d = ray.direction;
    //Vector3f oc = p - center;
    // t² ||d||² + 2t d.(o-c) + ||o-c||² – r² = 0

    // A = t² ||d||²
    float A = d.squaredNorm();
    // B = 2t d.(o-c)
    float B = (2.0*d).dot(o-c);
    // C = ||o-c||² – r²
    float C = (o-c).squaredNorm() - (r*r);

    float discriminant = (B*B) - (4*A*C);
    
    // pas de solution
    if(discriminant < 0){
        return false;
    }

    // Une solution
    else if (discriminant == 0) {
        float point = -B/(2*A);
        //on verifie que le point est plus près que l'intersection déjà existante
        if(point< hit.t()){
            hit.setT(point);
            hit.setNormal(Point3f(ray.at(hit.t())-m_center).normalized());
            hit.setShape(this);
            return true;
        }
        return false;
    }

    // Deux Solutions
    else{
        float point1 = (-B -sqrt(discriminant)) / (2*A);
        float point2 = (-B +sqrt(discriminant)) / (2*A);

        //on verifie que un des points est plus près que l'intersection déjà existante
        if(point1< hit.t() || point2 < hit.t()){
            
            //on garde le point le plus proche
            if(point1<point2)
                hit.setT(point1);
            else
                hit.setT(point2);
            hit.setNormal(Point3f(ray.at(hit.t())-m_center).normalized());
            hit.setShape(this);
            return true;
        }
        return false;
    }

    return false;

    throw RTException("Sphere::intersect not implemented yet.");

    return false;
}

REGISTER_CLASS(Sphere, "sphere")
