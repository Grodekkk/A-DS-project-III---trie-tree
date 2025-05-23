//TODO: understand pointers deeply, spend even hour relearning stuff because you won't understand a thing either way.
//TODO: when adding values bigger or smaller than minmax or any other, skip

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


//=============================================================================================================================
//======================================= INSERT COMMAND ======================================================================
//=============================================================================================================================

//main function
void insertKey(int key)
{
    printf("I should insert %d\n", key);
}


//=============================================================================================================================
//======================================= LOOK COMMAND ========================================================================
//=============================================================================================================================

//main function
void lookForKey(int key)
{
    printf("I should look for %d\n", key);
}


//=============================================================================================================================
//======================================= PRINT COMMAND =======================================================================
//=============================================================================================================================

//main function
void printTree()
{
    printf("I should print the tree\n");
}


//=============================================================================================================================
//======================================= DELETE COMMAND ======================================================================
//=============================================================================================================================

//main function
void deleteKey(int key)
{
    printf("I should delete %d\n", key);
}

//=============================================================================================================================
//======================================= MAIN FUNCTION =======================================================================
//=============================================================================================================================

int main()
{
    //get information about the data
    MetaInfo TreeInfo;
    scanf("%d", &TreeInfo.numOfTests);                     
    scanf("%d %d", &TreeInfo.minNum, &TreeInfo.maxNum);             
    scanf("%d %d", &TreeInfo.rootConnections, &TreeInfo.childConnections); 
    

    //initialise TRIE TREE (root with no value and array of nullptrs)  //TODO: REDO LIST TYPE
    Root trieRoot;
    trieRoot.value = nullptr;
    trieRoot.descendants = new Child * [TreeInfo.rootConnections];
    for (int i = 0; i < TreeInfo.rootConnections; ++i) 
    {
        trieRoot.descendants[i] = nullptr;
    }

    //read blank line
    scanf("\n");

    //read n number of commands
    for (int i = 0; i < TreeInfo.numOfTests; ++i)
    {
        char command;
        int key;
        scanf(" %c %d", &command, &key);
        
        switch (command)
        {
            case 'I':
                insertKey(key);
                break;
            case 'L':
                lookForKey(key);
                break;
            case 'P':
                printTree();
                break;
            case 'D':
                deleteKey(key);
                break;
            default:
                break;
        }
    }
    
    return 0;
}
