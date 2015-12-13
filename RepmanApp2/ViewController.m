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

@end

@implementation ViewController

int repsPerSet = 0;
int setsPerExercise = 0;
int restBetweenSets = 0;

- (IBAction)didClickBackground:(id)sender {
    [self.view endEditing:YES];
}

- (IBAction)editingDidEnd:(id)sender {
    [sender resignFirstResponder];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

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
