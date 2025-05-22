#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

struct MetaInfo
{
    int numOfTests;
    int rootConnections;
    int childConnections;
    int minNum;
    int maxNum;
};

struct Child;

struct Root
{
    int value;
    Child** descendants;
    
};

struct Child
{
    int value;
    Child** descendants;
};

int main()
{
    //get information about the data
    MetaInfo TreeInfo;
    scanf("%d", &TreeInfo.numOfTests);                     
    scanf("%d %d", &TreeInfo.minNum, &TreeInfo.maxNum);             
    scanf("%d %d", &TreeInfo.rootConnections, &TreeInfo.childConnections); 
    printf("%d, %d, %d, %d, %d\n", TreeInfo.numOfTests, TreeInfo.minNum, TreeInfo.maxNum, TreeInfo.rootConnections, TreeInfo.childConnections);
    return 0;
}
