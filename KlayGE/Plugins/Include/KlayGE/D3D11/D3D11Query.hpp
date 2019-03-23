// D3D11Query.hpp
// KlayGE D3D11��ѯ�� ʵ���ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2009
// Homepage: http://www.klayge.org
//
// 3.8.0
// ���ν��� (2009.1.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D11QUERY_HPP
#define _D3D11QUERY_HPP

#pragma once

#include <KlayGE/Query.hpp>

namespace KlayGE
{
	class D3D11Query
	{
	public:
		D3D11Query();
		virtual ~D3D11Query();

		void SignalFence();
		void WaitForFence();

	protected:
		FencePtr fence_;
		uint64_t fence_val_;
	};

	class D3D11OcclusionQuery : public OcclusionQuery, public D3D11Query
	{
	public:
		D3D11OcclusionQuery();

		void Begin();
		void End();

		uint64_t SamplesPassed();

	private:
		ID3D11QueryPtr query_;
	};

	class D3D11ConditionalRender : public ConditionalRender, public D3D11Query
	{
	public:
		D3D11ConditionalRender();

		void Begin();
		void End();

		void BeginConditionalRender();
		void EndConditionalRender();

		bool AnySamplesPassed();

	private:
		ID3D11PredicatePtr predicate_;
	};

	class D3D11TimerQuery : public TimerQuery, public D3D11Query
	{
	public:
		D3D11TimerQuery();

		void Begin();
		void End();

		double TimeElapsed() override;

	private:
		ID3D11QueryPtr timestamp_disjoint_query_;
		ID3D11QueryPtr timestamp_start_query_;
		ID3D11QueryPtr timestamp_end_query_;
	};

	class D3D11SOStatisticsQuery : public SOStatisticsQuery, public D3D11Query
	{
	public:
		D3D11SOStatisticsQuery();

		void Begin();
		void End();

		uint64_t NumPrimitivesWritten() override;
		uint64_t PrimitivesGenerated() override;

	private:
		ID3D11QueryPtr so_stat_query_;
	};
}

#endif		// _D3D11QUERY_HPP
