#include "app.h"
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Renderer/command.h>
#include <Renderer/camera.h>
#include <Renderer/Renderer2D.h>

#include <iostream>


namespace Chaf
{
    AppLayer::AppLayer()
        : Layer("AppLayer")//, m_CameraController(23.0f, 16.0f / 9.0f, CameraType::Perspective)
    {
        
    }

    void AppLayer::OnAttach()
    {
       /*Ref<TriMesh> m_Mesh = CreateRef<TriMesh>();
        m_Mesh->Create("assets/mesh/cyborg/cyborg.obj");
        //m_Mesh->Create(MeshType::Sphere);
        m_Mesh->SetTexture("assets/mesh/cyborg/cyborg_diffuse.png", 0);
        m_Mesh->AddTexture("assets/mesh/planet/mars.png");
        SceneLayer::GetInstance()->PushMesh(m_Mesh);

        Ref<TriMesh> m_Mesh1 = CreateRef<TriMesh>();
        m_Mesh1->Create("assets/mesh/planet/planet.obj");
        //m_Mesh->Create(MeshType::Sphere);
        m_Mesh1->SetTexture("assets/mesh/planet/mars.png", 0);
        m_Mesh1->SetPosition(0.0f, 15.0f, 0.0f);
        SceneLayer::GetInstance()->PushMesh(m_Mesh1);*/


    }

    void AppLayer::OnDetach()
    {
        
    }

    void AppLayer::OnUpdate(Timestep timestep)
    {
        
    }

    void AppLayer::OnImGuiRender()
    {
        
    }

    void AppLayer::OnEvent(Event& event)
    {

    }
}