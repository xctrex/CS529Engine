/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: LineDrawing.cpp
Purpose: Implementation file for graphical line drawing component
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "LineDrawing.h"
#include "RigidBody.h"
#include "GraphicsSystem.h"

namespace Framework
{
    LineDrawing::LineDrawing()
    {
        m_Type = COMPONENT_TYPE_LINE_DRAWING;

        g_ComponentHandleTable[this->GetHandleIndex()] = this;
    }

    void LineDrawing::Destroy()
    {
        g_GRAPHICS->m_LineDrawingList.remove(ComponentHandle(*this));
    }

    void LineDrawing::Initialize(tinyxml2::XMLElement *txmlElement)
    {
        InitializeAttribute(txmlElement, m_Name, "Name");

        
        //================================================================
        // RigidBody attributes
        //================================================================
        // Can override values from the parent's transform here.
        // TODO: Not sure if this is desireable
        if (!m_hRigidBody.ToComponent())
        {
            // Get the transform from the parent, otherwise throw
            m_hRigidBody = m_Parent->GetComponentHandle(COMPONENT_TYPE_RIGID_BODY);

            // TODO: Would be better to enforce this in the .xml rather than the code, but this will do for now
            ThrowErrorIf(!m_hRigidBody.ToComponent(), "Parent of a line component must have a rigid body");
        }

        if (m_RecursionLevel == 0)
        {
            g_GRAPHICS->m_LineDrawingList.push_back(ComponentHandle(*this));
        }
        else
        {
            --m_RecursionLevel;
        }
    }
    
    void LineDrawing::Draw(ComPtr<ID2D1DeviceContext> sp_DeviceContext, ComPtr<ID2D1SolidColorBrush> sp_Brush)
    {
        // Convert the position from world coordinates to window coordinates
        Vector2D P0WindowCoords = g_GRAPHICS->WorldCoordsToWindowCoords(static_cast<RigidBody*>(m_hRigidBody.ToComponent())->GetP0());

        Vector2D P1WindowCoords = g_GRAPHICS->WorldCoordsToWindowCoords(static_cast<RigidBody*>(m_hRigidBody.ToComponent())->GetP1());

        sp_DeviceContext->DrawLine(
            D2D1::Point2F(
                P0WindowCoords.x,
                P0WindowCoords.y
                ),
            D2D1::Point2F(
                P1WindowCoords.x,
                P1WindowCoords.y
                ),
            sp_Brush.Get(),
            3.0f
            );
    }
}
