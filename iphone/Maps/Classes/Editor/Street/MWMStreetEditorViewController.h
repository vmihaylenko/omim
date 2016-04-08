#import "MWMTableViewController.h"

#include "indexer/editable_map_object.hpp"

#include "std/string.hpp"
#include "std/utility.hpp"
#include "std/vector.hpp"

using namespace osm;

@protocol MWMStreetEditorProtocol <NSObject>

- (EditableMapObject::TLocalizedStreet const &)currentStreet;
- (void)setNearbyStreet:(EditableMapObject::TLocalizedStreet const &)street;
- (vector<EditableMapObject::TLocalizedStreet> const &)nearbyStreets;

@end

@interface MWMStreetEditorViewController : MWMTableViewController

@property (weak, nonatomic) id<MWMStreetEditorProtocol> delegate;

@end
