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
    int* value;
    Child** descendants;
    
};

struct Child
{
    int* value;
    Child** descendants;
};
void insertChild(MetaInfo* TreeInfo, Child* CurrentLevel, int* key)
{
    //check correct place in tree 
    int childDestination = *key % TreeInfo->childConnections;

    //check if that place is not occupied

    //if is, check if it's by the key

    //go one level deeper

    //TODO: maybe while(true) would work better in this case since it consumes less memory
}

//add a new key to the search-> add [I x] command
void insertKey(MetaInfo* TreeInfo, Root* trieRoot, int* key)
{
    //###step 0 check if root is empty###
    if (trieRoot->value == nullptr)
    {
        trieRoot->value = key;
        return;
    }


    //###step 1 check root values###
    if (*trieRoot->value == *key) //key must be passed as pointer, maybe it can be tided up before calling function
    {
        printf("%d, %s\n", *key, " exist");
        return;
    }

    //###step 2: go to the root descendant###
    int rootDestination = *key % TreeInfo->rootConnections;

    //if destination is empty
    if (trieRoot->descendants[rootDestination] == nullptr)
    {
        //create new connection at that point, append value, and add initialise child connections
        trieRoot->descendants[rootDestination] = new Child;
        trieRoot->descendants[rootDestination]->value = new int;
        *trieRoot->descendants[rootDestination]->value = *key;
        trieRoot->descendants[rootDestination]->descendants = new Child * [TreeInfo->childConnections];

        //fill connections with nullptrs
        for (int i = 0; i < TreeInfo->childConnections; i++)
        {
            trieRoot->descendants[rootDestination]->descendants[i] = nullptr;
        }
    }
    else   //go to the child and check from this point everything again
    {
        //propably checkchild(metainfo*, child*, key*)
    //check if there is our key at that point
        if (*trieRoot->descendants[rootDestination]->value == *key)
        {
            printf("%d, %s\n", *key, " exist");
            return;
        }

    //if field is occupied but not by our key, go further into the tree
        

    }
}

int main()
{
    //get information about the data
    MetaInfo TreeInfo;
    scanf("%d", &TreeInfo.numOfTests);                     
    scanf("%d %d", &TreeInfo.minNum, &TreeInfo.maxNum);             
    scanf("%d %d", &TreeInfo.rootConnections, &TreeInfo.childConnections); 
    //printf("%d, %d, %d, %d, %d\n", TreeInfo.numOfTests, TreeInfo.minNum, TreeInfo.maxNum, TreeInfo.rootConnections, TreeInfo.childConnections);

    //initialise TRIE TREE (root with no value and array of nullptrs)
    Root trieRoot;
    trieRoot.value = nullptr;
    trieRoot.descendants = new Child * [TreeInfo.rootConnections];
    for (int i = 0; i < TreeInfo.rootConnections; ++i) 
    {
        trieRoot.descendants[i] = nullptr;
    }

    //read blank line

    //read n number of commands

    
    return 0;
}
