class Solution {
public:
    bool lastStoneWeightII(vector<int>&stones,vector<vector<int>>&dp,int i,int target) {
        if(target==0){
            return true;
        }
        if(i>=stones.size()||target<0){
            return false;
        }
        if(dp[i][target]!=-1){
            return dp[i][target];
        }

        bool take=false;
        if(stones[i]<=target){
            take=lastStoneWeightII(stones,dp,i+1,target-stones[i]);
        }

        bool skip=lastStoneWeightII(stones,dp,i+1,target);

        return dp[i][target]=take||skip;
    }

    int lastStoneWeightII(vector<int>&stones) {
        int sum=0;
        for(int i=0;i<stones.size();i++){
            sum=sum+stones[i];
        }

        int target=sum/2;
        vector<vector<int>>dp(stones.size(),vector<int>(target+1,-1));

        for(int currsum=target;currsum>=0;currsum--){
            if(lastStoneWeightII(stones,dp,0,currsum)){
                return sum-2*currsum;
            }
        }

        return 0;
    }
};