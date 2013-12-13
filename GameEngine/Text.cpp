#include "Text.h"
#include "Transform.h"
#include "GraphicsSystem.h"

namespace Framework
{
    Text::Text() :
        m_TextContent("DefaultText"),
        m_pTransform(NULL),
        m_Rect({0.0f, 0.0f, 512.0f, 512.0f}),
        m_Font("Gabriola")
    {
        m_Type = COMPONENT_TYPE_TEXT;
        
        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };
    Text::~Text()
    {
        // Only free the transform if it does not belong to the parent of the text
        Transform* pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));
        if (pTransform == NULL)
        {
            if (m_pTransform)
            {
                free(m_pTransform);
            }
        }
    };

    void Text::Initialize(tinyxml2::XMLElement *txmlElement)
    {
        if (!m_pTransform)
        {
            // Get the transform from the parent, otherwise create a new one
            m_pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));
            if (!m_pTransform)
                m_pTransform = new Transform();
        }

        if (txmlElement->Attribute("Archetype"))
        {
            //TODO: add code for parsing Archetype XML
            // Should use a recursive function, something like InitializeArchetype,
            // That can recursively keep looking for more archetypes
            tinyxml2::XMLDocument txmlDoc;
            ThrowErrorIf(
                tinyxml2::XML_SUCCESS != txmlDoc.LoadFile("Assets\\Archetypes.xml"),
                "Failed to load Assets\\Archetypes.xml"
                );

            tinyxml2::XMLElement* txmlRecursiveElement = txmlDoc.FirstChildElement();
            while (txmlRecursiveElement)
            {
                if (strcmp(txmlElement->Attribute("Archetype"), txmlRecursiveElement->Name()) == 0)
                {
                    ++m_RecursionLevel;
                    this->Initialize(txmlRecursiveElement);
                    break;
                }
                txmlRecursiveElement = txmlRecursiveElement->NextSiblingElement();
            }
        }
        if (txmlElement->Attribute("Name"))
        {
            m_Name = txmlElement->Attribute("Name");
        }
        if (txmlElement->Attribute("TextContent"))
        {
            m_TextContent = txmlElement->Attribute("TextContent");
        }
        if (txmlElement->Attribute("PositionX"))
        {
            m_pTransform->m_Position.x = txmlElement->FloatAttribute("PositionX");
            //m_Position.x = txmlElement->FloatAttribute("PositionX");
        }
        if (txmlElement->Attribute("PositionY"))
        {
            m_pTransform->m_Position.y = txmlElement->FloatAttribute("PositionY");
            //m_Position.y = txmlElement->FloatAttribute("PositionY");
        }
        //TODO: add initialization for m_Color
        if (txmlElement->Attribute("Rotation"))
        {
            m_pTransform->m_Rotation = txmlElement->FloatAttribute("Rotation");
            //m_Rotation = txmlElement->FloatAttribute("Rotation");
        }
        if (txmlElement->Attribute("ScaleX"))
        {
            m_pTransform->m_Scale.x = txmlElement->FloatAttribute("ScaleX");
            //m_Scale.x = txmlElement->FloatAttribute("ScaleX");
        }
        if (txmlElement->Attribute("ScaleY"))
        {
            m_pTransform->m_Scale.y = txmlElement->FloatAttribute("ScaleY");
            //m_Scale.y = txmlElement->FloatAttribute("ScaleY");
        }
        if (txmlElement->Attribute("Font"))
        {
            m_Font = txmlElement->Attribute("Font");
        }
        //TODO: add initialization for origin and scale
        

        if (m_RecursionLevel == 0)
        {
            g_GRAPHICS->m_TextList.push_back(*this);
        }
        else
        {
            --m_RecursionLevel;
        }
    }

    void Text::Draw(ComPtr<ID2D1DeviceContext1> sp_DeviceContext, ComPtr<ID2D1SolidColorBrush> sp_Brush, ComPtr<IDWriteFactory2> sp_DWriteFactory)
    {
        sp_DeviceContext->BeginDraw();
        wchar_t content[512];
        swprintf(content, m_TextContent.length(), L"%hs", m_TextContent.c_str());
        Vector2D WindowCoords = g_GRAPHICS->WorldCoordsToWindowCoords(m_pTransform->m_Position);

        m_Rect.left = WindowCoords.x;
        m_Rect.top = WindowCoords.y;

        ComPtr<IDWriteTextFormat> sp_DWriteTextFormat;
        if (strcmp(m_Font.c_str(), "Gabriola") == 0)
        {
            sp_DWriteFactory->CreateTextFormat(
                L"Gabriola",
                NULL,
                DWRITE_FONT_WEIGHT_REGULAR,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                32.0f,
                L"en-us",
                &sp_DWriteTextFormat
                );
        }
        else
        {
            // TODO: actually dynamically choose a font
            sp_DWriteFactory->CreateTextFormat(
                L"Brush Script Std",
                NULL,
                DWRITE_FONT_WEIGHT_REGULAR,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                32.0f,
                L"en-us",
                &sp_DWriteTextFormat
                );
           // swprintf(content, m_TextContent.length(), L"%hs", m_TextContent.c_str());

            
        }
        sp_DeviceContext->DrawTextA(
            content,
            m_TextContent.length(),
            sp_DWriteTextFormat.Get(),
            m_Rect,
            sp_Brush.Get()
            );
        sp_DeviceContext->EndDraw();

    }


    void Text::SetPosition(float x, float y)
    {
        m_pTransform->m_Position.x = x;
        //m_Position.x = x;
        m_pTransform->m_Position.y = y;
        //m_Position.y = y;
    }
}
