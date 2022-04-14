#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

struct polygon 
{ 
	std::vector<int> x; 
	std::vector<int> y; 
};

class Intersect {
public:
  static bool overlap(polygon polygon1, polygon polygon2);


private:

};


bool Intersect::overlap(polygon polygon1, polygon polygon2) {


	// // SEPARATING AXIS THEOREM:
	// If we find an axis that separates the both polygons, then the polygons cannot overlap! This works only for convex polygons!

	// Loop over the sides of first polygon

	double axis[2]			= {};
	double perpendicular[2] = {};
	std::vector<double> projectionsA;
	std::vector<double> projectionsB;
	for (int i = 0; i < polygon1.x.size()-1; i++)
	{
		projectionsA.clear();
		projectionsB.clear();
		// construct normalized projection axis
		// compute axis of polygon sides
		axis[0] = (double)polygon1.x[i + 1] - (double)polygon1.x[i];
		axis[1] = (double)polygon1.y[i + 1] - (double)polygon1.y[i];

		// normalize vector
		double length_axis = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
		axis[0] = axis[0] / length_axis;
		axis[1] = axis[1] / length_axis;

		// find the perpendicular axis [x,y] = [-y,x]
		perpendicular[0] = -axis[1];
		perpendicular[1] =  axis[0];

		// // project all the points on the projection axis
		// Loop over vertices of 1st polygon
		for (int j = 0; j < polygon1.x.size(); j++)
		{
			// dot product maps vertices on 1D-axis
			double dot_product = polygon1.x[j] * perpendicular[0] + polygon1.y[j] * perpendicular[1];
			projectionsA.push_back(dot_product);
		}
		// Loop over vertices of 2nd polygon
		for (int j = 0; j < polygon2.x.size(); j++)
		{
			// dot product maps vertices on 1D-axis
			double dot_product = polygon2.x[j] * perpendicular[0] + polygon2.y[j] * perpendicular[1];
			projectionsB.push_back(dot_product);
		}

		// find min and max values of both projections of the polygons
		double minA, minB, maxA, maxB;
		minA = *std::min_element(projectionsA.begin(), projectionsA.end());
		maxA = *std::max_element(projectionsA.begin(), projectionsA.end());
		minB = *std::min_element(projectionsB.begin(), projectionsB.end());
		maxB = *std::max_element(projectionsB.begin(), projectionsB.end());

		if (maxA < minB || minA > maxB) { 
			return false; // return false, if a separating axis is found and the polygons are not overlapping
		}
	} // end for-loop over the sides of 1st polygon

	// Loop over sides of second polygon
	for (int i = 0; i < polygon2.x.size() - 1; i++)
	{
	// // construct normalized projection axis
		// double axis[2] = {};
		// double perpendicular[2] = {};
		projectionsA.clear();
		projectionsB.clear();

		// compute axis of polygon sides
		axis[0] = (double)polygon2.x[i + 1] - (double)polygon2.x[i];
		axis[1] = (double)polygon2.y[i + 1] - (double)polygon2.y[i];

		// normalize vector
		double length_axis = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
		axis[0] = axis[0] / length_axis;
		axis[1] = axis[1] / length_axis;

		// find the perpendicular axis [x,y] = [-y,x]
		perpendicular[0] = -axis[1];
		perpendicular[1] = axis[0];

		// // project all the points on the projection axis
		// Loop over vertices of 1st polygon
		for (int j = 0; j < polygon1.x.size(); j++)
		{
			// dot product maps vertices on 1D-line
			double dot_product = polygon1.x[j] * perpendicular[0] + polygon1.y[j] * perpendicular[1];
			projectionsA.push_back(dot_product);
		}
		// Loop over vertices of 2nd polygon
		for (int j = 0; j < polygon2.x.size(); j++)
		{
			// dot product maps vertices on 1D-line
			double dot_product = polygon2.x[j] * perpendicular[0] + polygon2.y[j] * perpendicular[1];
			projectionsB.push_back(dot_product);
		}

		// find min and max values of both projections of the polygons
		double minA, minB, maxA, maxB;
		minA = *std::min_element(projectionsA.begin(), projectionsA.end());
		maxA = *std::max_element(projectionsA.begin(), projectionsA.end());
		minB = *std::min_element(projectionsB.begin(), projectionsB.end());
		maxB = *std::max_element(projectionsB.begin(), projectionsB.end());

		if (maxA < minB || minA > maxB) {
			return false; // return false, if a separating axis is found and the polygons are not overlapping
		}		
	} // end for-loop over sides of 2nd polygon


  return true;
}
