// LensFlare.hpp
// KlayGE Lens Flare header file
// Ver 3.11.0
// Copyright(C) Minmin Gong, 2010
// Homepage: http://www.klayge.org
//
// 3.11.0
// First release (2010.8.21)
//
// CHANGE LIST
/////////////////////////////////////////////////////////////////////////////////

#ifndef _LENSFLARE_HPP
#define _LENSFLARE_HPP

#pragma once

#include <KlayGE/RenderableHelper.hpp>
#include <KlayGE/SceneNodeHelper.hpp>

namespace KlayGE
{
	class KLAYGE_CORE_API LensFlareRenderable : public Renderable
	{
	public:
		LensFlareRenderable();

		void FlareParam(std::vector<float3> const & param, float alpha_fac);

		void OnRenderBegin();
	};

	class KLAYGE_CORE_API LensFlareSceneObject : public SceneNode
	{
	public:
		LensFlareSceneObject();

		void Direction(float3 const & dir);
		float3 const & Direction() const;

		void MainThreadUpdateFunc();

	private:
		float3 dir_;
	};
}

#endif		// _LENSFLARE_HPP
