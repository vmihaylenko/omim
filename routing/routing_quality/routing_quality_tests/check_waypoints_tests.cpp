#include "testing/testing.hpp"

#include "routing/routing_quality/utils.hpp"
#include "routing/routing_quality/waypoints.hpp"

#include <utility>
#include <vector>

using namespace std;
using namespace routing_quality;

UNIT_TEST(RoutingTools_CompareSmoke)
{
  {
    // From office to Aseeva 6.
    RouteParams params;
    params.m_coords = {{55.797232, 37.537774}, {55.806349, 37.528864}};

    vector<Waypoints> candidates;
    Waypoints first;
    first.m_coords = {{55.796763, 37.541387}, {55.799148, 37.535821}, {55.803533, 37.524782}, {55.805564, 37.527700}};
    first.m_factor = 1.0;
    candidates.emplace_back(move(first));

    TEST_EQUAL(CheckWaypoints(move(params), move(candidates)), 0.25, ());
  }

  {
    // From Zelenograd to Domodedovo.
    RouteParams params;
    params.m_coords = {{55.983014, 37.211419}, {55.4208, 37.8936}};

    vector<Waypoints> candidates;

    // Through M-11 and MCA.
    Waypoints first;
    first.m_coords = {{55.997516, 37.238042},
                      {56.007198, 37.285330},
                      {55.887592, 37.480689},
                      {55.835139, 37.395690},
                      {55.571034, 37.692032}};
    first.m_factor = 1.0;
    candidates.emplace_back(move(first));

    // Through M-10 and MCA.
    Waypoints second;
    second.m_coords = {{55.997755, 37.249418},
                       {55.886276, 37.439159},
                       {55.868827, 37.407842},
                       {55.586450, 37.716723},
                       {55.578554, 37.754688}};
    second.m_factor = 1.0;
    candidates.emplace_back(move(second));

    // Through M-10 and Moscow center.
    Waypoints third;
    third.m_coords = {{55.9897496, 37.2696651},
                      {55.8762511, 37.4512971},
                      {55.7828833, 37.5711815},
                      {55.7609266, 37.6008701},
                      {55.7566258, 37.5986119},
                      {55.7497665, 37.6065493},
                      {55.7317331, 37.6206013},
                      {55.7178558, 37.6223762},
                      {55.6561533, 37.6462353},
                      {55.5785543, 37.7546882}};
    third.m_factor = 1.0;
    candidates.emplace_back(move(third));

    TEST_EQUAL(CheckWaypoints(move(params), move(candidates)), 1.0, ());
  }
}
