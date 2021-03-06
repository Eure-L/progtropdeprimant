#include "light.h"

class PointLight : public Light
{
public:
    PointLight(const PropertyList &propList)
        : Light(propList.getColor("intensity", Color3f(1.f)))
    {
        m_position = propList.getPoint("position", Point3f::UnitX());
    }

    Vector3f direction(const Point3f& x, float* dist = 0) const
    {
        /// TODO
        //throw RTException("PointLight::direction not implemented yet.");
        if (dist != NULL) {
          *dist = (m_position - x).norm();
        }
        return (m_position-x).normalized();
    }

    Color3f intensity(const Point3f& x) const
    {
        /// TODO
        //throw RTException("PointLight::intensity not implemented yet.");

        return m_intensity/((m_position-x)).squaredNorm();
    }

    std::string toString() const {
        return tfm::format(
            "PointLight[\n"
            "  intensity = %s\n"
            "  position = %s\n"
            "]", m_intensity.toString(),
                 ::toString(m_position));
    }

protected:
    Point3f m_position;
};

REGISTER_CLASS(PointLight, "pointLight")
