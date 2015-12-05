#ifndef TRA_TRAJECTORY_H
#define TRA_TRAJECTORY_H
#include <vector>
#include <cstdio>
#include <boost/serialization/access.hpp>
#include "sample_point.h"
#include "utils.h"

class SamplePoint;

class Trajectory {
private:
		friend class boost::serialization::access;

    template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
       ar & tra_id_;
				ar & point_list_;
        ar & tra_counter;
    }

public:
    Trajectory() { tra_id_ = tra_counter++; }
   
    void PushPoint(SamplePoint& point);

    const std::vector<SamplePoint>& point_list() const { return point_list_; }
		const std::vector<SamplePoint>& special_point() const { return special_point_; }
    int id() const { return tra_id_; }
    int point_size() const { return point_list_.size(); } 

private:
		void UpdateSpecialPoint();
    int tra_id_;
    std::vector<SamplePoint> point_list_;
		std::vector<SamplePoint> special_point_;
    static int tra_counter;
};
#endif
