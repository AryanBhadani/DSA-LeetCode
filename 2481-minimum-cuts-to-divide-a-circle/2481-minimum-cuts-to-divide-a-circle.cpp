class Solution {
public:
    int numberOfCuts(int n) {
        int ans=0;
        if(n==1){
            return ans=0;
        }
        if(n%2==0){
            ans=n/2;

        }
        else{
            ans=n;
        }
        return ans;
        
    }
};