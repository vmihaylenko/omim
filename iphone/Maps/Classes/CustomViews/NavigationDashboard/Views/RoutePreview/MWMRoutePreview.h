#import "MWMNavigationView.h"

@class MWMNavigationDashboardEntity;

@interface MWMRoutePreview : MWMNavigationView

@property (weak, nonatomic) IBOutlet UIButton * pedestrian;
@property (weak, nonatomic) IBOutlet UIButton * vehicle;

- (void)configureWithEntity:(MWMNavigationDashboardEntity *)entity;
- (void)statePlaning;
- (void)stateError;

- (void)setRouteBuildingProgress:(CGFloat)progress;
- (void)showGoButtonAnimated:(BOOL)show;

@end
