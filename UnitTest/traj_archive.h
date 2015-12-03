#ifndef TRAJ_ARCHIVE_H_
#define TRAJ_ARCHIVE_H_
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include "../Tra/trajectory.h"
//#include <vector>
class TrajData{
public:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & trajs;
	}

	std::vector<Trajectory> trajs;
};
#endif
