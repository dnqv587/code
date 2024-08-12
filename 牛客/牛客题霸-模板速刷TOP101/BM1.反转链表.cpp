/**
 * @nc app=nowcoder id=75e878df47f24fdc9dc3e400ec6058ca topic=295 question=23286 lang=C++
 * 2024-08-12 23:43:40
 * https://www.nowcoder.com/practice/75e878df47f24fdc9dc3e400ec6058ca?tpId=295&tqId=23286
 * [BM1] 反转链表
 */

/** @nc code=start */

/**
 * struct ListNode {
 *	int val;
 *	struct ListNode *next;
 *	ListNode(int x) : val(x), next(nullptr) {}
 * };
 */
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param head ListNode类 
     * @return ListNode类
     */
    ListNode* ReverseList(ListNode* head) {
        // write code here
        ListNode* currNode= head;
        ListNode* preNode = nullptr;
        while(currNode){
            ListNode* tempNode = currNode->next;
            currNode->next = preNode;
            preNode = currNode;
            currNode = tempNode;
        }
        return preNode;
    }
};

/** @nc code=end */
