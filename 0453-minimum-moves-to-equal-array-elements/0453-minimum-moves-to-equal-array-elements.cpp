class Solution {
public:
    int minMoves(vector<int>& nums) {
        
        int mini=nums[0];
        int moves=0;
        
        int n=nums.size();
        for(int i=0;i<n;i++){
             if (nums[i]<mini) {
                mini = nums[i];
             }
        }
        
        for(int i=0;i<n;i++)
        { 
            int diff=nums[i]-mini;
            moves = moves + diff;
        }
        return moves;

    }
};