class Solution {
public:
    bool canPartition(vector<int>& nums,vector<vector<int>>&dp,int target,int i) {
        if(target==0){
            return true;
        }
        if(i>=nums.size()||target<0){
            return false;
        }
        if(dp[i][target]!=-1){
            return dp[i][target];
        }
        bool take=false;
        if(nums[i]<=target){
            take=canPartition(nums,dp,target-nums[i],i+1);
        }
        bool skip=canPartition(nums,dp,target,i+1);
        return dp[i][target]=take||skip;
        

    }
    bool canPartition(vector<int>&nums){
        int sum=0;
        for(int i=0;i<nums.size();i++){
            sum=sum+nums[i];
        }
        if(sum%2!=0){
            return false;
        }
        int target=sum/2;
        vector<vector<int>>dp(nums.size(),vector<int>(target+1,-1));

        return canPartition(nums,dp,target,0);
    }
};