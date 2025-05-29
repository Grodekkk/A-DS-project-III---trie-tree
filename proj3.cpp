//TODO: understand pointers deeply, spend even hour relearning stuff because you won't understand a thing either way.
//TODO: when adding values bigger or smaller than minmax or any other, skip
//TODO: when test ends with print, it doesnt end
//TODO: changing the child and root structs into one -> TreeNode struct, limits, are not used elsewhere [what about empty root mr]
// // TODO: HOW DIV AND MOD ARE PASSED IN INSERT AND LOOK FUNCTIONS?
//TODO: inserting too big numbers? skip.
//TODO: look is buggy as hell, go for it step by step

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
    Child nextTreeNode;                                //next node of tree itself
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
    //NewNode->nextTreeNode = new Child;

    //next treeNode gets key, max children, and empty list
    NewNode->nextTreeNode.key = key;
    NewNode->nextTreeNode.listMaxSize = MetaInfo->childConnections;
    NewNode->nextTreeNode.startOfList = nullptr;
}

//when cleaning -> merging root as Child node you can merge it with upper funtion
void handleListEmptyListInsert(int key, int mod, MetaInfo* MetaInfo, Child* ChildNode)
{
    //rootList is no longer empty, we attach new node to it
    Node* NewNode = new Node;
    ChildNode->startOfList = NewNode;

    //new list node gets its order, next node
    NewNode->listnum = mod;
    NewNode->nextListNode = nullptr;

    //next treeNode gets key, max children, and empty list
    NewNode->nextTreeNode.key = key;
    NewNode->nextTreeNode.listMaxSize = MetaInfo->childConnections;
    NewNode->nextTreeNode.startOfList = nullptr;
}

//returns pointer to the node with same modulo as key, nullptr if such node don't exist
Node* isKeyInList(int mod, Root* TreeRoot)
{
    Node* currentNode = TreeRoot->startOfList;
    while (currentNode != nullptr)
    {
        if (currentNode->listnum == mod)
        {
            return currentNode;
        }
        //check if current node is bigger than modulo, return null
        if (currentNode->listnum > mod)
        {
            return nullptr;
        }

        currentNode = currentNode->nextListNode;
    }
    return nullptr;
}

//returns pointer to the node with same modulo as key, nullptr if such node don't exist  TODO: merge with root in future
Node* isKeyInListButForChild(int mod, Child* ChildNode)
{
    Node* currentNode = ChildNode->startOfList;
    while (currentNode != nullptr)
    {
        if (currentNode->listnum == mod)
        {
            return currentNode;
        }
        //check if current node is bigger than 
        if (currentNode->listnum > mod)
        {
            return nullptr;
        }

        currentNode = currentNode->nextListNode;
    }
    return nullptr;
}

//helper function initialising New nodes in the list (new TreeNodes also)
void fillOutNewNode(Node* newNode, Node* nextNode, int key, int mod, MetaInfo* MetaInfo)
{
    newNode->listnum = mod;
    newNode->nextListNode = nextNode;
    newNode->nextTreeNode.key = key;
    newNode->nextTreeNode.listMaxSize = MetaInfo->childConnections;
    newNode->nextTreeNode.startOfList = nullptr;
}

//TODO: function of assigning child values to the given pointer, pointer manipulation in list is different but adding child
// is always the same
//add the key to the nonEmpty list (we know our field is empty)  TODO: compilable for root and children -> metainfo to number like int limit
void addKeyToList(int key, int mod, MetaInfo* MetaInfo, Root* TreeRoot)
{
    //get pointers to first two elements in the list, if there is at least two elements in the list
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
        //TODO: whole inside can be another function
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
    //printf("more than one element case: %d\n", mod);

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
            Node* nextNode = currentNode;
            previousNode->nextListNode = new Node;

            fillOutNewNode(previousNode->nextListNode, nextNode, key, mod, MetaInfo);
            return;
        }

        //check if it's end of the list
        if (currentNode->nextListNode == nullptr)
        {
            if (currentNode->listnum < mod)
            {
                currentNode->nextListNode = new Node;
                Node* nextNode = nullptr;

                fillOutNewNode(currentNode->nextListNode, nextNode, key, mod, MetaInfo);
                return;
            }
        }

        previousNode = currentNode;
        currentNode = currentNode->nextListNode;
    }

    printf("adding this failed %d\n", mod);
}

