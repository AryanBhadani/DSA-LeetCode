class Solution {
public:
    int minMoves2(vector<int>& nums) {
        int n=nums.size();
        long long moves=0;
        sort(nums.begin(),nums.end());
        int mid=n/2;
        int target=nums[mid];
        for(int i=0;i<n;i++){
            if (nums[i]>target){
                moves=moves+nums[i]-target;}
            else{                                
                moves=moves+target-nums[i];
                    }
        }
        return moves;
    }
};