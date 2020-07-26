#pragma once

#include <Engine/core.h>
#include <Scene/primitive.h>
#include <string>

namespace Chaf
{
	class CHAF_API TriMesh
	{
	public:
		TriMesh(const std::string& name = "mesh")
			:m_Type(MeshType::None), m_Name(name), m_Path(" ") {}
		~TriMesh() = default;

		void Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		void Create(const MeshType& type, const uint32_t& sample = 1);
		void Create(const std::string& path);

		void Draw(bool lineMode = false);	//	Draw vertices
	public:
		//	Get attribute
		const size_t GetVerticesNum() { return m_Vertices.size(); }
		const size_t GetTriangleNum() { return m_Triangle.size(); }
		const MeshType GetMeshType() { return m_Type; }
		const std::string GetPathName() { return m_Path; }
		const bool HasNormal() { return m_HasNormal; }
		const bool HasTexCoord() { return m_HasTexCoord; }

	private:
		void GenVertexArray();
	private:
		void CreatePlane(const uint32_t& sample);
		void CreateCube(const uint32_t& sample);
		void CreateSphere(const uint32_t& sample);
		void ObjLoader(const std::string& path);

	private:
		std::string m_Name;
		std::string m_Path = std::string();
		MeshType m_Type;

		//	buffer
		Ref<VertexArray> m_VertexArray;
		//	mesh data
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<Triangle> m_Triangle;
		//	feature
		bool m_HasNormal = false;
		bool m_HasTexCoord = false;
	};
}