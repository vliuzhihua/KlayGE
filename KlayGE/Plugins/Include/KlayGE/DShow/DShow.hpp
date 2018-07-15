// DShow.hpp
// KlayGE DirectShow ���������� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.klayge.org
//
// 2.0.0
// ���ν��� (2003.9.3)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////

#ifndef _DSHOW_HPP
#define _DSHOW_HPP

#pragma once

#include <string>

#include <KlayGE/Show.hpp>

struct IGraphBuilder;
struct IBaseFilter;
struct IMediaControl;
struct IMediaEvent;
struct IVMRSurfaceAllocator9;

namespace KlayGE
{
	class DShowEngine : public ShowEngine
	{
	public:
		DShowEngine();
		~DShowEngine();

		bool IsComplete();

		void Load(std::string const & fileName);
		TexturePtr PresentTexture();

		ShowState State(long msTimeout = -1);

	private:
		std::shared_ptr<IGraphBuilder>		graph_;
		std::shared_ptr<IBaseFilter>		filter_;
		std::shared_ptr<IMediaControl>		media_control_;
		std::shared_ptr<IMediaEvent>		media_event_;
		std::shared_ptr<IVMRSurfaceAllocator9> vmr_allocator_;

	private:
		void Init();
		void Free();

		virtual void DoSuspend() override;
		virtual void DoResume() override;

		void DoPlay();
		void DoStop();
		void DoPause();
	};
}

#endif		// _DSHOW_HPP
