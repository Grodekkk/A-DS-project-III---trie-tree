//TODO: understand pointers deeply, spend even hour relearning stuff because you won't understand a thing either way.
//TODO: when adding values bigger or smaller than minmax or any other, skip
//TODO: when test ends with print, it doesnt end
//TODO: changing the child and root structs into one -> TreeNode struct, limits, are not used elsewhere [what about empty root mr]

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

//=============================================================================================================================
//======================================= DATA STRUCTURES =====================================================================
//=============================================================================================================================

struct Child;
struct Node;

//stores information about the test
struct MetaInfo
{
    int numOfTests;
    int rootConnections;
    int childConnections;
    int minNum;
    int maxNum;
};


//root of trie tree
struct Root
{
    bool empty;                                         //true if root contains any key
    int key;                                            //key stored
    int listMaxSize;                                    //max lenght of list
    Node* startOfList;                                  //pointer to first element in the list
};


//every other node of trie tree
struct Child
{
    int key;                                            //key stored
    int listMaxSize;                                    //max lenght of list
    Node* startOfList;                                  //pointer to first element in the list
};


//one element of the list
struct Node
{
    Child* nextTreeNode;                                //next node of tree itself
    Node* nextListNode;                                 //next element in this list
    int listnum;                                        //which element of the list it is eg. 0,1,7 etc.
};


//=============================================================================================================================
//======================================= SHARED/MISC. FUNCTIONS ==============================================================
//=============================================================================================================================

//initialise root node of the tree
void initialiseRoot(MetaInfo* MetaInfo, Root* TreeRoot)
{
    //root starts empty, with empty childrenList, max connections stated in program
    TreeRoot->empty = true;
    TreeRoot->listMaxSize = MetaInfo->rootConnections;
    TreeRoot->startOfList = nullptr;
}

//=============================================================================================================================
//======================================= INSERT COMMAND ======================================================================
//=============================================================================================================================

//handle adding key to the root with empty list TODO: might be the same for child, change metainfo for lenght thats all
void handleRootEmptyListInsert(int key, int mod, MetaInfo* MetaInfo, Root* TreeRoot)
{
    //rootList is no longer empty, we attach new node to it
    Node* NewNode = new Node;
    TreeRoot->startOfList = NewNode;

    //new list node gets its order, next node, pointer to child
    NewNode->listnum = mod;
    NewNode->nextListNode = nullptr;
    NewNode->nextTreeNode = new Child;

    //next treeNode gets key, max children, and empty list
    NewNode->nextTreeNode->key = key;
    NewNode->nextTreeNode->listMaxSize = MetaInfo->childConnections;
    NewNode->nextTreeNode->startOfList = nullptr;
}

//TODO:
//handle adding key to the nonempty root list TODO: bool and different function for going into child
//nullptr serves as false 
//non null ptr serves as true and get's us adress to the child element
bool isKeyInList(int mod,  Root* TreeRoot)
{
    Node* currentNode = TreeRoot->startOfList;
    while (currentNode != nullptr)
    {
        if (currentNode->listnum == mod)
        {
            //and maybe get pointer to this exact field //TODO:
            return true;
        }
        //check if current node is bigger than 
        if (currentNode->listnum > mod)
        {
            return false;
        }

        currentNode = currentNode->nextListNode;
    }
    return false;
}

//helper function initialising New nodes in the list (new TreeNodes also)
void fillOutNewNode(Node* newNode, Node* nextNode, int key, int mod, MetaInfo* MetaInfo)
{
    newNode->listnum = mod;
    newNode->nextListNode = nextNode;
    newNode->nextTreeNode = new Child;
    newNode->nextTreeNode->key = key;
    newNode->nextTreeNode->listMaxSize = MetaInfo->childConnections;
    newNode->nextTreeNode->startOfList = nullptr;
}

