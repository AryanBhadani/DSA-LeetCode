
class Solution {
public:
    int t[1001][1001];

    bool longestPalindrome(string &s,int m, int n) {
        if (m >= n) {
            return true;
        }

        if (t[m][n]!=-1) {
            return t[m][n];
        }

        if (s[m]==s[n]) {
            return t[m][n] = longestPalindrome(s,m+1,n-1);
        }

        return t[m][n]=false;
    }

    string longestPalindrome(string s) {
        memset(t,-1,sizeof(t));

        int S = s.size();
        int maxLen =0;
        int start =0;

        for (int m=0;m<S;m++) {
            for (int n=m; n<S; n++) {
                if(longestPalindrome(s,m,n) && n-m+1>maxLen) {
                    maxLen=n-m+1;
                    start=m;
                }
            }
        }

        return s.substr(start,maxLen);
    }
};
