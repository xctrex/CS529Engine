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

        CommonComponentInitialization(txmlElement);
        
        //================================================================
        // Text specific initialization
        //================================================================
        if (txmlElement->Attribute("TextContent"))
        {
            m_TextContent = txmlElement->Attribute("TextContent");
        }
        if (txmlElement->Attribute("Font"))
        {
            m_Font = txmlElement->Attribute("Font");
        }
        
        //================================================================
        // Transform attributes
        //================================================================
        // Can override values from the parent's transform here.
        // TODO: Not sure if this is desireable
        if (!m_pTransform)
        {
            // Get the transform from the parent, otherwise throw
            m_pTransform = static_cast<Transform*>(m_Parent->GetComponent(COMPONENT_TYPE_TRANSFORM));

            // TODO: Would be better to enforce this in the .xml rather than the code, but this will do for now
            ThrowErrorIf(!m_pTransform, "Parent of a text component must have a transform");
        }        

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
