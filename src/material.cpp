#include "material.h"
#include <math.h>

Phong::Phong(const PropertyList &propList)
    : Diffuse(propList.getColor("diffuse",Color3f(0.2)))
{
    m_specularColor = propList.getColor("specular",Color3f(0.9));
    m_reflectivity = propList.getColor("reflectivity",Color3f(0.0));
    m_exponent = propList.getFloat("exponent",0.2);

    std::string texturePath = propList.getString("texture","");
    if(texturePath.size()>0){
        filesystem::path filepath = getFileResolver()->resolve(texturePath);
        loadTextureFromFile(filepath.str());
        setTextureScale(propList.getFloat("scale",1));
        setTextureMode(TextureMode(propList.getInteger("mode",0)));
    }
}

Color3f Phong::brdf(const Vector3f& viewDir, const Vector3f& lightDir, const Normal3f& normal, const Vector2f& uv) const
{
    /// TODO: implement Phong brdf
    // cf dessin cours

    //pd = md
    //ps = ms(cos(alpha)^s) = ms . (max(r.v,0))^s
    // ms: la couleur spéculaire de l'objet, 
    // r: vecteur l reflechit à la normale n
    // r = 2(l.n)*(n-l) 

    
    Vector3f r = 2*(normal.dot(lightDir))*normal-lightDir;

    float a = r.dot(viewDir);

    if(a<0){
      a=0;
    }

    Color3f ps = m_specularColor*pow((fmax(a,0.f)),m_exponent);
    Color3f pd = m_diffuseColor;
    Color3f p = ps + pd;

    //seg faults
    //p = ps + diffuseColor(uv);


    //throw RTException("Phong::brdf not implemented yet.");

    return p;
}

std::string Phong::toString() const {
    return tfm::format(
        "Phong[\n"
        "  diffuse color = %s\n"
        "  specular color = %s\n"
        "  exponent = %f\n"
        "]", m_diffuseColor.toString(),
             m_specularColor.toString(),
             m_exponent);
}

REGISTER_CLASS(Phong, "phong")