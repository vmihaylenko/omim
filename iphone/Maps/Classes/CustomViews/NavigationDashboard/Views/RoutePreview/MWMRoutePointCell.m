//
//  MWMRoutePointCell.m
//  Maps
//
//  Created by v.mikhaylenko on 22.09.15.
//  Copyright © 2015 MapsWithMe. All rights reserved.
//

#import "MWMRoutePointCell.h"

@interface MWMRoutePointCell ()

@property (weak, nonatomic) IBOutlet UIView * moveView;

@end

@implementation MWMRoutePointCell

- (void)awakeFromNib
{
  [super awakeFromNib];
  UIPanGestureRecognizer * pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(didPan:)];
  [self.moveView addGestureRecognizer:pan];
 }

- (void)didPan:(UIPanGestureRecognizer *)sender
{
  [self.delegate didPan:sender cell:self];
}

@end
