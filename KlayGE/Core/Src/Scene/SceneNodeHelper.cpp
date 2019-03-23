// SceneObjectHelper.cpp
// KlayGE һЩ���õĿ���Ⱦ���� ʵ���ļ�
// Ver 3.12.0
// ��Ȩ����(C) ������, 2005-2011
// Homepage: http://www.klayge.org
//
// 3.10.0
// SceneObjectSkyBox��SceneObjectHDRSkyBox������Technique() (2010.1.4)
//
// 3.9.0
// ������SceneObjectHDRSkyBox (2009.5.4)
//
// 2.7.1
// ������RenderableHelper���� (2005.7.10)
//
// 2.6.0
// ������RenderableSkyBox (2005.5.26)
//
// 2.5.0
// ������RenderablePoint��RenderableLine��RenderableTriangle (2005.4.13)
//
// 2.4.0
// ���ν��� (2005.3.22)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KFL/ErrorHandling.hpp>
#include <KFL/Util.hpp>
#include <KFL/Math.hpp>
#include <KlayGE/RenderableHelper.hpp>
#include <KlayGE/SkyBox.hpp>
#include <KlayGE/Mesh.hpp>
#include <KlayGE/Light.hpp>
#include <KlayGE/Camera.hpp>

#include <boost/assert.hpp>

#include <KlayGE/SceneNodeHelper.hpp>

namespace KlayGE
{
	SceneObjectLightSourceProxy::SceneObjectLightSourceProxy(LightSourcePtr const & light)
		: SceneObjectLightSourceProxy(light, CreateMeshFactory<RenderableLightSourceProxy>)
	{
	}

	SceneObjectLightSourceProxy::SceneObjectLightSourceProxy(LightSourcePtr const & light, RenderModelPtr const & light_model)
		: light_(light), light_model_(light_model)
	{
		model_scaling_ = float4x4::Identity();

		for (uint32_t i = 0; i < light_model->NumMeshes(); ++ i)
		{
			checked_pointer_cast<RenderableLightSourceProxy>(light_model->Mesh(i))->AttachLightSrc(light);
		}

		this->RootNode()->OnMainThreadUpdate().Connect([this](float app_time, float elapsed_time)
			{
				KFL_UNUSED(app_time);
				KFL_UNUSED(elapsed_time);

				auto const & node = this->RootNode();

				node->TransformToParent(model_scaling_ * MathLib::to_matrix(light_->Rotation())
					* MathLib::translation(light_->Position()));
				if (LightSource::LT_Spot == light_->Type())
				{
					float radius = light_->CosOuterInner().w();
					node->TransformToParent(MathLib::scaling(radius, radius, 1.0f) * node->TransformToParent());
				}

				node->ForEachRenderable([](Renderable& mesh)
					{
						auto& light_mesh = *checked_cast<RenderableLightSourceProxy*>(&mesh);
						light_mesh.Update();
					});
			});
	}

	SceneObjectLightSourceProxy::SceneObjectLightSourceProxy(LightSourcePtr const & light,
			std::function<StaticMeshPtr(std::wstring_view)> CreateMeshFactoryFunc)
		: SceneObjectLightSourceProxy(light, this->LoadModel(light, CreateMeshFactoryFunc))
	{
	}

	void SceneObjectLightSourceProxy::Scaling(float x, float y, float z)
	{
		model_scaling_ = MathLib::scaling(x, y, z);
	}

	void SceneObjectLightSourceProxy::Scaling(float3 const & s)
	{
		model_scaling_ = MathLib::scaling(s);
	}

	RenderModelPtr SceneObjectLightSourceProxy::LoadModel(LightSourcePtr const & light,
		std::function<StaticMeshPtr(std::wstring_view)> CreateMeshFactoryFunc)
	{
		char const * mesh_name;
		switch (light->Type())
		{
		case LightSource::LT_Ambient:
			mesh_name = "AmbientLightProxy.glb";
			break;

		case LightSource::LT_Point:
		case LightSource::LT_SphereArea:
			mesh_name = "PointLightProxy.glb";
			break;

		case LightSource::LT_Directional:
			mesh_name = "DirectionalLightProxy.glb";
			break;

		case LightSource::LT_Spot:
			mesh_name = "SpotLightProxy.glb";
			break;

		case LightSource::LT_TubeArea:
			mesh_name = "TubeLightProxy.glb";
			break;

		default:
			KFL_UNREACHABLE("Invalid light type");
		}
		return SyncLoadModel(mesh_name, EAH_GPU_Read | EAH_Immutable,
			SceneNode::SOA_Cullable | SceneNode::SOA_Moveable | SceneNode::SOA_NotCastShadow,
			nullptr, CreateModelFactory<RenderModel>, CreateMeshFactoryFunc);
	}


	SceneObjectCameraProxy::SceneObjectCameraProxy(CameraPtr const & camera)
		: SceneObjectCameraProxy(camera, CreateMeshFactory<RenderableCameraProxy>)
	{
	}

	SceneObjectCameraProxy::SceneObjectCameraProxy(CameraPtr const & camera, RenderModelPtr const & camera_model)
		: camera_(camera), camera_model_(camera_model)
	{
		model_scaling_ = float4x4::Identity();

		for (uint32_t i = 0; i < camera_model->NumMeshes(); ++ i)
		{
			checked_pointer_cast<RenderableCameraProxy>(camera_model->Mesh(i))->AttachCamera(camera);
		}

		this->RootNode()->OnMainThreadUpdate().Connect([this](float app_time, float elapsed_time)
			{
				KFL_UNUSED(app_time);
				KFL_UNUSED(elapsed_time);

				auto const & node = this->RootNode();

				node->TransformToParent(model_scaling_ * camera_->InverseViewMatrix());
			});
	}

	SceneObjectCameraProxy::SceneObjectCameraProxy(CameraPtr const & camera,
			std::function<StaticMeshPtr(std::wstring_view)> CreateMeshFactoryFunc)
		: SceneObjectCameraProxy(camera, this->LoadModel(CreateMeshFactoryFunc))
	{
	}

	void SceneObjectCameraProxy::Scaling(float x, float y, float z)
	{
		model_scaling_ = MathLib::scaling(x, y, z);
	}

	void SceneObjectCameraProxy::Scaling(float3 const & s)
	{
		model_scaling_ = MathLib::scaling(s);
	}

	RenderModelPtr SceneObjectCameraProxy::LoadModel(std::function<StaticMeshPtr(std::wstring_view)> CreateMeshFactoryFunc)
	{
		return SyncLoadModel("CameraProxy.glb", EAH_GPU_Read | EAH_Immutable,
			SceneNode::SOA_Cullable | SceneNode::SOA_Moveable | SceneNode::SOA_NotCastShadow,
			nullptr, CreateModelFactory<RenderModel>, CreateMeshFactoryFunc);
	}
}
