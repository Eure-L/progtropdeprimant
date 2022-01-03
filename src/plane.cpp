#include "plane.h"

Plane::Plane()
{
}

Plane::Plane(const PropertyList &propList)
{
    m_position = propList.getPoint("position",Point3f(0,0,0));
    m_normal = propList.getVector("normal",Point3f(0,0,1));
}

Plane::~Plane()
{
}

bool Plane::intersect(const Ray& ray, Hit& hit) const
{
    //printf("plane intersect\n");

    /// TODO
    // Un plan infini peut être décrit par un point a à sa surface et sa normale n.
    // Un point p appartient alors au plan si (p−a)⋅n=0, c'est-à-dire si leur produit scalaire est nul. Comme pour la sphère, 
    // substituez p par l équation du rayon et résolvez l'équation linéaire en t.
    // Quatre cas sont alors à gérer :
        // t infini : le rayon est parallèle et distinct du plan
        // t non-défini : le rayon est confondu avec le plan
        // t<0 : l intersection est derrière la caméra
        // t>0 : intersection est devant la caméra

    float num = (m_position - ray.origin).dot(m_normal);
    float d = ray.direction.dot(m_normal);

    if (d == Epsilon){ 
      return false;
    }

    float t = num/d;

    if (t > 0 && t < hit.t()) {

      hit.setShape(this);
      hit.setT(t);
      hit.setNormal(m_normal);

      // Texturing
      float u = ray.at(hit.t()).x();
      float v = ray.at(hit.t()).y();
      Vector2f uv(u, v);
      hit.setUV(uv);

      return true;
    }
    return false;


}

REGISTER_CLASS(Plane, "plane")
