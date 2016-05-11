#import "MapsAppDelegate.h"
#import "MWMController.h"
#import "MWMNavigationController.h"
#import "UIViewController+Navigation.h"

#import <SafariServices/SafariServices.h>

@interface MWMNavigationController () <UINavigationControllerDelegate>

@end

@implementation MWMNavigationController

- (UIStatusBarStyle)preferredStatusBarStyle
{
  return UIStatusBarStyleLightContent;
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  self.delegate = self;
}

- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
  if ([viewController isKindOfClass:[SFSafariViewController class]])
  {
    [navigationController setNavigationBarHidden:YES];
    return;
  }

  NSAssert([viewController conformsToProtocol:@protocol(MWMController)], @"Controller must inherit ViewController or TableViewController class");
  id<MWMController> vc = static_cast<id<MWMController>>(viewController);
  [navigationController setNavigationBarHidden:!vc.hasNavigationBar animated:animated];

  if ([navigationController.viewControllers count] > 1)
    [viewController showBackButton];
}

- (BOOL)shouldAutorotate
{
  return YES;
}

@end
