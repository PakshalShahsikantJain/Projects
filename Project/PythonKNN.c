/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Author : Pakshal Shashikant Jain 
//  Date : 11/04/2021
//  Program : Python SciKitLearn KNN Algorithm Tried To implement in C Programming Language
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include<math.h>

int MySklearnKNN(int TrainData[],int TrainTarget[],int TestData[],int TestTarget[]) 
{   
    printf("Inside MySklearn\n");
    Fit(TrainData,TrainTarget);
    Predict(TestData,TrainTarget);
    
}   

int Fit(int data_training[],int target_training[])
{
    printf("Inside Fit\n");

    int i = 0;
}

int Predict(int Data_test[],int Target_Train[]) 
{
    int arr[30];
    int i = 0;
    int label = 0;
    int brr[30];

    printf("Inside Predict\n");

    for(i = 0;i < 5;i++)
    {
        arr[i] = Data_test[i];
        printf("%d\t",arr[i]);
    }

    for(i = 0;i < 5;i++)
    {
        label = Shortest(arr[i],Data_test,Target_Train);
        brr[i] = label;
    }
    return brr;
}

int Shortest(int crr[],int Data_Test[],int Target_Train[]) 
{
    int i = 0;
    int Distance = 0;
    int FinalDistance = 0;
    int minindex = 0;
    int minDistance = 0;
    int FinalMinDistance = 0;

    minDistance = ((crr[i + 1] - crr[i])*(crr[i + 1] - crr[i])) + ((Data_Test[i + 1] - Data_Test[i]) * (Data_Test[i + 1] - Data_Test[i]));
    FinalMinDistance = sqrt(minDistance);

    for(i = 0;i < 5;i++) 
    {
        Distance = ((crr[i + 1] - crr[i])*(crr[i + 1] - crr[i])) + ((Data_Test[i + 1] - Data_Test[i]) * (Data_Test[i + 1] - Data_Test[i])); 
        FinalDistance = sqrt(Distance);
        if(FinalDistance < FinalMinDistance)
        {
            FinalMinDistance = FinalDistance;
            minindex = i;
        }
    }

    return Target_Train[minindex];
}

int main() 
{
    printf("Jay Ganesh........");

    int No = 0;
    int arr[30];
    int brr[30];
    int crr[30];
    int drr[30];

    int i = 0;
    int j = 0;

    printf("\nEnter Elements in Array\n");
    
    for(i = 0;i < 5;i++) 
    {
        scanf("%d",&arr[i]);
    }
    MySklearnKNN(arr,brr,crr,drr);
}