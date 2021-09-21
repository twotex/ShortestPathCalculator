#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Node
{
private:
    string nodeName;
    bool visited;

public:
    vector<Node*> connectedNodes;
    void addConnectionNode(Node* nodeToAdd)
    {
        connectedNodes.push_back(nodeToAdd);
    }

    Node()
    {
        nodeName = "";
        visited = false;
    }

    Node(string theNode)
    {
        nodeName = theNode;
        visited = false;
    }

    string getNodeName()
    {
        return nodeName;
    }

    void setVisitedStatus(bool status)
    {
        visited = status;
    }

    bool getVisitedStatus()
    {
        return visited;
    }
};

class SearchResults
{
private:
    vector<Node*> theNodesSearched;
    int numOfNodesSearched;
    bool theResults;

public:
    SearchResults(vector<Node*> nodesTraversed, int numOfNodesTraversed, bool found)
    {
        theNodesSearched = nodesTraversed;
        numOfNodesSearched = numOfNodesTraversed;
        theResults = found;
    }

    vector<Node*> getNodesSearched()
    {
        return theNodesSearched;
    }

    int getNumOfNodesSearched()
    {
        return numOfNodesSearched;
    }

    bool foundEndpointResults()
    {
        return theResults;
    }

    string returnNodesTraversed()
    {
        string stringToReturn = "";
        for (Node* theNode : theNodesSearched)
        {
            stringToReturn.append(theNode->getNodeName());

            if (theNode != theNodesSearched.back())
            {
                stringToReturn.append("-");
            }
        }

        return stringToReturn;
    }
};

SearchResults searchNodeForEndPoint(Node* originalPoint, Node* endPoint, vector<Node*> nodesSearched)
{
    nodesSearched.push_back(originalPoint);
    originalPoint->setVisitedStatus(true);

    if (originalPoint == endPoint)
    {
        originalPoint->setVisitedStatus(false);
        return SearchResults(nodesSearched, nodesSearched.size(), true);
    }

    else if (originalPoint->connectedNodes.empty() == false)
    {
        int shortestPathSize = -1;
        vector<Node*> shortestPathOfNodes;
        bool pResults = false;

        for (Node* theNode : originalPoint->connectedNodes)
        {
            if (theNode->getVisitedStatus() == false)
            {
                SearchResults tempResults = searchNodeForEndPoint(theNode, endPoint, nodesSearched);

                //Found the node we were searching for
                if (tempResults.foundEndpointResults() == true)
                {
                    pResults = true;

                    //initialized the shortest path
                    if (shortestPathSize == -1)
                    {
                        shortestPathSize = tempResults.getNumOfNodesSearched();
                        shortestPathOfNodes = tempResults.getNodesSearched();
                    }

                    //replaced the current shortest path
                    else if (tempResults.getNumOfNodesSearched() < shortestPathSize)
                    {
                        shortestPathSize = tempResults.getNumOfNodesSearched();
                        shortestPathOfNodes = tempResults.getNodesSearched();
                    }
                }
            }
        }

        originalPoint->setVisitedStatus(false);
        return SearchResults(shortestPathOfNodes, shortestPathSize, pResults);
    }

    else
    {
        originalPoint->setVisitedStatus(false);
        return SearchResults(nodesSearched, nodesSearched.size(), false);
    }
}

string ShortestPath(string strArr[], int arrLength) {

    int numOfNodes = stoi(strArr[0]);
    vector<Node*> allNodesVector;

    for (int count = 1; count <= numOfNodes; count++)
    {
        allNodesVector.push_back(new Node(strArr[count]));
    }

    if (arrLength > numOfNodes + 1)
    {
        for (int rCount = numOfNodes + 1; rCount < arrLength; rCount++)
        {
            int indexOfDash = 0;

            for (char s : strArr[rCount])
            {
                if (s == '-')
                {
                    break;
                }
                indexOfDash++;
            }

            string firstNodeName = strArr[rCount].substr(0, indexOfDash);
            string secondNodeName = strArr[rCount].substr(indexOfDash + 1);

            Node* firstNodeReference = nullptr;
            Node* secondNodeReference = nullptr;

            for (Node* i : allNodesVector)
            {
                if (i->getNodeName().compare(firstNodeName) == 0)
                {
                    firstNodeReference = i;
                }

                else if (i->getNodeName().compare(secondNodeName) == 0)
                {
                    secondNodeReference = i;
                }
            }

            for (Node* theNode : allNodesVector)
            {
                if (theNode->getNodeName().compare(firstNodeName) == 0)
                {
                    theNode->addConnectionNode(secondNodeReference);
                }

                else if (theNode->getNodeName().compare(secondNodeName) == 0)
                {
                    theNode->addConnectionNode(firstNodeReference);
                }
            }
        }
    }

    Node* startingNode = allNodesVector.front();
    Node* endingNode = allNodesVector.back();
    vector<Node*> searchedList;
    SearchResults test = searchNodeForEndPoint(startingNode, endingNode, searchedList);

    if (test.foundEndpointResults() == true)
    {
        return test.returnNodesTraversed();
    }

    else
    {
        return "-1";
    }
}

int main(void) {

    // keep this function call here
    string A[] = { "7", "A", "B", "C", "D", "E", "F", "G", "A-B", "A-E", "B-C", "C-D", "D-F", "E-D", "F-G" };
    int arrLength = sizeof(A) / sizeof(*A);
    cout << ShortestPath(A, arrLength);
    return 0;

}