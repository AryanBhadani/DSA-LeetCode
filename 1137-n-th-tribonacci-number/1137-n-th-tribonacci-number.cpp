class Solution {
public:
    int tribonacci(int n,vector<int>&dp) {
        if(n==0){
            return 0;
        }
        if(n==1||n==2){
            return n==1?1:1;
        }
        if(dp[n]!=-1){
            return dp[n];

        }
         int a=tribonacci(n-1,dp);
        int b=tribonacci(n-2,dp);
        int c=tribonacci(n-3,dp);
        dp[n]=a+b+c;
        return dp[n];
    }
    int tribonacci(int n){
        vector<int>dp(n+1,-1);
        return tribonacci(n,dp);
        
    }
};