#include "routing/routing_quality/routing_quality_tool/parse_input_files.hpp"

#include "routing/routing_quality/waypoints.hpp"
#include "routing/routing_quality/utils.hpp"

#include "base/assert.hpp"

#include <string>

#include "3party/gflags/src/gflags/gflags.h"

DEFINE_string(cmd, "", "command:\n"
                       "waypoints - check that maps.me route contains specified waypoints.");

DEFINE_string(routeParamsFile, "", "File contains two or more route points listed as latitude and longitude "
                                   "separated by comma. Each coordinate should be separated by semicolon. "
                                   "Last symbol is a numeric value of routing::VehicleType enum. "
                                   "At least two points and vehicle type are required. "
                                   "All points and vehicle type will be serialized into routing_quality::RouteParams.");

DEFINE_string(waypointsFile, "", "File contains a list of waypoints as latitudes and longitudes separated by "
                                 "semicolon which we expect should be in the route and also a factor value "
                                 "which represents route quality and is in range (0.0, 1.0]. "
                                 "There can be one or more lines, each one will be serialized into "
                                 "routing_quality::Waypoints.");

using namespace std;

int main(int argc, char ** argv)
{
  using namespace routing_quality;

  google::ParseCommandLineFlags(&argc, &argv, true /* remove_flags */);

  auto const & cmd = FLAGS_cmd;
  CHECK(!cmd.empty(), ("cmd parameter is empty"));

  if (cmd == "waypoints")
  {
    auto const & routeParamsFilePath = FLAGS_routeParamsFile;
    CHECK(!routeParamsFilePath.empty(), ());
    auto const & waypointsFilePath = FLAGS_waypointsFile;
    CHECK(!waypointsFilePath.empty(), ());
    CheckWaypoints(SerializeRouteParams(routeParamsFilePath), SerializeWaypoints(waypointsFilePath));
  }
  else
  {
    CHECK(false, ("Incorrect command", cmd));
  }

  return 0;
}
