#import "Common.h"
#import "MWMNavigationDashboardEntity.h"
#import "MWMRoutePointCell.h"
#import "MWMRoutePointLayout.h"
#import "MWMRoutePreview.h"
#import "UIColor+MapsMeColor.h"

@interface MWMRoutePreview () <MWMRoutePointCellDelegate>

@property (weak, nonatomic) IBOutlet NSLayoutConstraint * planingRouteViewHeight;
@property (weak, nonatomic) IBOutlet UIButton * extendButton;
@property (weak, nonatomic) IBOutlet UICollectionView * collectionView;
@property (weak, nonatomic) IBOutlet MWMRoutePointLayout * layout;
@property (weak, nonatomic) IBOutlet UIImageView * arrowImageView;
@property (nonatomic) UIImageView * movingCellImage;

@end

@implementation MWMRoutePreview

- (void)awakeFromNib
{
  [super awakeFromNib];
  [self setupActualPlaningRouteHeight];
  [self.collectionView registerNib:[UINib nibWithNibName:[MWMRoutePointCell className] bundle:nil] forCellWithReuseIdentifier:[MWMRoutePointCell className]];
}

- (void)configureWithEntity:(MWMNavigationDashboardEntity *)entity { }

- (void)remove
{
  [super remove];
  self.pedestrian.enabled = YES;
  self.vehicle.enabled = YES;
}

- (void)statePlaning { }

- (void)stateError { }

- (void)layoutSubviews
{
  [self setupActualPlaningRouteHeight];
  [super layoutSubviews];
}

- (void)showGoButtonAnimated:(BOOL)show
{
  [self layoutIfNeeded];
  self.showGoButton = show;
  [UIView animateWithDuration:kDefaultAnimationDuration animations:^{ [self layoutIfNeeded]; }];
}

#pragma mark - Properties

- (CGRect)defaultFrame
{
  CGRect frame = super.defaultFrame;
  if (IPAD)
    frame.size.width -= frame.origin.x;
  return frame;
}

- (void)setShowGoButton:(BOOL)showGoButton { }

- (CGFloat)visibleHeight
{
  CGFloat height = 0.;
  for (UIView * v in self.subviews)
    if (![v isEqual:self.statusbarBackground])
      height += v.height;
  return height;
}

- (void)setRouteBuildingProgress:(CGFloat)progress { }

- (IBAction)extendTap
{
  BOOL const isExtended = !self.extendButton.selected;
  self.extendButton.selected = isExtended;
  [self layoutIfNeeded];
  [self setupActualPlaningRouteHeight];
  [UIView animateWithDuration:kDefaultAnimationDuration animations:^
  {
    self.arrowImageView.transform = isExtended ? CGAffineTransformMakeRotation(M_PI) : CGAffineTransformIdentity;
    [self layoutIfNeeded];
  }];
}

- (void)setupActualPlaningRouteHeight
{
  BOOL const isPortrait = self.superview.height > self.superview.width;
  CGFloat const height = isPortrait ? 140. : 96.;
  self.planingRouteViewHeight.constant = self.extendButton.selected ? height : 44.;
}

#pragma mark - MWMRoutePointCellDelegate

- (void)didPan:(UIPanGestureRecognizer *)pan cell:(MWMRoutePointCell *)cell
{
  CGPoint const locationPoint = [pan locationInView:self.collectionView];
  static BOOL isNeedToMove;
  static NSIndexPath * indexPathOfMovingCell;

  if (pan.state == UIGestureRecognizerStateBegan)
  {
    self.layout.isNeedToInitialLayout = NO;
    isNeedToMove = NO;
    indexPathOfMovingCell = [self.collectionView indexPathForCell:cell];
    UIGraphicsBeginImageContextWithOptions(cell.bounds.size, NO, 0.);
    [cell drawViewHierarchyInRect:cell.bounds afterScreenUpdates:YES];
    UIImage * image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    self.movingCellImage = [[UIImageView alloc] initWithImage:image];
    [self.collectionView addSubview:self.movingCellImage];
    self.movingCellImage.center = {locationPoint.x - cell.width / 2, locationPoint.y};
    [UIView animateWithDuration:kDefaultAnimationDuration animations:^
    {
      cell.contentView.alpha = 0.;
      CGFloat const scale = 1.05;
      self.movingCellImage.transform = CGAffineTransformMakeScale(scale, scale);
    }];
  }

  if (pan.state == UIGestureRecognizerStateChanged)
  {
    self.movingCellImage.center = {locationPoint.x - cell.width / 2, locationPoint.y};
    NSIndexPath * finalIndexPath = [self.collectionView indexPathForItemAtPoint:locationPoint];
    if (finalIndexPath && ![finalIndexPath isEqual:indexPathOfMovingCell])
    {
      if (isNeedToMove)
        return;
      isNeedToMove = YES;
      [self.collectionView performBatchUpdates:^
      {
        [self.collectionView moveItemAtIndexPath:finalIndexPath toIndexPath:indexPathOfMovingCell];
        indexPathOfMovingCell = finalIndexPath;
      } completion:nil];
    }
    else
    {
      isNeedToMove = NO;
    }
  }

  if (pan.state == UIGestureRecognizerStateEnded)
  {
    self.layout.isNeedToInitialLayout = YES;
    NSIndexPath * finalIndexPath = [self.collectionView indexPathForItemAtPoint:locationPoint];
    if (finalIndexPath && ![finalIndexPath isEqual:indexPathOfMovingCell])
    {
      isNeedToMove = YES;
      cell.contentView.alpha = 1.;
      [self.collectionView performBatchUpdates:^
      {
        [self.collectionView moveItemAtIndexPath:indexPathOfMovingCell toIndexPath:finalIndexPath];
      }
      completion:^(BOOL finished)
      {
        [self.movingCellImage removeFromSuperview];
        self.movingCellImage.transform = CGAffineTransformIdentity;
      }];
    }
    else
    {
      isNeedToMove = NO;
      cell.contentView.alpha = 1.;
      [self.movingCellImage removeFromSuperview];
    }
  }
}

@end

#pragma mark - UICollectionView

@interface MWMRoutePreview (UICollectionView) <UICollectionViewDataSource, UICollectionViewDelegate>

@end

@implementation MWMRoutePreview (UICollectionView)

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
  return 2;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
  MWMRoutePointCell * cell = [collectionView dequeueReusableCellWithReuseIdentifier:[MWMRoutePointCell className] forIndexPath:indexPath];
  cell.number.text = @(indexPath.row + 1).stringValue;
  cell.title.text = !indexPath.row ? @"Current position" : @"";
  cell.delegate = self;
  return cell;
}

@end