//TODO: function of assigning child values to the given pointer, pointer manipulation in list is different but adding child
// is always the same
// 
//add the key to the nonEmpty list (we know our field is empty)  TODO: compilable for root and children -> metainfo to number like int limit
void addKeyToList(int key, int mod, MetaInfo* MetaInfo, Root* TreeRoot)
{
    Node* currentNode;
    Node* previousNode;
    //if there is more than one element
    if (TreeRoot->startOfList->nextListNode != nullptr)
    {
        currentNode = TreeRoot->startOfList->nextListNode;
        previousNode = TreeRoot->startOfList;
    }
    else
    {
        currentNode = TreeRoot->startOfList;
        previousNode = nullptr;
    }



    //=========== ONLY ONE ELEMENT IN LIST=========================
    if (previousNode == nullptr)
    {
        //our node goes to the beggining
        if (currentNode->listnum > mod)
        {
            //save the pointer to beggining of the list
            Node* nextNode = currentNode;

            //update beggining of the list
            TreeRoot->startOfList = new Node;
            fillOutNewNode(TreeRoot->startOfList, nextNode, key, mod, MetaInfo);
            return;
        }
        //new Node will be last
        else if (currentNode->listnum < mod)
        {
            //last node points nowhere
            Node* nextNode = nullptr;
            currentNode->nextListNode = new Node;
            fillOutNewNode(currentNode->nextListNode, nextNode, key, mod, MetaInfo);
            return;
        }
        printf("one element list adding error % d\n", mod);
        return;
    }
    printf("more than one element case: %d\n", mod);

    //============= MORE THAN ONE ELEMENT IN THE LIST ==========================

    //"edge case" -> value inserted is smaller than two smallest values
    if (previousNode->listnum > mod)
    {
        //save the pointer to beggining of the list
        Node* nextNode = previousNode;

        //update beggining of the list
        TreeRoot->startOfList = new Node;
        fillOutNewNode(TreeRoot->startOfList, nextNode, key, mod, MetaInfo);
        return;
    }

    //handling "in between" cases, and "last one" cases
    while (currentNode != nullptr)
    {
        //check if our value can "fit" inbetween
        if (previousNode->listnum < mod && currentNode->listnum > mod)
        {
            previousNode->nextListNode = new Node;
            previousNode->nextListNode->nextListNode = currentNode;
            previousNode->nextListNode->listnum = mod;
            previousNode->nextListNode->nextTreeNode = new Child;
            previousNode->nextListNode->nextTreeNode->key = key;
            previousNode->nextListNode->nextTreeNode->listMaxSize = MetaInfo->childConnections;
            previousNode->nextListNode->nextTreeNode->startOfList = nullptr;
            return;
        }

        //check if it's end of the list
        if (currentNode->nextListNode == nullptr)
        {
            if (currentNode->listnum < mod)
            {
                currentNode->nextListNode = new Node;
                currentNode->nextListNode->nextListNode = nullptr;
                currentNode->nextListNode->listnum = mod;
                currentNode->nextListNode->nextTreeNode = new Child;
                currentNode->nextListNode->nextTreeNode->key = key;
                currentNode->nextListNode->nextTreeNode->listMaxSize = MetaInfo->childConnections;
                currentNode->nextListNode->nextTreeNode->startOfList = nullptr;
                return;
            }
        }

        previousNode = currentNode;
        currentNode = currentNode->nextListNode;
    }
    
    printf("adding this failed %d\n", mod);
}

