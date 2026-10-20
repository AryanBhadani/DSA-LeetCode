class Solution {
public:
    int mincostTickets(vector<int>& days,int i, vector<int>& costs,vector<int>&dp) {
        if(i>=days.size())
        return 0;
        if(dp[i]!=-1){
            return dp[i];
        }
        int next1 = i + 1;

        int next7 =i;
        while (next7<days.size()&&days[next7]<days[i]+7) {
            next7++;
        }

        int next30 = i;
        while (next30<days.size()&&days[next30]<days[i]+30) {
            next30++;
        }

        int option1 = costs[0] + mincostTickets(days,next1,costs,dp);
        int option2 = costs[1] + mincostTickets(days,next7,costs,dp);
        int option3 = costs[2] + mincostTickets(days,next30,costs,dp);

            dp[i] = min({option1, option2, option3});
                return dp[i];
         }
         int mincostTickets(vector<int>& days, vector<int>& costs) {
        vector<int> dp(days.size(), -1);

        return mincostTickets(days, 0, costs, dp);
    }
};