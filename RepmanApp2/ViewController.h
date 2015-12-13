//
//  ViewController.h
//  RepmanApp
//
//  Created by Anatol Liu on 12/12/15.
//  Copyright © 2015 TheBestTeam. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <UITextFieldDelegate>

@property (weak, nonatomic) IBOutlet UITextField *repsPerSetField;
@property (weak, nonatomic) IBOutlet UITextField *setsPerExerciseField;
@property (weak, nonatomic) IBOutlet UITextField *restField;

- (void)parseFields;
- (IBAction)didClickBackground:(id)sender;
- (BOOL)textFieldShouldReturn:(UITextField *)textField;
- (IBAction)editingDidEnd:(id)sender;

@end

