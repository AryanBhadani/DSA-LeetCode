class Solution {
public:
    int rob(vector<int>& nums,int i ,vector<int>&dp) {
        if(i>=nums.size()){
            return 0;
        }
        if(dp[i]!=-1){
            return dp[i];
        }
        
        int take = nums[i] +rob(nums,i+2, dp);
        int skip = rob(nums,i+1, dp);

        dp[i] = max(take, skip);

        return dp[i];
    }
        int rob(vector<int>& nums){
        vector<int> dp(nums.size(), -1);
        return rob(nums,0,dp);
        }
    
};