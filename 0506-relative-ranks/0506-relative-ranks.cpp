class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& score) {
        int n = score.size();
        vector<int>arr=score;
        sort(arr.rbegin(),arr.rend());

        vector<string>ans(n);

        for(int i=0;i<n;i++) {
            for(int j=0;j<n;j++) {
                if(score[j]==arr[i]) {
                    if(i==0)ans[j]="Gold Medal";
                    else if(i == 1) ans[j] ="Silver Medal";
                    else if(i == 2) ans[j] ="Bronze Medal";
                    else ans[j] =to_string(i + 1);
                }
            }
        }

        return ans;
    }
};