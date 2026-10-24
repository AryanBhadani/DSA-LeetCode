class Solution {
public:
    int maxTotalReward(int i,vector<int>& rewardValues,vector<int>&dp) {
        if(i>=rewardValues.back()) {
            return 0;
        }

        if(dp[i]!= -1) {
            return dp[i];
        }

        int ans=0;

        for(int j=0;j<rewardValues.size();j++) {
            if(rewardValues[j]>i) {
                ans = max(ans,rewardValues[j]+maxTotalReward(i+rewardValues[j],rewardValues,dp));
            }
        }

        return dp[i]=ans;
    }

    int maxTotalReward(vector<int>& rewardValues) {
        sort(rewardValues.begin(), rewardValues.end());

        int m =rewardValues.back();
        vector<int>dp(2*m,-1);

        return maxTotalReward(0,rewardValues,dp);
    }
};