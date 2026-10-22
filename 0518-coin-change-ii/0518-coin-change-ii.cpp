class Solution {
public:
    int change(vector<int>& coins,int amount,int i,vector<vector<int>>&dp) {
        if(amount==0){
            return 1;
        }
        if(i==coins.size()){
            return 0;
        }
        if(dp[i][amount]!=-1){
            return dp[i][amount];
        }
        int skip=change(coins,amount,i+1,dp);
        int take=0;
        if(coins[i]<=amount){
            take=change(coins,amount-coins[i],i,dp);
        }
        int ans=take+skip;
        return dp[i][amount]=ans;

    
        
    }
    int change(int amount,vector<int>& coins){
    vector<vector<int>> dp(coins.size(),vector<int>(amount+1,-1));
    return change(coins,amount,0,dp);
    }

};