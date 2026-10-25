class Solution {
public:
    int smallestRangeI(vector<int>& nums, int k) {
        int n=nums.size();
        int maxi=nums[0];
        int mini=nums[0];
        for(int i=1;i<n;i++){
            mini = min(mini,nums[i]);
            maxi = max(maxi,nums[i]);
        }

        return max(0,maxi-mini-2*k);
        
        
    }
};