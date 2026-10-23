class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        
        int sum =0;
        int n =nums.size();
        for(int i=0;i<n;i++){
            sum=sum+nums[i];

        }
        if(abs(target)>sum){
            return 0;
        }
        else if((sum+target)%2!=0){
            return 0;
        }
        int s=(sum+target)/2;
        vector<vector<int>> dp(n+1,vector<int>(s+1,0));

        dp[0][0]=1;
        for(int i=1;i<=n;i++){
            for(int j=0;j<=s;j++){
                int skip =dp[i-1][j];
                int take=0;

                if(nums[i-1]<=j){
                    take=dp[i-1][j-nums[i-1]];
                }
                dp[i][j]=take+skip;
            }
        }
        return dp[n][s];
    }
};