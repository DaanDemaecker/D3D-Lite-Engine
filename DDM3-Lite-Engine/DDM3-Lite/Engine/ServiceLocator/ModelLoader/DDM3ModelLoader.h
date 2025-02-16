// D3DModelLoader.h
// This class is the standard model loader, users can replace it by registering another model loader to the service locator

#ifndef D3DModelLoaderIncluded
#define D3DModelLoaderIncluded

// File includes
#include "ModelLoader.h"

// Standard library includes
#include <memory>

namespace DDM3
{
	// Class forward declarations
	class ObjLoader;
	class FbxLoader;

	class DDM3ModelLoader final : public ModelLoader
	{
	public:
		DDM3ModelLoader();
		virtual ~DDM3ModelLoader() override;

		// Load in a 3D model given a file path
		// Parameters:
		//     - path: The path to the model file
		//     - vertices: The vector that will be used to store the vertices
		//     - indices: The vector that will be used to store the indices
		virtual void LoadModel(const std::string& path, std::vector<DDM3::Vertex>& vertices, std::vector<uint32_t>& indices) override;

		// Load in animations given a file path
		// Parameters:
		//     - path: The path to the file
		//     - animationClips: The vector that will be used to store the animationc lips
		virtual void LoadAnimations(const std::string& path, std::vector<std::unique_ptr<AnimationClip>>& animationClips) override;


	private:
		std::unique_ptr<ObjLoader> m_pObjLoader{};

		std::unique_ptr<FbxLoader> m_pFbxLoader{};
	};
}

#endif // !D3DModelLoaderIncluded