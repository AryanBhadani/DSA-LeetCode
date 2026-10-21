
class Solution {
public:
    long long maxAlternatingSum(vector<int>& nums,vector<vector<long long>>&dp,int i,int sign) {
        if(i>=nums.size()){
            return 0;
        }
        if(dp[i][sign]!=-1){
            return dp[i][sign];
        }

        long long take;
        if(sign==0){
            take=nums[i]+maxAlternatingSum(nums,dp,i+1,1);
        }
        else{
            take=-nums[i]+maxAlternatingSum(nums,dp,i+1,0);
        }

        long long skip=maxAlternatingSum(nums,dp,i+1,sign);

        dp[i][sign]=max(take,skip);
        return dp[i][sign];
    }

    long long maxAlternatingSum(vector<int>& nums){
        vector<vector<long long>>dp(nums.size(),vector<long long>(2,-1));
        return maxAlternatingSum(nums,dp,0,0);
    }
};
