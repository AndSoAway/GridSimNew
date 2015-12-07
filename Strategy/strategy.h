#ifndef STRATEGY_STRATEGY_H
#define STRATEGY_STRATEGY_H
#include <vector>
#include <list>

#include "../Grid/grid_panel.h"

#include "../Tra/trajectory.h"
#include "../Tra/sample_point.h"

class GridPanel;

class Strategy {
public:
	Strategy() {	}

	virtual void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates) = 0;	

	virtual void GetCandidateTrajs(const GridPanel* grid_panel, const SamplePoint& point, double dis, std::list<int>& candidates, bool is_end = false);

	virtual ~Strategy() { }
protected:
	virtual void TrajMergeJoin(const std::list<int>& father_trajs, const std::list<int>& child_trajs, std::list<int>& res) const;	
	virtual void TrajMergeUnion(std::list<int>& father_trajs, const std::list<int>& child_trajs) const;
};
#endif
