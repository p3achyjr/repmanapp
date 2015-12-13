//
//  ViewController.m
//  RepmanApp
//
//  Created by Anatol Liu on 12/12/15.
//  Copyright © 2015 TheBestTeam. All rights reserved.
//

#import "ViewController.h"
#import "BLBeanStuff.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UITextField *repsPerSetField;
@property (weak, nonatomic) IBOutlet UITextField *setsPerExerciseField;
@property (weak, nonatomic) IBOutlet UITextField *restField;

@end

@implementation ViewController

int repsPerSet = 0;
int setsPerExercise = 0;
int restBetweenSets = 0;

- (void)parseFields {
    repsPerSet = self.repsPerSetField.text.intValue;
    setsPerExercise = self.setsPerExerciseField.text.intValue;
    restBetweenSets = self.restField.text.intValue;

}

- (IBAction)onUpload:(id)sender {
    [self parseFields];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
