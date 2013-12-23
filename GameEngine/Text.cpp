/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Text.cpp
Purpose: Implementation file for text component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "Text.h"
#include "Transform.h"
#include "GraphicsSystem.h"
#include <strsafe.h>

namespace Framework
{
    Text::Text() :
        m_TextContent("DefaultText"),
        m_pTransform(NULL),
		m_Rect(D2D1::RectF(0.0f, 0.0f, 640.0f, 360.0f)),
        m_Font("Segoe UI")
    {
        m_Type = COMPONENT_TYPE_TEXT;
        
        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    };
    Text::~Text()
    {

    };

    void Text::Initialize(tinyxml2::XMLElement *txmlElement)
    {

        CommonComponentInitialization(txmlElement);
        
        //================================================================
        // Text specific initialization
        //================================================================
        InitializeAttribute(txmlElement, m_TextContent, "TextContent");
        InitializeAttribute(txmlElement, m_Font, "Font");
        
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
            g_GRAPHICS->m_TextList.push_back(ComponentHandle(*this));
        }
        else
        {
            --m_RecursionLevel;
        }
    }

    void Text::CreateFontFaceFromFontFile(ComPtr<IDWriteFactory1> &sp_DWriteFactory, ComPtr<IDWriteFontFace> &sp_FontFace)
    {
        // Gets a filename from a application directory
        UINT curDirLength = GetCurrentDirectory(
            MAX_PATH,
            "Assets"
            );

        ThrowErrorIf(curDirLength == 0, "Font Directory not found");
        
        DXThrowIfFailed(
            StringCchCatW(
                L"Assets",
                MAX_PATH,
                L"\\"
                )
            );

        DXThrowIfFailed(
            StringCchCatW(
                L"Assets",
                MAX_PATH,
                (STRSAFE_LPCWSTR)m_Font.c_str()
                )
            );

        ComPtr<IDWriteFontFile> sp_FontFile;
        DXThrowIfFailed(
            sp_DWriteFactory->CreateFontFileReference(
                L"Assets",
                NULL,
                &sp_FontFile
                )
            );
    
        DXThrowIfFailed(
            sp_DWriteFactory->CreateFontFace(
                DWRITE_FONT_FACE_TYPE_TRUETYPE,
                1, // file count
                &sp_FontFile,
                0,
                DWRITE_FONT_SIMULATIONS_NONE,
                &sp_FontFace
                )
            );
    }


    void Text::Draw(ComPtr<ID2D1DeviceContext> sp_DeviceContext, ComPtr<ID2D1SolidColorBrush> sp_Brush, ComPtr<IDWriteFactory1> sp_DWriteFactory)
    {
        // Convert the text content and the font to wide strings
        wchar_t content[512];
        swprintf(content, m_TextContent.length(), L"%hs", m_TextContent.c_str());
        wchar_t font[512];
        swprintf(font, m_TextContent.length(), L"%hs", m_Font.c_str());

        // Convert the position from world coordinates to window coordinates
        Vector2D WindowCoords = g_GRAPHICS->WorldCoordsToWindowCoords(m_pTransform->m_Position);

        m_Rect.left = WindowCoords.x;
        m_Rect.top = WindowCoords.y;
        /*
        ComPtr<IDWriteFontFace> sp_FontFace;
        CreateFontFaceFromFontFile(sp_DWriteFactory, sp_FontFace);
        //sp_DWriteFactory->CreateFontFileReference(
            ComPtr<IDWriteFontFile> ff;
            ff->GetLoader(

        
        ComPtr<IDWriteFontCollection> sp_DWriteFontCollection;
        sp_DWriteFactory->CreateCustomFontCollection(
            sp_DWriteFontCollectionLoader,
            key,
            sizeof(collectionkeysize),
            &sp_DWriteFontCollection
            );
        */
        
        ComPtr<IDWriteTextFormat> sp_DWriteTextFormat;
        DXThrowIfFailed(
            sp_DWriteFactory->CreateTextFormat(
                font,
                NULL,
                DWRITE_FONT_WEIGHT_REGULAR,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                32.0f,
                L"en-us",
                &sp_DWriteTextFormat
            )
        );

        sp_DeviceContext->DrawTextA(
            content,
            m_TextContent.length(),
            sp_DWriteTextFormat.Get(),
            m_Rect,
            sp_Brush.Get()
            );

        DXThrowIfFailed(
            sp_DeviceContext->EndDraw()
            );
    }

    void Text::SetPosition(float x, float y)
    {
        m_pTransform->m_Position.x = x;
        m_pTransform->m_Position.y = y;
    }
}
