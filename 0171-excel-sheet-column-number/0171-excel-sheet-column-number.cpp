class Solution {
public:
    int titleToNumber(string columnTitle) {
        int ct=columnTitle.size();
        int ans=0;
        for(int i=0;i<ct;i++){
        ans=ans*26+(columnTitle[i]-'A'+1);
        }
        return ans;
    }
};