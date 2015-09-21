//
//  MWMRoutePointCell.h
//  Maps
//
//  Created by v.mikhaylenko on 22.09.15.
//  Copyright © 2015 MapsWithMe. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MWMRoutePointCell;

@protocol MWMRoutePointCellDelegate <NSObject>

@required
- (void)didPan:(UIPanGestureRecognizer *)pan cell:(MWMRoutePointCell *)cell;

@end

@interface MWMRoutePointCell : UICollectionViewCell

@property (weak, nonatomic) IBOutlet UITextField * title;
@property (weak, nonatomic) IBOutlet UILabel * number;
@property (weak, nonatomic) id<MWMRoutePointCellDelegate> delegate;

@end
