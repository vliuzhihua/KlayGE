/**
 * @file D3D12GraphicsBuffer.hpp
 * @author Minmin Gong
 *
 * @section DESCRIPTION
 *
 * This source file is part of KlayGE
 * For the latest info, see http://www.klayge.org
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * You may alternatively use this source under the terms of
 * the KlayGE Proprietary License (KPL). You can obtained such a license
 * from http://www.klayge.org/licensing/.
 */

#ifndef _D3D12GRAPHICSBUFFER_HPP
#define _D3D12GRAPHICSBUFFER_HPP

#pragma once

#include <list>

#include <KlayGE/ElementFormat.hpp>
#include <KlayGE/GraphicsBuffer.hpp>
#include <KlayGE/D3D12/D3D12Typedefs.hpp>
#include <KlayGE/D3D12/D3D12RenderView.hpp>
#include <KlayGE/D3D12/D3D12Resource.hpp>

namespace KlayGE
{
	class D3D12GraphicsBuffer : public GraphicsBuffer, public D3D12Resource, public std::enable_shared_from_this<D3D12GraphicsBuffer>
	{
	public:
		D3D12GraphicsBuffer(BufferUsage usage, uint32_t access_hint,
			uint32_t size_in_byte, ElementFormat fmt);

		ID3D12ResourcePtr const & D3DBufferCounterUpload() const
		{
			return buffer_counter_upload_;
		}

		D3D12ShaderResourceViewSimulationPtr const & D3DShaderResourceView() const
		{
			return d3d_sr_view_;
		}

		D3D12UnorderedAccessViewSimulationPtr const & D3DUnorderedAccessView() const
		{
			return d3d_ua_view_;
		}

		void CopyToBuffer(GraphicsBuffer& target) override;
		void CopyToSubBuffer(GraphicsBuffer& target,
			uint32_t dst_offset, uint32_t src_offset, uint32_t size) override;

		virtual void CreateHWResource(void const * init_data) override;
		virtual void DeleteHWResource() override;

		void UpdateSubresource(uint32_t offset, uint32_t size, void const * data) override;

		uint32_t CounterOffset() const
		{
			return counter_offset_;
		}

		D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress() const
		{
			return gpu_vaddr_;
		}

	private:
		void* Map(BufferAccess ba);
		void Unmap();

	private:
		ID3D12ResourcePtr buffer_counter_upload_;
		D3D12ShaderResourceViewSimulationPtr d3d_sr_view_;
		D3D12UnorderedAccessViewSimulationPtr d3d_ua_view_;
		uint32_t counter_offset_;
		D3D12_GPU_VIRTUAL_ADDRESS gpu_vaddr_;

		ElementFormat fmt_as_shader_res_;

		BufferAccess mapped_ba_;
	};
	typedef std::shared_ptr<D3D12GraphicsBuffer> D3D12GraphicsBufferPtr;
}

#endif			// _D3D12GRAPHICSBUFFER_HPP
