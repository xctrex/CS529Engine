#include "tinyXML2\tinyxml2.h"

namespace Framework
{
    class Component
    {
    public:
        virtual void Initialize(tinyxml2::XMLElement *txmlElement) = 0;
        virtual void SendEvent() = 0;
        virtual void OnEvent() = 0;
        virtual void Serialize(tinyxml2::XMLDocument *txmlDoc) = 0;
    };
}