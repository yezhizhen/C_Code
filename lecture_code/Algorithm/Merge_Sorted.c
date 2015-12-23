/** 
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode *pre;
    struct ListNode *head;
    
    struct ListNode *left = l1;
    struct ListNode *right = l2;
    
    if(left)
    {
        //right not null
        if(right)
        {
            if(left->val > right->val)
            {
                
                pre = right;
                head = pre;
                right = right->next;
            }
            else
            {
                pre = left;
                head = pre;
                left = left->next;
            }
        }
        //right is null
        else
        {
            return left;
        }    
    }
    else
    {
        return right;
    }
    
    //while both are not NULL
    while(left && right)
    {
        if(left->val > right->val)
        {
               pre->next = right;
               pre = right;
               right = right->next;
        }
        else
        {
            pre->next = left;
            pre = left;
            left = left->next;
        }
    }
    //Now, check
    if(left)
    {
        pre->next = left;       
    }
    else if(right)
    {
        pre->next = right;
    }
    return head;
    
}
