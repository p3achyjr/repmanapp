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

@property (nonatomic,strong) NSArray *beans;
@property (nonatomic,strong) BLBeanStuff *myBeanStuff;
@property (nonatomic,strong) NSString *targetBeanIdentifier;
@property (nonatomic,strong) PTDBean *connectedBean;

@end

@implementation ViewController

@synthesize warningLabel;

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

- (BOOL)parseFields {
    repsPerSet = self.repsPerSetField.text.intValue;
    setsPerExercise = self.setsPerExerciseField.text.intValue;
    restBetweenSets = self.restField.text.intValue;
    
    if (repsPerSet == 0 || setsPerExercise == 0 || restBetweenSets == 0) {
        return NO;
    }
    
    return YES;
}

- (IBAction)onUpload:(id)sender {
    BOOL success = [self parseFields];
    if (success == NO) {
        self.warningLabel.text = @"Please enter valid values";
        return;
    }
    NSString *stuff = [NSString stringWithFormat:@"%d,%d,%d,\n", repsPerSet, setsPerExercise, restBetweenSets];
    if (self.connectedBean != NULL) {
        [self.connectedBean sendSerialData:[stuff dataUsingEncoding:NSASCIIStringEncoding]];
    }
}

- (void) connect {
    NSUUID *beanID=[[NSUUID alloc] initWithUUIDString:self.targetBeanIdentifier];
    
    if (![self.myBeanStuff connectToBeanWithIdentifier:beanID] ) {  // Connect directly if we can
        [self.myBeanStuff startScanningForBeans];                   // Otherwise scan for the bean
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.targetBeanIdentifier = @"3C3100BB-39D1-7EAF-6504-86F561D73348";
    
    self.myBeanStuff=[BLBeanStuff sharedBeanStuff];
    self.myBeanStuff.delegate=self;
    
    [self.myBeanStuff startScanningForBeans];
    self.connectedBean = NULL;
    NSLog(@"WORKKKKK");
}

- (void) didConnectToBean:(PTDBean *)bean {
    bean.delegate = self;
    self.connectedBean = bean;
    self.warningLabel.text = @"";
}

- (void) didUpdateDiscoveredBeans:(NSArray *)discoveredBeans withBean:(PTDBean *)newBean
{
    if ([newBean.identifier.UUIDString isEqualToString:self.targetBeanIdentifier]) {
        [self connect];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
