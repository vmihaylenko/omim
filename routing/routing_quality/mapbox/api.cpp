#include "routing/routing_quality/mapbox/api.hpp"

#include "routing/vehicle_mask.hpp"

#include "coding/file_writer.hpp"
#include "coding/serdes_json.hpp"
#include "coding/url_encode.hpp"
#include "coding/writer.hpp"

#include "platform/http_client.hpp"

#include "geometry/latlon.hpp"

#include "base/assert.hpp"
#include "base/logging.hpp"

#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace
{
string const kBaseURL = "https://api.mapbox.com/";
string const kDirectionsApiVersion = "v5";
string const kStylesApiVersion = "v1";
string const kAccessToken = "pk.eyJ1Ijoidm1paGF5bGVua28iLCJhIjoiY2prbzV1OXZpMmt3NzNwbzZjZjNoZDE5cyJ9.mWADIKC24F7spwfGyXsmKQ";

string VehicleTypeToMapboxType(routing::VehicleType type)
{
  using routing::VehicleType;

  switch (type)
  {
  case VehicleType::Car:
    return "mapbox/driving";
  case VehicleType::Pedestrian:
    return "mapbox/walking";
  case VehicleType::Bicycle:
    return "mapbox/cycling";
  case VehicleType::Transit:
  case VehicleType::Count:
    CHECK(false, ());
    return "";
  }

  CHECK_SWITCH();
}

string LatLonsToString(vector<ms::LatLon> const & coords)
{
  ostringstream oss;
  auto const size = coords.size();
  for (auto i = 0UL; i < size; ++i)
  {
    auto const & ll = coords[i];
    oss << ll.lon << "," << ll.lat;
    if (i != size - 1)
      oss << ";";
  }

  oss << ".json";
  return UrlEncode(oss.str());
}

string GetDirectionsURL(routing_quality::RouteParams const & params)
{
  ostringstream oss;
  oss << kBaseURL << "directions/" << kDirectionsApiVersion << "/" << VehicleTypeToMapboxType(params.m_type) << "/";
  oss << LatLonsToString(params.m_coords) << "?";
  oss << "access_token=" << kAccessToken << "&";
  oss << "overview=simplified&" << "geometries=geojson";
  return oss.str();
}

string GetRoutesRepresentationURL(routing_quality::mapbox::Geometry const & mapboxRoute,
                                  routing_quality::mapbox::Geometry const & mapsmeRoute)
{
  using Sink = MemWriter<string>;

  string mapboxData;
  {
    Sink sink(mapboxData);
    coding::SerializerJson<Sink> ser(sink);
    ser(mapboxRoute);
  }

  mapboxData = UrlEncode(mapboxData);

  string mapsmeData;
  {
    Sink sink(mapsmeData);
    coding::SerializerJson<Sink> ser(sink);
    ser(mapsmeRoute);
  }

  mapsmeData = UrlEncode(mapsmeData);

  ostringstream oss;
  oss << kBaseURL << "styles/" << kStylesApiVersion << "/mapbox/streets-v10/static/";
  oss << "geojson(" << mapboxData << ")"
      /*<< ",geojson(" << mapsmeData << ")"*/<< "/auto/1000x1000?" << "access_token=" << kAccessToken;
  return oss.str();
}
}

namespace routing_quality
{
namespace mapbox
{
// static
DirectionsResponce Api::MakeDirectionsRequest(RouteParams const & params)
{
  platform::HttpClient request(GetDirectionsURL(params));
  if (request.RunHttpRequest() && !request.WasRedirected() && request.ErrorCode() == 200)
  {
    auto const & response = request.ServerResponse();
    CHECK(!response.empty(), ());
    DirectionsResponce ret;
    {
      coding::DeserializerJson des(response);
      des(ret);
    }

    return ret;
  }

  CHECK(false, (request.ErrorCode(), request.ServerResponse()));
  return {};
}

// static
void Api::DrawRoutes(Geometry const & mapboxRoute, Geometry const & mapsmeRoute)
{
  platform::HttpClient request(GetRoutesRepresentationURL(mapboxRoute, mapsmeRoute));
  if (request.RunHttpRequest() && !request.WasRedirected() && request.ErrorCode() == 200)
  {
    auto const & response = request.ServerResponse();
    FileWriter writer("/Users/v.mikhaylenko/mapsme/CMAKE/test.png");
    writer.Write(response.data(), response.size());
    return;
  }

  CHECK(false, (request.ErrorCode()));
}
}  // namespace mapbox
}  // namespace routing_quality