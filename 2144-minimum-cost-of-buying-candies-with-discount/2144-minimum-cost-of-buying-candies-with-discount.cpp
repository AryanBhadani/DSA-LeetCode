class Solution {
public:
    int minimumCost(vector<int>& cost) {
        sort(cost.rbegin(),cost.rend());
        int sum=0;
        int c=cost.size();
        for(int i=0;i<c;i++){
            if(i%3==0||i%3==1)
            sum+=cost[i];
        }
        return sum;
    }
};