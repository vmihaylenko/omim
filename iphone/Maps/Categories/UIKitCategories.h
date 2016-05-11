#import "Macros.h"

static inline CGPoint SubtractCGPoint(CGPoint p1, CGPoint p2)
{
  return CGPointMake(p1.x - p2.x, p1.y - p2.y);
}

static inline CGPoint AddCGPoint(CGPoint p1, CGPoint p2)
{
  return CGPointMake(p1.x + p2.x, p1.y + p2.y);
}

static inline CGPoint MultiplyCGPoint(CGPoint point, CGFloat multiplier)
{
  return CGPointMake(point.x * multiplier, point.y * multiplier);
}

static inline CGFloat LengthCGPoint(CGPoint point)
{
  return (CGFloat)sqrt(point.x * point.x + point.y * point.y);
}

@interface NSObject (Optimized)

+ (NSString *)className;
- (void)performAfterDelay:(NSTimeInterval)delayInSec block:(TMWMVoidBlock)block;

@end

@interface UIColor (HexColor)

+ (UIColor *)colorWithColorCode:(NSString *)colorCode;
+ (UIColor *)applicationBackgroundColor;
+ (UIColor *)applicationColor;
+ (UIColor *)navigationBarColor;

@end

@interface UIView (Coordinates)

@property (nonatomic) CGFloat minX;
@property (nonatomic) CGFloat minY;
@property (nonatomic) CGFloat midX;
@property (nonatomic) CGFloat midY;
@property (nonatomic) CGFloat maxX;
@property (nonatomic) CGFloat maxY;
@property (nonatomic) CGPoint origin;
@property (nonatomic) CGFloat width;
@property (nonatomic) CGFloat height;
@property (nonatomic) CGSize size;

+ (void)animateWithDuration:(NSTimeInterval)duration delay:(NSTimeInterval)delay damping:(double)dampingRatio initialVelocity:(double)springVelocity options:(UIViewAnimationOptions)options animations:(TMWMVoidBlock)animations completion:(void (^)(BOOL finished))completion;
- (void)sizeToIntegralFit;

@end

@interface UIView (Refresh)

- (void)mwm_refreshUI;

@end

@interface UIImageView (IOS7Workaround)

- (void)makeImageAlwaysTemplate;

@end

@interface UIApplication (URLs)

- (void)rateVersionFrom:(NSString *)launchPlaceName;

@end

@interface SolidTouchView : UIView

@end

@interface SolidTouchImageView : UIImageView

@end


typedef void (^MWMAlertViewBlock) (UIAlertView * alertView);
typedef void (^MWMAlertViewCompletionBlock) (UIAlertView * alertView, NSInteger buttonIndex);

@interface UIAlertView (Blocks)

@property (copy, nonatomic) MWMAlertViewCompletionBlock tapBlock;
@property (copy, nonatomic) MWMAlertViewCompletionBlock willDismissBlock;
@property (copy, nonatomic) MWMAlertViewCompletionBlock didDismissBlock;

@property (copy, nonatomic) MWMAlertViewBlock willPresentBlock;
@property (copy, nonatomic) MWMAlertViewBlock didPresentBlock;
@property (copy, nonatomic) MWMAlertViewBlock cancelBlock;

@property (copy, nonatomic) BOOL(^shouldEnableFirstOtherButtonBlock)(UIAlertView * alertView);

@end

@interface UINavigationController (Autorotate)

- (BOOL)shouldAutorotate;
- (NSUInteger)supportedInterfaceOrientations;

@end

@interface UIImage (ImageWithColor)

+ (UIImage *)imageWithColor:(UIColor *)color;

@end

@interface UIViewController (SafariURL)

/*!
 * @discussion Open url in SFSafariViewController or in Safari app (if target version is lower than iOS 9) or just open url by UIApplication's instance method if url's scheme doesn't start with "http://"
 * @param url An URL to be used for opening
 * @return YES if app can to open url, otherwise NO.
*/

- (BOOL)openURL:(NSURL *)url;

@end