//same as above, merge it pleasw!!
void addKeyToListChild(int key, int mod, MetaInfo* MetaInfo, Child* ChildNode)
{
    //get pointers to first two elements in the list, if there is at least two elements in the list
    Node* currentNode;
    Node* previousNode;
    //if there is more than one element
    if (ChildNode->startOfList->nextListNode != nullptr)
    {
        currentNode = ChildNode->startOfList->nextListNode;
        previousNode = ChildNode->startOfList;
    }
    else
    {
        currentNode = ChildNode->startOfList;
        previousNode = nullptr;
    }



    //=========== ONLY ONE ELEMENT IN LIST=========================
    if (previousNode == nullptr)
    {
        //if modulo of listStart is higher, we insert key at beggining
        if (currentNode->listnum > mod)
        {
            //save the pointer to beggining of the list
            Node* nextNode = currentNode;

            //update beggining of the list
            ChildNode->startOfList = new Node;
            fillOutNewNode(ChildNode->startOfList, nextNode, key, mod, MetaInfo);
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
        //printf("one element list adding error % d\n", mod); [debug]
        return;
    }
    // printf("more than one element case: %d\n", mod);

     //============= MORE THAN ONE ELEMENT IN THE LIST ==========================

     //"edge case" -> value inserted is smaller than two smallest values
    if (previousNode->listnum > mod)
    {
        //save the pointer to beggining of the list
        Node* nextNode = previousNode;

        //update beggining of the list
        ChildNode->startOfList = new Node;
        fillOutNewNode(ChildNode->startOfList, nextNode, key, mod, MetaInfo);
        return;
    }

    //handling "in between" cases, and "last one" cases
    while (currentNode != nullptr)
    {
        //check if our value can "fit" inbetween
        if (previousNode->listnum < mod && currentNode->listnum > mod)
        {
            Node* nextNode = currentNode;
            previousNode->nextListNode = new Node;

            fillOutNewNode(previousNode->nextListNode, nextNode, key, mod, MetaInfo);
            return;
        }

        //check if it's end of the list
        if (currentNode->nextListNode == nullptr)
        {
            if (currentNode->listnum < mod)
            {
                currentNode->nextListNode = new Node;
                Node* nextNode = nullptr;

                fillOutNewNode(currentNode->nextListNode, nextNode, key, mod, MetaInfo);
                return;
            }
        }

        previousNode = currentNode;
        currentNode = currentNode->nextListNode;
    }

    printf("adding this failed %d\n", mod);
}

//adding keys in levels deeper the RootList
void insertKeyInDeeperLevels(int key, int div, MetaInfo* MetaInfo, Child* ChildN)
{
    //important, key don't change, but we div and modulo change every iteration, div is passed to deeper levels

    Child* ChildNode = ChildN;
    int division = div;
    int level = 1;
    while (true)
    {
        //step 1: check if value in this node is the same as key, //TODO: this can be unnecesarry double checked
        if (ChildNode->key == key)
        {
            printf("%d exist\n", key);
            return;
        }

        //getting new keyOrders, this time we divide the result of previous division
        // MOD -> for order in this exact list   |||    DIV -> to be passed to the next node   ||| key -> value itself
        int mod = division % MetaInfo->childConnections;
        division = division / MetaInfo->childConnections;

        //step 2.1 check if list is empty
        if (ChildNode->startOfList == nullptr)
        {
            handleListEmptyListInsert(key, mod, MetaInfo, ChildNode);
            //printf("%d added at level %d, parrent: %d\n", key, level, ChildNode->key); [DEBUG]
            return;
        }
        else
        {
            //return a pointer to node with same modulo, nullptr if don't exists
            Node* keyInList = isKeyInListButForChild(mod, ChildNode);
            if (keyInList != nullptr)
            {
                //check if pointer itself is our key value
                if (keyInList->nextTreeNode.key == key)
                {
                    printf("%d exist\n", key);
                    return;
                }

                //get one level deeper
                ChildNode = &keyInList->nextTreeNode;
                level++;
                continue;
            }
            else
            {
                //add key at "current level"
                addKeyToListChild(key, mod, MetaInfo, ChildNode);
                //printf("%d added at level %d, parrent: %d\n", key, level, ChildNode->key);  [debug]
                return;
            }
        }
    }
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
    else
    {
        if (TreeRoot->key == key)
        {
            printf("%d exist\n", key);
            return;
        }
    }

    //step 1.5 edge case handling
    if (key > MetaInfo->maxNum || key < MetaInfo->minNum)
    {
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
    //insert at "RootList" level, or deeper
    else
    {
        //returns pointer to the node with same modulo as key, nullptr if such node don't exist
        Node* keyInList = isKeyInList(mod, TreeRoot);
        if (keyInList != nullptr)
        {
            //check if pointer itself is our key value
            if (keyInList->nextTreeNode.key == key)
            {
                printf("%d exist\n", key);
                return;
            }

            //jump to the child node
            insertKeyInDeeperLevels(key, div, MetaInfo, &keyInList->nextTreeNode);
            return;
        }
        //there is no Node with same modulo as our key -> not in RootList
        else
        {
            //add key at "RootList"
            addKeyToList(key, mod, MetaInfo, TreeRoot);
            return;
        }
    }
}


//=============================================================================================================================
//======================================= LOOK COMMAND ========================================================================
//=============================================================================================================================

//helper, iterative in children looking
void lookForKeyInChildren(int key, int div, Child* CurrentChildren, MetaInfo* Info)
{
    Child* CurrentChild = CurrentChildren;
    int division = div;

    while (true)
    {
        //step 1 check if our list is empty
        if (CurrentChild->startOfList == nullptr)
        {
            printf("%d not exist\n", key);
            return;
        }

        //calculate new div and mod
        //TODO: why it work -> on piece of paper and stanford tree
        int mod1 = division % Info->childConnections;
        division = division / Info->childConnections;

        //check if there is node with same mod
        Node* CurrentNode = CurrentChild->startOfList;
        while (CurrentNode != nullptr)
        {
            //if we get into node with same modulo, check if it's our key
            if (CurrentNode->listnum == mod1)
            {
                if (CurrentNode->nextTreeNode.key == key)
                {
                    printf("%d exist\n", key);
                    return;
                }
                else
                {
                    CurrentChild = &CurrentNode->nextTreeNode;
                    break;
                }

            }
            //TODO: think about when it won't work
            //if node is higher, it means we key do not exists.
            if (CurrentNode->listnum > mod1)
            {
                printf("%d not exist\n", key);
                return;
            }
            CurrentNode = CurrentNode->nextListNode;
        }
    }
}

//main function
void lookForKey(int key, Root* TreeRoot, MetaInfo* Info)
{
    //edge case check if root is empty:
    if (TreeRoot->empty == true)
    {
        printf("%d not exist\n", key);
        return;
    }

    //step 1: check root:
    if (TreeRoot->key == key)
    {
        printf("%d exist\n", key);
        return;
    }

    int div = key / Info->rootConnections;
    int mod = key % Info->rootConnections;
    //step 2: rootlist

    //check if there is node with same mod
    Node* CurrentNode = TreeRoot->startOfList;
    while (CurrentNode != nullptr)
    {
        //if we get into node with same modulo, check if it's our key
        if (CurrentNode->listnum == mod)
        {
            if (CurrentNode->nextTreeNode.key == key)
            {
                printf("%d exist\n", key);
                return;
            }
            else
            {
                Child* CurrentChild = &CurrentNode->nextTreeNode;
                lookForKeyInChildren(key, div, CurrentChild, Info);
                return;
            }

        }

        //if node is higher, it means we key do not exists.
        if (CurrentNode->listnum > mod)
        {
            printf("%d not exist\n", key);
            return;
        }
        CurrentNode = CurrentNode->nextListNode;
    }
}


//=============================================================================================================================
//======================================= PRINT COMMAND =======================================================================
//=============================================================================================================================

//iterative function printing current branch of the tree (one children of root and every of his descendant)
void printTreeBranch(Child* CurrentNode)
{
    //print current node
    printf("%d ", CurrentNode->key);

    //get pointer to start of the list
    Node* ChildList = CurrentNode->startOfList;

    //going deeper always has bigger priority
    while (ChildList != nullptr)
    {
        printTreeBranch(&ChildList->nextTreeNode);
        ChildList = ChildList->nextListNode;
    }
}

//main function
void printTree(Root* TreeRoot)
{
    //edge case: root is empty
    if (TreeRoot->empty == true)
    {
        return;
    }

    //print root value
    printf("%d ", TreeRoot->key);

    Node* RootList = TreeRoot->startOfList;
    //maybe do iteration of everynode while every node is different
    while (RootList != nullptr)
    {
        printTreeBranch(&RootList->nextTreeNode);
        RootList = RootList->nextListNode;
    }
    //go for the leftmost and again again again recursion lalalal


    printf("\n");
}


//=============================================================================================================================
//======================================= DELETE COMMAND ======================================================================
//=============================================================================================================================

//============================= COMMANDS FOR LOOKING ==============================================================
//============================= CAN BE MERGED WITH LOOK FUNCTION ==================================================

//checks if key is in rootlist
bool keyInRootList(int key, Root* TreeRoot)
{
    Node* StartOfList = TreeRoot->startOfList;
    //possible modulo check for optimalisation -> return false if modulo is higher

    while (StartOfList != nullptr)
    {
        if (StartOfList->nextTreeNode.key == key)
        {
            return true;
        }

        StartOfList = StartOfList->nextListNode;
    }

    return false;
}

//look for node leftmost child node
//error if returns nullptr
int removeRootHelper(Child* CurrentChildren)
{
    int result;
    //since we know this child have at least one child we can assign values like that
    Child* CurrentChild = &CurrentChildren->startOfList->nextTreeNode;
    Child* PreviousChild = CurrentChildren;

    while (true)
    {
        //check if current node has no children
        if (CurrentChild->startOfList == nullptr)
        {
            //get the key from node
            result = CurrentChild->key;

            //rearange the list in previous Node;
            Node* newStart = PreviousChild->startOfList->nextListNode;
            delete PreviousChild->startOfList;                //delete from list
            PreviousChild->startOfList = newStart;            //update start of the list
            return result;

        }

        PreviousChild = CurrentChild;
        CurrentChild = &CurrentChild->startOfList->nextTreeNode;
    }
}

//root deletion function
void removeRoot(Root* TreeRoot)
{
    //check if root have any children
    if (TreeRoot->startOfList == nullptr)
    {
        TreeRoot->empty = true;
        return;
    }
    else
    {
        int lastChildKey;
        //case 1: leftmost child is childless (1 layer deep)
        if (TreeRoot->startOfList->nextTreeNode.startOfList == nullptr)
        {
            lastChildKey = TreeRoot->startOfList->nextTreeNode.key;
            TreeRoot->key = lastChildKey;
            Node* newStart = TreeRoot->startOfList->nextListNode;
            delete TreeRoot->startOfList;
            TreeRoot->startOfList = newStart;
            return;
        }

        //case 2: leftmost child have childlren (1+ layer deep)
        lastChildKey = removeRootHelper(&TreeRoot->startOfList->nextTreeNode);
        TreeRoot->key = lastChildKey;
    }
}

// find leftmostchild of this node, remove it and return the key to it
int FindAndDeleteLeftMostChild(Node* CurrentNode)
{
    Child* PreviousChild = &CurrentNode->nextTreeNode;
    Child* CurrentChild = &CurrentNode->nextTreeNode.startOfList->nextTreeNode;

    //iterate until finding leftmost child
    while (CurrentChild->startOfList != nullptr)
    {
        PreviousChild = CurrentChild;
        CurrentChild = &CurrentChild->startOfList->nextTreeNode;
    }

    //upon finding, do logic accordingly
    //child is always first in list

    // rearange PreviousChild List
    Node* toDelete = PreviousChild->startOfList;
    int result = CurrentChild->key;
    PreviousChild->startOfList = PreviousChild->startOfList->nextListNode;
    delete toDelete;
    return result;
}

//helper function    Node with value, Node before value
void removeRootListHelper(Node* CurrentNode, Node* PreviousNode, Root* TreeRoot)
{
    //case 1: Node has no children
    if (CurrentNode->nextTreeNode.startOfList == nullptr)
    {
        //we only delete node and rearange list, no need to move keys

        //if Node was first -> update first element
        if (PreviousNode == nullptr)
        {
            TreeRoot->startOfList = CurrentNode->nextListNode;
            delete CurrentNode;
            return;
        }

        //if not update pointer of previous Node 
        PreviousNode->nextListNode = CurrentNode->nextListNode;
        delete CurrentNode;
        return;
    }

    //case 2: Node has 1 Children
    if (CurrentNode->nextTreeNode.startOfList != nullptr)
    {
        //since Node has child, manipulation of RootList nodes is not needed
        int newKey = FindAndDeleteLeftMostChild(CurrentNode);
        CurrentNode->nextTreeNode.key = newKey;
        return;
    }
}

//remove key from RootList
void removeRootList(int key, Root* TreeRoot)
{
    Node* CurrentNode = TreeRoot->startOfList;
    Node* PreviousNode = nullptr;

    while (CurrentNode != nullptr)
    {
        //check if current node has our key
        if (CurrentNode->nextTreeNode.key == key)
        {
            removeRootListHelper(CurrentNode, PreviousNode, TreeRoot);
            return;
        }


        PreviousNode = CurrentNode;
        CurrentNode = CurrentNode->nextListNode;
    }
}

//same as removing nodes with Rootlist
void removeDeeperFoundHelper(Node* CurrentNode, Node* PreviousNode, Child* ChildBeforeKey)
{
    //we start with currentNode -> pointing to Child with key, and previousNode

    //case with our key having no children
    if (CurrentNode->nextTreeNode.startOfList == nullptr)
    {
        //we only delete node and rearange list, no need to move keys

        //if Node was first -> update first element
        if (PreviousNode == nullptr)
        {
            //trzeba podmianke
            ChildBeforeKey->startOfList = CurrentNode->nextListNode;
            delete CurrentNode;
            return;
        }

        //if not update pointer of previous Node 
        PreviousNode->nextListNode = CurrentNode->nextListNode;
        delete CurrentNode;
        return;
    }

    //case 2: Node has 1 Children   //TODO: this is straight up copied form RootList function, need to check if it works correctly
    if (CurrentNode->nextTreeNode.startOfList != nullptr)
    {
        //since Node has child, manipulation of RootList nodes is not needed
        int newKey = FindAndDeleteLeftMostChild(CurrentNode);
        CurrentNode->nextTreeNode.key = newKey;
        return;
    }

}

//handle looking dor key and deletion it in pure Child cases
void removeDeeperHelper(int div, int key, Node* CurrentNode, MetaInfo* Info)
{
    //we start at "rootlist level" of tree, we know modulo is not here, currentChild is at level bellow

    //step 1: find our key
    int division = div;

    //NOTE: check with many cases
    int newDiv = division;
    int newMod = division % Info->childConnections;
    Node* previousNode = nullptr;
    Node* currentNode = CurrentNode->nextTreeNode.startOfList;
    Child* ChildBeforeKey = &CurrentNode->nextTreeNode;

    //iterate through whole tree until you find key (or not)
    while (currentNode != nullptr)
    {
        if (currentNode->listnum == newMod)
        {
            if (currentNode->nextTreeNode.key == key)
            {
                //remove [finally] key from found node
                removeDeeperFoundHelper(currentNode, previousNode, ChildBeforeKey); //very good we pass all of this node parrent data for no child exception
                return;
            }
            else
            {
                ChildBeforeKey = &currentNode->nextTreeNode;
                currentNode = currentNode->nextTreeNode.startOfList;
                previousNode = nullptr;

                newDiv = newDiv / Info->childConnections;
                newMod = newDiv % Info->childConnections;
                continue;
            }
        }

        if (currentNode->listnum > newMod)
        {
            printf("%d not exist\n", key);
            return;
        }


        previousNode = currentNode;
        currentNode = currentNode->nextListNode;
    }

    //if loop broke without giving result it means there is no key
    printf("%d not exist\n", key);
}

//handle removing from deeper levels (not connected to the root)
void removeDeeper(int key, Root* TreeRoot, MetaInfo* Info)
{
    //step 1: look for the key, print "[key] not exist" when not found

    //we check if root is empty, but not rootlist
    if (TreeRoot->startOfList == nullptr)
    {
        printf("%d not exist\n", key);
        return;
    }

    //iterate through Rootlist, we know key is not direct descendant of root (case 2 in main function)
    Node* CurrentNode = TreeRoot->startOfList;
    int mod = key % Info->rootConnections;
    int div = key / Info->rootConnections;

    while (CurrentNode != nullptr)
    {
        //if we reached same modulo, get deeper
        if (CurrentNode->listnum == mod)
        {
            removeDeeperHelper(div, key, CurrentNode, Info);
            return;
        }

        //if higher modulo reached, it means our key do not exists
        if (CurrentNode->listnum > mod)
        {
            printf("%d not exist\n", key);
            return;
        }
        CurrentNode = CurrentNode->nextListNode;
    }

    //if while loop reached end it means there is no our key ex. modulo 7 in list 0->1->4->NULL
    printf("%d not exist\n", key);
}

//main function
void deleteKey(int key, Root* TreeRoot, MetaInfo* Info)
{
    //=== special case TRIE tree is empty ===
    if (TreeRoot->empty == true)
    {
        printf("%d not exist)\n", key);
        return;
    }

    //step 1: removing key from the root
    if (TreeRoot->key == key)
    {
        removeRoot(TreeRoot);
        return;
    }

    //step 2: removing from rootlist  NOTE: is checking like this affects performance?
    if (keyInRootList(key, TreeRoot) == true)
    {
        removeRootList(key, TreeRoot);
        return;
    }

    //step 3: if not in root or Rootlist, look for node
    removeDeeper(key, TreeRoot, Info);

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
            lookForKey(key, &Root, &TreeInfo);
            break;
        case 'P':
            printTree(&Root);
            break;
        case 'D':
            deleteKey(key, &Root, &TreeInfo);
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


////remove keys attached to the root
//void removeRootListKey(int key, Root* TreeRoot, MetaInfo* Info)
//{
//    int mod = key % Info->rootConnections;
//    Node* ListStart = TreeRoot->startOfList;
//
//    while (ListStart != nullptr)
//    {
//        if (ListStart->listnum == mod)
//        {
//            if (ListStart->nextTreeNode->key == key)
//            {
//                if (ListStart->nextTreeNode->startOfList == nullptr)
//                {
//
//                }
//                //case no children
//                // 
//                // case one children
//                // 
//                // case one+ children
//                // deleting from rootlist helper (this node) (rootlist)
//            }
//            else
//            {
//                return;
//            }
//        }
//
//        if (ListStart->listnum > mod)
//        {
//            return;
//        }
//    }
//}


//=============================================================================================================================
//======================================= DEBBUGING FUNCTIONS =================================================================
//=============================================================================================================================

//void printRoot(Root* TreeRoot)
//{
//    printf("value of the root itself: %d\n", TreeRoot->key);
//    printf("\n");
//    Node* currentNode = TreeRoot->startOfList;
//
//    while (currentNode != nullptr)
//    {
//        printf("index of arraylist %d\n", currentNode->listnum);
//        printf("key of children: %d\n", currentNode->nextTreeNode->key);
//        printf("\n");
//        currentNode = currentNode->nextListNode;
//    }
//
//}
