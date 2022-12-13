#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include <algorithm>

using namespace std;

typedef map<string, float> avCountsValue;
typedef map<string, avCountsValue> avCountsType;

int _counter = 0;

const auto ALL_ATTRS = [](string first, avCountsValue second)->bool{ return true; };

struct BestChildren {
    void* bestCu;
    void* best1; 
    void* best2;
};
struct BestOperation {
    void* ignore;
    void* bestAction; 
};
class CobWebNode {
public:
    int conceptId;
    float count;
    avCountsType avCounts;    
    vector<CobWebNode *> children;
    CobWebNode *parent;
    CobWebNode *treeRoot;
    friend ostream& operator<<(ostream& os, const CobWebNode& cwn);
    CobWebNode() {
        this->conceptId = this->gensym();
        this->count = 0;
    }
    CobWebNode(CobWebNode *otherNode) {
        CobWebNode();
        this->treeRoot = otherNode->treeRoot;
        this->parent = otherNode->parent;
        this->updateCountsFromNode(otherNode);
        for(auto child: otherNode->children)  {
            this->children.push_back((CobWebNode*)&child);
        }
    }
    void updateCountsFromNode(CobWebNode* node) {
        this->count += node->count;
        for(auto attrRow: node->attrs(ALL_ATTRS)) {
            string attr = attrRow.first;
            if(this->avCounts.find(attr) == this->avCounts.end()) {
                avCountsValue attrValue;
                this->avCounts[attr] = attrValue;
            }
            for(auto valRow: node->avCounts[attr]) {
                string val = valRow.first;
                if((node->avCounts[attr]).find(val) == node->avCounts[attr].end()) {
                    node->avCounts[attr][val] = 0;
                }
                this->avCounts[attr][val] += node->avCounts[attr][val];
            }
        }
    }
    void shallowCopy() {
    } 
    avCountsType attrs(function<bool(string, avCountsValue)> attrFilter) {
        map<string, avCountsValue> filtered;
        for(auto avCount: this->avCounts) {
            if(attrFilter(avCount.first, avCount.second)) {
                filtered[avCount.first] = avCount.second;
            }
        }
        return filtered;
    }
    avCountsType attrs() {
        return this->attrs(
            [](string first, avCountsValue second) -> bool {
                return first != "_";
            }
        );
    }
    void incrementCounts(void* instances) {
    }
    void expectedCorrectGuesses() {
    }
    void categoryUtility() {
    }
    void getBestOperation(void* instance, void* best1, void* best2, void* best1Cu, vector<string> possibleOps) {
    }
    struct BestChildren twoBestChildren(void* instance){
    }
    struct BestOperation getBestOperations(void* instance, void* best1, void* best2, void* bestCu){
    }
    void computeRelativeCUConstant(void* instance){
    }
    void relativeCUForInsert(void* child, void* instance){
    }
    void cuForInsert(void* child, void* instance){
    }
    CobWebNode* createNewChild(void* instance){
    }
    void createChildWithCurrentCounts(){
    }
    void cuForNewChild(void* instance){
    }
    void merge(void* best1, void* best2){
    }
    void cuForMerge(void* best1, void* best2, void* instance){
    }
    void split(void* best){
    }
    void cuForFringeSplit(void* instance){
    }
    void cuForSplit(void* best){
    }
    bool isExactMatch(void* instance){
    }
    void hash(){
    }
    int gensym() {
        _counter++;
        return _counter;
    }
    void __str__(){
    }
    void pretty_print(){
    }
    void depth(){
    }
    void isParent(){
    }
    void numConcepts(){
    }
    void outputJson(){
    }
    void getWeightedValues(){
    }
    void predict(){
    }
    void probability(){
    }
    void logLikelihood(){
    }
};
class CobWebTree {
public:
    CobWebNode* root;
    friend ostream& operator<<(ostream& os, const CobWebTree& cwt);
    CobWebTree() {
        root = new CobWebNode();
        root->treeRoot = this->root;
    }
    void clear() {
        root = new CobWebNode();
        root->treeRoot = this->root;
    }
    void sanityCheckInstance(void* instance) {

    }
    void ifit(void* instance) {
        this->sanityCheckInstance(instance);
        return this->cobweb(instance);
    }
    void fit(vector<void*> instances, int iterations, bool randomizeFirst) {
        for(int i=0;i<iterations;i++) {
            if(i == 0 && randomizeFirst) {
                random_shuffle(instances.begin(), instances.end());
            }
            for(auto instance: instances) {
                this->ifit(instance);
            }
            random_shuffle(instances.begin(), instances.end());
        }
    }
    void cobweb(void* instance) {
        auto current = this->root;
        while(current != NULL) {
            if(current->children.size() == 0 && (current->isExactMatch(instance) || current->count == 0)) {
                current->incrementCounts(instance);
                break;
            } else if(current->children.size() == 0) {
                auto newCurrent = current;
                current->parent = newCurrent;
                newCurrent->children.push_back(current);
                if(newCurrent->parent != NULL) {
                    auto currentIter = find(
                        newCurrent->parent->children.begin(),
                        newCurrent->parent->children.end(),
                        current
                    );
                    newCurrent->parent->children.erase(currentIter);
                    newCurrent->parent->children.push_back(newCurrent);
                } else {
                    this->root = newCurrent;
                }
                newCurrent->incrementCounts(instance);
                current = newCurrent->createNewChild(instance);
                break;
            } else {
                auto newChild = current;
                struct BestChildren bestChildren = current->twoBestChildren(instance);
                void* bestCu = bestChildren.bestCu;
                void* best1 = bestChildren.best1;
                void* best2 = bestChildren.best2;
                struct BestOperation bestOperation = current -> getBestOperations(instance, best1, best2, bestCu);
                void* bestAction = bestOperation.bestAction;
                if(bestAction == "best")
                {
                    current->incrementCounts(instance);
                    // yet to change current = best1;
                }
                else if(bestAction == "new")
                { 
                    current->incrementCounts(instance);
                    current = current->createNewChild(instance);
                    break;
                }
                else if(bestAction == "merge")
                {
                    current->incrementCounts(instance);
                    // yet to change newChild = current->merge(best1, best2);
                    current = newChild;
                }
                else if(bestAction == "split")
                {
                    current->split(best1);
                }
                else
                {
                    throw std::invalid_argument("Best action choice not a recognized option. This should be impossible...");
                }
            }
        }
    }
    CobWebNode* cobwebCategorize(void* instance) 
    {
        auto current = this->root;
        while(current != NULL) {
            if(current->children.size()==0)
            {
                return current;
            }
            struct BestChildren bestChildren = current->twoBestChildren(instance);
            current = (CobWebNode*) bestChildren.best1;
        }
        return current;
    }
    void inferMissing(void* instance, string choiceFunction, bool allowNone) {
        // this->sanityCheckInstance(instance);
        // CobWebNode *temp_instance = new CobWebNode(*((*CobWebNode)instance)); // creating a copy of instance
        // CobWebNode* concept = this->cobwebCategorize(temp_instance);

    } 
    CobWebNode* categorize(void* instance) {
        this->sanityCheckInstance(instance);
        return this->cobwebCategorize(instance);
    }
};
ostream& operator<<(ostream& os, const CobWebTree& cwt) {
    os << (*cwt.root);
    return os;
}
ostream& operator<<(ostream& os, const CobWebNode& cwn) {
    os << "Yet to develop";
    return os;
}
int main() {
    auto cwt = new CobWebTree();
    cout << *cwt << endl;
    return 0;
}