//main insert function
void insertKey(int key, MetaInfo* MetaInfo, Root* TreeRoot)
{
    //step 1: check if root is empty
    if (TreeRoot->empty == true)
    {
        TreeRoot->empty = false;
        TreeRoot->key = key;
        return;
    }

    //step 2 root list handling
    int div = key / MetaInfo->rootConnections;
    int mod = key % MetaInfo->rootConnections;
    //step 2.1 check if list is empty
    if (TreeRoot->startOfList == nullptr)
    {
        handleRootEmptyListInsert(key, mod, MetaInfo, TreeRoot);
        return;
    }
    else
    {
        //TODO: change this function to pointer returning one, null as false, != null as true, and direction to the child!
        //check if there is node with the same MOD -> can't insert on this level
        if (isKeyInList(mod, TreeRoot) == true)
        {
            //jump to the child node
            printf("this should not be active");
        }
        else
        {
            //add key at "current level"
            addKeyToList(key, mod, MetaInfo, TreeRoot);
            return;
        }
    }

    printf("I should insert %d, but i am not done :(\n", key);
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
//======================================= DEBBUGING FUNCTIONS =================================================================
//=============================================================================================================================

void printRoot(Root* TreeRoot)
{
    printf("value of the root itself: %d\n", TreeRoot->key);
    printf("\n");
    Node* currentNode = TreeRoot->startOfList;

    while (currentNode != nullptr)
    {
        printf("index of arraylist %d\n", currentNode->listnum);
        printf("key of children: %d\n", currentNode->nextTreeNode->key);
        printf("\n");
        currentNode = currentNode->nextListNode;
    }

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
    

    //initialise TRIE TREE root with no value and empty list
    Root Root;
    initialiseRoot(&TreeInfo, &Root);
    
    ////read blank line
    scanf("\n");

    ////read n number of commands
    for (int i = 0; i < TreeInfo.numOfTests; ++i)
    {
        char command;
        int key;
        scanf(" %c %d", &command, &key);
        
        switch (command)
        {
            case 'I':
                insertKey(key, &TreeInfo, &Root);
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
            //for debugging functions
            case '@':
                printRoot(&Root);
                break;
            default:
                break;
        }
    }
    
    return 0;
}


//=============================================================================================================
//================================= DEBUGGING / OLD CODE ======================================================
//=============================================================================================================

//===================== CHECKING THE METAINFO INPUT ===========================================================
//printf("%d, %d, %d, %d, %d\n", TreeInfo.numOfTests, TreeInfo.minNum, TreeInfo.maxNum, TreeInfo.rootConnections, TreeInfo.childConnections);



//===================== CHECKING INPUT COMMANDS ===============================================================
//printf("%c\n", command);
//printf("%d\n", key);



//===================== OLD ARRAY POINTER INSERT SOLUTION =====================================================
//void insertChild(MetaInfo* TreeInfo, Child* CurrentLevel, int* key)
//{
//    //check correct place in tree 
//    int childDestination = *key % TreeInfo->childConnections;
//
//    //check if that place is not occupied
//
//    //if is, check if it's by the key
//
//    //go one level deeper
//
//    //TODO: maybe while(true) would work better in this case since it consumes less memory
//}

//add a new key to the search-> add [I x] command
//void insertKey(MetaInfo* TreeInfo, Root* trieRoot, int* key)
//{
//    //###step 0 check if root is empty###
//    if (trieRoot->value == nullptr)
//    {
//        trieRoot->value = key;
//        return;
//    }
//
//
//    //###step 1 check root values###
//    if (*trieRoot->value == *key) //key must be passed as pointer, maybe it can be tided up before calling function
//    {
//        printf("%d, %s\n", *key, " exist");
//        return;
//    }
//
//    //###step 2: go to the root descendant###
//    int rootDestination = *key % TreeInfo->rootConnections;
//
//    //if destination is empty
//    if (trieRoot->descendants[rootDestination] == nullptr)
//    {
//        //create new connection at that point, append value, and add initialise child connections
//        trieRoot->descendants[rootDestination] = new Child;
//        //create new pointer value and attach its value from keyvalue
//        trieRoot->descendants[rootDestination]->value = new int(*key);
//        trieRoot->descendants[rootDestination]->descendants = new Child * [TreeInfo->childConnections];
//
//        //fill connections with nullptrs
//        for (int i = 0; i < TreeInfo->childConnections; i++)
//        {
//            trieRoot->descendants[rootDestination]->descendants[i] = nullptr;
//        }
//    }
//    else   //go to the child and check from this point everything again
//    {
//        //propably checkchild(metainfo*, child*, key*)
//    //check if there is our key at that point
//        if (*trieRoot->descendants[rootDestination]->value == *key)
//        {
//            printf("%d, %s\n", *key, " exist");
//            return;
//        }
//
//    //if field is occupied but not by our key, go further into the tree
//        
//
//    }
//}


//========================= MAIN TEST CZY JEST W LISCIE =============================//
//Node* n1 = new Node{ nullptr, nullptr, 1 };
//Node* n3 = new Node{ nullptr, nullptr, 3 };
//Node* n5 = new Node{ nullptr, nullptr, 5 };
//n1->nextListNode = n3;
//n3->nextListNode = n5;
//Root.startOfList = n1;
//
//printf("Czy jest 3? %s\n", isKeyInList(3, &Root) ? "TAK" : "NIE"); // TAK
//printf("Czy jest 2? %s\n", isKeyInList(2, &Root) ? "TAK" : "NIE"); // NIE
//printf("Czy jest 6? %s\n", isKeyInList(6, &Root) ? "TAK" : "NIE"); // NIE
