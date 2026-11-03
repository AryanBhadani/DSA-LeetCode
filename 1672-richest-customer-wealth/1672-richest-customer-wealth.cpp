class Solution {
public:
    int maximumWealth(vector<vector<int>>& accounts) {
        int a=accounts.size();
        int maxwealth=0;
        for(int i=0;i<a;i++){
            int total=0;
            for(int j=0;j<accounts[i].size();j++){
                total+=accounts[i][j];
            }
            if(total>maxwealth){
                maxwealth=total;
            }
        }
        return maxwealth;
        
    }
};