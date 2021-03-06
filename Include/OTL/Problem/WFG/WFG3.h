/*!
Copyright (C) 2014, 申瑞珉 (Ruimin Shen)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "StdWFG.h"
#include "WFG2.h"
#include "Utility.h"
#include "Transform/Degenerate.h"
#include "Shape/Linear.h"

namespace otl
{
namespace problem
{
namespace wfg
{
template <typename _TReal>
class WFG3 : public StdWFG<_TReal>
{
public:
	typedef _TReal TReal;
	typedef StdWFG<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;

	WFG3(const size_t nObjectives, const size_t posGroups = 2, const size_t distDecisions = 20);
	~WFG3(void);
	static void Transition1(TDecision &decision, const size_t nPosDecisions);
	static void Transition2(TDecision &decision, const size_t nPosDecisions);
	static TReal Transition3(const TDecision &decision, const size_t nPosDecisions, TDecision &posDecision);
	static void Shape(const TDecision &decision, const _TReal distance, std::vector<TReal> &objective);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
};

template <typename _TReal>
WFG3<_TReal>::WFG3(const size_t nObjectives, const size_t posGroups, const size_t distDecisions)
	: TSuper(nObjectives, posGroups, distDecisions)
{
}

template <typename _TReal>
WFG3<_TReal>::~WFG3(void)
{
}

template <typename _TReal>
void WFG3<_TReal>::Transition1(TDecision &decision, const size_t nPosDecisions)
{
	WFG2<TReal>::Transition1(decision, nPosDecisions);
}

template <typename _TReal>
void WFG3<_TReal>::Transition2(TDecision &decision, const size_t nPosDecisions)
{
	WFG2<TReal>::Transition2(decision, nPosDecisions);
}

template <typename _TReal>
_TReal WFG3<_TReal>::Transition3(const TDecision &decision, const size_t nPosDecisions, TDecision &posDecision)
{
	return WFG2<TReal>::Transition3(decision, nPosDecisions, posDecision);
}

template <typename _TReal>
void WFG3<_TReal>::Shape(const TDecision &decision, const _TReal distance, std::vector<TReal> &objective)
{
	assert(decision.size() > 0);
	TDecision _decision = decision;
	std::vector<TReal> degenerate(_decision.size(), 0);
	degenerate.front() = 1;
	transform::Degenerate(_decision, degenerate, distance);
	shape::Linear(_decision.begin(), _decision.end(), objective.begin(), objective.end(), (TReal)1);
	Scale(distance, objective);
}

template <typename _TReal>
size_t WFG3<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void WFG3<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	TDecision _decision = decision;
	Normalize(_decision);
	Transition1(_decision, TSuper::GetPosDecisions());
	Transition2(_decision, TSuper::GetPosDecisions());
	TDecision posDecision(TSuper::GetNumberOfObjectives() - 1);
	TReal distance = Transition3(_decision, TSuper::GetPosDecisions(), posDecision);
	Shape(posDecision, distance, objective);
}
}
}
}
